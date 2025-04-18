/*
 * File:   ADC_VARIABLE.c
 * Author: Lenovo
 *
 * Created on April 18, 2025
 */

#define F_CPU 8000000UL  // Th?ch anh 8MHz
#include <avr/io.h>
#include <util/delay.h>

// H�m kh?i t?o ADC
void adc_init(void)
{
    ADMUX = (1 << REFS0); // AVCC l�m ?i?n �p tham chi?u, ch?n ADC0 (PF0)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // B?t ADC, prescaler = 64
}

// H�m ??c ADC
uint16_t read_adc(void)
{
    ADCSRA |= (1 << ADSC);          // B?t ??u chuy?n ??i
    while (ADCSRA & (1 << ADSC));   // Ch? chuy?n ??i xong
    return ADC;                     // Tr? v? gi� tr? 10-bit
}

// H�m kh?i t?o PWM tr�n Timer1 (cho motor tr�n PB5)
void init_pwm_motor(void)
{
    // C?u h�nh Timer1 ? ch? ?? Fast PWM, TOP = ICR1
    TCCR1A = (1 << COM1A1) | (1 << WGM11); // Clear OC1A on compare match, Fast PWM
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8
    ICR1 = 1000; // Chu k? PWM, t?n s? ~1kHz (F_CPU = 8MHz)
    DDRB |= (1 << PB5); // PB5 (OC1A) l� ng� ra PWM
}

// H�m t?o ?? tr? ??ng
void delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++) {
        _delay_ms(1); // ?? tr? 1ms c? ??nh
    }
}

// H�m ?i?u khi?n LED nh�y v?i t?n s? d?a tr�n speed_level
void display_leds(uint8_t speed_level)
{
    uint16_t blink_delay; // ??i t�n bi?n ?? tr�nh xung ??t v?i h�m delay_ms
    
    // Ch?n th?i gian delay d?a tr�n speed_level (t?n s? nh�y t?ng d?n)
    switch (speed_level)
    {
        case 0: // 0% - LED t?t
            PORTC = 0xFF; // T?t LED (active-low)
            return; // Kh�ng nh�y
        case 1: // 20% - Nh�y ch?m (~1Hz, 500ms b?t/t?t)
            blink_delay = 500;
            break;
        case 2: // 40% - Nh�y nhanh h?n (~2Hz, 250ms)
            blink_delay = 250;
            break;
        case 3: // 60% - Nh�y nhanh h?n (~3Hz, 167ms)
            blink_delay = 167;
            break;
        case 4: // 80% - Nh�y nhanh h?n (~4Hz, 125ms)
            blink_delay = 125;
            break;
        case 5: // 100% - Nh�y nhanh nh?t (~5Hz, 100ms)
            blink_delay = 100;
            break;
        default:
            blink_delay = 500;
            break;
    }
    
    // B?t/t?t LED ?? t?o hi?u ?ng nh�y
    PORTC = 0x00; // B?t LED (active-low)
    delay_ms(blink_delay);
    PORTC = 0xFF; // T?t LED
    delay_ms(blink_delay);
}

int main(void)
{
    // C?u h�nh PORTC l�m ng� ra cho LED
    DDRC = 0xFF;
    PORTC = 0xFF; // T?t t?t c? LED ban ??u (active-low)
    
    // C?u h�nh PF0 l�m ng� v�o cho ADC
    DDRF &= ~(1 << PF0);
    
    // Kh?i t?o ADC v� PWM
    adc_init();
    init_pwm_motor();
    
    uint8_t speed_level = 0; // M?c t?c ??: 0 (0%), 1 (20%), 2 (40%), 3 (60%), 4 (80%), 5 (100%)
    uint16_t last_adc_value = 0; // Gi� tr? ADC tr??c ?�
    
    while (1)
    {
        // ??c gi� tr? ADC t? bi?n tr?
        uint16_t adc_value = read_adc();
        
        // Ch? c?p nh?t speed_level n?u ADC thay ??i ?�ng k? (ch?ng nhi?u)
        if (adc_value < last_adc_value - 50 || adc_value > last_adc_value + 50)
        {
            // �nh x? ADC th�nh 6 m?c (bao g?m 0%)
            if (adc_value <= 100)
                speed_level = 0; // 0% - LED t?t, motor ch?m nh?t
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
        }
        
        // C?p nh?t PWM cho motor d?a tr�n speed_level
        switch (speed_level)
        {
            case 0: // 0% - Motor ch?m nh?t
                OCR1A = 200; // 20% duty cycle
                break;
            case 1: // 20%
                OCR1A = 200; // 20% duty cycle
                break;
            case 2: // 40%
                OCR1A = 400; // 40% duty cycle
                break;
            case 3: // 60%
                OCR1A = 600; // 60% duty cycle
                break;
            case 4: // 80%
                OCR1A = 800; // 80% duty cycle
                break;
            case 5: // 100%
                OCR1A = 1000; // 100% duty cycle
                break;
            default:
                OCR1A = 200;
                break;
        }
        
        // G?i display_leds ?? ?i?u khi?n LED nh�y
        display_leds(speed_level);
    }
    
    return 0;
}