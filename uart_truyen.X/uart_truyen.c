#define F_CPU 8000000UL  // Th?ch anh 8MHz
#include <avr/io.h>
#include <util/delay.h>

// Ch??ng tr�nh con ph�t d? li?u UART
void uart_char_tx(unsigned char chr) {
    while (!(UCSR0A & (1 << UDRE0)));  // Ch? ??n khi b? ??m tr?ng (UDRE0 = 1)
    UDR0 = chr;  // Ghi k� t? v�o thanh ghi UDR0 ?? g?i
}

void uart_init(void) {
    // ??t t?c ?? baud 9600 v?i F_CPU = 8MHz
    // UBRR = (F_CPU / (16 * BAUD_RATE)) - 1 = (8,000,000 / (16 * 9600)) - 1 = 51
    UBRR0H = 0;
    UBRR0L = 51;

    // C?u h�nh khung truy?n: 8 data bits, 1 stop bit, kh�ng parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // K�ch ho?t b? truy?n UART
    UCSR0B = (1 << TXEN0);
}

int main(void) {
    uart_init();  // Kh?i t?o UART

    while (1) {
        uart_char_tx('A');  // G?i k� t? 'A' (ASCII 65)
        _delay_ms(2000);    // Ch? 1 gi�y tr??c khi g?i k� t? ti?p theo
    }

    return 0;
}