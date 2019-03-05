#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.h"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\io.c"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\bit.h"
#include "C:\Users\Rashid Goshtasbi\Desktop\includes\timer.h"
unsigned char flash = 0;
unsigned char highscore_total = 0; //HIGHSCORE COUNTER
uint8_t final_highscore; // EEPROM

unsigned char start_l1 = 0;
unsigned char start_l2 = 0;
unsigned char start_l3 = 0;
unsigned char start_l4 = 0;
unsigned char start_l5 = 0;

unsigned char end_l1 = 0;
unsigned char choose_level_gl = 0;
unsigned char level = 1;
unsigned char highscore_gl = 0;

unsigned char * init_string = "Simon Says Hi!  Press Any Key!";
unsigned char * menu_string = "1.Start 2.Choose3.Scores  Level";
unsigned char * start_string = "Begin Game!";
unsigned char * win_string = "You Win!        Congrats!";
unsigned char * wrong_string = "Sorry, Incorrect";

unsigned char * key_press1 = "Enter Key Press 1";
unsigned char * key_press2 = "Enter Key Press 2";
unsigned char * key_press3 = "Enter Key Press 3";
unsigned char * key_press4 = "Enter Key Press 4";
unsigned char * key_press5 = "Enter Key Press 5";
unsigned char * key_press6 = "Enter Key Press 6";
unsigned char * key_press7 = "Enter Key Press 7";

unsigned char customChar[8] = {
	0b01110,
	0b01110,
	0b01110,
	0b00100,
	0b10101,
	0b00100,
	0b01010,
	0b01010
};

//DDRB = 0xFF; PORTB = 0x0F; TOP LEFT
//DDRC = 0xFF; PORTC = 0x0F;
	
//DDRB = 0xFF; PORTB = 0x0F; TOP RIGHT
//DDRC = 0xFF; PORTC = 0xF0;
	
//DDRB = 0x0F; PORTB = 0xF0; BOTTOM LEFT
//DDRC = 0xFF; PORTC = 0x0F;
	
//DDRB = 0x0F; PORTB = 0xF0; BOTTOM RIGHT
//DDRC = 0xFF; PORTC = 0xF0;

//DDRB = 0xFF; PORTB = 0x0F; TOP TWO
//DDRC = 0xFF; PORTC = 0xFF;

//DDRB = 0xFF; PORTB = 0xF0; BOTTOM TWO
//DDRC = 0xFF; PORTC = 0xFF

unsigned char bc_top = 0xFF;
unsigned char b_bot = 0x0F;


unsigned char b_top_left = 0x0F;
unsigned char c_top_left = 0x0F;

unsigned char b_top_right = 0x0F;
unsigned char c_top_right = 0xF0;

unsigned char b_bot_left = 0xF0;
unsigned char c_bot_left = 0x0F;

unsigned char b_bot_right = 0xF0;
unsigned char c_bot_right = 0xF0;

unsigned char b0 = 0;
unsigned char b1 = 0;
unsigned char b2 = 0;
unsigned char b3 = 0;
unsigned char b4 = 0;

