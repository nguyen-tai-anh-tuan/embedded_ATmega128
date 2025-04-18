/*
 * File:   BAI_1_BUTTON_7SEG.c
 * Author: Lenovo
 *
 * Created on March 28, 2025, 4:28 PM
 */


#include <xc.h>
#include <util/delay.h>

void main(void) 
{
    //DDRC=0b11111111;
    DDRC = 0xff;
    unsigned char i = 0, dem = 1;
    //unsigned char bangma[3]={0b00111111,0b00000110,0b01001111};
    unsigned char bangma[16]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};   //7seg cathode
    while(1)
    {
        //DDRD=DDRD&(0b11111110);
        DDRD = DDRD & (0xfe);
    
        if((PIND & (0x01)) == 0)      //0b00000001
        {
            _delay_ms(20);
            if((PIND & (0x01)) == 0)    //0b00000001
            {           
                PORTC = bangma[i];
                i++;
                if(i == 16) i = 0;
               _delay_ms(500);
            }
        }
        else 
        {
            DDRC = 0xff;  //0b11111111
            PORTC = PORTC;
        }
    }
}
