#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> // For random number generation

// Pin definitions:
#define DATAPIN_PORT PORTD
#define DATAPIN_PIN PD2
#define DATAPIN_DDR DDRD

#define CLOCKPIN_PORT PORTD
#define CLOCKPIN_PIN PD3
#define CLOCKPIN_DDR DDRD

#define LATCHPIN_PORT PORTD
#define LATCHPIN_PIN PD4
#define LATCHPIN_DDR DDRD

const uint16_t delayTime = 100; // Time (milliseconds) to pause between LEDs

// Global variable for the data we're sending to the shift register:
uint8_t data = 0;

void setup()
{
    // Set the three SPI pins to be outputs:
    DATAPIN_DDR |= (1 << DATAPIN_PIN);
    CLOCKPIN_DDR |= (1 << CLOCKPIN_PIN);
    LATCHPIN_DDR |= (1 << LATCHPIN_PIN);
}

void shiftWrite(uint8_t desiredPin, uint8_t desiredState)
{
    // This function lets you make the shift register outputs
    // HIGH or LOW in exactly the same way that you use digitalWrite().

    if (desiredState)
    {
        data |= (1 << desiredPin); // Set the desired bit to 1
    }
    else
    {
        data &= ~(1 << desiredPin); // Clear the desired bit to 0
    }

    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & (1 << (7 - i)))
        {
            DATAPIN_PORT |= (1 << DATAPIN_PIN); // Set data pin high
        }
        else
        {
            DATAPIN_PORT &= ~(1 << DATAPIN_PIN); // Set data pin low
        }

        // Clock the pin for pushing the data into the register
        CLOCKPIN_PORT &= ~(1 << CLOCKPIN_PIN); // Clock low
        CLOCKPIN_PORT |= (1 << CLOCKPIN_PIN);  // Clock high
    }

    // Toggle the latchPin to make "data" appear at the outputs
    LATCHPIN_PORT |= (1 << LATCHPIN_PIN);
    LATCHPIN_PORT &= ~(1 << LATCHPIN_PIN);
}

void oneAfterAnother()
{
    // This function will turn on all the LEDs, one-by-one,
    // and then turn them off all off, one-by-one.

    // Turn all the LEDs on one-by-one
    for (int8_t index = 0; index < 8; index++)
    {
        shiftWrite(index, 1);
        _delay_ms(delayTime);
    }

    _delay_ms(100); // Wait a bit
    // Turn all the LEDs off one-by-one
    for (int8_t index = 7; index >= 0; index--)
    {
        shiftWrite(index, 0);
        _delay_ms(delayTime);
    }

    _delay_ms(500); // Wait for 500ms before repeating the sequence
}

void oneOnAtTime()
{

    const uint16_t delayTime = 200;
    for (int8_t index = 0; index < 8; index++)
    {
        shiftWrite(index, 1); // Turn on the LED at the current index
        _delay_ms(delayTime); // Wait for a short time
        shiftWrite(index, 0); // Turn off the LED at the current index
    }
    _delay_ms(500); // Wait a bit
}

void pingPong() {
    const uint16_t delayTime = 200;
    for (int8_t index = 0; index < 8; index++)
    {
        shiftWrite(index, 1); 
        _delay_ms(delayTime); 
        shiftWrite(index, 0);
    }
    for (int8_t index = 6; index >= 1; index--)
    {
        shiftWrite(index, 1); 
        _delay_ms(delayTime); 
        shiftWrite(index, 0);
    }
}

int8_t curentRandomIndex = 0;

int8_t randomIndex() {
    const int8_t randomIndex = rand() % 8;
    return randomIndex;
}

void randomLed()
{
    int8_t randomIndexResult = randomIndex();


    while (randomIndexResult == curentRandomIndex)
    {
        randomIndexResult = randomIndex();
    }

    curentRandomIndex = randomIndexResult;    
    
    shiftWrite(curentRandomIndex, 1);
    _delay_ms(100);
    shiftWrite(curentRandomIndex, 0);

    _delay_ms(200);

}


void marquee()
{
    const uint16_t delayTime = 200;
    int8_t index = 0;

    for (int8_t i = 0; i < 3; i++)
    {
        shiftWrite(index, 1); 
        shiftWrite(index + 4, 1); 
        _delay_ms(delayTime); 
        shiftWrite(index, 0);
        shiftWrite(index + 4, 0); 
        index++;
    }
}

void binaryCount()
{
    const uint16_t delayTime = 500;
    for (int8_t i = 0; i < 256; i++)
    {
        shiftWrite(0, (i & 1));
        shiftWrite(1, (i & 2) >> 1);
        shiftWrite(2, (i & 4) >> 2);
        shiftWrite(3, (i & 8) >> 3);
        shiftWrite(4, (i & 16) >> 4);
        shiftWrite(5, (i & 32) >> 5);
        shiftWrite(6, (i & 64) >> 6);
        shiftWrite(7, (i & 128) >> 7);
        _delay_ms(delayTime);
    }
}



int main(void)
{
    setup(); // Setup the pins for output
    while (1)
    {
        randomLed();
    }
    return 0;
}
