/*
 * File:   timer_couter.c
 * Author: Lenovo
 *
 * Created on April 4, 2025, 2:36 PM
 */


#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // C?u h�nh PB4 (OC0) l� ??u ra
    DDRB |= (1 << PB4);  // OC0 pin (PB4) l� output
    PORTB = 0;           // Ban ??u t?t

    // C?u h�nh Timer0 ? ch? ?? Fast PWM
    TCCR0 = 0;  // X�a thanh ghi tr??c khi c?u h�nh
    TCCR0 |= (1 << WGM01) | (1 << WGM00);  // Fast PWM Mode (Mode 3)
    TCCR0 |= (1 << COM01);                  // Non-Inverting Mode (COM01=1, COM00=0)
    TCCR0 |= (1 << CS01);                   // Prescaler = 8

    OCR0 = 128;  // Duty cycle = 50% (128/256)

    while (1) {
        // Thay ??i duty cycle n?u c?n
        _delay_ms(1000);  // Ch? 1 gi�y
        OCR0 += 10;       // T?ng duty cycle
        if (OCR0 > 255) OCR0 = 0;  // Reset v? 0 n?u v??t qu� 255
    }

    return 0;
}