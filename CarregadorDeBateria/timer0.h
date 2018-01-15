#include <pic18f2620.h>
#include <xc.h>

/*
 *                      Cálculos
 * 
 *               4 * (TMR0H | TMR0L) * Prescaler
 *        t(s) = ----------------------------------
 *                         Fosc
 * 
 * 
 *      Exemplo (com o código abaixo):
 * 
 *               4 * 62500 * 4 
 *       t(s) = ------------------- = 1s
 *                    1000000
 */

void configureTimer0();
void desabilitarTimer0();
void habilitarTimer0();

void configureTimer0() {
    T0CONbits.T08BIT = 0;   // Timer0 is configured as a 16-bit timer/counter
    T0CONbits.T0CS = 0;     // Internal instruction cycle clock (CLKO)
    T0CONbits.T0SE = 1;     // Increment on high-to-low transition on T0CKI pin
    T0CONbits.PSA = 0;      // Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.
    T0CONbits.T0PS = 0b001; // 1:4 Prescale value
    TMR0L = 0xDB;
    TMR0H = 0x0B;
    T0CONbits.TMR0ON = 1;   // Enables Timer0
}

void habilitarTimer0() {
    T0CONbits.TMR0ON = 1;   // Enables Timer0
}

void desabilitarTimer0() {
    T0CONbits.TMR0ON = 0;   // Enables Timer0
}