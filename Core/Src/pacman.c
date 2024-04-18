/*
 * pacman.c
 *
 *  Created on: Apr 15, 2024
 *      Author: Darryl
 */


#include "pacman.h"
#include "lcd.h"
#include "maze.h"
#include <stdio.h>
#include <string.h>

PacmanGameData PACMAN_GAMEDATA;

// Private functions
void createMaze(const void * maze){
	memcpy(PACMAN_GAMEDATA.mazeData, maze, sizeof (char) * 26 * 23);
}

// Public Functions
void Pacman_gameloop(){
	  if(PACMAN_GAMEDATA.prevDirection != RIGHT && PACMAN_GAMEDATA.prevDirection != LEFT){
		  Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, LEFT);
		  PACMAN_GAMEDATA.prevDirection = LEFT;
	  }
	  else if(PACMAN_GAMEDATA.prevDirection != DOWN || PACMAN_GAMEDATA.pacman.direction == STOP){
		  Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, UP);
		  PACMAN_GAMEDATA.prevDirection = UP;
	  }
	  else if(PACMAN_GAMEDATA.prevDirection != LEFT || PACMAN_GAMEDATA.pacman.direction == STOP){
		  Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, RIGHT);
		  PACMAN_GAMEDATA.prevDirection = RIGHT;
	  }
	  else if(PACMAN_GAMEDATA.prevDirection != UP || PACMAN_GAMEDATA.pacman.direction == STOP){
		  Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, DOWN);
		  PACMAN_GAMEDATA.prevDirection = DOWN;
	  }
	  else{
		  PACMAN_GAMEDATA.prevDirection = STOP;
	  }

	  char scoreDisplay[20];
	  sprintf(PACMAN_GAMEDATA.scoreString, "%d", PACMAN_GAMEDATA.pacman.score);
	  strcpy(scoreDisplay, "Score: ");
	  strcat(scoreDisplay, PACMAN_GAMEDATA.scoreString);
	  LCD_DrawString_Color (0, 280, scoreDisplay, BLACK, YELLOW);
}

void Pacman_gamestart(){
	createMaze(MAZE1);
	LCD_Clear(0, 0, 240, 320, BLACK);
	initMaze(mazeStartX, mazeStartY, PACMAN_GAMEDATA.mazeData, &PACMAN_GAMEDATA.pacman);
}


void Pacman_update(Pacman* pacman, char (*mazeData)[23], Direction direction) {
	//checking movement allowed or not?

	uint16_t curX = pacman->curX;
	uint16_t curY = pacman->curY;

	pacman->pastX = pacman->curX;
	pacman->pastY = pacman->curY;

	char mazeChar;

    switch (direction) {
        case LEFT:
        	mazeChar = mazeData[curY][curX - 1];
        	if(mazeChar != '#' ){
        		pacman->curX--;
        		pacman->direction = LEFT;
        	}
        	else{
        		pacman->direction = STOP;
        	}
            break;
        case RIGHT:
            mazeChar = mazeData[curY][curX + 1];
            if(mazeChar != '#' ){
            	pacman->curX++;
            	pacman->direction = RIGHT;
			}
			else{
				pacman->direction = STOP;
			}
            break;
        case UP:
        	mazeChar = mazeData[curY - 1][curX];
        	if(mazeChar != '#' ){
        		pacman->curY--;
				pacman->direction = UP;
			}
			else{
				pacman->direction = STOP;
			}
            break;
        case DOWN:
        	mazeChar = mazeData[curY + 1][curX];
			if(mazeChar != '#' ){
				pacman->curY++;
				pacman->direction = DOWN;
			}
			else{
				pacman->direction = STOP;
			}
            break;
        case STOP:
        	pacman->direction = STOP;
        	break;

    }

    mazeChar = mazeData[pacman->curY][pacman->curX];
    if(mazeChar == '*'){
    	pacman->score += 10;
    	mazeData[pacman->curY][pacman->curX] = ' ';
    }
    else if(mazeChar == '@'){
    	pacman->score += 50;
    	mazeData[pacman->curY][pacman->curX] = ' ';
    }

    if(pacman->direction != STOP){
    	uint16_t borderStartX = mazeStartX + pacman->curX * gamePixelSize;
		uint16_t borderStartY = mazeStartY + pacman->curY * gamePixelSize;
		LCD_DrawPacman(pacman, borderStartX, borderStartY, 9, YELLOW);

    	borderStartX = mazeStartX + pacman->pastX * gamePixelSize;
		borderStartY = mazeStartY + pacman->pastY * gamePixelSize;
		LCD_DrawPixel(borderStartX, borderStartY, gamePixelSize, BLACK);
    }
}
