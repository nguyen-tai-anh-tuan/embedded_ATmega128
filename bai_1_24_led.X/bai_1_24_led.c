/*
 * File:   BAI_1_24LED.c
 * Author: Lenovo 
 * Created on March 28, 2025, 4:08 PM
 */

#include <xc.h>
#include <util/delay.h>

// ??nh ngh?a các c?ng
#define LED_PORTC PORTC
#define LED_PORTB PORTB
#define LED_PORTA PORTA
#define LED_DDRC  DDRC
#define LED_DDRB  DDRB
#define LED_DDRA  DDRA

void main(void) 
{
    // C?u hình t?t c? các chân c?a PORTA, PORTB, PORTC làm output
    LED_DDRA = 0xFF;  // PORTA: 8 bit output (PA0 - PA7)
    LED_DDRB = 0xFF;  // PORTB: 8 bit output (PB0 - PB7)
    LED_DDRC = 0xFF;  // PORTC: 8 bit output (PC0 - PC7)

    while (1) 
    {
        // D?ch t? trái sang ph?i (PC0 -> PA7)
        uint32_t led_pattern = 0x000001;  // B?t ??u t? bit 0 (PC0)
        for (uint8_t i = 0; i < 24; i++) 
        {
            // Gán giá tr? cho t?ng c?ng
            LED_PORTC = (uint8_t)(led_pattern & 0xFF);        // 8 bit th?p (PC0-PC7)
            LED_PORTB = (uint8_t)((led_pattern >> 8) & 0xFF); // 8 bit gi?a (PB0-PB7)
            LED_PORTA = (uint8_t)((led_pattern >> 16) & 0xFF); // 8 bit cao (PA0-PA7)
            
            _delay_ms(200);  // ??i 200ms
            led_pattern <<= 1;  // D?ch trái 1 bit
        }

        // D?ch t? ph?i sang trái (PA7 -> PC0)
        led_pattern = 0x800000;  // B?t ??u t? bit 23 (PA7)
        for (uint8_t i = 0; i < 24; i++) 
        {
            // Gán giá tr? cho t?ng c?ng
            LED_PORTC = (uint8_t)(led_pattern & 0xFF);        // 8 bit th?p
            LED_PORTB = (uint8_t)((led_pattern >> 8) & 0xFF); // 8 bit gi?a
            LED_PORTA = (uint8_t)((led_pattern >> 16) & 0xFF); // 8 bit cao
            
            _delay_ms(200);  // ??i 200ms
            led_pattern >>= 1;  // D?ch ph?i 1 bit
        }
    }
}