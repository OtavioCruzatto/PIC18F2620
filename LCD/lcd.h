/*
 * File:   lcd.h
 * Author: Otavio
 * Versão: 1.0
 *
 * ARQUIVO CONTENDO FUNCOES NECESSÁRIAS PARA ESCRITA EM UM DISPLAY DE LCD DE
 * 16X4 (PODENDO SER AJUSTADO PARA 20X4)
 *
 */

//**********************************************************//
//*                                                        *//
//*                                                        *//
//*                PIC18F2620                              *//
//*             -----------------                          *//
//*            |                 |                         *//
//*            |                 |                         *//
//*            |                 |                         *//
//*            |                 |                         *//
//*            |              RB4|--> DATA 4 (D4)          *//
//*            |              RB5|--> DATA 5 (D5)          *//
//*            |              RB6|--> DATA 6 (D6)          *//
//*            |              RB7|--> DATA 7 (D7)          *//
//*            |              RB3|--> ENABLE (E)           *//
//*            |              RB2|--> REGISTER SELECT (RS) *//
//*            |                 |-->                      *//
//*            |                 |-->                      *//
//*            |                 |                         *//
//*                                                        *//
//*                                                        *//
//*                                                        *//
//**********************************************************//

void enviarComandoLcd(char comando);
void inicializarLCD();
void enviarCaracterLcd(char caracter);
void escreverStringLcd(char linha , char *texto, int valor);
void configurarGpiosLcd();
void escreverStringLcdAuxiliar(char *str);
void limparLcd();

#include <pic18f2620.h>
#include <stdio.h>

#define LCD_DATA	PORTB
#define LCD_EN		PORTBbits.RB3
#define LCD_RS		PORTBbits.RB2

void configurarGpiosLcd() {
    ADCON1bits.PCFG = 0b1111;   // Todos os pinos digitais
	TRISB = 0X00;               // Todo PORTB como saída
    PORTB = 0X00;               // Todo PORTB em nível lógico baixo
}

void inicializarLCD() {
    __delay_ms(1);
    enviarComandoLcd(0X28); // 4 bits, 2 linhas ou mais, caracter 5x7
    __delay_ms(1);
    enviarComandoLcd(0X14); // Cursor se move, deslocamento à direita
    __delay_ms(1);
    enviarComandoLcd(0X01); // Limpa o display e seta o endereço para zero
    __delay_ms(2);
    enviarComandoLcd(0X02); // Seta o endereço para zero, retorna o cursor para posição inicial
    __delay_ms(2);
    enviarComandoLcd(0X04); // 
    __delay_ms(1);
    enviarComandoLcd(0X0C); // Liga o display, desabilita o cursor 
    __delay_ms(1);
}

void enviarComandoLcd(char comando) {
    
    // ENVIA OS 4 BITS MAIS SIGNIFICATIVOS DO comando PARA A PARTE ALTA DO PORTB (RB7,RB6,RB5,RB4)
    LCD_DATA = (comando & 0xF0);
    LCD_RS = 0;
    LCD_EN = 1;
    __delay_us(5);
    LCD_EN = 0;
    __delay_us(5);

    // ENVIA OS 4 BITS MENOS SIGNIFICATIVOS DO comando PARA A PARTE ALTA DO PORTB (RB7,RB6,RB5,RB4)
    LCD_DATA = ((comando << 4) & 0xF0);
    LCD_RS = 0;
    LCD_EN = 1;
    __delay_us(5);
    LCD_EN = 0;
    __delay_us(5);
}

void enviarCaracterLcd(char caracter) {
    
    // ENVIA OS 4 BITS MAIS SIGNIFICATIVOS DO caracter PARA A PARTE ALTA DO PORTB (RB7,RB6,RB5,RB4)
    LCD_DATA = (caracter & 0xF0);
    LCD_RS = 1;
    LCD_EN = 1;
    __delay_us(50);
    LCD_EN = 0;
    __delay_us(50);

    // ENVIA OS 4 BITS MENOS SIGNIFICATIVOS DO caracter PARA A PARTE ALTA DO PORTB (RB7,RB6,RB5,RB4)
    LCD_DATA = ((caracter << 4) & 0xF0);   
    LCD_RS = 1;
    LCD_EN = 1;
    __delay_us(50);
    LCD_EN = 0;
    __delay_us(50);
}

void escreverStringLcdAuxiliar(char *str) {
    while(*str){
    	enviarCaracterLcd(*str);
    	str++;
    }
}

void escreverStringLcd(char linha , char *texto, int valor) {
    
	if(linha == 1) {
        // EXEMPLO: putstring_lcd(1, TESTE: %d, 1);
		enviarComandoLcd(0X80);
		char str[18];
		sprintf(str, texto, valor);
		escreverStringLcdAuxiliar(str);
	}
	else if(linha == 2) {
        // EXEMPLO: putstring_lcd(2, TESTE: %d, 2);
		enviarComandoLcd(0XC0);
		char str[18];
		sprintf(str, texto, valor);
		escreverStringLcdAuxiliar(str);
	}
	else if(linha == 3) {
        // EXEMPLO: putstring_lcd(3, TESTE: %d, 3);
		enviarComandoLcd(0X90);
		char str[18];
		sprintf(str, texto, valor);
		escreverStringLcdAuxiliar(str);
	}
	else if(linha == 4) {
        // EXEMPLO: putstring_lcd(4, TESTE: %d, 4);
		enviarComandoLcd(0XD0);
		char str[18];
		sprintf(str, texto, valor);
		escreverStringLcdAuxiliar(str);
	}
    
}

void limparLcd() {
	enviarComandoLcd(0X01);
	__delay_ms(2);
}
