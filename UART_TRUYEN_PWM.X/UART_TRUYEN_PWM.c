#define F_CPU 8000000UL  // Th?ch anh 8MHz
#include <avr/io.h>
#include <util/delay.h>

// Ch??ng tr�nh con ph�t d? li?u UART
void uart_char_tx(unsigned char chr) {
    while (!(UCSR0A & (1 << UDRE0)));  // Ch? ??n khi b? ??m tr?ng
    UDR0 = chr;
}

void uart_init(void) {
    UBRR0H = 0;
    UBRR0L = 51;  // Baud rate 9600 v?i F_CPU = 8MHz
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8-bit, 1 stop bit, kh�ng parity
    UCSR0B = (1 << TXEN0);  // B?t b? truy?n UART
}

int main(void) {
    uart_init();  // Kh?i t?o UART

    // C?u h�nh PD2 l�m input ?? ??c n�t nh?n
    DDRD &= ~(1 << DDD2);  // PD2 l� input
    PORTD |= (1 << PORTD2);  // K�ch ho?t pull-up tr�n PD2

    uint8_t speed_level = 0;  // C?p t?c ??: 0 (ch?m), 1 (trung b�nh), 2 (nhanh)
    uint8_t last_button_state = 1;  // Tr?ng th�i n�t nh?n tr??c ?� (1 = kh�ng nh?n)

    while (1) {
        // ??c tr?ng th�i n�t nh?n tr�n PD2
        uint8_t button_state = (PIND & (1 << PIND2)) >> PIND2;

        // Ph�t hi?n c?nh xu?ng (nh?n n�t)
        if (button_state == 0 && last_button_state == 1) {
            // T?ng c?p t?c ??
            speed_level = (speed_level + 1) % 3;  // Chuy?n ??i gi?a 0, 1, 2
            _delay_ms(50);  // Ch?ng d?i ph�m (debounce)
        }
        last_button_state = button_state;

        // G?i k� t? 'A'
        uart_char_tx('A');
        // G?i k� t? t?c ?? ('1', '2', '3')
        uart_char_tx('1' + speed_level);  // '1' (ch?m), '2' (trung b�nh), '3' (nhanh)

        // Ch?n th?i gian delay d?a tr�n speed_level
        switch (speed_level) {
            case 0:  // Ch?m: 1000ms
                _delay_ms(1000);
                break;
            case 1:  // Trung b�nh: 500ms
                _delay_ms(500);
                break;
            case 2:  // Nhanh: 200ms
                _delay_ms(200);
                break;
        }
    }

    return 0;
}