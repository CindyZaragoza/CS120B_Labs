//Sample: Program

#include <stdlib.h>
#include <util/delay.h>

#include <avr/io.h>
#include <C:/Users/student/Desktop/ucr/bit.h>
#include <C:/Users/student/Desktop/ucr/io.c>
#include <C:/Users/student/Desktop/ucr/io.h>
#include <C:/Users/student/Desktop/ucr/keypad.h>
#include <C:/Users/student/Desktop/ucr/timer.h>

//Find GCD Function
unsigned long int findGCD (unsigned long int a,
unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}

//---Task scheduler data structure---
// Struct for Tasks represent a running process in our
// simple real-time operating system.
/*Tasks should have members that include: state, period, a
measurement of elapsed time, and a function pointer.*/
typedef struct _task {
	//Task's current state, period, and the time elapsed
	// since the last tick
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	//Task tick function
	int (*TickFct)(int);
} task;

/* Configuration for 3x3 LED Matrix Wiring (lower 3 bits for each byte)
	Binary = 0000 0000
			 Col. Row
	LED Lights are numbered as below, corresponding to the following keypad buttons:
			 ----------			----------
			|1	 2	 3 |		| 6  9  # |
			|4	 5	 6 |		| 5  8  0 |
			|7	 8	 9 |		| 4  7  * |
			 ----------			----------
*/
//Blinks the proper LED in the CPU sequence
void blinkLED(unsigned char arr[], unsigned int i){
	if(arr[i] == 1){
		PORTA = 0xE1; //Lights LED 1
	}else if(arr[i] == 2){
		PORTA = 0xD1; //Lights LED 2
	}else if(arr[i] == 3){
		PORTA = 0xB1; //Lights LED 3
	}else if(arr[i] == 4){
		PORTA = 0xE2; //Lights LED 4
	}else if(arr[i] == 5){
		PORTA = 0xD2; //Lights LED 5
	}else if(arr[i] == 6){
		PORTA = 0xB2; //Lights LED 6
	}else if(arr[i] == 7){
		PORTA = 0xE4; //Lights LED 7
	}else if(arr[i] == 8){
		PORTA = 0xD4; //Lights LED 8
	}else if(arr[i] == 9){
		PORTA = 0xB4; //Lights LED 9
	}
	delay_ms(1000); //Delay between lights
	PORTA = 0x00;
	delay_ms(250);
}

unsigned char Key_Flag = 0; //Ensures a single button press is counted only once
unsigned int currGuess(){
	volatile unsigned char x = 0x00;
	unsigned int guess = 0; //Note: Guess numbering corresponds to the LED matrix layout
		Key_Flag = 0;
		x = GetKeypadKey();
		switch (x) {
			case '\0':
				guess = 0;
				break;
			case '1': //Keypad Row 1
				//Unused
				//LCD_DisplayString(1, "1");
				break;
			case '2':
				//Unused
				//LCD_DisplayString(1, "2");
				break;
			case '3':
				//Unused
				//LCD_DisplayString(1, "3");
				break;
			case 'A':
				//Unused - Do nothing
				break;
			case '4': //Keypad Row 2
				//LCD_DisplayString(1, "4");
				if(Key_Flag == 0){
					guess = 7;
				}
				//LCD_DisplayString(1, "4");
				Key_Flag = 1;
				break;
			case '5':
				//LCD_DisplayString(1, "5");
				if(Key_Flag == 0){
					guess = 4;
				}
				//LCD_DisplayString(1, "5");
				Key_Flag = 1;
				break;
			case '6':
				//LCD_DisplayString(1, "6");
				if(Key_Flag == 0){
					guess = 1;
				}
				//LCD_DisplayString(1, "6");
				Key_Flag = 1;
				break;
			case 'B':
				//Unused - Do nothing
				break;
			case '7': //Keypad Row 3
				//LCD_DisplayString(1, "7");
				if(Key_Flag == 0){
					guess = 8;
				}
				Key_Flag = 1;
				break;
			case '8':
				//LCD_DisplayString(1, "8");
				if(Key_Flag == 0){
					guess = 5;
				}
				Key_Flag = 1;
				break;
			case '9':
				//LCD_DisplayString(1, "9");
				if(Key_Flag == 0){
					guess = 2;
				}
				Key_Flag = 1;
				break;
			case 'C':
				//Unused - Do nothing
				break;
			case '*': //Keypad Row 4
				//LCD_DisplayString(1, "*");
				if(Key_Flag == 0){
					guess = 9;
				}
				break;
			case '0':
				//LCD_DisplayString(1, "0");
				if(Key_Flag == 0){
					guess = 6;
				}
				Key_Flag = 1;
				break;
			case '#':
				//LCD_DisplayString(1, "#");
				if(Key_Flag == 0){
					guess = 3;
				}
				Key_Flag = 1;
				break;
			case 'D':
				//Unused - Do nothing
				break;
		}
	Key_Flag = 0;
	return guess;
}

