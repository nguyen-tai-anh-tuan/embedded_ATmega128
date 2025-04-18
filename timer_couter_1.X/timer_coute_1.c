/*
 * File:   timer_counter.c
 * Author: Lenovo
 *
 * Created on April 4, 2025, 2:36 PM
 */

#include <avr/io.h>
#include <util/delay.h>

void setupFastPWMNonInverted() {
    // C?u hình Timer0 ? ch? ?? PWM nhanh không ??o ng??c (Hình 1 và Hình 3)
    TCCR0 = 0;  // Xóa thanh ghi tr??c khi c?u hình
    TCCR0 |= (1 << WGM01) | (1 << WGM00);  // Ch? ?? PWM nhanh (Fast PWM Mode)
    TCCR0 |= (1 << COM01);                  // Ch? ?? không ??o ng??c (Non-Inverted)
    TCCR0 |= (1 << CS00);                   // B? chia t?n = 1 (CS00 = 1)
}

void setupFastPWMInverted() {
    // C?u hình Timer0 ? ch? ?? PWM nhanh ??o ng??c (Hình 2)
    TCCR0 = 0;  // Xóa thanh ghi tr??c khi c?u hình
    TCCR0 |= (1 << WGM01) | (1 << WGM00);  // Ch? ?? PWM nhanh (Fast PWM Mode)
    TCCR0 |= (1 << COM01) | (1 << COM00);  // Ch? ?? ??o ng??c (Inverted)
    TCCR0 |= (1 << CS00);                   // B? chia t?n = 1
}

void setupPhaseCorrectPWMInverted() {
    // C?u hình Timer0 ? ch? ?? PWM ?úng pha ??o ng??c (Hình 4)
    TCCR0 = 0;  // Xóa thanh ghi tr??c khi c?u hình
    TCCR0 |= (1 << WGM00);                  // Ch? ?? PWM ?úng pha (Phase Correct PWM)
    TCCR0 |= (1 << COM01) | (1 << COM00);  // Ch? ?? ??o ng??c (Inverted)
    TCCR0 |= (1 << CS00);                   // B? chia t?n = 1
}

int main(void) {
    // C?u hình PB4 (OC0) là ??u ra
    DDRB |= (1 << PB4);  // OC0 pin (PB4) là output
    PORTB = 0;           // Ban ??u t?t

    while (1) {
        // --- Hình 1: PWM nhanh không ??o ng??c ---
        setupFastPWMNonInverted();
        OCR0 = 128;  // Chu k? làm vi?c ban ??u = 50%
        for (uint8_t i = 0; i < 50; i++) {  // Ch?y trong 5 giây (50 * 100ms)
            _delay_ms(100);  // Ch? 100ms
            OCR0 += 5;       // T?ng chu k? làm vi?c
            if (OCR0 > 255) OCR0 = 0;  // ??t l?i n?u v??t quá 255
        }

        // --- Hình 2: PWM nhanh ??o ng??c ---
        setupFastPWMInverted();
        OCR0 = 128;  // Chu k? làm vi?c ban ??u = 50%
        for (uint8_t i = 0; i < 50; i++) {  // Ch?y trong 5 giây
            _delay_ms(100);
            OCR0 += 5;       // T?ng chu k? làm vi?c
            if (OCR0 > 255) OCR0 = 0;
        }

        // --- Hình 3: PWM nhanh không ??o ng??c v?i OCR0 = 0xFF ---
        setupFastPWMNonInverted();
        OCR0 = 255;  // Chu k? làm vi?c g?n 100%
        _delay_ms(5000);  // Ch?y trong 5 giây, không thay ??i chu k? làm vi?c

        // --- Hình 4: PWM ?úng pha ??o ng??c ---
        setupPhaseCorrectPWMInverted();
        OCR0 = 128;  // Chu k? làm vi?c ban ??u = 50%
        for (uint8_t i = 0; i < 50; i++) {  // Ch?y trong 5 giây
            _delay_ms(100);
            OCR0 += 5;       // T?ng chu k? làm vi?c
            if (OCR0 > 255) OCR0 = 0;
        }
    }

    return 0;
}