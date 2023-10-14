#include <avr/io.h>



void uart_SetBaudRate(int baud)


{
int ubrr = ((F_CPU)/(baud*16UL)-1);
UBRR0H = (ubrr>>8);                                      // shifting the register right by 8 bits
UBRR0L = ubrr;                                           // setting the baud rate bits [7 : 0]
}



void uart_init (int baud)
{
uart_SetBaudRate(baud);
UCSR0B|= (1<<TXEN0)|(1<<RXEN0);                           // enabling receiver and transmitter
UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);                         // function to create the 8bit data format
}


                                                          // function to receive data

unsigned char uart_recieve (void)
{
while(!uart_recieve_ready ());                            // checking if data is received
return UDR0;                                              // return 8-bit data
}

void UART_rstring(char *str, unsigned char maxLength) {
unsigned char i = 0;
while (i < (maxLength - 1)) {
while (!(UCSR0A & (1 << RXC0)));                          // checking if data is received
unsigned char receivedByte = UDR0;                        // reading the received data


                                                          // check for end of string


if (receivedByte == '\0' || receivedByte == '\n' || receivedByte == '\r') {
break;
}
    
str[i++] = receivedByte;
}
str[i] = '\0';                                            // null-terminating the string
}




int uart_recieve_ready (void)
{
int flag = ((UCSR0A) & (1<<RXC0)) != 0;                   // check if the data has been recieved
return flag;
}



                                                         // function to send data


void uart_transmit (unsigned char data)
{ 
int udr_empty;
do 
{
udr_empty = (UCSR0A & (1<<UDRE0)) != 0;
}while(!udr_empty);                                       // waiting for the register to be empty
UDR0 = data;                                              // send data to the register
}


void uart_tarray(char* data, int size){
for(int i = 0; i < size; i++){
while(!(UCSR0A & (1<<UDRE0)));
UDR0 = (int) data[i];
}
}
