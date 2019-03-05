/* Name and email: Cindy Zaragoza czara007@ucr.edu

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <CS120B_Labs/czara007_CS120B_FinalProject/bit.h>
#include <CS120B_Labs/czara007_CS120B_FinalProject/io.c>
#include <CS120B_Labs/czara007_CS120B_FinalProject/io.h>
#include <CS120B_Labs/czara007_CS120B_FinalProject/timer.h>

//////////////////////////////////////////////////////////////
//Gane Scoring
unsigned char level1 = 0;
unsigned char level2 = 0;
unsigned char level3 = 0;

unsigned char score

//DISPLAY
const unsigned char* Welcome_string = "Welcome, Simon says Hi";
unsigned char* Menu_string = "1. Begin game  2. Level 3. Reset/Scores";
unsigned char* Begin_string = "Begin";
unsigned char* win_string = "Congrats you won";
unsigned char* Error_string = "Sorry wrong entry ";

//LED Matrix 
unsigned char top_left = 0;
unsigned char top_right = 0;
unsigned char b_left = 0; // bottom left
unsigned char b_right = 0; // bottom right
unsigned char button1.button2,button3,button4; // buttons that operate the menu selections
unsigned char i;

//

/////////////////////////////////////////////////////////////////
enum Menu_States{SM_Start, Begin, Level, Reset}State;

void Menu_Fct(){
  switch(State){
    
  
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
   

  while(1){}
  
}

      