unsigned char hiName[3] = "DOG";
unsigned long currHighScore = 0;
unsigned int Confirm_Flag = 0; //Checks when user accepts a char
unsigned int selection = 0;
unsigned char currChar = 'A';
unsigned char Select_Flag = 0;
unsigned int last_select = 0;

void newName(){
	LCD_DisplayString(1, "New Hi-score!   Enter name: AAA");
	LCD_Cursor(29);
	currChar = 'A';
	last_select = selection;
	Confirm_Flag = 0;
	
	while(Confirm_Flag == 0){ //Select Character One
		selection = currGuess(); //Gets button press
		if(selection == last_select){
			Select_Flag = 0;
		}else{
			Select_Flag = 1;
			last_select = selection;
		}
		if((selection == 7) && (Select_Flag)){ //Increment pressed
			if(currChar == 'Z'){
				//Do nothing, at max value
			}else{
				currChar = currChar + 1;
				LCD_WriteData(currChar);
				LCD_Cursor(29);
			}
		}else if((selection == 4) && (Select_Flag)){//Decrement pressed
			if(currChar == 'A'){
				//Do nothing, at min value
			}else{
				currChar = currChar - 1;
				LCD_WriteData(currChar);
				LCD_Cursor(29);
			}
		}else if((selection == 1) && (Select_Flag)){//Accept pressed
			hiName[0] = currChar;
			Confirm_Flag = 1;
		}
	}
	LCD_Cursor(30);
	Confirm_Flag = 0;
	currChar = 'A';
	selection = 0;
	
	while(Confirm_Flag == 0){ //Select Character One
		selection = currGuess(); //Gets button press
		if(selection == last_select){
			Select_Flag = 0;
		}else{
			Select_Flag = 1;
			last_select = selection;
		}
		if((selection == 7) && (Select_Flag)){ //Increment pressed
			if(currChar == 'Z'){
				//Do nothing, at max value
				}else{
				currChar = currChar + 1;
				LCD_WriteData(currChar);
				LCD_Cursor(30);
			}
			}else if((selection == 4) && (Select_Flag)){//Decrement pressed
			if(currChar == 'A'){
				//Do nothing, at min value
				}else{
				currChar = currChar - 1;
				LCD_WriteData(currChar);
				LCD_Cursor(30);
			}
			}else if((selection == 1) && (Select_Flag)){//Accept pressed
			hiName[1] = currChar;
			Confirm_Flag = 1;
		}
	}
	LCD_Cursor(31);
	Confirm_Flag = 0;
	currChar = 'A';
	selection = 0;
	
	while(Confirm_Flag == 0){ //Select Character One
		selection = currGuess(); //Gets button press
		if(selection == last_select){
			Select_Flag = 0;
		}else{
			Select_Flag = 1;
			last_select = selection;
		}
		if((selection == 7) && (Select_Flag)){ //Increment pressed
			if(currChar == 'Z'){
				//Do nothing, at max value
				}else{
				currChar = currChar + 1;
				LCD_WriteData(currChar);
				LCD_Cursor(31);
			}
			}else if((selection == 4) && (Select_Flag)){//Decrement pressed
			if(currChar == 'A'){
				//Do nothing, at min value
				}else{
				currChar = currChar - 1;
				LCD_WriteData(currChar);
				LCD_Cursor(31);
			}
			}else if((selection == 1) && (Select_Flag)){//Accept pressed
			hiName[2] = currChar;
			Confirm_Flag = 1;
		}
	}
}

void highScore(){
	LCD_DisplayString(1, "Hi-score ");
	LCD_Cursor(10);
	for(unsigned i = 0; i < 3; i++){ //Write Hi-score Name
		LCD_WriteData(hiName[i]);
	}
	unsigned int temp = 0;
	if((currHighScore > 9) && (currHighScore < 99)){
		temp = currHighScore / 10;
		LCD_Cursor(14);
		LCD_WriteData(temp + '0');
		temp = currHighScore % 10;
		LCD_WriteData(temp + '0');
	}else if(currHighScore > 99){ //Double check this works
		temp = currHighScore / 100;
		LCD_Cursor(14);
		LCD_WriteData(temp + '0');
		temp = (currHighScore - (100 * temp)) / 10;
		LCD_WriteData(temp + '0');
		temp = currHighScore % 10;
		LCD_WriteData(temp + '0');
	}else{
		LCD_Cursor(14);
		LCD_WriteData(currHighScore + '0');
	}
}

unsigned long cnt = 0; //counts a timer num to pass to srand()
unsigned char Rand_Flag = 0; //Ensures srand() is only called once
volatile unsigned char Start_Flag = 0;
unsigned char arr[512]; //Limits game sequence to 512 light combinations
unsigned char Game_Turn = 0; //Determines whose turn it is: 0 = CPU, 1 = Player
unsigned long idx = 0; //Stores the index of arr[]
unsigned int guess = 0; //Player's current guess in sequence
unsigned char NewHS_Flag = 0;

