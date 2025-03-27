#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Set pin 7 (PORTD7) as input
    DDRD &= ~(1 << PD7);  // Set pin 7 (PORTD7) as input
    
    // Set pin 13 (PORTB5) as output for onboard LED
    DDRB |= (1 << PB5);   // Set pin 13 (PORTB5) as output


    while(1) {
        // Check the state of pin 7 (PD7)
        if (PIND & (1 << PD7)) {
            // If pin 7 is HIGH (5V), turn on the LED (pin 13 HIGH)
            PORTB |= (1 << PB5); // Set pin 13 HIGH (turn LED on)
        } else {
            // If pin 7 is LOW (0V) or disconnected (pull-up keeps it LOW), turn off the LED
            PORTB &= ~(1 << PB5); // Set pin 13 LOW (turn LED off)
        }

        // Small delay to debounce and reduce CPU usage
    }

    return 0;
}
