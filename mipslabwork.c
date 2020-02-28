/* mipslabwork.c
   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall
   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):
   This file modified 2017-04-31 by Ture Teknolog
   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdio.h>

   //Här är massa ändrat

#define Fosc 80000000
#define PreScalar 256
#define IPerSec Fosc / PreScalar

uint8_t screen[128 * 4] = { 0 };
uint8_t screen2[128 * 4] = { 0 };
int pos = 256;
int n = 0;
int count;
int pos2 = 128;
int bpos = 128;
int m = 255;
int k = 0;
int gameState;
//Gamestate = 2 => pregame
//Gamestate = 1 => game over
//Gamestate = 0 => ongoing game
int counter;

int gameScore1;
int gameScore2;
int gameScore3;
int score;
int ticks;
int gameOn;

//gamecounter ökar varje gång ny bil kommer, på port e

int btnpressed = 0;

int gamepaused = 0;

char textstring[] = "text, more text, and even more text!";
volatile int* _PORTE = (volatile int*)0xbf886110;

/* Interrupt Service Routine */
void user_isr(void)
{
	return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{
	gameState = 2;
	volatile int* trise = (volatile int*)0xbf886100;
	*trise = *trise & 0xffffff00;


	*_PORTE = *_PORTE & 0xffffff00;


	TRISD = TRISD & 0x0fe0;

	IFS(0) = 0;

	TMR2 = 0;               // set clock to 0
	PR2 = IPerSec / 50;     // roll over value
	T2CON = 0x08070;         // 1000 0000 0111 0000    bit 1: external/ internal clock
	int i;
	for (i = 0; i < 8; i++)
		screen[pos + i] = 255;

	display_image(0, screen);


}

void StartCountDown(void) {

	display_string(2, "     READY");
	display_update();
	delay(1000);

	display_string(2, "      SET");
	display_update();
	delay(1000);

	display_string(2, "      GO!");
	display_update();
	delay(1000);

	display_string(2, "      ");
	display_update();
	//delay(1000);

	display_update();

	gameState = 0;

}


// Fade-effekt när knapp 4 trycks
void MainMenuFade(void) {

	display_string(2, "        Speedo");
	display_update();
	delay(35);
	display_string(2, "         Speedo");
	display_update();
	delay(35);
	display_string(2, "           Speedo");
	display_update();
	delay(35);
	display_string(2, "            Speedo");
	display_update();
	delay(35);
	display_string(2, "             Speedo");
	display_update();
	delay(35);
	display_string(2, "              Speedo");
	display_update();
	delay(35);
	display_string(2, "               Speedo");
	display_update();
	delay(35);
	display_string(2, "                Speedo");
	display_update();
	delay(35);
	display_string(2, "                 Speedo");
	display_update();
	delay(35);
	display_string(2, "                   Speedo");
	display_update();
	delay(35);
	display_string(2, "                    Speedo");
	display_update();
	delay(35);
	display_string(2, "                     Speedo");
	display_update();
	delay(35);
	display_string(2, "                      Speedo");
	display_update();
	delay(35);
	display_string(2, "                       Speedo");
	display_update();
	delay(35);
	display_string(2, "                        Speedo");
	display_update();
	delay(35);


	delay(500);

}

/* This function is called repetitively from the main program */
void labwork(void)
{   
	int btnvalue = getbtns();

	if (gameState == 1) {
		game_over();
		return;
	}
	if (gameState == 3) {
		high_score();
		return;
	}

	if (gameState == 4)
	{
		restartgame();
		return;
	}

	
	if (gameState == 2) {                //2 = Main Menu/Start Screen
		while (gameState == 2) {
			delay(10);
			display_string(2, "    Speedo");
			display_update();
			if (getbtns() != 0)
			{
				gameState = 0;
			}
		}

	}


	

	display_image(0, screen); // update image on screen


	if (IFS(0)) {
		IFS(0) = 0;
		movedownlogic();
		
	}

	if ((gameState == 5) && (gameOn == 1)) {

		MainMenuFade();
		StartCountDown();
		
		
	}

	if (btnvalue != 0 && btnpressed == 0) {
		btnpressed = 1;

		if ((btnvalue & 0x04) == 4) {
			MainMenuFade();
			StartCountDown();
			delay(500);

		}

		if ((btnvalue & 0x01) == 1 && (pos < 384))
			leftbtnpressed();

		if ((btnvalue & 0x02) == 2 && (pos >= 128 || n >= 1))
			rightbtnpressed();

		if ((btnvalue & 0x04) == 4) {

			gameState = 0;

			
		}
	}
	else if (btnvalue == 0)
		btnpressed = 0;
}

/* every tick the block moves down and checks if tetris */
void movedownlogic(void) {
	int i;
	if (screen[pos2 - 1] == 0) {
		if (bpos - 7 > pos2) {
			screen[pos2 + 7] = 0;
			
		}

			screen[--pos2] = m;
		
	

	}
	else if ((screen[pos2 - 1] == 254 || screen[pos2 - 1] == 252 || screen[pos2 - 1] == 248 || screen[pos2 - 1] == 240 || screen[pos2 - 1] == 224 || screen[pos2 - 1] == 192 || screen[pos2 - 1] == 128 || screen[pos2 - 1] == 1 || screen[pos2 - 1] == 2 || screen[pos2 - 1] == 4 || screen[pos2 - 1] == 8 || screen[pos2 - 1] == 16 || screen[pos2 - 1] == 32 || screen[pos2 - 1] == 64)&& (bpos-120 == pos2|| bpos - 121 == pos2|| bpos - 122 == pos2|| bpos - 123 == pos2|| bpos - 124 == pos2||bpos-125 == pos2|| bpos - 126 == pos2|| bpos - 127 == pos2|| bpos - 128 == pos2) ) {


		gameState = 1;
		game_over();


	}
	if (bpos - 128 == pos2) {
		newblock();
		score++;
	}


}



// add new block at the top of the screen 
void newblock(void) {
	int blocktype = 0;//(pos+n)%4;  // value between [0-3]
	int i;
	counter++;

	for (i = 0; i < 8; i++) {
		screen[pos2 + i] = 255;

	}

	if (blocktype == 0)
		for(i = 0; i< 8; i++)
			screen[pos2 + i] = 0;
			pos2 += 128;
			score++;
			count = 0;
}

//right button = go down
void rightbtnpressed(void) {

	
	move(2);

}

//left button = go up
void leftbtnpressed(void) {

	
	move(1);

}

void move(int k) {
	//go up
	if (k == 2) {
		n--;
		if (n < 0) {
			n = 7;
			pos -= 128;
		}

		int i;
		for (i = 0; i < 8; i++) {
			int value = 1;
			int j;
			for (j = 0; j < n; j++)
				value *= 2;
			screen[pos + i] += value;
			screen[pos + 128 + i] -= value;
		}

	}

	//go down
	if (k == 1) {
		int i;
		for (i = 0; i < 8; i++) {
			int value = 1;
			int j;
			for (j = 0; j < n; j++)
				value *= 2;
			screen[pos + i] -= value;
			screen[pos + 128 + i] += value;
		}
		n++;
		if (n > 7) {
			n = 0;
			pos += 128;
		}

	}
}
void addscore(void) {
	if (gameScore1 == 0) {
		gameScore1 = score;
	}
	else {
		sortscore();
	}
}
void sortscore(void) {
	if (gameScore2 == 0) {
		gameScore2 = score;
	}
	if (gameScore3 == 0) {
		gameScore3 = score;
	}
	else {
		if (score > gameScore3 && score < gameScore2) {
			gameScore3 = score;
		}
		if (score > gameScore2&& score < gameScore1) {
			gameScore2 = score;
		}
		else {
			gameScore1 = score;
		}
	}
}
void game_over(void) {
	display_string(0, "GAME OVER");
	display_update();
	if ((getbtns() & 0x01) == 1) {
		gameState = 3;
	}


}
void restartgame(void) {
	int score = 0;
	int pos2 = 128;
	int bpos = 128;
	int pos = 256;
	screen_clear();
	display_string(0, "");
	display_string(1, "");
	display_string(2, "");
	display_string(3, "");
	display_update();
	//delay(2000);
	gameState = 4;
	gameOn = 1;
	btnpressed = 0;
	if ((getbtns() & 0x01) == 1) {
		gameState = 5;
	}


}

void high_score(void) {
	addscore();
	int i;
	char* c1 = itoaconv(gameScore1);
	display_string(0, "Highscore: ");
	for (i = 0; i < 3; i++) {
		if (i == 0) {
			display_string(1, c1);
		}
		if (i == 1){
			char* c2 = itoaconv(gameScore2);
				display_string(2, c2); }
		if (i == 2) {
			char* c3 = itoaconv(gameScore3);
				display_string(3, c3);
		}
	}
	display_update();
	delay(200);

	if ((getbtns() & 0x01) == 1) {
		//delay(5000);
		//restartgame();
		gameState = 4;
	}
	
}





