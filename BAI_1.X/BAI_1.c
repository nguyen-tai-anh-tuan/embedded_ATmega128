/*
 * File:   BAI_1.c
 * Author: Lenovo
 *
 * Created on March 28, 2025, 3:05 PM
 */


#include <xc.h>
#include <util/delay.h>

uint8_t i;

void main(void) 
{
    DDRA = 0xff;  //d�ng ?? thi?t l?p input v� output cho portA v?i: 0000: l� ?? ??c; 1111: l� ?? ghi
    //PORTA = PORTA | 0b00001111; 
    //PORTA = 0x01;
    while (1)
    {
        PORTA = 0x01;
        for(i=0; i<7; i++)
        {
            _delay_ms(200);
            PORTA = (PORTA << 1)  ;   
        }
        
        for(i=0; i<7; i++)
        {
            _delay_ms(500);
            PORTA = (PORTA >> 1)  ;   
        }
        
    }
    //return;
}
