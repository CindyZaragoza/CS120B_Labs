/* Name and email: Cindy Zaragoza czara007@ucr.edu

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#icnlude <avr/eeprom.h>
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
	
		// prevents OCR3A from underflowing, using prescaler 64	// 31250 is largest frequency that will not result in underflow
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
unsigned char high_score = 0;
unsigned char score=0;
unsigned char total_score = 0;
unsigned char select_level=0;
unsigned char level0= 1;// game originally begins at
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
unsigned char temp;

//Highscore function
void highScore(){
	LCD_DisplayString(1, win_string);
	LCD_Cursor(10);
	/*for(unsigned i = 0; i < 3; i++){ //Write Hi-score Name
		LCD_WriteData(hiName[i]);
	*/}
	temp = 0;
	if((high_score > 9) && (high_score < 99)){
		temp = high_score / 10;
		LCD_Cursor(14);
		LCD_WriteData(temp + '0');
		temp = high_score % 10;
		LCD_WriteData(temp + '0');
	}else if(high_score > 99){ //Double check this works
		temp = high_score / 100;
		LCD_Cursor(14);
		LCD_WriteData(temp + '0');
		temp = (high_score - (100 * temp)) / 10;
		LCD_WriteData(temp + '0');
		temp = high_score % 10;
		LCD_WriteData(temp + '0');
	}else{
		LCD_Cursor(14);
		LCD_WriteData(high_score + '0');
	}
}

/////////////////////////////////////////////////////////////////
enum Menu_States{SM_Start, Menu, Begin, Level, Reset, wait}State;

void Menu_TickFct(){
    level1 = 0;
    level2 = 0;
    level3 = 0;
    joystick_button = ~PIND & 0X80; //PIND at D3
    buttom1 = ~PIND & 0X10; //PIND at D4 
    reset_buttom = ~PIND & 0X02; //PIND at D5
    
	switch(State){//transitions
		case SM_Start:
        	State = Menu;
      
		case Menu:
			if(button1){
				State = Begin;
        		}
        		else if(joystick_button && !end_level){
         			 State = Level;
	  			LCD_ClearScreen();
	  			LCD_Cursor(1);
	  			LCD_DisplayString(1,Begin_String);
        		}
       		 	else if( reset_button && !end_level){
          			State = Reset;
			}
			else{
	  			State = Menu;
			}
		break;

		case Begin;
		State = wait;
		break;
		
		case Level:
			if(button1){
	  			State = Begin;
			}
			else if(joystick_button %% (level0<3)){
	 		 	level0++;
			}
			else if (joystick_button && (level1>1){
	   			level0--;
			}
			else{
	   			State = Menu;
			}
		break;
		
		case Reset:
			if(reset_button){
	   			
	   			State = wait;
			}
			else 
  	   			State = Menu;
		break;
		default:
		State = Menu;
		break;
  	}

	switch(State){//actions
       		case SM_Start:
	 	State = Menu;
       		break;

		case Menu:
		TimerSet(500);
		LCD_ClearScreen();
		LCD_DisplayString(1, Menu_string);
		LCD_WriteData(0);
		break;
		
		case Begin:
			if(level0 == 1){
				level = 1;
			}
			else if(level0 == 2){
				level = 1;
			}
			else if(level0 == 3){
				level = 1;
			}
		break;
		
		case Level:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(level0 + '0');
		break;
		
		case Reset:
		LCD_Screen();
		LCD_Cursor(1);
		highScore();
		
		default:
		break;
	}	 
}
/////////////////////////////////////////////////////////////////////////
unsigned char* k1 = "Press 1";
unsigned char* k2 = "Press 2";
unsigned char* k3 = "Press 3";
enum Level1{SM_Start, play, display, win, wrong, level1_score}State1;
void Level1_TickFct(){
	joystick_button = ~PIND & 0X80; //PIND at D3
   	buttom1 = ~PIND & 0X10; //PIND at D4 
   	reset_buttom = ~PIND & 0X02; //PIND at D5
	switch(State1){//transitions
		case SM_Start:
		end_level = 0;
			if(!level1){
				State1 = Sm_Start;
			}
			else if(level1){
				total_score =0;
				State1 = play;
				end_level = 1;
			}
		break;
		
		case play:
		State1 = display;
		break;
		
		case display:
			if(reset_button){
				total_score++;
				if(total_score >= score){
					score = total_score;
				}
				State1 = win;
			}
			else if(joystick_button){
				if(total_score>= score){
					score = total_score;
				}
				State1 = wrong;
			}
			else{
				State1 = SM_Start;
			}
		break;
		
		case win:
		State1 = SM_Start;
		break;
		
		case wrong:
		State1 = SM_Start;
		break;
	}

	switch(State1){
		case SM_Start:
		TimerSet(500);
		level1=0;
		end_level=0;
		break;
	
		case play:
		TimerSet(2000);
		DDRB = 0XFF; PORTB = 0X0F;
		DDRA = 0XFF; PORTA = 0X0F;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, Begin_string);
		break;
		
		case display:
		TimerSet(500);
		DDRB = 0XFF; PORTB = 0X0F;
		DDRA = 0XFF; PORTA = 0X0F;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, k1);
		break;
			
		case win:
		highScore();
		TimerSet(1000);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, Error_string);
		LCD_WriteData(0);
		break;
		
		case wrong:
		highScore();
		TimerSet(1000);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, win_string);
		LCD_WriteData(0);
		break;
	}

}		 


int main(){
DDRA = 0XFF; PORTA = 0X00;// Column(Left to Right)
DDRB = 0XFF; PORTB = 0XFF;//Row(Top to Bottom): 
	
DDRC = 0X03; PORTC = 0XFC; //LCD data lines
DDRD = 0XFF; PORTD = 0X00; //LCD control lines

LCD_WriteCommand(0x40);//locations 0x40,0x48,0x50,0x58,0x60,68,70,78
	for(unsigned char i=0; i<8; i++)
	{
		LCD_WriteData(array[i]);
	}
LCD_WriteCommand(0x80); //put back to normal
// Initializes the LCD display
LCD_init();   
// Starting at position 1 on the LCD screen, writes Hello World
LCD_DisplayString(1, Welcome_string);// will display the welcome string 
TimerSet(500);
TimerOn();

Menu_States = State;
Level1 = State1;
set_PWM();
highScore();


  while(1){
  	Menu_TickFct();
	Level1_TickFct();
	 while(!TimerFlag){}
	 TimerFlag = 0;
  }
  
}

      
