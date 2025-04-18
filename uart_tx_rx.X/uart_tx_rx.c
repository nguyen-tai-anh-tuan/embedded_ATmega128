/*
 * File: uart_tx_rx.c
 * Author: Lenovo
 * Created on April 11, 2025, 1:53 PM
 */
#define F_CPU 8000000UL  // Th?ch anh 8MHz
#include <avr/io.h>
#include <util/delay.h>

// G?i m?t k� t? qua UART
void uart_char_tx(unsigned char chr) {
    while (!(UCSR0A & (1 << UDRE0)));  // Ch? khi b? ??m tr?ng
    UDR0 = chr;
}

// Nh?n m?t k� t? qua UART
unsigned char uart_char_rx(void) {
    while (!(UCSR0A & (1 << RXC0)));  // Ch? ??n khi c� d? li?u
    return UDR0;
}

void uart_init(void) {
    UBRR0H = 0;
    UBRR0L = 51;  // Baud rate 9600 v?i F_CPU = 8MHz
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8-bit, 1 stop bit, kh�ng parity
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // B?t c? TX v� RX
}

int main(void) {
    uart_init();
    
    while (1) {
        uart_char_tx('A');  // G?i k� t? 'A'
        _delay_ms(1000);    // Ch? 1 gi�y
        
        // Nh?n ph?n h?i t? "NHAN"
        unsigned char received = uart_char_rx();
        // C� th? th�m logic x? l� ph?n h?i n?u c?n
    }

    return 0;
}