enum menu_sms {menu, start, choose_level, highscore, wait} menu_sm;
void menu_sm_tick()
{
	start_l1 = 0;
	start_l2 = 0;
	start_l3 = 0;
	b0 = ~PINA & 0x04;
	b1 = ~PINA & 0x08;
	b2 = ~PINA & 0x10;
	b3 = ~PINA & 0x20;
	b4 = ~PINA & 0x40;
	switch (menu_sm)
	{
		case menu:
		if (b4)
		{
			menu_sm = menu;
		}
		else if (b0 && !end_l1)
		{
			menu_sm = start;
		}
		else if (b1 && !end_l1)
		{
			menu_sm = choose_level;
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_DisplayString(1, start_string);
		}
		else if(b2 && !end_l1)
		{
			menu_sm = highscore;
		}
		else
		{
			menu_sm = menu;
		}
		break;
		
		//------------------------------
		
		case start:
		menu_sm = wait;
		break;
		
		//-----------------------------
		
		case choose_level:
		if (b4)
		{
			menu_sm = menu;
		}
		else if (b0 && (level < 5))
		{
			level++;
		}
		else if (b1 && (level > 1))
		{
			level--;
		}
		else if (b2)
		{
			menu_sm = menu;
		}
		break;
		
		//-----------------------------
		
		case highscore:
		if (b4)
		{
			menu_sm = menu;
		}
		else
		{
			menu_sm = highscore;
		}
		break;
		
		//-----------------------------
		
		default:
		menu_sm = menu;
		break;
	}
	
	
	switch (menu_sm)
	{	
		case menu:
		TimerSet(300);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, menu_string);
		LCD_WriteData(0);
		break;
		
		//-----------------------------
		case start:
		if (level == 1)
		{
			start_l1 = 1;
		}
		else if (level == 2)
		{
			start_l2 = 1;
		}
		else if (level == 3)
		{
			start_l3 = 1;
		}
		else if (level == 4)
		{
			start_l4 = 1;
		}
		else if (level == 5)
		{
			start_l5 = 1;
		}
		break;
		
		case choose_level:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(level + '0');
		break;
		
		//-----------------------------
		
		case highscore:
		LCD_ClearScreen();
		LCD_Cursor(1);
		uint8_t byteRead = eeprom_read_byte((uint8_t*)46);
		LCD_WriteData(byteRead +'0');
		break;
		
		//-----------------------------
		
		default:
		break;
		
	}
}

enum level1s {l1_init, blink1, display, wrong, win, highscore1} level1;
void level1_tick()
{
	b0 = ~PINA & 0x04;
	b1 = ~PINA & 0x08;
	b2 = ~PINA & 0x10;
	b3 = ~PINA & 0x20;
	b4 = ~PINA & 0x40;
	switch (level1)
	{
		case l1_init:
		end_l1 = 0;
		if (!start_l1)
		{
			level1 = l1_init;
		}
		else if (start_l1)
		{
			highscore_total = 0;
			level1 = blink1;
			end_l1 = 1;
		}
		break;
		//-----------------------------
		case blink1:
		level1 = display;
		break;
		//-----------------------------
		case display:
		if (b0)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level1 = win;
		}
		else if (b1 || b2 || b3)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level1 = wrong;
		}
		else if (b4)
		{
			level1 = l1_init;
		}
		break;
		//-----------------------------
		case wrong:
		level1 = l1_init;
		break;
		//-----------------------------
		case win:
		level1 = l1_init;
		break;
		//-----------------------------
	}
	
	switch(level1)
	{
		case l1_init:
		TimerSet(500);
		start_l1 = 0;
		end_l1 = 0;
		break;
		//-----------------------------
		case blink1:
		TimerSet(2000);
		//TOP LEFT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0x0F;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case display:
		TimerSet(500);
		//TURNS OFF SCREEN
		DDRB = 0xFF; PORTB = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press1);
		break;
		//-----------------------------
		case wrong:
		eeprom_write_byte((uint8_t*)46, final_highscore);
		TimerSet(1000);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, wrong_string);
		LCD_WriteData(0);
		break;
		//-----------------------------
		case win:
		eeprom_write_byte((uint8_t*)46, final_highscore);
		TimerSet(1000);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, win_string);
		LCD_WriteData(0);
		break;
		//-----------------------------
	}
}

