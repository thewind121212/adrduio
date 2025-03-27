#include <avr/io.h>
#include <util/delay.h>

#define LED1_PIN PD2  // Chân kết nối LED 1 (PB2 trên ATmega328P)
#define LED2_PIN PD3  // Chân kết nối LED 2 (PB3 trên ATmega328P)
#define LED3_PIN PD4  // Chân kết nối LED 3 (PB4 trên ATmega328P)


void setup() {
    DDRD |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN); // Set chân LED 1, 2, 3 là output
}

void onLed1() {
    PORTD |= (1 << LED1_PIN);  // Bật LED 1
    PORTD &= ~(1 << LED2_PIN); // Tắt LED 2
    PORTD &= ~(1 << LED3_PIN); // Tắt LED 3
}

void onLed2() {
    PORTD &= ~(1 << LED1_PIN); // Tắt LED 1
    PORTD |= (1 << LED2_PIN);  // Bật LED 2
    PORTD &= ~(1 << LED3_PIN); // Tắt LED 3
}

void onLed3() {
    PORTD &= ~(1 << LED1_PIN); // Tắt LED 1
    PORTD &= ~(1 << LED2_PIN); // Tắt LED 2
    PORTD |= (1 << LED3_PIN);  // Bật LED 3
}


void loop() {
    onLed1(); // Bật LED 1 ( Đèn sáng màu đỏ)
    _delay_ms(4000);
    onLed2(); // Bật LED 3 ( Đèn sáng màu xanh)
    _delay_ms(5000);
    onLed3();
    _delay_ms(2000);
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
    return 0; // Không bao giờ thực thi đến đây
}