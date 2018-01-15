#include <pic18f2620.h>
#include <xc.h>
#include "configurationBits.h"

void habilitarCarregamentoDaBateria();
void habilitarDescarregamentoDaBateria();
void desabilitarDescarregamentoDaBateria();
void desabilitarCarregamentoDaBateria();

void desabilitarDescarregamentoDaBateria() {
    PORTBbits.RB1 = 1;              // Corta o MOSFET da carga resistiva
    __delay_ms(20);
}

void desabilitarCarregamentoDaBateria() {
    PORTBbits.RB0 = 1;              // Corta o MOSFET de carregamento
    __delay_ms(20);
}

void habilitarCarregamentoDaBateria() {
   
    if(PORTBbits.RB1 == 1) {
        PORTBbits.RB0 = 0;          // Ativa o MOSFET de carregamento
    }
    else {
        PORTBbits.RB0 = 1;          // Corta o MOSFET de carregamento
    }
    
}

void habilitarDescarregamentoDaBateria() {
   
    if(PORTBbits.RB0 == 1) {
        PORTBbits.RB1 = 0;          // Ativa o MOSFET de carga resistiva
    }
    else {
        PORTBbits.RB1 = 1;          // Corta o MOSFET de carga resistiva
    }
    
}
