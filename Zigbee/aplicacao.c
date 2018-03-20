/*
 * File:   aplicacao.c
 * Author: otavio
 *
 * Created on 16 de Março de 2018, 17:05
 */

#include <xc.h>
#include "configurationBits.h"
#include "uart.h"
#include "timer0.h"
#include "interrupts.h"
#include <pic18f2620.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ENVIO_DE_COMANDO_NAO_LIBERADO   0
#define ENVIO_DE_COMANDO_LIBERADO       1
#define CONSULTA                        2
#define CONFIGURACAO                    3

#define COMANDO_AT_PADRAO                       "AT\r"
#define SETAR_BAUDRATE_1200BPS                  "ATBD0\r"
#define SETAR_BAUDRATE_2400BPS                  "ATBD1\r"
#define SETAR_BAUDRATE_4800BPS                  "ATBD2\r"
#define SETAR_BAUDRATE_9600BPS                  "ATBD3\r"
#define SETAR_BAUDRATE_19200BPS                 "ATBD4\r"
#define SETAR_BAUDRATE_38400BPS                 "ATBD5\r"
#define SETAR_BAUDRATE_57600BPS                 "ATBD6\r"
#define SETAR_BAUDRATE_115200BPS                "ATBD7\r"
#define LER_BAUDRATE_DO_ZIGBEE                  "ATBD\r"
#define LER_PARTE_ALTA_DO_ID_DE_DESTINO         "ATDH\r"
#define LER_PARTE_BAIXA_DO_ID_DE_DESTINO        "ATDL\r"
#define SETAR_PARTE_ALTA_DO_ID_DE_DESTINO       "ATDH"
#define SETAR_PARTE_BAIXA_DO_ID_DE_DESTINO      "ATDL"
#define LER_PARTE_ALTA_DO_ID_DO_ZIGBEE          "ATSH\r"
#define LER_PARTE_BAIXA_DO_ID_DO_ZIGBEE         "ATSL\r"
#define LER_PANID_DO_ZIGBEE                     "ATID\r"
#define SETAR_PANID_DO_ZIGBEE                   "ATID"
#define LER_STACK_PROFILE_DO_ZIGBEE             "ATZS\r"
#define SETAR_STACK_PROFILE_DO_ZIGBEE           "ATZS"
#define LER_A_PARIDADE_DO_ZIGBEE                "ATNB\r"
#define SETAR_PARIDADE_DO_ZIGBEE                "ATNB"
#define LER_A_QTD_DE_STOP_BITS                  "ATSB\r"
#define SETAR_A_QTD_DE_STOP_BITS                "ATSB"
#define LER_O_STATUS_DE_DIO7                    "ATD7\r"
#define SETAR_O_VALOR_DE_DIO7                   "ATD7"
#define LER_O_STATUS_DE_DIO6                    "ATD6\r"
#define SETAR_O_VALOR_DE_DIO6                   "ATD6"
#define LER_A_TENSAO_DE_ALIMENTACAO             "AT%V\r"
#define SAIR_DO_MODO_DE_COMANDO                 "ATCN\r"
#define SALVAR_CONFIGURACOES_DO_ZIGBEE          "ATWR\r"
#define RETORNAR_O_ZIGBEE_AO_PADRAO_DE_FABRICA  "ATRE\r"
#define ENTRAR_EM_MODO_DE_COMANDO               "+++"

char statusDeEnvioDeComando = ENVIO_DE_COMANDO_LIBERADO;
char tipoDeComando = CONSULTA;
char contadorBuffer = 0;
char bufferSerial[50];

void configurarGPIO();
void entrarEmModoDeComando();
void setarParteAltaDoIdDeDestinoZigbee(char parteAlta[]);
void setarParteBaixaDoIdDeDestinoZigbee(char parteBaixa[]);
void sairDoModoDeComando();
void setarPanIdDoZigbee(char panId[]);
void salvarAsConfiguracoesDoZigbee();
void setarModoDeFabricaNoZigbee();

void main(void) {
    configureUart();
    configureTimer0();
    configureInterrupts();
    configurarGPIO();
    
    __delay_ms(3000);
    
    entrarEmModoDeComando();
    setarParteAltaDoIdDeDestinoZigbee("0013A200");
    setarParteBaixaDoIdDeDestinoZigbee("98765432");
    sairDoModoDeComando();
    
    while(1);
        
}

