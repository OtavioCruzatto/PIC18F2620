/*
 * File:   aplicativo.c
 * Author: otavio
 *
 * Created on 29 de Dezembro de 2017, 13:51
 */

#include <xc.h>
#include <pic18f2620.h>
#include "configurationBits.h"
#include <stdio.h>
#include "uart.h"
#include "adc.h"

void main(void) {
    
    configureUart();
    configureAdc();
    
    int valorLidoDoAdc = 0;
    float tensaoNoAdc = 0;
    
    while(1) {
        valorLidoDoAdc = lerADC();
        tensaoNoAdc = ((5.14 * valorLidoDoAdc) / 1023);
        
        escreverStringUart("Tensao: ");
        escreverStringUart(converterFloatParaString(tensaoNoAdc));
        escreverStringUart(" Volts\n");
        
        __delay_ms(1000);
    }
    
}

