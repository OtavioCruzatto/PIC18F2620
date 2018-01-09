#include <xc.h>
#include <pic18f2620.h>

void configurarClockInterno(char freqEmMegaHertz);

void configurarClockInterno(char freqEmMegaHertz) {
    switch(freqEmMegaHertz) {
        case 1:
            OSCCONbits.IRCF = 100;
            __delay_ms(1000);
        break;
        
        case 8:
            OSCCONbits.IRCF = 111;
            __delay_ms(1000);
        break;
    }
}