enum level2s {l2_init, blink21, blink22, display21, display22, wrong2, win2} level2;
void level2_tick()
{
	b0 = ~PINA & 0x04;
	b1 = ~PINA & 0x08;
	b2 = ~PINA & 0x10;
	b3 = ~PINA & 0x20;
	b4 = ~PINA & 0x40;
	switch (level2)
	{
		case l2_init:
		end_l1 = 1;
		if (!start_l2)
		{
			level2 = l2_init;
		}
		else if (start_l2)
		{
			highscore_total = 0;
			level2 = blink21;
			end_l1 = 1;
		}
		break;
		//-----------------------------
		case blink21:
		level2 = blink22;
		break;
		//-----------------------------
		case blink22:
		level2 = display21;
		break;
		//-----------------------------
		case display21:
		if (b0)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level2 = display22;
		}
		else if (b1 || b2 || b3)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level2 = wrong2;
		}
		else if (b4)
		{
			level2 = l2_init;
		}
		break;
		//-----------------------------
		case display22:
		if (b3)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level2 = win2;
		}
		else if (b1 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level2 = wrong2;
		}
		else if (b4)
		{
			level2 = l2_init;
		}
		break;
		//-----------------------------
		case wrong2:
		level2 = l2_init;
		break;
		//-----------------------------
		case win2:
		level2 = l2_init;
		break;
	}
	
	switch(level2)
	{
		case l2_init:
		start_l2 = 0;
		end_l1 = 0;
		break;
		//-----------------------------
		case blink21:
		TimerSet(1500);
		//TOP LEFT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0x0F;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink22:
		//BOTTOM RIGHT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0xF0;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case display21:
		TimerSet(300);
		//TURNS OFF SCREEN
		DDRB = 0xFF; PORTB = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press1);
		break;
		//-----------------------------
		case display22:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press2);
		break;
		//-----------------------------
		case wrong2:
		TimerSet(1000);
		eeprom_write_byte((uint8_t*)46, final_highscore);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, wrong_string);
		LCD_WriteData(0);
		break;
		//-----------------------------
		case win2:
		TimerSet(1000);
		eeprom_write_byte((uint8_t*)46, final_highscore);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, win_string);
		LCD_WriteData(0);
		break;
	}
}

enum level3s {l3_init, blink31, blink32, blinkoff32_33, blink33, blink34, blink35, display31, display32, display33, display34, display35, wrong3, win3} level3;
void level3_tick()
{
	b0 = ~PINA & 0x04;
	b1 = ~PINA & 0x08;
	b2 = ~PINA & 0x10;
	b3 = ~PINA & 0x20;
	b4 = ~PINA & 0x40;
	switch (level3)
	{
		case l3_init:
		end_l1 = 1;
		if (!start_l3)
		{
			level3 = l3_init;
		}
		else if (start_l3)
		{
			highscore_total = 0;
			level3 = blink31;
			end_l1 = 1;
		}
		break;
		//-----------------------------
		case blink31:
		level3 = blink32;
		break;
		//-----------------------------
		case blink32:
		level3 = blinkoff32_33;
		break;
		//-----------------------------
		case blinkoff32_33:
		level3 = blink33;
		break;
		//-----------------------------
		case blink33:
		level3 = blink34;
		break;
		//-----------------------------
		case blink34:
		level3 = blink35;
		break;
		//-----------------------------
		case blink35:
		level3 = display31;
		break;
		//-----------------------------
		case display31:
		if (b0)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = display32;
		}
		else if (b1 || b2 || b3)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = wrong3;
		}
		else if (b4)
		{
			level3 = l3_init;
		}
		break;
		//-----------------------------
		case display32:
		if (b1)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = display33;
		}
		else if (b3 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = wrong3;
		}
		else if (b4)
		{
			level3 = l3_init;
		}
		break;
		//-----------------------------
		case display33:
		if (b1)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = display34;
		}
		else if (b3 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = wrong3;
		}
		else if (b4)
		{
			level3 = l3_init;
		}
		break;
		//-----------------------------
		case display34:
		if (b2)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = display35;
		}
		else if (b1 || b3 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = wrong3;
		}
		else if (b4)
		{
			level3 = l3_init;
		}
		break;
		//-----------------------------
		case display35:
		if (b3)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = win3;
		}
		else if (b1 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level3 = wrong3;
		}
		else if (b4)
		{
			level3 = l3_init;
		}
		break;
		//-----------------------------
		case wrong3:
		level3 = l3_init;
		break;
		//-----------------------------
		case win3:
		level3 = l3_init;
		break;
	}
	
	switch(level3)
	{
		case l3_init:
		start_l3 = 0;
		end_l1 = 0;
		break;
		//-----------------------------
		case blink31:
		TimerSet(500);
		//TOP LEFT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0x0F;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink32:
		//BOTTOM LEFT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0x0F;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blinkoff32_33:
		TimerSet(75);
		//BLINKS OFF FOR NEXT REPEAT STATE
		DDRB = 0xFF; PORTB = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		break;
		//-----------------------------
		case blink33:
		TimerSet(500);
		//BOTTOM LEFT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0x0F;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink34:
		//TOP RIGHT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0xF0;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink35:
		//BOTTOM RIGHT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0xF0;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case display31:
		TimerSet(300);
		//TURNS OFF SCREEN
		DDRB = 0xFF; PORTB = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press1);
		break;
		//-----------------------------
		case display32:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press2);
		break;
		//-----------------------------
		case display33:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press3);
		break;
		//-----------------------------
		case display34:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press4);
		break;
		//-----------------------------
		case display35:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press5);
		break;
		//-----------------------------
		case wrong3:
		TimerSet(1000);
		eeprom_write_byte((uint8_t*)46, final_highscore);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, wrong_string);
		LCD_WriteData(0);
		break;
		//-----------------------------
		case win3:
		TimerSet(1000);
		eeprom_write_byte((uint8_t*)46, final_highscore);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, win_string);
		LCD_WriteData(0);
		break;
	}
}

