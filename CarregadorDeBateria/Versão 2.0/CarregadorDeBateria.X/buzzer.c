/*
 * File:   buzzer.c
 * Author: otavio
 *
 * Created on 23 de Janeiro de 2018, 16:38
 */


#include <xc.h>
#include "configurationBits.h"
#include "buzzer.h"

void sinalizacaoAprovada() {
    char repeticoes = 0;
    for(repeticoes = 0; repeticoes < 3; repeticoes++) {
        buzzer = 1;
        __delay_ms(50);
        buzzer = 0;
        __delay_ms(50);
    }
}

void sinalizacaoFalha() {
    buzzer = 1;
    __delay_ms(1000);
    buzzer = 0;
}

void sinalizacaoBeep() {
    buzzer = 1;
    __delay_ms(200);
    buzzer = 0;
}