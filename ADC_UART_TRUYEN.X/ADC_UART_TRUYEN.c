/*
 * File:   ADC_UART_TRUYEN.c
 * Author: Lenovo
 *
 * Created on April 18, 2025
 */

#define F_CPU 8000000UL  // Th?ch anh 8MHz
#include <avr/io.h>
#include <util/delay.h>

// Hàm kh?i t?o ADC
void adc_init(void)
{
    ADMUX = (1 << REFS0); // AVCC làm ?i?n áp tham chi?u, ch?n ADC0 (PF0)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // B?t ADC, prescaler = 64
}

// Hàm ??c ADC
uint16_t read_adc(void)
{
    ADCSRA |= (1 << ADSC);          // B?t ??u chuy?n ??i
    while (ADCSRA & (1 << ADSC));   // Ch? chuy?n ??i xong
    return ADC;                     // Tr? v? giá tr? 10-bit
}

// Hàm kh?i t?o PWM trên Timer1 (cho motor trên PB5)
void init_pwm_motor(void)
{
    TCCR1A = (1 << COM1A1) | (1 << WGM11); // Clear OC1A, Fast PWM
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8
    ICR1 = 1000; // T?n s? ~1kHz
    DDRB |= (1 << PB5); // PB5 (OC1A) là ngõ ra PWM
}

// Hàm kh?i t?o UART (baud rate 9600)
void uart_init(void)
{
    uint16_t ubrr = F_CPU / 16 / 9600 - 1; // UBRR cho 9600 baud
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0); // Kích ho?t truy?n
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, 1 stop bit
}

// Hàm truy?n 1 byte qua UART
void uart_transmit(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0))); // Ch? buffer tr?ng
    UDR0 = data; // G?i d? li?u
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
            PORTC = 0xFF; // T?t LED (active-low)
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
            blink_delay = 500;
            break;
    }
    PORTC = 0x00; // B?t LED
    delay_ms(blink_delay);
    PORTC = 0xFF; // T?t LED
    delay_ms(blink_delay);
}

int main(void)
{
    // C?u hình PORTC làm ngõ ra cho LED
    DDRC = 0xFF;
    PORTC = 0xFF; // T?t LED ban ??u
    
    // C?u hình PF0 làm ngõ vào cho ADC
    DDRF &= ~(1 << PF0);
    
    // Kh?i t?o ADC, PWM, UART
    adc_init();
    init_pwm_motor();
    uart_init();
    
    uint8_t speed_level = 0;
    uint16_t last_adc_value = 0;
    uint16_t transmit_counter = 0; // ??m ?? g?i ??nh k?
    
    while (1)
    {
        // ??c ADC t? bi?n tr?
        uint16_t adc_value = read_adc();
        
        // C?p nh?t speed_level n?u ADC thay ??i
        if (adc_value < last_adc_value - 50 || adc_value > last_adc_value + 50)
        {
            if (adc_value <= 100)
                speed_level = 0; // 0%
            else if (adc_value <= 300)
                speed_level = 1; // 20%
            else if (adc_value <= 500)
                speed_level = 2; // 40%
            else if (adc_value <= 700)
                speed_level = 3; // 60%
            else if (adc_value <= 900)
                speed_level = 4; // 80%
            else
                speed_level = 5; // 100%
            
            last_adc_value = adc_value;
            uart_transmit(speed_level); // G?i ngay khi thay ??i
        }
        
        // G?i speed_level ??nh k? m?i ~100ms
        transmit_counter++;
        if (transmit_counter >= 100) { // ~100ms
            uart_transmit(speed_level);
            transmit_counter = 0;
        }
        
        // C?p nh?t PWM motor
        switch (speed_level)
        {
            case 0: OCR1A = 200; break; // 20%
            case 1: OCR1A = 200; break; // 20%
            case 2: OCR1A = 400; break; // 40%
            case 3: OCR1A = 600; break; // 60%
            case 4: OCR1A = 800; break; // 80%
            case 5: OCR1A = 1000; break; // 100%
            default: OCR1A = 200; break;
        }
        
        // ?i?u khi?n LED
        display_leds(speed_level);
    }
    
    return 0;
}