enum level4s {l4_init, blink41, blink42, blink43, blink44, blink45, blink46, blink47, display41, display42, display43, display44, display45, display46, display47, wrong4, win4} level4;
void level4_tick()
{
	b0 = ~PINA & 0x04;
	b1 = ~PINA & 0x08;
	b2 = ~PINA & 0x10;
	b3 = ~PINA & 0x20;
	b4 = ~PINA & 0x40;
	switch (level4)
	{
		case l4_init:
		end_l1 = 1;
		if (!start_l4)
		{
			level4 = l4_init;
		}
		else if (start_l4)
		{
			highscore_total = 0;
			level4 = blink41;
			end_l1 = 1;
		}
		break;
		//-----------------------------
		case blink41:
		level4 = blink32;
		break;
		//-----------------------------
		case blink42:
		level4 = blink43;
		break;
		//-----------------------------
		case blink43:
		level4 = blink44;
		break;
		//-----------------------------
		case blink44:
		level4 = blink45;
		break;
		//-----------------------------
		case blink45:
		level4 = blink46;
		break;
		//-----------------------------
		case blink46:
		level4 = blink47;
		break;
		//-----------------------------
		case blink47:
		level4 = display41;
		break;
		//-----------------------------
		case display41:
		if (b3)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = display42;
		}
		else if (b1 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = wrong4;
		}
		else if (b4)
		{
			level4 = l4_init;
		}
		break;
		//-----------------------------
		case display42:
		if (b2)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = display43;
		}
		else if (b3 || b1 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = wrong4;
		}
		else if (b4)
		{
			level4 = l4_init;
		}
		break;
		//-----------------------------
		case display43:
		if (b0)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = display44;
		}
		else if (b3 || b2 || b1)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = wrong4;
		}
		else if (b4)
		{
			level4 = l4_init;
		}
		break;
		//-----------------------------
		case display44:
		if (b3)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = display45;
		}
		else if (b1 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = wrong4;
		}
		else if (b4)
		{
			level4 = l4_init;
		}
		break;
		//-----------------------------
		case display45:
		if (b1)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = display46;
		}
		else if (b3 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = wrong4;
		}
		else if (b4)
		{
			level4 = l4_init;
		}
		break;
		//-----------------------------
		case display46:
		if (b2)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = display47;
		}
		else if (b1 || b3 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = wrong4;
		}
		else if (b4)
		{
			level4 = l4_init;
		}
		break;
		//-----------------------------
		case display47:
		if (b0)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = win4;
		}
		else if (b1 || b2 || b3)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level4 = wrong4;
		}
		else if (b4)
		{
			level4 = l4_init;
		}
		break;
		//-----------------------------
		case wrong4:
		level4 = l4_init;
		break;
		//-----------------------------
		case win4:
		level4 = l4_init;
		break;
	}
	
	switch(level4)
	{
		case l4_init:
		start_l4 = 0;
		end_l1 = 0;
		break;
		//-----------------------------
		case blink41:
		TimerSet(500);
		//BOTTOM RIGHT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0xF0;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink42:
		//TOP RIGHT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0xF0;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink43:
		//TOP LEFT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0x0F;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink44:
		//BOTTOM RIGHT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0xF0;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink45:
		//BOTTOM LEFT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0x0F;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink46:
		//TOP RIGHT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0xF0;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink47:
		//TOP LEFT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0x0F;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case display41:
		TimerSet(300);
		//TURNS OFF SCREEN
		DDRB = 0xFF; PORTB = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press1);
		break;
		//-----------------------------
		case display42:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press2);
		break;
		//-----------------------------
		case display43:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press3);
		break;
		//-----------------------------
		case display44:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press4);
		break;
		//-----------------------------
		case display45:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press5);
		break;
		//-----------------------------
		case display46:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press6);
		break;
		//-----------------------------
		case display47:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press7);
		break;
		//-----------------------------
		case wrong4:
		TimerSet(1000);
		eeprom_write_byte((uint8_t*)46, final_highscore);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, wrong_string);
		LCD_WriteData(0);
		break;
		//-----------------------------
		case win4:
		TimerSet(1000);
		eeprom_write_byte((uint8_t*)46, final_highscore);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, win_string);
		LCD_WriteData(0);
		break;
	}
}

