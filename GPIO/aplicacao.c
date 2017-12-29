/*
 * File:   aplicacao.c
 * Author: otavio
 *
 * Created on 28 de Dezembro de 2017, 10:47
 */


#include <xc.h>
#include <pic18f2620.h>
#include "configurationBits.h"

#define _XTAL_FREQ 1000000

void main(void) {
    
    TRISAbits.TRISA0 = 0;
    PORTAbits.RA0 = 0;
    
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG0 = 1;
    
    CMCONbits.CM2 = 1;
    CMCONbits.CM1 = 1;
    CMCONbits.CM0 = 1;
    
    CVRCONbits.CVREN = 0;
    CVRCONbits.CVROE = 0;
    
    while(1) {
        PORTAbits.RA0 = 1;
        __delay_ms(500);
        PORTAbits.RA0 = 0;
        __delay_ms(500);
    }
}
