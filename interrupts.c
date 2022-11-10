
/********************************************************************************
* interrupts.c: Innehåller avbrottsrutiner.
********************************************************************************/
#include "header.h"

/********************************************************************************
* ISR (INT0_vect): Avbrottsrutin, som anropas vid nedtryckning av reset-knappen
*                  ansluten till pin 2 (PORTD2). Vid reset så återställs
*                  tillståndsmaskinen till startläget (lysdioderna släcks).
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
* ISR (PCINT0_vect): Avbrottsrutin för PCI-avbrott på I/O-port B, som anropas
*                    vid uppsläppning eller nedtryckning av .
					 Om någon av
*                    dessa knappar trycks ned så uppdateras tillståndsmaskinen.
********************************************************************************/
ISR (PCINT0_vect)
{
	if (NEXT1_BUTTON_PRESSED || NEXT2_BUTTON_PRESSED || REVERSE1_BUTTON_PRESED)
	{
		fsm_update();
	}

	return;
}
