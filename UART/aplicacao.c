/*
 * File:   aplicacao.c
 * Author: otavio
 *
 * Created on 29 de Dezembro de 2017, 10:11
 */


#include <xc.h>
#include <pic18f2620.h>
#include "configurationBits.h"
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

void configureGpio();

void main(void) {
    
    configureUart();
    configureGpio();
    
    while(1);
    
}

void configureGpio() {
    TRISAbits.TRISA0 = 0;
    PORTAbits.RA0 = 0;
}

/**
 * Função para tratamento das interrupções
 */
void interrupt tratamento() {
    
    if(PIE1bits.RCIE && PIR1bits.RCIF) {
        
        escreverCharUart(RCREG);
        escreverStringUart("\n");
        
        if(RCREG == '0') {
            escreverStringUart("Led desligado!\n\n");
            PORTAbits.RA0 = 0;
        }
        else if(RCREG == '1') {
            escreverStringUart("Led ligado!\n\n");
            PORTAbits.RA0 = 1;
        }
        
        PIR1bits.RCIF = 0;
    }
    
}