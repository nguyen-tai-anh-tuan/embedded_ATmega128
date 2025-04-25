/*
 * File:   PWM_ADC_UART_NHAN.c
 * Author: Lenovo
 *
 * Created on April 25, 2025, 10:21 AM
 */


/*
 * File:   PWM_ADC_NHAN.c
 * Author: Lenovo
 *
 * Created on April 21, 2025, 1:01 PM
 */

#define F_CPU 8000000UL  // Thach anh 8MHz
#include <avr/io.h>

// Khoi tao PWM tren Timer1 (OC1A, PB5)
void init_pwm_timer1(void) {
    TCCR1A = (1 << COM1A1) | (1 << WGM11);  
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);  
    ICR1 = 2666;  // TOP value for ~3 kHz PWM
    OCR1A = 267;  // Duty cycle mac dinh 10%
    DDRB |= (1 << PB5);  // Chan OC1A (PB5) la output
}

// Khoi tao UART0
void init_uart(void) {
    UBRR0H = 0;
    UBRR0L = 51;  // Baud rate 9600 voi F_CPU = 8MHz
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable ca RX va TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8-bit data, 1 stop bit
}

// Nhan 1 byte qua UART
uint8_t uart_receive(void) {
    // Kiem tra co du lieu hay khong
    while (!(UCSR0A & (1 << RXC0))) {
        // Kiem tra loi
        if (UCSR0A & (1 << FE0)) {  // Frame Error
            UDR0;  // Doc bo de xoa loi
            return 0;  // Tra ve 0 neu co loi
        }
        if (UCSR0A & (1 << DOR0)) {  // Data OverRun
            UDR0;  // Doc bo de xoa loi
            return 0;  // Tra ve 0 neu co loi
        }
    }
    return UDR0;  // Tra ve du lieu
}

int main(void) {
    // Khoi tao
    init_pwm_timer1();
    init_uart();
    DDRB |= 0x1F;  // PB0-PB4 la output (LED), PB5 da duoc cau hinh cho PWM

    uint8_t level;

    while (1) {
        // Nhan muc tu TRUYEN qua UART
        level = uart_receive();

        // Dieu chinh PWM va hien thi muc tren LED
        if (level >= 1 && level <= 5) {
            switch (level) {
                case 1:
                    OCR1A = 267;  // 10%
                    PORTB = (PORTB & 0xE0) | 0x01;  // LED PB0, giu nguyen PB5
                    break;
                case 2:
                    OCR1A = 800;  // 30%
                    PORTB = (PORTB & 0xE0) | 0x02;  // LED PB1, giu nguyen PB5
                    break;
                case 3:
                    OCR1A = 1334;  // 50%
                    PORTB = (PORTB & 0xE0) | 0x04;  // LED PB2, giu nguyen PB5
                    break;
                case 4:
                    OCR1A = 1867;  // 70%
                    PORTB = (PORTB & 0xE0) | 0x08;  // LED PB3, giu nguyen PB5
                    break;
                case 5:
                    OCR1A = 2400;  // 90%
                    PORTB = (PORTB & 0xE0) | 0x10;  // LED PB4, giu nguyen PB5
                    break;
                default:
                    OCR1A = 267;  // Mac dinh 10%
                    PORTB = (PORTB & 0xE0);  // Tat LED PB0-PB4 neu loi
                    break;
            }
        } else {
            OCR1A = 267;  // Mac dinh 10%
            PORTB = (PORTB & 0xE0);  // Tat LED PB0-PB4 neu loi
        }
    }

    return 0;
}