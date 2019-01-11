/*
 * czara007_hvenk001_Lab[2]_Part[1].c
 *
 * Created: 1/11/2019 2:26:04 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>

//unsigned char tempA, tempB;
int main(void)
{
   DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
   DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
   unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
   unsigned char tmpA1 = 0x00;
   unsigned char tmpA0 = 0x00; // Temporary variable to hold the value of A
   while(1)
   {
	   tmpA1 = PINA & 0x01;
	   tmpA0 = PINA & 0x02;
		if((tmpA1 ==0x01) | (tmpA0 == 0x02))
		{
			tmpB = 0x01;
		}
		
		else {
			tmpB = 0X00;
		}PORTB = tmpB;
   }
   return 0;

}

