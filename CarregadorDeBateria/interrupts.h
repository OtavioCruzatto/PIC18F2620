#include <pic18f2620.h>
#include <xc.h>

void configureInterrupts();

void configureInterrupts() {
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
}