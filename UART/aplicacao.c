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
#include <string.h>
#include <ctype.h>

void configureGpio();
void tratarComando(char comando[]);

char contadorBuffer = 0;

void main(void) {
    
    configureUart();
    configureGpio();
    
    escreverStringUart("Inicio...\n\n");
    while(1);
    
}

void configureGpio() {
    TRISAbits.TRISA0 = 0;
    PORTAbits.RA0 = 0;
}

void tratarComando(char comando[]) {
    
    if((strcmp(comando, "LIGAR")) == 0) {
        escreverStringUart("Led ligado!\n\n");
        PORTAbits.RA0 = 1;
    }
    else if((strcmp(comando, "DESLIGAR")) == 0) {
        escreverStringUart("Led desligado!\n\n");
        PORTAbits.RA0 = 0;
    }
    else {
        escreverStringUart("Comando invalido!\n\n");
    }
    
}

/**
 * Função para tratamento das interrupções
 */
void interrupt tratamento() {
    
    char bufferSerial[50];
    
    if(PIE1bits.RCIE && PIR1bits.RCIF) {
        
        if(RCREG != 13) {
            escreverCharUart(RCREG);
            bufferSerial[contadorBuffer] = toupper(RCREG);
            contadorBuffer++;
            if(contadorBuffer == 50) {
                limparBuffer(bufferSerial);
                contadorBuffer = 0;
                escreverStringUart("\n\n\n!!! ESTOURO DE BUFFER !!!\n\n\n");
            }
        }
        else if(RCREG == 13) {
            escreverStringUart("\n");
            verificarBuffer(bufferSerial,contadorBuffer);
            contadorBuffer = 0;
            tratarComando(bufferSerial);
            limparBuffer(bufferSerial); 
        }
        
        PIR1bits.RCIF = 0;
    }
    
}