#define F_CPU 8000000UL  // Th?ch anh 8MHz
#include <avr/io.h>
#include <util/delay.h>

// Ch??ng trình con phát d? li?u UART
void uart_char_tx(unsigned char chr) {
    while (!(UCSR0A & (1 << UDRE0)));  // Ch? ??n khi b? ??m tr?ng
    UDR0 = chr;
}

void uart_init(void) {
    UBRR0H = 0;
    UBRR0L = 51;  // Baud rate 9600 v?i F_CPU = 8MHz
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8-bit, 1 stop bit, không parity
    UCSR0B = (1 << TXEN0);  // B?t b? truy?n UART
}

int main(void) {
    uart_init();  // Kh?i t?o UART

    // C?u hình PD2 làm input ?? ??c nút nh?n
    DDRD &= ~(1 << DDD2);  // PD2 là input
    PORTD |= (1 << PORTD2);  // Kích ho?t pull-up trên PD2

    uint8_t speed_level = 0;  // C?p t?c ??: 0 (ch?m), 1 (trung bình), 2 (nhanh)
    uint8_t last_button_state = 1;  // Tr?ng thái nút nh?n tr??c ?ó (1 = không nh?n)

    while (1) {
        // ??c tr?ng thái nút nh?n trên PD2
        uint8_t button_state = (PIND & (1 << PIND2)) >> PIND2;

        // Phát hi?n c?nh xu?ng (nh?n nút)
        if (button_state == 0 && last_button_state == 1) {
            // T?ng c?p t?c ??
            speed_level = (speed_level + 1) % 3;  // Chuy?n ??i gi?a 0, 1, 2
            _delay_ms(50);  // Ch?ng d?i phím (debounce)
        }
        last_button_state = button_state;

        // G?i ký t? 'A'
        uart_char_tx('A');
        // G?i ký t? t?c ?? ('1', '2', '3')
        uart_char_tx('1' + speed_level);  // '1' (ch?m), '2' (trung bình), '3' (nhanh)

        // Ch?n th?i gian delay d?a trên speed_level
        switch (speed_level) {
            case 0:  // Ch?m: 1000ms
                _delay_ms(1000);
                break;
            case 1:  // Trung bình: 500ms
                _delay_ms(500);
                break;
            case 2:  // Nhanh: 200ms
                _delay_ms(200);
                break;
        }
    }

    return 0;
}