/*
 * File:   timer_counter.c
 * Author: Lenovo
 *
 * Created on April 4, 2025, 2:36 PM
 */

#include <avr/io.h>
#include <util/delay.h>

void setupFastPWMNonInverted() {
    // C?u h�nh Timer0 ? ch? ?? PWM nhanh kh�ng ??o ng??c (H�nh 1 v� H�nh 3)
    TCCR0 = 0;  // X�a thanh ghi tr??c khi c?u h�nh
    TCCR0 |= (1 << WGM01) | (1 << WGM00);  // Ch? ?? PWM nhanh (Fast PWM Mode)
    TCCR0 |= (1 << COM01);                  // Ch? ?? kh�ng ??o ng??c (Non-Inverted)
    TCCR0 |= (1 << CS00);                   // B? chia t?n = 1 (CS00 = 1)
}

void setupFastPWMInverted() {
    // C?u h�nh Timer0 ? ch? ?? PWM nhanh ??o ng??c (H�nh 2)
    TCCR0 = 0;  // X�a thanh ghi tr??c khi c?u h�nh
    TCCR0 |= (1 << WGM01) | (1 << WGM00);  // Ch? ?? PWM nhanh (Fast PWM Mode)
    TCCR0 |= (1 << COM01) | (1 << COM00);  // Ch? ?? ??o ng??c (Inverted)
    TCCR0 |= (1 << CS00);                   // B? chia t?n = 1
}

void setupPhaseCorrectPWMInverted() {
    // C?u h�nh Timer0 ? ch? ?? PWM ?�ng pha ??o ng??c (H�nh 4)
    TCCR0 = 0;  // X�a thanh ghi tr??c khi c?u h�nh
    TCCR0 |= (1 << WGM00);                  // Ch? ?? PWM ?�ng pha (Phase Correct PWM)
    TCCR0 |= (1 << COM01) | (1 << COM00);  // Ch? ?? ??o ng??c (Inverted)
    TCCR0 |= (1 << CS00);                   // B? chia t?n = 1
}

int main(void) {
    // C?u h�nh PB4 (OC0) l� ??u ra
    DDRB |= (1 << PB4);  // OC0 pin (PB4) l� output
    PORTB = 0;           // Ban ??u t?t

    while (1) {
        // --- H�nh 1: PWM nhanh kh�ng ??o ng??c ---
        setupFastPWMNonInverted();
        OCR0 = 128;  // Chu k? l�m vi?c ban ??u = 50%
        for (uint8_t i = 0; i < 50; i++) {  // Ch?y trong 5 gi�y (50 * 100ms)
            _delay_ms(100);  // Ch? 100ms
            OCR0 += 5;       // T?ng chu k? l�m vi?c
            if (OCR0 > 255) OCR0 = 0;  // ??t l?i n?u v??t qu� 255
        }

        // --- H�nh 2: PWM nhanh ??o ng??c ---
        setupFastPWMInverted();
        OCR0 = 128;  // Chu k? l�m vi?c ban ??u = 50%
        for (uint8_t i = 0; i < 50; i++) {  // Ch?y trong 5 gi�y
            _delay_ms(100);
            OCR0 += 5;       // T?ng chu k? l�m vi?c
            if (OCR0 > 255) OCR0 = 0;
        }

        // --- H�nh 3: PWM nhanh kh�ng ??o ng??c v?i OCR0 = 0xFF ---
        setupFastPWMNonInverted();
        OCR0 = 255;  // Chu k? l�m vi?c g?n 100%
        _delay_ms(5000);  // Ch?y trong 5 gi�y, kh�ng thay ??i chu k? l�m vi?c

        // --- H�nh 4: PWM ?�ng pha ??o ng??c ---
        setupPhaseCorrectPWMInverted();
        OCR0 = 128;  // Chu k? l�m vi?c ban ??u = 50%
        for (uint8_t i = 0; i < 50; i++) {  // Ch?y trong 5 gi�y
            _delay_ms(100);
            OCR0 += 5;       // T?ng chu k? l�m vi?c
            if (OCR0 > 255) OCR0 = 0;
        }
    }

    return 0;
}