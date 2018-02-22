/*
 * File:   aplicacao.c
 * Author: otavio
 *
 * Created on 11 de Janeiro de 2018, 14:06
 * Versão funcional 2.0
 */

/*
 * char resultadoDosTestes[0]: Polaridade
 * char resultadoDosTestes[1]: Tensao na bateria durante a carga
 * char resultadoDosTestes[2]: Corrente na bateria durante a carga
 * char resultadoDosTestes[3]: Tensao na bateria durante a descarga
 * char resultadoDosTestes[4]: Corrente na bateria durante a descarga
 * 
 */

#include <xc.h>
#include <pic18f2620.h>
#include "adc.h"
#include "configurationBits.h"
#include "uart.h" 
#include "timer0.h"
#include "interrupts.h"
#include "controleDaBateria.h"
#include "buzzer.h"
#include "analiseDeDados.h"

#define testeDePolaridade                        0
#define carregamentoHabilitado                   1
#define descarregamentoHabilitado                2
#define fimDeTeste                               3
#define reprovado                                0
#define aprovado                                 1
#define botao                               PORTBbits.RB2
#define ledAprovado                         PORTBbits.RB4
#define ledEmTeste                          PORTBbits.RB5
#define ledReprovado                        PORTBbits.RB6

void configureGpios();

char contador = 0;
char tempoEmSegundos = 0;
char statusDaBateria = testeDePolaridade;
float tensaoNaBateria[5] = {0,0,0,0,0};
float correnteNaBateria[5] = {0,0,0,0,0};
char resultadoDosTestes[5] = {0,0,0,0,0};

void main(void) {
    
    __delay_ms(1000);
    
    configureAdc();
    configureUart();
    configureGpios();
    configureTimer0();
    configureInterrupts();
    escreverStringUart("\n\n\n\n*** Jiga de testes da bateria EVO - FW V2.0 ***\n");
    escreverStringUart("\n*** Pressione o botao para iniciar os testes! ***\n");
    desabilitarTimer0();
    
    while(1){
        if(botao == 0) {
            __delay_ms(20);
            if(botao == 0) {
                while(botao == 0);
                sinalizacaoBeep();
                ledAprovado = 0;
                ledReprovado = 0;
                ledEmTeste = 1;
                escreverStringUart("\n*** Iniciando testes... ***\n\n");
                escreverStringUart("Teste de Polaridade Habilitado!");
                statusDaBateria = testeDePolaridade;
                tempoEmSegundos = 0;
                contador = 0;
                habilitarTimer0();
            }
        }
    }
    
}

void configureGpios() {
    TRISB = 0b00000100;
    PORTB = 0b00000000;
    
    desabilitarCarregamentoDaBateria();
    desabilitarDescarregamentoDaBateria();
}

