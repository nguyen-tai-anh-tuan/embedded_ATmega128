/*
 * File: uart_nhan.c
 * Author: Lenovo
 * Created on April 11, 2025, 2:31 PM
 * Description: Code for the "NHAN" microcontroller (U2) in the Proteus schematic.
 *              Receives a character via UART from "TRUYEN", toggles LED on PC0, and sends the character back.
 */

#define F_CPU 8000000UL  // Define CPU frequency as 8 MHz (matches 8 MHz crystal in schematic)
#include <avr/io.h>
#include <avr/interrupt.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)  // Calculate UBRR value for baud rate 9600

// Function to initialize UART
void uart_init(void) {
    // Set baud rate to 9600 with F_CPU = 8 MHz
    // UBRR = (F_CPU / (16 * BAUD_RATE)) - 1 = (8,000,000 / (16 * 9600)) - 1 = 51
    UBRR0H = (BAUD_PRESCALE >> 8);  // High byte of UBRR
    UBRR0L = BAUD_PRESCALE;         // Low byte of UBRR

    // Configure UART frame: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // Enable UART transmitter (TX), receiver (RX), and RX interrupt
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);

    // Enable global interrupts
    sei();
}

// Function to send a character via UART
void uart_char_tx(unsigned char chr) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait until the transmit buffer is empty
    UDR0 = chr;  // Load character into the transmit buffer
}

// Interrupt Service Routine (ISR) for UART receive
ISR(USART0_RX_vect) {
    unsigned char data = UDR0;  // Read the received character from UDR0

    // If the received character is 'A', toggle the LED on PC0
    if (data == 'A') {
        PORTC ^= (1 << PORTC0);  // Toggle LED (on if off, off if on)
    }

    // Send the received character back to "TRUYEN" as a response
    uart_char_tx(data);
}

int main(void) {
    // Initialize UART
    uart_init();

    // Configure PC0 as output for the LED
    DDRC |= (1 << DDC0);
    PORTC &= ~(1 << PORTC0);  // Turn off LED initially

    while (1) {
        // Main loop does nothing, all processing is handled in the ISR
    }

    return 0;
}