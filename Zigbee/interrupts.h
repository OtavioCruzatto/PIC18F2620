#include <pic18f2620.h>
#include <xc.h>

void configureInterrupts();

void configureInterrupts() {
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;     // Habilita a interrup��o do RX da uart
    PIR1bits.RCIF = 0;     // Limpa a flag da interrup��o do RX
}