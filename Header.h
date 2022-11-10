#ifndef HEADER_H_
#define HEADER_H_

/* Klockfrekvens (beh�vs f�r f�rdr�jningsgenerering med funktionen _delay_ms): */
#define F_CPU 16000000UL /* 16 MHz. */

/* Inkluderingsdirektiv: */
#include <avr/io.h>        /* Inneh�ller information om I/O-register s�som DDRB och PORTB. */
#include <avr/interrupt.h> /* Inneh�ller information om avbrottsvektorer s�som PCINT0_vect. */
#include <util/delay.h>    /* Inneh�ller funktionalitet f�r f�rdr�jningsgenerering. */

/* Makrodefinitioner: */
#define LED1 0 /* Lysdiod 1 ansluten till pin 8 / PORTB0. */
#define LED2 1 /* Lysdiod 2 ansluten till pin 9 / PORTB1. */
#define LED3 2 /* Lysdiod 3 ansluten till pin 10 / PORTB2. */

#define LED1_ON PORTB |= (1 << LED1) /* T�nder lysdiod 1. */
#define LED2_ON PORTB |= (1 << LED2) /* T�nder lysdiod 2. */
#define LED3_ON PORTB |= (1 << LED3) /* T�nder lysdiod 3. */

#define LED1_OFF PORTB &= ~(1 << LED1) /* Sl�cker lysdiod 1. */
#define LED2_OFF PORTB &= ~(1 << LED2) /* Sl�cker lysdiod 2. */
#define LED3_OFF PORTB &= ~(1 << LED3) /* Sl�cker lysdiod 3. */

#define LEDS_ON PORTB |= (1 << LED1) | (1 << LED2) | (1 << LED3) /* T�nder lysdioderna. */
#define LEDS_OFF PORTB &= ~((1 << LED1) | (1 << LED2) | (1 << LED3))  /* Sl�cker lysdioderna. */

#define RESET_BUTTON 2
#define NEXT1_BUTTON 4
#define NEXT2_BUTTON 5
#define REVERSE1_BUTTON 3
#define REVERSE2_BUTTON 3

#define RESET_BUTTON_PRESSED (PIND & (1 << RESET_BUTTON)) /* Indikerar nedtryckning av reset-knappen. */
#define NEXT1_BUTTON_PRESSED (PINB & (1 << NEXT1_BUTTON)) /* Indikerar byte till n�sta tillst�nd */
#define NEXT2_BUTTON_PRESSED (PINB & (1 << NEXT2_BUTTON)) /* Indikerar byte till D�refter g�ende tillst�nd */
#define REVERSE1_BUTTON_PRESED (PINB & (1 << REVERSE1_BUTTON)) /* Indikerar byte till f�reg�ende tillst�nd. */
#define REVERSE2_BUTTON_PRESED (PIND & (1 << REVERSE2_BUTTON)) /* Indikerar byte till d�rf�reg�ende tillst�nd */


/********************************************************************************
* led_state: Datatyp f�r lagring av lysdiodernas tillst�nd.
********************************************************************************/

enum led_state
{
	LED_STATE_OFF, /* Samtliga lysdioder h�lls sl�ckta kontinuerligt. */
	LED_STATE_SLOW, /* De tre lysdioderna blinkar i en slinga var 500:e millisekund. */
	LED_STATE_MEDIUM, /* De tre lysdioderna blinkar i en slinga var 250:e millisekund. */
	LED_STATE_FAST, /* De tre lysdioderna blinkar i en slinga var 50:e millisekund. */
	LED_STATE_ON /* Samtliga lysdioder h�lls t�nda kontinuerligt. */
	
};

/********************************************************************************
* bool: Datatyp som indikerar sant eller falskt. Denna datatyp �r vanlig i
*       flertalet programmeringsspr�k. I C utg�r bool dock inte en primitiv
*       datatyp, men kan implementeras via en enumeration som h�r eller via
*       inkludering av standardbiblioteket stdbool.h (f�r C99 och senare).
********************************************************************************/
typedef enum { false, true } bool;



/********************************************************************************
* setup: Initierar mikrodatorn vid start.
********************************************************************************/
void setup(void);

/********************************************************************************
* fsm_reset: �terst�ller tillst�ndsmaskinen till startl�get, vilket inneb�r
*            att lysdioderna sl�cks.
********************************************************************************/
void fsm_reset(void);

/********************************************************************************
* fsm_uppdate: Uppdaterar tillst�ndsmaskinen utefter nuvarande tillst�nd samt
*              vilken tryckknapp som trycks ned.
********************************************************************************/
void fsm_update(void);

/********************************************************************************
* fsm_set_output: Uppdaterar tillst�ndsmaskinens utsignaler utefter aktuellt
*                 tillst�nd.
*
*                 blink_speed_ms: Lysdiodernas blinkhastighet vid blinkning.
********************************************************************************/
void fsm_set_output(void);

#endif /* HEADER_H_ */