//Game Start SM
enum Game_States {Game_Init, Game_On, Game_Off} Game_State;
int Game_TF(int Game_State){
	switch(Game_State){ //Transitions
		case Game_Init:
			if(~PINC & 0x80){ //Start Button is pressed
				Start_Flag = 1;
				Game_State = Game_On;
			}else{
				Game_State = Game_Init;
			}
			break;
		case Game_On:
			if(Start_Flag == 1){ //While Game is started
				Game_State = Game_On;
				if(Rand_Flag == 0){
					srand(cnt); //srand() is only called once, ever
					Rand_Flag = 1;
				}
			}else if(Start_Flag == 0){
				Game_State = Game_Init;
			}
			break;
		default:
			Game_State = Game_Init;
			break;
	}
	
	switch(Game_State){ //Actions
		case Game_Init:
			Game_Turn = 0;
			if(idx != 0){
				idx = 0; //Resets size of current sequence
				highScore();
			}
			break;
		case Game_On:
			if((Game_Turn == 0) && (Start_Flag)){//CPU turn, display sequence
				arr[idx] = (rand() % 9) + 1; //Creates 9 possibilities for each LED in the matrix
				
				if(idx > currHighScore){
					NewHS_Flag = 1;
					currHighScore = idx;
					if(hiName != "YOU"){ //Assigns HS name to 'You' if you're setting a new HS
						hiName[0] = 'Y';
						hiName[1] = 'O';
						hiName[2] = 'U';
					}
					highScore();
				}
				idx++; //Increments the Game Sequence
				LCD_Cursor(17);
				LCD_WriteData('C');
				LCD_WriteData('P');
				LCD_WriteData('U');
				
				for(unsigned i = 0; i < idx; i++){
					blinkLED(arr, i);
				}
				Game_Turn = 1;
			}else if((Game_Turn == 1) && (Start_Flag)){ //Player turn, enter sequence
				LCD_Cursor(17);
				LCD_WriteData('P');
				LCD_WriteData('L');
				LCD_WriteData('Y');
				LCD_WriteData('R');
				for(unsigned i = 0; i < idx; i++){ //Check input against sequence
					guess = 0;
					while(guess == 0){ //Guess 0 means no guess has been made
						guess = currGuess(guess);
					}
					if(guess != arr[i]){ //End and reset game
						LCD_Cursor(21);
						LCD_WriteData(' ');
						LCD_WriteData('W');
						LCD_WriteData('R');
						LCD_WriteData('O');
						LCD_WriteData('N');
						LCD_WriteData('G');
						delay_ms(1000);
						LCD_Cursor(21);
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_Cursor(21);
						LCD_DisplayString(1, "Game Over");
						delay_ms(1000); //Keep me!
						i = idx; //Exits current For loop
						Start_Flag = 0; //Restarts game
						if(NewHS_Flag == 1){ //New Hi-score obtained
							newName();
							NewHS_Flag = 0;
						}
					}else if(guess == arr[i]){
						LCD_Cursor(21);
						LCD_WriteData(' ');
						LCD_WriteData('C');
						LCD_WriteData('O');
						LCD_WriteData('R');
						LCD_WriteData('R');
						LCD_WriteData('E');
						LCD_WriteData('C');
						LCD_WriteData('T');
						delay_ms(1000);
						LCD_Cursor(21);
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_WriteData(' ');
						LCD_Cursor(21);
						Game_Turn = 0;
					}
				}
				if(Start_Flag){
					delay_ms(2500); //Delay allows player to react to new sequence
				}
				highScore(); //Reset the LCD
			}
			break;
		default:
			break;
	}
	return Game_State;
}

int main(){
	//Initialize Inputs and Outputs
	DDRA = 0xFF; PORTA = 0x00; //Controls LED Lights
	DDRB = 0xF0; PORTB = 0x0F; //Keypad, PB7..4 outputs init 0s, PB3..0 inputs init 1s
	DDRC = 0x03; PORTC = 0xFC; //Last 2 bits control LCD, others are inputs for buttons
	DDRD = 0xFF; PORTD = 0x00;
	
	LCD_init();
	LCD_ClearScreen();
	highScore();
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 100;
	unsigned long int SMTick2_calc = 100;
	//Calculating GCD
	unsigned long int tmpGCD = SMTick1_calc;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	//Greatest common divisor for all tasks
	// or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	//Declare an array of tasks
	static task task2;
	task *tasks[] = { &task2 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	// Task 1
	//task1.state = -1;
	//task1.period = SMTick1_period;
	//task1.elapsedTime = SMTick1_period;
	//task1.TickFct = &Key_TF;
	
	// Task 2
	task2.state = -1;
	task2.period = SMTick2_period;
	task2.elapsedTime = SMTick2_period;
	task2.TickFct = &Game_TF;
	
	TimerSet(GCD);
	TimerOn();
	
	unsigned short i;
	while(1) {
		cnt++;
		
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
