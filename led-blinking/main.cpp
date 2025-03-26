#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN 5  // Pin 13 on Arduino Uno (mapped to pin 5)

int main(void) {
    // Set pin 13 (digital pin 5) as output
    DDRB |= (1 << LED_PIN);
    
    while (1) {
        // Turn LED on
        PORTB |= (1 << LED_PIN);
        
        // Wait for 1 second
        _delay_ms(400);
        
        // Turn LED off
        PORTB &= ~(1 << LED_PIN);
        
        // Wait for 1 second
        _delay_ms(400);
    }
    
    return 0;
}
