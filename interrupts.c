
/********************************************************************************
* interrupts.c: Inneh�ller avbrottsrutiner.
********************************************************************************/
#include "header.h"

/********************************************************************************
* ISR (INT0_vect): Avbrottsrutin, som anropas vid nedtryckning av reset-knappen
*                  ansluten till pin 2 (PORTD2). Vid reset s� �terst�lls
*                  tillst�ndsmaskinen till startl�get (lysdioderna sl�cks).
********************************************************************************/
ISR (INT0_vect)
{
	fsm_reset();	
	return;
}

ISR (INT1_vect)
{
	fsm_update();
	return;
}

/********************************************************************************
* ISR (PCINT0_vect): Avbrottsrutin f�r PCI-avbrott p� I/O-port B, som anropas
*                    vid uppsl�ppning eller nedtryckning av .
					 Om n�gon av
*                    dessa knappar trycks ned s� uppdateras tillst�ndsmaskinen.
********************************************************************************/
ISR (PCINT0_vect)
{
	if (NEXT1_BUTTON_PRESSED || NEXT2_BUTTON_PRESSED || REVERSE1_BUTTON_PRESED)
	{
		fsm_update();
	}

	return;
}
