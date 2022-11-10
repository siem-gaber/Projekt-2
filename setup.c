/********************************************************************************
* setup.c: Innehåller initieringsrutiner.
********************************************************************************/
#include "header.h"

/* Statiska funktioner: */
static inline void init_ports(void);
static inline void init_interrupts(void);

/********************************************************************************
* setup: Initierar mikrodatorn vid start.
********************************************************************************/
void setup(void)
{
	 init_ports();
	 init_interrupts();
	 fsm_reset();
	 return;
}

/********************************************************************************
* init_ports: Sätter lysdiodernas pinnar till utportar och sätter tryckknapparnas
*             pinnar till aktiva inportar (insignalerna blir alltid 0 eller 1).
********************************************************************************/
static inline void init_ports(void)
{
	DDRB = (1 << LED1) | (1 << LED2) | (1 << LED3);
	PORTB = (1 << REVERSE1_BUTTON) | (1 << NEXT1_BUTTON) | (1<< NEXT2_BUTTON);
	PORTD = (1 << RESET_BUTTON) | (1 << REVERSE2_BUTTON);
	return;
}


/********************************************************************************
* init_interrupts: Aktiverar PCI-avbrott på pin 11 - 13 / PORTB4 - PORTB5 samt
*                  externt avbrott INT0 på pin 2 (PORTD2) på stigande flank.
*
*                  PCI-avbrotten har avbrottsvektor PCINT0_vect.
*                  Externt avbrott INT0 har avbrottsvektor INT0_vect.
********************************************************************************/
static inline void init_interrupts(void)
{
   asm("SEI");
   PCICR = (1 << PCIE0);
   PCMSK0 = (1 << NEXT1_BUTTON) | (1 << REVERSE1_BUTTON) | (1 << NEXT2_BUTTON);

   EICRA = (1 << ISC11) | (1 << ISC10) | (1 << ISC01); // Ställer in pin 3 på stigande flank och pin 2 på fallande flank.
   EIMSK = (1 << INT1) | (1 << INT0); // Sätter på avbrott på pin 3 samt pin 2.
   return;
}