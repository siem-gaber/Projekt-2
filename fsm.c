/********************************************************************************
* fsm.c: Inneh�ller funktionalitet f�r implementering av tillst�ndsmaskinen
*        f�r styrning av lysdioderna. Pekare anv�nds vid passering av
*        blinkhastigheten f�r att direkt kunna avbryta en blinkning ifall
*        aktuellt tillst�nd �verg�r fr�n blinkande till annat tillst�nd.
*        F�rdr�jningstiden som refereras till nollst�lls n�r detta sker,
*        vilket medf�r att kvarvarande f�rdr�jningstid s�tts till noll.
********************************************************************************/
#include "header.h"

/* Statiska funktioner: */
static void leds_blink(const volatile uint16_t* blink_speed_ms);
static void delay_ms(const volatile uint16_t* delay_time_ms);

/* Statiska datamedlemmar: */
static enum led_state state = LED_STATE_OFF; /* Lagrar d�rrens tillst�nd. */
static uint16_t speed_ms = 0;                /* Lysdiodernas blinkhastighet i ms.*/

/********************************************************************************
* fsm_reset: �terst�ller tillst�ndsmaskinen till startl�get, vilket inneb�r
*            att lysdioderna sl�cks.
********************************************************************************/
void fsm_reset(void)
{
	state = LED_STATE_OFF;
	speed_ms = 0;
	LEDS_OFF;
	return;
}

/********************************************************************************
* fsm_uppdate: Uppdaterar tillst�ndsmaskinen utefter nuvarande tillst�nd samt
*              vilken tryckknapp som trycks ned. F�r att avbryta eventuell
*              blinkning s� nollst�lls lysdiodernas blinkhastighet tempor�rt
*              n�r aktuellt tillst�nd �verg�r fr�n blinkande till n�got annat.
*              Tillst�ndsmaskinens utsignaler uppdateras innan funktionen
*              avslutas (eventuell blinkning genomf�rs ej, utan kr�ver att
*              anv�ndaren k�r tillst�ndsmaskinen via funktionen fsm_set_output).
********************************************************************************/
void fsm_update(void)
{
	switch (state)
	{
		case LED_STATE_OFF:
		{
			if (NEXT1_BUTTON_PRESSED)
			{
				state = LED_STATE_SLOW;
			}
			else if (REVERSE1_BUTTON_PRESED)
			{
				state = LED_STATE_ON;
			}
			else if (NEXT2_BUTTON_PRESSED)
			{
				state = LED_STATE_MEDIUM;
			}
			else if (REVERSE2_BUTTON_PRESED)
			{
				state = LED_STATE_FAST;
			}
			break;
		}
		
		case LED_STATE_SLOW:
		{
			if (NEXT1_BUTTON_PRESSED)
			{
				state = LED_STATE_MEDIUM;
			}
			else if (REVERSE1_BUTTON_PRESED)
			{
				state = LED_STATE_OFF;
			}
			else if (NEXT2_BUTTON_PRESSED)
			{
			state = LED_STATE_FAST;
			}
			else if (REVERSE2_BUTTON_PRESED)
			{
			state = LED_STATE_ON;
			}
			break;
			}
		
		
		case LED_STATE_MEDIUM:
		{
			if (NEXT1_BUTTON_PRESSED)
			{
				state = LED_STATE_FAST;
			}
			else if (REVERSE1_BUTTON_PRESED)
			{
				state = LED_STATE_SLOW;
			}
			else if (NEXT2_BUTTON_PRESSED)
			{
				state = LED_STATE_ON;
			}
			else if (REVERSE2_BUTTON_PRESED)
			{
				state = LED_STATE_OFF;
			}
			break;
		}
		case LED_STATE_FAST:
		{
			if (NEXT1_BUTTON_PRESSED)
			{
				state = LED_STATE_ON;
			}
			else if (REVERSE1_BUTTON_PRESED)
			{
				state = LED_STATE_MEDIUM;
			}
			else if (NEXT2_BUTTON_PRESSED)
			{
				state = LED_STATE_OFF;
			}
			else if (REVERSE2_BUTTON_PRESED)
			{
				state = LED_STATE_SLOW;
			}
			break;
		}
		
		case LED_STATE_ON:
		{
			if (NEXT1_BUTTON_PRESSED)
			{
				state = LED_STATE_OFF;
			}
			else if (REVERSE1_BUTTON_PRESED)
			{
				state = LED_STATE_FAST;
			}
			else if (NEXT2_BUTTON_PRESSED)
			{
				state = LED_STATE_SLOW;
			}
			else if (REVERSE2_BUTTON_PRESED)
			{
				state = LED_STATE_MEDIUM;
			}
			break;
		}
		default:
		{
			fsm_reset();
			break;
		}
	}
		if (state == LED_STATE_OFF) speed_ms = 0;
		else if (state == LED_STATE_SLOW) speed_ms = 500;
		else if (state == LED_STATE_MEDIUM) speed_ms = 250;
		else if (state == LED_STATE_FAST) speed_ms = 50;
		else if (state == LED_STATE_ON) speed_ms = 0;
		return;
}

/********************************************************************************
* leds_blink: Blinkar lysdioderna i en slinga med angiven blinkhastighet.
*             Blinkhastigheten refereras till via en pekare f�r att direkt kunna
*             uppdateras, exempelvis f�r att avbrytas, vilket �stadkommes genom
*             att v�rdet p� den passerade adressen nollst�lls).
*
*             - blink_speed_ms: Pekare till blinkhastigheten m�tt i ms.
********************************************************************************/
static void leds_blink (const volatile uint16_t* blink_speed_ms)
{
	LEDS_OFF;
	LED1_ON;
	delay_ms(blink_speed_ms);

	LED1_OFF;
	LED2_ON;
	delay_ms(blink_speed_ms);

	LED2_OFF;
	LED3_ON;
	delay_ms(blink_speed_ms);

	LED3_OFF;
	return;
}

/********************************************************************************
* fsm_set_output: Uppdaterar tillst�ndsmaskinens utsignaler utefter aktuellt
*                 tillst�nd.
*
*                 speed_ms: Lysdiodernas blinkhastighet vid blinkning.
********************************************************************************/
void fsm_set_output(void)
{
	LEDS_OFF;
	if (state == LED_STATE_ON) LEDS_ON;
	else if (state == LED_STATE_OFF) LEDS_OFF;
	else if (state == LED_STATE_SLOW) leds_blink(&speed_ms);
	else if (state == LED_STATE_MEDIUM) leds_blink(&speed_ms);
	else if (state == LED_STATE_FAST) leds_blink(&speed_ms);
	return;
}

/********************************************************************************
* delay_ms: Genererar f�rdr�jning som m�tt i millisekunder. F�rdr�jningstiden
*           refereras till via en pekare f�r att direkt kunna uppdateras,
*           exempelvis f�r att avbrytas (�stadkommes genom att v�rdet p� den
*           passerade adressen nollst�lls).
*
*           - delay_time_ms: Pekare till f�rdr�jningstiden m�tt i ms.
********************************************************************************/
static void delay_ms(const volatile uint16_t* delay_time_ms)
{
	for (uint16_t i = 0; i < *delay_time_ms; i++)
	{
		_delay_ms(1);
	}
	return;
}
