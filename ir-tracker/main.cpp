#include <avr/io.h>
#include <stdio.h>
#include <string.h>


// Pin definitions:
#define PORT_IR_TRACKER PD2
#define PORT_LED_RED PB5


#define F_CPU 16000000UL

// Define the desired baud rate
#define BAUD 9600

// Calculate the UBRR value
// Formula for normal speed mode: UBRR = (F_CPU / (16 * BAUD)) - 1
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void UART_init(void) {
    // Set baud rate (put the high and low bytes into UBRRH and UBRRL)
    // UBRR_VALUE is a 16-bit value.
    // UBRRH is the high byte, UBRRL is the low byte.
    UBRR0H = (unsigned char)(UBRR_VALUE >> 8); // Shift right by 8 bits to get the high byte
    UBRR0L = (unsigned char)UBRR_VALUE;        // Cast to char to get the low byte

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    UCSR0C = (0 << UMSEL01) | (0 << UMSEL00) | // Asynchronous mode
             (0 << UPM01) | (0 << UPM00) |     // No parity
             (0 << USBS0) |                    // 1 stop bit
             (1 << UCSZ01) | (1 << UCSZ00);    // 8 data bits (UCSZ02 is in UCSR0B if needed for 9-bit)
            

}

void UART_transmit(unsigned char data) {
    // Wait for empty transmit buffer
    // UDRE0 flag is set when the transmit buffer is empty
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer, sends the data
    UDR0 = data;
}




void setup() {
    // Initialize UART
    UART_init();
    // Set the IR tracker pin as input
    DDRD &= ~(1 << PORT_IR_TRACKER);

    // Set the LED pin as output
    DDRB |= (1 << PORT_LED_RED);
}

uint8_t current_state = 0;
uint8_t count = 0;
// Buffer free dont need to static buffer
char message_buffer[30];

void loop() {
    // Read the IR tracker pin
    uint8_t ir_tracker_state = PIND & (1 << PORT_IR_TRACKER);

    // Check if the state has actually changed before processing/printing
    if (ir_tracker_state != current_state) {
        count++; // Increment count only on state change

        if (ir_tracker_state) { // Pin is HIGH (Assuming this means "Far" based on original code)
            PORTB &= ~(1 << PORT_LED_RED); // <<< Turn LED OFF (original logic was confusing)
            // Format the string into the buffer
            sprintf(message_buffer, "Far Count: %u\n", count);
        } else { // Pin is LOW (Assuming this means "Near")
            PORTB |= (1 << PORT_LED_RED); // <<< Turn LED ON (original logic was confusing)
            // Format the string into the buffer
            sprintf(message_buffer, "Near Count: %u\n", count);
        }

        // Transmit the formatted message
        for (uint8_t i = 0; message_buffer[i] != '\0'; i++) { // Loop until null terminator
            UART_transmit(message_buffer[i]);
        }

        current_state = ir_tracker_state; // Update the state *after* processing the change
    }
    // No delay here, loop runs continuously checking the pin
    // Add a small delay if needed, e.g., _delay_ms(10); requires <util/delay.h>
}


int main(void) {
    setup();
    while (1) {
        loop();
    }
    return 0;
}

