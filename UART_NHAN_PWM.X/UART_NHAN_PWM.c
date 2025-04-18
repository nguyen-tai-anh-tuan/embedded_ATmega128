#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void uart_init(void) {
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    sei();
}

void uart_char_tx(unsigned char chr) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = chr;
}

volatile uint8_t speed_level = 0;  // C?p t?c ??: 0 (ch?m), 1 (trung bình), 2 (nhanh)
volatile uint8_t received_a = 0;  // C? ?? xác nh?n nh?n ???c 'A'

ISR(USART0_RX_vect) {
    unsigned char data = UDR0;
    if (data == 'A') {
        received_a = 1;  // ?ánh d?u ?ã nh?n ???c 'A'
    } else if (received_a && data >= '1' && data <= '3') {
        speed_level = data - '1';  // Chuy?n ký t? '1', '2', '3' thành 0, 1, 2
        received_a = 0;  // Reset c?
    }
    uart_char_tx(data);  // G?i l?i ký t? nh?n ???c
}

int main(void) {
    uart_init();
    
    // C?u hình PC0 làm output ?? ?i?u khi?n LED
    DDRC |= (1 << DDC0);
    PORTC &= ~(1 << PORTC0);  // T?t LED ban ??u

    while (1) {
        // Nh?p nháy LED theo t?c ??
        PORTC ^= (1 << PORTC0);  // ??o tr?ng thái LED
        switch (speed_level) {
            case 0:  // Ch?m: 500ms
                _delay_ms(500);
                break;
            case 1:  // Trung bình: 250ms
                _delay_ms(250);
                break;
            case 2:  // Nhanh: 100ms
                _delay_ms(100);
                break;
        }
    }

    return 0;
}