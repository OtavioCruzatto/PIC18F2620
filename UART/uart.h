#include <xc.h>
#include <pic18f2620.h>
#include "configurationBits.h"
#include <stdio.h>
#include <stdlib.h>

void configureUart();
void escreverCharUart(char caracter);
char converterIntParaString(int valorParaSerConvertido);
int converterStringParaInt(char stringParaSerConvertida);
int tamanhoDaString(char string[]);
void escreverStringUart(char string[]);
char converterFloatParaString(float valorParaSerConvertido);

void configureUart() {
    
    /*
     *  Configura��o dos pinos de RX e TX, exigido pelo datasheet
     */
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 1;
    
    TXSTAbits.CSRC = 0;  // N�o importa para comunica��o ass�ncrona
    TXSTAbits.TX9 = 0;   // Seleciona transmiss�o de 8 bits
    TXSTAbits.TXEN = 1;  // Transmiss�o: 1-Habilitado ; 0-Desabilitado
    TXSTAbits.SYNC = 0;  // Comunica��o ass�ncrona
    TXSTAbits.SENDB = 0; // Sync Break transmission completed
    TXSTAbits.BRGH = 1;  // Velocidade do Baudrate: 1-Alta ; 0-Baixa
    
    RCSTAbits.SPEN = 1; // Habilita a porta serial
    RCSTAbits.RX9 = 0;  // Recep��o de 8 bits
    RCSTAbits.SREN = 0; // N�o importa para comunica��o ass�ncrona
    RCSTAbits.CREN = 1; // Recep��o: 1-Habilitado ; 0-Desabilitado
    RCSTAbits.ADDEN = 0; // N�o importa para RX9=0
    
    BAUDCONbits.BRG16 = 1;  
    
    SPBRG = 25; // Baudrate: 9600
    
    INTCONbits.GIE = 1;     // Habilita as interrup��es globais
    INTCONbits.PEIE = 1;    // Habilita as interrup��es de perif�ricos
    PIE1bits.RCIE = 1;     // Habilita a interrup��o do RX da uart
    PIR1bits.RCIF = 0;     // Limpa a flag da interrup��o do RX
}

/**
 * Fun��o para escrever um char na uart.
 * @param caracter
 */
void escreverCharUart(char caracter) {
    TXREG = caracter;
    while(TXSTAbits.TRMT == 0);
}

/**
 * Fun��o para converter uma vari�vel int em string
 * @param valorParaSerConvertido
 * @return 
 */
char converterIntParaString(int valorParaSerConvertido) {
    char string[7];
    sprintf(string, "%d", valorParaSerConvertido);
    return string;
}

/**
 * Fun��o para converter uma vari�vel float em string
 * @param valorParaSerConvertido
 * @return 
 */
char converterFloatParaString(float valorParaSerConvertido) {
    char string[10];
    sprintf(string, "%f", valorParaSerConvertido);
    return string;
}

/**
 * Fun��o para converter uma string em um int
 * @param stringParaSerConvertida
 * @return 
 */
int converterStringParaInt(char stringParaSerConvertida) {
    int i = atoi(stringParaSerConvertida);
    return i;
}

/**
 * Fun��o que retorna o tamanho da string passada como par�metro.
 * @param string
 * @return 
 */
int tamanhoDaString(char string[]) {
    int lenght = 0;
    while(string[lenght] != '\0') {
        lenght++;
    }
    return lenght;
}

/**
 * Fun��o para escrever uma string na uart.
 * @param string
 */
void escreverStringUart(char string[]) {
    int contador = 0;
    int tamanhoDaStr = tamanhoDaString(string);
    
    for(contador = 0; contador < tamanhoDaStr; contador++) {
        escreverCharUart(string[contador]);
    }
}