void interrupt tratamento() {
    
    if((INTCONbits.TMR0IE == 1) && (INTCONbits.TMR0IF == 1)) {
        
        if(statusDaBateria == testeDePolaridade) {
            desabilitarDescarregamentoDaBateria();
            habilitarCarregamentoDaBateria();
            
            __delay_ms(500);
            
            tensaoNaBateria[tempoEmSegundos] = ((5.0 * lerAdcAn0()) / 1023);
            correnteNaBateria[tempoEmSegundos] = ((((5.0 * lerAdcAn1()) / 1023) - tensaoNaBateria[tempoEmSegundos]) / 0.47);
            
            escreverStringUart("\nCarregamento: Habilitado | Descarregamento: Desabilitado | Teste: ");
            escreverStringUart(converterIntParaString(tempoEmSegundos + 1));
            escreverStringUart(" | ");
            escreverStringUart("Tensao: ");
            escreverStringUart(converterFloatParaString(tensaoNaBateria[tempoEmSegundos]));
            escreverStringUart(" V | ");
            escreverStringUart("Corrente: ");
            escreverStringUart(converterFloatParaString(correnteNaBateria[tempoEmSegundos]));
            escreverStringUart(" A");
            
            tempoEmSegundos++;
            
            if(tempoEmSegundos == 5) {
                contador = 0;
                tempoEmSegundos = 0;
                float tensaoMedia = ((tensaoNaBateria[0] + tensaoNaBateria[1] + tensaoNaBateria[2] + tensaoNaBateria[3] + tensaoNaBateria[4]) / 5);
                float correnteMedia = ((correnteNaBateria[0] + correnteNaBateria[1] + correnteNaBateria[2] + correnteNaBateria[3] + correnteNaBateria[4]) /5);
                
                escreverStringUart("\nTensao media: ");
                escreverStringUart(converterFloatParaString(tensaoMedia));
                escreverStringUart(" V");
                
                escreverStringUart("\nCorrente media: ");
                escreverStringUart(converterFloatParaString(correnteMedia));
                escreverStringUart(" A");
                
                if((tensaoMedia >= 2) && (correnteMedia >= 0.200)) {
                    resultadoDosTestes[0] = aprovado;
                    escreverStringUart("\nTeste de polaridade: OK");
                    escreverStringUart("\n\nTeste de Carregamento habilitado!");
                    statusDaBateria = carregamentoHabilitado;
                }
                else {
                    resultadoDosTestes[0] = reprovado;
                    escreverStringUart("\nTeste de Polaridade: NOK");
                    escreverStringUart("\n\nTeste de Carregamento nao habilitado!\n");
                    statusDaBateria = fimDeTeste;
                }
            }
            
        }
        else if(statusDaBateria == carregamentoHabilitado) {
            desabilitarDescarregamentoDaBateria();
            habilitarCarregamentoDaBateria();
            tempoEmSegundos++;
            
            if(tempoEmSegundos == 8 || tempoEmSegundos == 16 || tempoEmSegundos == 24 || tempoEmSegundos == 32 || tempoEmSegundos == 40) {
                tensaoNaBateria[contador] = ((5.0 * lerAdcAn0()) / 1023);
                correnteNaBateria[contador] = ((((5.0 * lerAdcAn1()) / 1023) - tensaoNaBateria[contador]) / 0.47);

                escreverStringUart("\nCarregamento: Habilitado | Descarregamento: Desabilitado | Teste: ");
                escreverStringUart(converterIntParaString(contador + 1));
                escreverStringUart(" | ");
                escreverStringUart("Tensao: ");
                escreverStringUart(converterFloatParaString(tensaoNaBateria[contador]));
                escreverStringUart(" V | ");
                escreverStringUart("Corrente: ");
                escreverStringUart(converterFloatParaString(correnteNaBateria[contador]));
                escreverStringUart(" A");

                contador++;

                if(tempoEmSegundos == 40) {
                    contador = 0;
                    tempoEmSegundos = 0;

                    float tensaoMedia = (( tensaoNaBateria[0] + tensaoNaBateria[1] + tensaoNaBateria[2] + tensaoNaBateria[3] + tensaoNaBateria[4] ) / 5);
                         
                    escreverStringUart("\nTensao Media: ");
                    escreverStringUart(converterFloatParaString(tensaoMedia));
                    escreverStringUart(" V\n");
                    if( (tensaoMedia >= 3.2) && (tensaoMedia <= 4.3) ) {   // AJUSTAR VALORES
                        resultadoDosTestes[1] = aprovado;
                        escreverStringUart("Teste de Tensao no Carregamento: OK\n");
                    }
                    else {
                        resultadoDosTestes[1] = reprovado;
                        escreverStringUart("Teste de Tensao no Carregamento: NOK\n");
                    }

                    float correnteMedia = (( correnteNaBateria[0] + correnteNaBateria[1] + correnteNaBateria[2] + correnteNaBateria[3] + correnteNaBateria[4] ) / 5);
                    escreverStringUart("Corrente Media: ");
                    escreverStringUart(converterFloatParaString(correnteMedia));
                    escreverStringUart(" A\n");
                    if( (correnteMedia >= 0.200) && (correnteMedia <= 0.350) ) { // AJUSTAR VALORES
                        resultadoDosTestes[2] = aprovado;
                        escreverStringUart("Teste de Corrente no Carregamento: OK\n");
                    }
                    else {
                        resultadoDosTestes[2] = reprovado;
                        escreverStringUart("Teste de Corrente no Carregamento: NOK\n");
                    }

                    if((resultadoDosTestes[1] == aprovado) && resultadoDosTestes[2] == aprovado) {
                        statusDaBateria = descarregamentoHabilitado;
                        escreverStringUart("\nTeste de Descarregamento Habilitado!");
                    }
                    else {
                        escreverStringUart("\nTeste de Descarregamento nao Habilitado!\n");
                        statusDaBateria = fimDeTeste;
                    }

               }

            }
        }
        else if(statusDaBateria == descarregamentoHabilitado) {
            desabilitarCarregamentoDaBateria();
            habilitarDescarregamentoDaBateria();
            tempoEmSegundos++;
            
            tensaoNaBateria[contador] = ((5.0 * lerAdcAn0()) / 1023);
            correnteNaBateria[contador] = (((tensaoNaBateria[contador]) - ((5.0 * lerAdcAn1()) / 1023)) / 0.47);
            
            escreverStringUart("\nCarregamento: Desabilitado | Descarregamento: Habilitado | Teste: ");
            escreverStringUart(converterIntParaString(contador + 1));
            escreverStringUart(" | ");
            escreverStringUart("Tensao: ");
            escreverStringUart(converterFloatParaString(tensaoNaBateria[contador]));
            escreverStringUart(" V | ");
            escreverStringUart("Corrente: ");
            escreverStringUart(converterFloatParaString(correnteNaBateria[contador]));
            escreverStringUart(" A");
            
            contador++;
            
            if(tempoEmSegundos == 5) {
                contador = 0;
                tempoEmSegundos = 0;
                
                float tensaoMedia = (( tensaoNaBateria[0] + tensaoNaBateria[1] + tensaoNaBateria[2] + tensaoNaBateria[3] + tensaoNaBateria[4] ) / 5);
                int indexTensaoMinima = indexDoMenorValor(tensaoNaBateria,5);
                
                escreverStringUart("\nTensao Media: ");
                escreverStringUart(converterFloatParaString(tensaoMedia));
                escreverStringUart(" V");
                escreverStringUart("\nMenor tensao medida: ");
                escreverStringUart(converterFloatParaString(tensaoNaBateria[indexTensaoMinima]));
                escreverStringUart(" V\n");
                if((tensaoMedia >= 3.0) && (tensaoNaBateria[indexTensaoMinima] >= 2.7)) {   // AJUSTAR VALORES
                    escreverStringUart("Teste de Tensao no Descarregamento: OK\n");
                    resultadoDosTestes[3] = aprovado;
                }
                else {
                    escreverStringUart("Teste de Tensao no Descarregamento: NOK\n");
                    resultadoDosTestes[3] = reprovado;
                }

                float correnteMedia = (( correnteNaBateria[0] + correnteNaBateria[1] + correnteNaBateria[2] + correnteNaBateria[3] + correnteNaBateria[4] ) / 5);
                escreverStringUart("Corrente Media: ");
                escreverStringUart(converterFloatParaString(correnteMedia));
                escreverStringUart(" A\n");
                if( (correnteMedia >= 0.050) && (correnteMedia <= 0.150) ) { // AJUSTAR VALORES
                    resultadoDosTestes[4] = aprovado;
                    escreverStringUart("Teste de Corrente no Descarregamento: OK\n");
                }
                else {
                    resultadoDosTestes[4] = reprovado;
                    escreverStringUart("Teste de Corrente no Descarregamento: NOK\n");
                }

                statusDaBateria = fimDeTeste;
            }
        }
        else if(statusDaBateria == fimDeTeste) {
            desabilitarTimer0();
            desabilitarCarregamentoDaBateria();
            desabilitarDescarregamentoDaBateria();
            statusDaBateria = testeDePolaridade;
            if((resultadoDosTestes[0] + resultadoDosTestes[1] + resultadoDosTestes[2] + resultadoDosTestes[3] + resultadoDosTestes[4]) == 5) {
                escreverStringUart("\n*** BATERIA APROVADA! ***\n");
                escreverStringUart("\n*** Testes finalizados ***\n");
                escreverStringUart("\n\n\n\n*** Pressione o botao para iniciar os testes! ***\n");
                ledAprovado = 1;
                ledEmTeste = 0;
                ledReprovado = 0;
                sinalizacaoAprovada();
            }
            else {
                escreverStringUart("\n*** BATERIA REPROVADA! ***\n");
                escreverStringUart("\n*** Testes finalizados ***\n");
                escreverStringUart("\n\n\n\n*** Pressione o botao para iniciar os testes! ***\n");
                ledAprovado = 0;
                ledEmTeste = 0;
                ledReprovado = 1 ;
                sinalizacaoFalha();
            }
        }
        
        TMR0L = 0xDB;
        TMR0H = 0x0B;
        INTCONbits.TMR0IF = 0;
        
    }
    
}
