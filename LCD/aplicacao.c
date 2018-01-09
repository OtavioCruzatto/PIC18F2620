/*
 * File:   aplicacao.c
 * Author: otavio
 *
 * Created on 5 de Janeiro de 2018, 15:55
 */

#include <xc.h>
#include <pic18f2620.h>
#include "configurationBits.h"
#include "lcd.h"
#include "clock.h"

void main(void) {
    
    configurarClockInterno(8);
    configurarGpiosLcd();
    inicializarLCD();
    limparLcd();
    
    escreverStringLcd(1,"   Teste: %d", 0);
    escreverStringLcd(2,"   Teste: %d", 1);
    escreverStringLcd(3,"   Teste: %d", 2);
    escreverStringLcd(4,"   Teste: %d", 3);

    while(1);
    
}
