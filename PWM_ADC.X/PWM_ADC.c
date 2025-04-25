/*
 * File:   PWM_ADC.c
 * Author: Lenovo
 *
 * Created on April 21, 2025, 1:01 PM
 */


#define F_CPU 8000000UL  // Th?ch anh 8MHz
#include <avr/io.h>

void init_pwm_timer1(void) {
    TCCR1A = (1 << COM1A1) | (1 << WGM11);  
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);  
    //ICR1 = 3999;  
    //OCR1A = 400;

    ICR1 = 2666;  // TOP value for 3 kHz PWM (333.25 us)
    OCR1A = 2400; // Duty cycle ~90% (300 us HIGH)
    DDRB |= (1 << PB5);  
}

int main(void) {
    init_pwm_timer1();

    while (1) {
    }

    return 0;
}