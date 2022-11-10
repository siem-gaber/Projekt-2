#include "Header.h"

/********************************************************************************
* main: Initierar systemet vid start (konfigurerar I/O-portar samt aktiverar
*       avbrott p� tryckknapparnas pinnar). Programmet h�lls sedan ig�ng
*       kontinuerligt s� l�nge matningssp�nning tillf�rs. Lysdioderna tillst�nd
*       uppdateras via avbrottsrutiner. Tillst�nsdmaskinens utsignaler
*       uppdateras kontinuerligt utefter aktuellt tillst�nd.
********************************************************************************/

int main(void)
{
	setup();
   
    while (1) 
    {
		fsm_set_output();
    }
	return 0;
}

