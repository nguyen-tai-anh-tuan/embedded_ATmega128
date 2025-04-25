/*
 * File:   PWM_ADC_UART_TRUYEN.c
 * Author: Lenovo
 *
 * Created on April 25, 2025, 10:17 AM
 */


/*
 * File:   PWM_ADC_TRUYEN.c
 * Author: Lenovo
 *
 * Created on April 21, 2025, 1:01 PM
 */

#define F_CPU 8000000UL  // Thach anh 8MHz
#include <avr/io.h>
#include <util/delay.h>

// Khoi tao PWM tren Timer1 (OC1A, PB5)
void init_pwm_timer1(void) {
    TCCR1A = (1 << COM1A1) | (1 << WGM11);  
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);  
    ICR1 = 2666;  // TOP value for ~3 kHz PWM
    OCR1A = 267;  // Duty cycle mac dinh 10%
    DDRB |= (1 << PB5);  // Chan OC1A (PB5) la output
}

// Khoi tao ADC
void init_adc(void) {
    ADMUX = (1 << REFS0);  // Vref = AVCC (5V)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Enable ADC, prescaler 128
}

// Doc gia tri ADC tu kenh 0 (PF0)
uint16_t read_adc(void) {
    ADMUX = (ADMUX & 0xF0) | 0x00;  // Chon kenh ADC0
    ADCSRA |= (1 << ADSC);  // Bat dau chuyen doi
    while (ADCSRA & (1 << ADSC));  // Cho chuyen doi hoan tat
    return ADC;
}

// Khoi tao UART0
void init_uart(void) {
    UBRR0H = 0;
    UBRR0L = 51;  // Baud rate 9600 voi F_CPU = 8MHz
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // Enable ca TX va RX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8-bit data, 1 stop bit
}

// Gui 1 byte qua UART
void uart_transmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));  // Cho den khi buffer trong
    UDR0 = data;  // Gui du lieu
}

int main(void) {
    // Khoi tao
    init_pwm_timer1();
    init_adc();
    init_uart();
    DDRB |= 0x1F;  // PB0-PB4 la output (LED), PB5 da duoc cau hinh cho PWM

    uint16_t adc_value;
    uint8_t level;

    while (1) {
        // Doc gia tri ADC tu bien tro
        adc_value = read_adc();

        // Chia 5 muc duty cycle
        if (adc_value <= 204) {
            OCR1A = 267;  // 10%
            level = 1;
            PORTB = (PORTB & 0xE0) | 0x01;  // LED PB0, giu nguyen PB5
        } else if (adc_value <= 409) {
            OCR1A = 800;  // 30%
            level = 2;
            PORTB = (PORTB & 0xE0) | 0x02;  // LED PB1, giu nguyen PB5
        } else if (adc_value <= 614) {
            OCR1A = 1334;  // 50%
            level = 3;
            PORTB = (PORTB & 0xE0) | 0x04;  // LED PB2, giu nguyen PB5
        } else if (adc_value <= 819) {
            OCR1A = 1867;  // 70%
            level = 4;
            PORTB = (PORTB & 0xE0) | 0x08;  // LED PB3, giu nguyen PB5
        } else {
            OCR1A = 2400;  // 90%
            level = 5;
            PORTB = (PORTB & 0xE0) | 0x10;  // LED PB4, giu nguyen PB5
        }

        // Gui muc qua UART
        uart_transmit(level);

        _delay_ms(500);  // Delay 500ms de dam bao NHAN co du thoi gian xu ly
    }

    return 0;
}
