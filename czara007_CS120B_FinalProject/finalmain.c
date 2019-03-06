/* Name and email: Cindy Zaragoza czara007@ucr.edu

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <CS120B_Labs/czara007_CS120B_FinalProject/bit.h>
#include <CS120B_Labs/czara007_CS120B_FinalProject/io.c>
#include <CS120B_Labs/czara007_CS120B_FinalProject/io.h>
#include <CS120B_Labs/czara007_CS120B_FinalProject/timer.h>

void set_PWM(double frequency) {//targets pb6 not pb3
	// Keeps track of the currently set frequency
  // Will only update the registers when the frequency
  // changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
	
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
	
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
	
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
		}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

//////////////////////////////////////////////////////////////
//Gane Scoring
unsigned char level1 = 0;
unsigned char level2 = 0;
unsigned char level3 = 0;

unsigned char score=0;
unsigned char total_score = 0;
unsigned char select_level=0;
unsigned char level0= 1;
unsigned char end_level = 0;

unsigned char array[8] = {0b01110, 0b01110, 0b01110, 0b00100, 0b10101, 0b00100, 0b01010, 0b01010};// 

//DISPLAY
const unsigned char* Welcome_string = "Welcome, Simon says Hi";
unsigned char* Menu_string = "1. Begin game  2. Level 3. Reset/Scores";
unsigned char* Begin_string = "Begin";
unsigned char* win_string = "Congrats you won";
unsigned char* Error_string = "Sorry wrong entry ";

//LED Matrix 
unsigned char BA_top = 0XFF;

unsigned char B_bottom = 0x0F;
unsigned char B_top_left = 0X0F;
unsigned char B_top_right = 0X0F;

unsigned char B_bottom_left = 0xF0; // bottom left
unsigned char B_bottom_right = 0XF0; // bottom right

unsigned char A_top_left = 0X0F;
unsigned char A_top_right = 0XF0;

unsigned char A_bottom_left = 0X0F;
unsigned char A_bottom_right = 0XF0;

unsigned char button1 = 0; // buttons that operate the menu selections
unsigned char reset_button = 0;
unsigned char i;

//

/////////////////////////////////////////////////////////////////
enum Menu_States{SM_Start, Menu, Begin, Level, Reset}State;

void Menu_Fct(){
    level1 = 0;
    level2 = 0;
    level3 = 0;
    joystick_button = ~PIND & 0X80; //PIND at D3
    buttom1 = ~PIND & 0X10; //PIND at D4 
    reset_buttom = ~PIND & 0X02; //PIND at D5
    
    switch(State){
      case SM_Start:
        State = Menu;
      
      case Menu:
        if(button1){
          State = Menu;
        }
        else if(joystick_button && !end_level){
          State = Begin;
        }
        else if( reset_button && !end_level){
          
        }
  
  }
}



int main(){
DDRA = 0X00; PORTA = 0XFF;
DDRB = 0XFF; PORTB = 0X00;
DDRC = 0XFF; PORTC = 0X00; //LCD data lines
DDRD = 0XFF; PORTD = 0X00; //LCD control lines

// Initializes the LCD display
LCD_init();   

// Starting at position 1 on the LCD screen, writes Hello World
LCD_DisplayString(1, Welcome_string);// will display the welcome string 

set_PWM();


  while(1){}
  
}

      
