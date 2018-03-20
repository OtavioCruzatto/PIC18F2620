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
void fecharBuffer(char string[], char contadorBuffer);
void limparBuffer(char string[]);

void configureUart() {
    
    /*
     *  Configuração dos pinos de RX e TX, exigido pelo datasheet
     */
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 1;
    
    TXSTAbits.CSRC = 0;  // Não importa para comunicação assíncrona
    TXSTAbits.TX9 = 0;   // Seleciona transmissão de 8 bits
    TXSTAbits.TXEN = 1;  // Transmissão: 1-Habilitado ; 0-Desabilitado
    TXSTAbits.SYNC = 0;  // Comunicação assíncrona
    TXSTAbits.SENDB = 0; // Sync Break transmission completed
    TXSTAbits.BRGH = 1;  // Velocidade do Baudrate: 1-Alta ; 0-Baixa
    
    RCSTAbits.SPEN = 1; // Habilita a porta serial
    RCSTAbits.RX9 = 0;  // Recepção de 8 bits
    RCSTAbits.SREN = 0; // Não importa para comunicação assíncrona
    RCSTAbits.CREN = 1; // Recepção: 1-Habilitado ; 0-Desabilitado
    RCSTAbits.ADDEN = 0; // Não importa para RX9=0
    
    BAUDCONbits.BRG16 = 1;  
    
    SPBRG = 25; // Baudrate: 9600
    
}

/**
 * Função para escrever um char na uart.
 * @param caracter
 */
void escreverCharUart(char caracter) {
    TXREG = caracter;
    while(TXSTAbits.TRMT == 0);
}

/**
 * Função para converter uma variável int em string
 * @param valorParaSerConvertido
 * @return 
 */
char converterIntParaString(int valorParaSerConvertido) {
    char string[7];
    sprintf(string, "%d", valorParaSerConvertido);
    return string;
}

/**
 * Função para converter uma variável float em string
 * @param valorParaSerConvertido
 * @return 
 */
char converterFloatParaString(float valorParaSerConvertido) {
    char string[10];
    sprintf(string, "%f", valorParaSerConvertido);
    return string;
}

/**
 * Função para converter uma string em um int
 * @param stringParaSerConvertida
 * @return 
 */
int converterStringParaInt(char stringParaSerConvertida) {
    int i = atoi(stringParaSerConvertida);
    return i;
}

/**
 * Função que retorna o tamanho da string passada como parâmetro.
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
 * Função para escrever uma string na uart.
 * @param string
 */
void escreverStringUart(char string[]) {
    int contador = 0;
    int tamanhoDaStr = tamanhoDaString(string);
    
    for(contador = 0; contador < tamanhoDaStr; contador++) {
        escreverCharUart(string[contador]);
    }
}

void fecharBuffer(char string[], char contadorBuffer) {
    
    if(string[contadorBuffer] != '\0'){
        string[contadorBuffer] = '\0';
    }
            
    contadorBuffer = 0;
    int i = 0;
    
    for(contadorBuffer = 0; contadorBuffer < tamanhoDaString(string); contadorBuffer++) {
        
        if(string[contadorBuffer] == '\n'){
            i = 1;
        }
        
        if(i == 1) {
            string[contadorBuffer] = string[contadorBuffer + 1];
        }
        
    }
}

void limparBuffer(char string[]) {
    
    int i = 0;
    for(i=0;i<50;i++){
        string[i] = 0;
    }
    
}
