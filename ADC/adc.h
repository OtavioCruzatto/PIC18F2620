
int lerADC();
void configureAdc();

int lerADC() {
    int valorLido = 0;                  // Declara um inteiro que guardar� o valor do ADC
    ADCON0bits.GO_nDONE = 1;            // Inicia a convers�o
    while(PIR1bits.ADIF == 0);          // Aguarda o fim da convers�o
    valorLido = ((ADRESH<<8) | ADRESL); // Desloca os dois bits mais significativos em 8 posi��es para a esquerda e soma com os 8 bits menos significativos
    PIR1bits.ADIF = 0;                  // Limpa a flag de interrup��o do ADC
    return valorLido;                   // Retorna o valor lido do ADC
}

void configureAdc() {
    ADCON0bits.CHS = 0b0000;    // Seleciona o canal AN0
    ADCON0bits.GO_nDONE = 0;    // Sem convers�o
    ADCON0bits.ADON = 1;        // Habilita o m�dulo de convers�o
    ADCON1bits.VCFG1 = 0;       // Vref- = GND = VSS
    ADCON1bits.VCFG0 = 0;       // Vref+ = VCC = VDD
    ADCON1bits.PCFG = 0b1110;   // Somente o pino do RA0 como entrada anal�gica
    ADCON2bits.ADFM = 1;        // Justifica os registradores do ADC � direita
    ADCON2bits.ACQT = 0b001;    // 2 Tad
    ADCON2bits.ADCS = 0b100;    // Fonte de clock: Fosc/4
    __delay_ms(10);
}