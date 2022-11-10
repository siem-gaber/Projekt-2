/********************************************************************************
* fsm.c: Innehåller funktionalitet för implementering av tillståndsmaskinen
*        för styrning av lysdioderna. Pekare används vid passering av
*        blinkhastigheten för att direkt kunna avbryta en blinkning ifall
*        aktuellt tillstånd övergår från blinkande till annat tillstånd.
*        Fördröjningstiden som refereras till nollställs när detta sker,
*        vilket medför att kvarvarande fördröjningstid sätts till noll.
********************************************************************************/
#include "header.h"

/* Statiska funktioner: */
static void leds_blink(const volatile uint16_t* blink_speed_ms);
static void delay_ms(const volatile uint16_t* delay_time_ms);

/* Statiska datamedlemmar: */
static enum led_state state = LED_STATE_OFF; /* Lagrar dörrens tillstånd. */
static uint16_t speed_ms = 0;                /* Lysdiodernas blinkhastighet i ms.*/

/********************************************************************************
* fsm_reset: Återställer tillståndsmaskinen till startläget, vilket innebär
*            att lysdioderna släcks.
********************************************************************************/
void fsm_reset(void)
{
	state = LED_STATE_OFF;
	speed_ms = 0;
	LEDS_OFF;
	return;
}

/********************************************************************************
* fsm_uppdate: Uppdaterar tillståndsmaskinen utefter nuvarande tillstånd samt
*              vilken tryckknapp som trycks ned. För att avbryta eventuell
*              blinkning så nollställs lysdiodernas blinkhastighet temporärt
*              när aktuellt tillstånd övergår från blinkande till något annat.
*              Tillståndsmaskinens utsignaler uppdateras innan funktionen
*              avslutas (eventuell blinkning genomförs ej, utan kräver att
*              användaren kör tillståndsmaskinen via funktionen fsm_set_output).
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
*             Blinkhastigheten refereras till via en pekare för att direkt kunna
*             uppdateras, exempelvis för att avbrytas, vilket åstadkommes genom
*             att värdet på den passerade adressen nollställs).
*
*             - blink_speed_ms: Pekare till blinkhastigheten mätt i ms.
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
* fsm_set_output: Uppdaterar tillståndsmaskinens utsignaler utefter aktuellt
*                 tillstånd.
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
* delay_ms: Genererar fördröjning som mätt i millisekunder. Fördröjningstiden
*           refereras till via en pekare för att direkt kunna uppdateras,
*           exempelvis för att avbrytas (åstadkommes genom att värdet på den
*           passerade adressen nollställs).
*
*           - delay_time_ms: Pekare till fördröjningstiden mätt i ms.
********************************************************************************/
static void delay_ms(const volatile uint16_t* delay_time_ms)
{
	for (uint16_t i = 0; i < *delay_time_ms; i++)
	{
		_delay_ms(1);
	}
	return;
}