enum level5s {l5_init, blink51, blink52, blink53, blink54, blink55, blink56, blink57, display51, display52, display53, display54, display55, display56, display57, wrong5, win5} level5;
void level5_tick()
{
	b0 = ~PINA & 0x04;
	b1 = ~PINA & 0x08;
	b2 = ~PINA & 0x10;
	b3 = ~PINA & 0x20;
	b4 = ~PINA & 0x40;
	switch (level5)
	{
		case l5_init:
		end_l1 = 1;
		if (!start_l5)
		{
			level5 = l5_init;
		}
		else if (start_l5)
		{
			highscore_total = 0;
			level5 = blink51;
			end_l1 = 1;
		}
		break;
		//-----------------------------
		case blink51:
		level5 = blink52;
		break;
		//-----------------------------
		case blink52:
		level5 = blink53;
		break;
		//-----------------------------
		case blink53:
		level5 = blink54;
		break;
		//-----------------------------
		case blink54:
		level5 = blink55;
		break;
		//-----------------------------
		case blink55:
		level5 = blink56;
		break;
		//-----------------------------
		case blink56:
		level5 = blink57;
		break;
		//-----------------------------
		case blink57:
		level5 = display51;
		break;
		//-----------------------------
		case display51:
		if (b0 && b2)
		{
			highscore_total += 2;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = display52;
		}
		else if (b1 || b0 || b2 || b3)
		{
			level5 = wrong5;
		}
		else if (b4)
		{
			level5 = l5_init;
		}
		break;
		//-----------------------------
		case display52:
		if (b2)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = display53;
		}
		else if (b3 || b1 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = wrong5;
		}
		else if (b4)
		{
			level5 = l5_init;
		}
		break;
		//-----------------------------
		case display53:
		if (b0)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = display54;
		}
		else if (b3 || b2 || b1)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = wrong5;
		}
		else if (b4)
		{
			level5 = l5_init;
		}
		break;
		//-----------------------------
		case display54:
		if (b3)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = display55;
		}
		else if (b1 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = wrong5;
		}
		else if (b4)
		{
			level5 = l5_init;
		}
		break;
		//-----------------------------
		case display55:
		if (b1)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = display56;
		}
		else if (b3 || b2 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = wrong5;
		}
		else if (b4)
		{
			level5 = l5_init;
		}
		break;
		//-----------------------------
		case display56:
		if (b2)
		{
			highscore_total++;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = display57;
		}
		else if (b1 || b3 || b0)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = wrong5;
		}
		else if (b4)
		{
			level5 = l5_init;
		}
		break;
		//-----------------------------
		case display57:
		if (b1 && b3)
		{
			highscore_total += 2;
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = win5;
		}
		else if (b0 || b1 || b3 || b2)
		{
			if (highscore_total >= final_highscore)
			{
				final_highscore = highscore_total;
			}
			level5 = wrong5;
		}
		else if (b4)
		{
			level5 = l5_init;
		}
		break;
		//-----------------------------
		case wrong5:
		level5 = l5_init;
		break;
		//-----------------------------
		case win5:
		level5 = l5_init;
		break;
	}
	
	switch(level5)
	{
		case l5_init:
		start_l5 = 0;
		end_l1 = 0;
		break;
		//-----------------------------
		case blink51:
		TimerSet(500);
		//TOP TWO
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0xFF;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink52:
		//TOP RIGHT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0xF0;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink53:
		//TOP LEFT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0x0F;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink54:
		//BOTTOM RIGHT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0xF0;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink55:
		//BOTTOM LEFT
		DDRB = 0x0F; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0x0F;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink56:
		//TOP RIGHT
		DDRB = 0xFF; PORTB = 0x0F;
		DDRC = 0xFF; PORTC = 0xF0;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case blink57:
		//BOTTOM TWO
		DDRB = 0xFF; PORTB = 0xF0;
		DDRC = 0xFF; PORTC = 0xFF;
		//LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, start_string);
		break;
		//-----------------------------
		case display51:
		TimerSet(300);
		//TURNS OFF SCREEN
		DDRB = 0xFF; PORTB = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press1);
		break;
		//-----------------------------
		case display52:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press2);
		break;
		//-----------------------------
		case display53:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press3);
		break;
		//-----------------------------
		case display54:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press4);
		break;
		//-----------------------------
		case display55:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press5);
		break;
		//-----------------------------
		case display56:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press6);
		break;
		//-----------------------------
		case display57:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, key_press7);
		break;
		//-----------------------------
		case wrong5:
		TimerSet(1000);
		eeprom_write_byte((uint8_t*)46, final_highscore);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, wrong_string);
		LCD_WriteData(0);
		break;
		//-----------------------------
		case win5:
		TimerSet(1000);
		eeprom_write_byte((uint8_t*)46, final_highscore);
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_DisplayString(1, win_string);
		LCD_WriteData(0);
		break;
	}
}

int main(void) //EE PROM
{
	DDRA = 0x03; PORTA = 0xFC;
	DDRD = 0xFF; PORTD = 0x00;
	
	DDRB = 0xFF; PORTB = 0xFF; //COLLUMN (LEFT->RIGHT): 0x00: ground 0 means ON
	DDRC = 0xFF; PORTC = 0x00; //ROW (TOP->BOTTOM): 0xFF: power 1 means on
	
	//CUSTOM CHARACTER
	LCD_WriteCommand(0x40);//locations 0x40,0x48,0x50,0x58,0x60,68,70,78
	for(unsigned char i=0; i<8; i++)
	{
		LCD_WriteData(customChar[i]);
	}
	LCD_WriteCommand(0x80); //put back to normal
	
	//EE PROM
	eeprom_read_byte((uint8_t*)46);
	
	TimerSet(500);
	TimerOn();
	menu_sm = menu;
	level1 = l1_init;
	
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
    while(1)
    {
		menu_sm_tick();
		level1_tick();
		level2_tick();
		level3_tick();
		level4_tick();
		level5_tick();
		while(!TimerFlag){}
		TimerFlag = 0;
    }
}
