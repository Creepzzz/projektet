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
uint8_t screen3[128 * 4] = { 0 };
int pos = 256;
int n = 0;
int count;
int pos2 = 128;
int pos3 = 384;
int bpos = 128;
int bpos2 = 0;
int m = 255;
int k = 0;
int gameState;
int spawnState;
//Gamestate = 2 => pregame
//Gamestate = 1 => game over
//Gamestate = 0 => ongoing game
int counter;
int res;
int gameScore1 = 0;
int gameScore2 = 0;
int gameScore3 = 0;
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
/*    while (gameState==0) {
        display_image(0, screen); // update image on screen


        if (IFS(0)) {
            IFS(0) = 0;
           movedownlogic();
        }
        if ((getbtns() & 0x01) == 1 && (pos < 384))
            
            leftbtnpressed();

        if ((getbtns() & 0x02) == 2 && (pos >= 128 || n >= 1))
            
            rightbtnpressed();
        
        if ((btnvalue & 0x04) == 4) {
            MainMenuFade();
            StartCountDown();
            delay(500);

        }
    }*/
    if (gameState == 2) {//2 = Main Menu/Start Screen
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
    if (gameState == 3) {
        high_score();
        return;
    }

    if (gameState == 4)
    {
        restartgame();
        return;
    }

    
    if ((gameState == 5) && (gameOn == 1)) {
        MainMenuFade();
        StartCountDown();
       // gameState = 0;
    }
    if(gameState == 7){
        int i;
        for(i = 0; i< 8; i++)
            screen[pos2 + i] = 0;
     n = 0;
     score = 0;
     pos2 = 128;
     bpos = 128;
     bpos2 = 0;
     m = 255;
        labinit();
        MainMenuFade();
               StartCountDown();
    }

	display_image(0, screen); // update image on screen

	if (IFS(0)) {
		IFS(0) = 0;
        movedownlogic();
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
	else if ((screen[pos2 - 1] == 255 || screen[pos2 - 1] == 254 || screen[pos2 - 1] == 252 || screen[pos2 - 1] == 248 || screen[pos2 - 1] == 240 || screen[pos2 - 1] == 224 || screen[pos2 - 1] == 192 || screen[pos2 - 1] == 128 || screen[pos2 - 1] == 1 || screen[pos2 - 1] == 2 || screen[pos2 - 1] == 4 || screen[pos2 - 1] == 8 || screen[pos2 - 1] == 16 || screen[pos2 - 1] == 32 ||screen[pos2 - 1-128] == 255 || screen[pos2 - 1-128] == 254 || screen[pos2 - 1-128] == 252 || screen[pos2 - 1-128] == 248 || screen[pos2 - 1-128] == 240 || screen[pos2 - 1-128] == 224 || screen[pos2 - 1-128] == 192 || screen[pos2 - 1-128] == 128 || screen[pos2 - 1-128] == 1 || screen[pos2 - 1-128] == 2 || screen[pos2 - 1-128] == 4 || screen[pos2 - 1-128] == 8 || screen[pos2 - 1-128] == 16 || screen[pos2 - 1-128] == 32 || screen[pos2 - 1-128] == 64) && (bpos-112 == pos2|| bpos - 113 == pos2|| bpos - 114 == pos2|| bpos - 115 == pos2|| bpos - 116 == pos2||bpos-117 == pos2|| bpos - 118 == pos2|| bpos - 119 == pos2|| bpos - 120 == pos2) ) {


		gameState = 1;
		game_over();


	}
	if (bpos - 128 == pos2) {
        ticks++;
        if(gameState == 0 || gameState == 6)
		newblock(ticks);
		score++;
	}


}


// add new block at the top of the screen
void newblock(int y) {
    spawnState = y % 3;
    int i;
    counter++;
    
    
    if(spawnState == 0){
        if(counter == 1){
            for(i = 0; i< 8; i++)
            screen[pos2 + i] = 0;
            bpos = 128;
            pos2 = 128;
            score++;
            gameState = 6;
        }
        if(counter == 0){
            for(i = 0; i< 8; i++)
                screen[pos2 + i] = 0;
                bpos = 384;
                pos2 = 384;
                score++;
                gameState = 6;
        }
        if(counter == 2){
            for(i = 0; i< 8; i++)
                screen[pos2 + i] = 0;
                bpos = 512;
                pos2 = 512;
                score++;
                gameState = 6;
                counter = 0;
            }
    }
    if(spawnState == 1){
        if(counter == 0){
        for(i = 0; i< 8; i++)
            screen[pos2 + i] = 0;
            bpos = 512;
            pos2 = 512;
            score++;
            gameState = 6;
        }
        if(counter == 1){
        for(i = 0; i< 8; i++)
            screen[pos2 + i] = 0;
            bpos = 256;
            pos2 = 256;
            score++;
            gameState = 6;
    }
    if(counter == 2){
        for(i = 0; i< 8; i++)
            screen[pos2 + i] = 0;
            bpos = 128;
            pos2 = 128;
            score++;
            count = 0;
            gameState = 6;
            counter = 0;
        }
    
    }

    if (spawnState==2) {
       
    
            for(i = 0; i< 8; i++)
                screen[pos2 + i] = 0;
                bpos = 384;
                pos2 = 384;
                score++;
                counter = 0;
                gameState = 6;
    }
    
    if(spawnState == 3){
        for(i = 0; i< 8; i++)
            screen[pos2 + i] = 0;
            bpos = 512;
            pos2 = 512;
            score++;
            counter = 0;
            gameState = 6;
    }

}

//right button = go down
void rightbtnpressed(void) {
	//points();
	move(2);

}

//left button = go up
void leftbtnpressed(void) {
	//points();
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
    else if(gameScore1 < score)
        sortscore(1);
    else if(gameScore1 > score)
        sortscore(2);
}
void sortscore(int x) {
    
    switch(x){
        case 1 :
        if (gameScore1 > gameScore2) {
            gameScore3 = gameScore2;
            gameScore2 = gameScore1;
                gameScore1 = score;
            }
            break;
        case 2:
            if(score > gameScore2){
                int temp;
                temp = gameScore2;
                gameScore3 = temp;
                gameScore2 = score;
            }
            else if(score > gameScore3 && score < gameScore2)
                gameScore3 = score;
}
}

void game_over(void) {
     /*   int i;
        for(i = 0; i< 8; i++)
    screen[pos2 + i] = 0;*/
    display_string(0, "GAME OVER");
    display_update();
    if ((getbtns() & 0x01) == 1) {
        gameState = 3;
    }


}
void restartgame(void) {
    display_string(0, "");
    display_string(1, "");
    display_string(2, "");
    display_string(3, "");
        screen_clear();
        display_update();
    //delay(2000);
    gameState = 4;
    gameOn = 1;
    btnpressed = 0;
    if ((getbtns() & 0x01) == 1) {
        gameState = 7;
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
