#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_PIN 5  // Pin 13 on Arduino Uno (mapped to pin 5)
#define BAUD 9600
#define F_CPU 16000000UL
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void uart_init(void) {
    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Put data into buffer, sends the data
    UDR0 = data;
}

int main(void) {
    // Initialize UART
    uart_init();
    
    // Set pin 13 (digital pin 5) as output
    DDRB |= (1 << LED_PIN);
    
    while (1) {
        // Turn LED on
        PORTB |= (1 << LED_PIN);
        uart_transmit('H');  // Send 'H' when LED turns on
        
        // Wait for 1 second
        _delay_ms(1000);
        
        // Turn LED off
        PORTB &= ~(1 << LED_PIN);
        uart_transmit('L');  // Send 'L' when LED turns off
        
        // Wait for 1 second
        _delay_ms(1000);
    }
    
    return 0;
}
