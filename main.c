#include "Header.h"

/********************************************************************************
* main: Initierar systemet vid start (konfigurerar I/O-portar samt aktiverar
*       avbrott på tryckknapparnas pinnar). Programmet hålls sedan igång
*       kontinuerligt så länge matningsspänning tillförs. Lysdioderna tillstånd
*       uppdateras via avbrottsrutiner. Tillstånsdmaskinens utsignaler
*       uppdateras kontinuerligt utefter aktuellt tillstånd.
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

