
#define buzzer  PORTBbits.RB3

void sinalizacaoBeep();
void sinalizacaoAprovada();
void sinalizacaoFalha();

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