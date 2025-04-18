/*
 * File:   ADC_UART_NHAN.c
 * Author: Lenovo
 *
 * Created on April 18, 2025
 */

#define F_CPU 8000000UL  // Th?ch anh 8MHz
#include <avr/io.h>
#include <util/delay.h>

// Hàm kh?i t?o UART (baud rate 9600)
void uart_init(void)
{
    uint16_t ubrr = F_CPU / 16 / 9600 - 1; // UBRR cho 9600 baud
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << RXEN0); // Kích ho?t nh?n
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, 1 stop bit
}

// Hàm nh?n 1 byte qua UART (không ch?n)
uint8_t uart_receive_nonblocking(uint8_t *data)
{
    if (UCSR0A & (1 << RXC0)) { // Có d? li?u m?i
        *data = UDR0; // L?y d? li?u
        return 1; // Nh?n thành công
    }
    return 0; // Không có d? li?u
}

// Hàm t?o ?? tr? ??ng
void delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++) {
        _delay_ms(1); // ?? tr? 1ms c? ??nh
    }
}

// Hàm ?i?u khi?n LED nháy
void display_leds(uint8_t speed_level)
{
    uint16_t blink_delay;
    switch (speed_level)
    {
        case 0: // 0% - LED t?t
            PORTB = 0xFF; // T?t LED (active-low)
            return;
        case 1: // 20% - ~1Hz (500ms)
            blink_delay = 500;
            break;
        case 2: // 40% - ~2Hz (250ms)
            blink_delay = 250;
            break;
        case 3: // 60% - ~3Hz (167ms)
            blink_delay = 167;
            break;
        case 4: // 80% - ~4Hz (125ms)
            blink_delay = 125;
            break;
        case 5: // 100% - ~5Hz (100ms)
            blink_delay = 100;
            break;
        default:
            blink_delay = 500; // M?c ??nh nháy ~1Hz
            break;
    }
    PORTB = 0x00; // B?t LED
    delay_ms(blink_delay);
    PORTB = 0xFF; // T?t LED
    delay_ms(blink_delay);
}

int main(void)
{
    // C?u hình PORTB làm ngõ ra cho LED
    DDRB = 0xFF;
    PORTB = 0xFF; // T?t LED ban ??u
    
    // Kh?i t?o UART
    uart_init();
    
    uint8_t current_level = 1; // M?c ??nh nháy ~1Hz n?u không nh?n d? li?u
    
    while (1)
    {
        // Nh?n speed_level t? UART
        uint8_t new_level;
        if (uart_receive_nonblocking(&new_level)) {
            if (new_level <= 5) { // Ki?m tra d? li?u h?p l?
                current_level = new_level; // C?p nh?t level
            }
        }
        
        // ?i?u khi?n LED liên t?c
        display_leds(current_level);
    }
    
    return 0;
}