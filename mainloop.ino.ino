#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / 16 / BAUD) - 1)


volatile uint8_t switchState = 0;                   // setting switch state


int main(void) {
                                                    // initialising UART

uart_init(BAUD);

                                                    // setting switch pin2 as input

DDRD &= ~(1 << DDD2);

                                                    // setting LED pin13 as output

DDRB |= (1 << DDB5);



char setString[] = "led_on";
char receivedString[50];                            // storing received string
unsigned char maxStringLength = sizeof(receivedString);

char button_statuson[19] = "Button is pressed\n";
char button_statusoff[20] = "Button is released\n";

while (1) {


                                                   // reading the switch state

if (PIND & (1 << PIND2)) {

                                                   // switch is not released

if (switchState == 1) {

uart_tarray(button_statuson, 20);                  // send 'Button is released' when the button is no longer pressed

switchState = 0;
}

} else {
                                                   // switch is pressed

if (switchState == 0) {
uart_tarray(button_statusoff, 19);                 // send 'Button is pressed' when the button is pressed
switchState = 1;
}

}    

                                                   // receiving a string from UART

UART_rstring(receivedString, maxStringLength);
int res = strcmp(receivedString, setString);

if (res == 0) {
PORTB |= (1 << PORTB5);                            // turning the LED on 
} else {
PORTB &= ~(1 << PORTB5);                           // turning the LED off
}
        

_delay_ms(1000);                                   // delaying for 1 second to avoid spamming the UART and corrupting the experiment 
}


return 0;
}
