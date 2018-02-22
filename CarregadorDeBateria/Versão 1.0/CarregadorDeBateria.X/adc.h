#include "configurationBits.h"

int lerAdcAn0();
int lerAdcAn1();
int lerAdcAn2();
void configureAdc();

int lerAdcAn0() {
    ADCON0bits.CHS = 0b0000;            // Seleciona o canal AN0
    ADCON0bits.ADON = 1;                // Liga o módulo de conversão
    int valorLido = 0;                  // Declara um inteiro que guardará o valor do ADC
    ADCON0bits.GO_nDONE = 1;            // Inicia a conversão
    while(PIR1bits.ADIF == 0);          // Aguarda o fim da conversão
    valorLido = ((ADRESH<<8) | ADRESL); // Desloca os dois bits mais significativos em 8 posições para a esquerda e soma com os 8 bits menos significativos
    PIR1bits.ADIF = 0;                  // Limpa a flag de interrupção do ADC
    ADCON0bits.ADON = 0;                // Desliga o módulo de conversão
    return valorLido;                   // Retorna o valor lido do ADC
}

int lerAdcAn1() {
    ADCON0bits.CHS = 0b0001;            // Seleciona o canal AN1
    ADCON0bits.ADON = 1;                // Liga o módulo de conversão
    int valorLido = 0;                  // Declara um inteiro que guardará o valor do ADC
    ADCON0bits.GO_nDONE = 1;            // Inicia a conversão
    while(PIR1bits.ADIF == 0);          // Aguarda o fim da conversão
    valorLido = ((ADRESH<<8) | ADRESL); // Desloca os dois bits mais significativos em 8 posições para a esquerda e soma com os 8 bits menos significativos
    PIR1bits.ADIF = 0;                  // Limpa a flag de interrupção do ADC
    ADCON0bits.ADON = 0;                // Desliga o módulo de conversão
    return valorLido;                   // Retorna o valor lido do ADC
}

int lerAdcAn2() {
    ADCON0bits.CHS = 0b0010;            // Seleciona o canal AN2
    ADCON0bits.ADON = 1;                // Liga o módulo de conversão
    int valorLido = 0;                  // Declara um inteiro que guardará o valor do ADC
    ADCON0bits.GO_nDONE = 1;            // Inicia a conversão
    while(PIR1bits.ADIF == 0);          // Aguarda o fim da conversão
    valorLido = ((ADRESH<<8) | ADRESL); // Desloca os dois bits mais significativos em 8 posições para a esquerda e soma com os 8 bits menos significativos
    PIR1bits.ADIF = 0;                  // Limpa a flag de interrupção do ADC
    ADCON0bits.ADON = 0;                // Desliga o módulo de conversão
    return valorLido;                   // Retorna o valor lido do ADC
}

void configureAdc() {
    ADCON0bits.GO_nDONE = 0;    // Sem conversão
    ADCON0bits.ADON = 0;        // Desliga o módulo de conversão
    ADCON1bits.VCFG1 = 0;       // Vref- = GND = VSS
    ADCON1bits.VCFG0 = 0;       // Vref+ = VCC = VDD
    ADCON1bits.PCFG = 0b1100;   // Pinos RA0, RA1 e RA2 como entradas analógicas
    ADCON2bits.ADFM = 1;        // Justifica os registradores do ADC à direita
    ADCON2bits.ACQT = 0b001;    // 2 Tad
    ADCON2bits.ADCS = 0b100;    // Fonte de clock: Fosc/4
    __delay_ms(10);
}