void entrarEmModoDeComando() {
    if(statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO) {
        __delay_ms(500);
        statusDeEnvioDeComando == ENVIO_DE_COMANDO_NAO_LIBERADO;
        escreverStringUart(ENTRAR_EM_MODO_DE_COMANDO);
        __delay_ms(2500);
    
        if((strncmp(bufferSerial,"OK",2)) == 0) {
            statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO;
        }
        
        limparBuffer(bufferSerial);
        contadorBuffer = 0;
    }
}

void setarParteAltaDoIdDeDestinoZigbee(char parteAlta[]) {
    if(statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO) {
        __delay_ms(500);
        statusDeEnvioDeComando == ENVIO_DE_COMANDO_NAO_LIBERADO;
        escreverStringUart(SETAR_PARTE_ALTA_DO_ID_DE_DESTINO);
        escreverStringUart(parteAlta);
        escreverStringUart("\r");
        __delay_ms(1000);

        if((strncmp(bufferSerial,"OK",2)) == 0) {
            statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO;
        }

        limparBuffer(bufferSerial);
        contadorBuffer = 0;
    }
}

void setarParteBaixaDoIdDeDestinoZigbee(char parteBaixa[]) {
    if(statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO) {
        __delay_ms(500);
        statusDeEnvioDeComando == ENVIO_DE_COMANDO_NAO_LIBERADO;
        escreverStringUart(SETAR_PARTE_BAIXA_DO_ID_DE_DESTINO);
        escreverStringUart(parteBaixa);
        escreverStringUart("\r");
        __delay_ms(1000);

        if((strncmp(bufferSerial,"OK",2)) == 0) {
            statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO;
        }

        limparBuffer(bufferSerial);
        contadorBuffer = 0;
    }
}

void sairDoModoDeComando() {
    if(statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO) {
        __delay_ms(500);
        statusDeEnvioDeComando == ENVIO_DE_COMANDO_NAO_LIBERADO;
        escreverStringUart(SAIR_DO_MODO_DE_COMANDO);
        __delay_ms(1000);

        if((strncmp(bufferSerial,"OK",2)) == 0) {
            statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO;
        }

        limparBuffer(bufferSerial);
        contadorBuffer = 0;
    }
}

void setarPanIdDoZigbee(char panId[]) {
    if(statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO) {
        __delay_ms(500);
        statusDeEnvioDeComando == ENVIO_DE_COMANDO_NAO_LIBERADO;
        escreverStringUart(SETAR_PANID_DO_ZIGBEE);
        escreverStringUart(panId);
        escreverStringUart("\r");
        __delay_ms(1000);

        if((strncmp(bufferSerial,"OK",2)) == 0) {
            statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO;
        }

        limparBuffer(bufferSerial);
        contadorBuffer = 0;
    }
}

void salvarAsConfiguracoesDoZigbee() {
    if(statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO) {
        __delay_ms(500);
        statusDeEnvioDeComando == ENVIO_DE_COMANDO_NAO_LIBERADO;
        escreverStringUart(SALVAR_CONFIGURACOES_DO_ZIGBEE);
        __delay_ms(3000);

        if((strncmp(bufferSerial,"OK",2)) == 0) {
            statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO;
        }

        limparBuffer(bufferSerial);
        contadorBuffer = 0;
    }
}

void setarModoDeFabricaNoZigbee() {
    if(statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO) {
        __delay_ms(500);
        statusDeEnvioDeComando == ENVIO_DE_COMANDO_NAO_LIBERADO;
        escreverStringUart(RETORNAR_O_ZIGBEE_AO_PADRAO_DE_FABRICA);
        __delay_ms(3000);

        if((strncmp(bufferSerial,"OK",2)) == 0) {
            statusDeEnvioDeComando == ENVIO_DE_COMANDO_LIBERADO;
        }

        limparBuffer(bufferSerial);
        contadorBuffer = 0;
    }
}

void configurarGPIO() {
    ADCON1bits.PCFG = 0b1111;
    CMCONbits.CM = 0b111;
    TRISAbits.TRISA0 = 0;
    PORTAbits.RA0 = 0;
}

/**
 * Função para tratamento das interrupções
 */
void interrupt tratamento() {
    
    if(PIE1bits.RCIE && PIR1bits.RCIF) {
        
        bufferSerial[contadorBuffer] = toupper(RCREG);
        contadorBuffer++;
        
        if(contadorBuffer == 50) {
            limparBuffer(bufferSerial);
            contadorBuffer = 0;
        }
        
        PIR1bits.RCIF = 0;
    }
    
}
