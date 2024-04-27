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
#include "keypad.h"

PacmanGameData PACMAN_GAMEDATA;

// Private functions
void createMaze(const void * maze){
	memcpy(PACMAN_GAMEDATA.mazeData, maze, sizeof (char) * 26 * 23);
}

void Pacman_handleInput(uint8_t input){
	switch (input) {
	  case 0:
		PACMAN_GAMEDATA.inputDirection= UP;
	    break;
	  case 1:
		PACMAN_GAMEDATA.inputDirection = DOWN;
	    break;
	  case 2:
		PACMAN_GAMEDATA.inputDirection = LEFT;
	    break;
	  case 3:
		PACMAN_GAMEDATA.inputDirection = RIGHT;
	    break;

	  default:
	    // code block
	}

}

void Pacman_handleKeypadInput(int timeout){

	switch (KeyPad_WaitForKeyGetChar(timeout)) {
	  case '2':
		PACMAN_GAMEDATA.inputDirection= UP;
	    break;
	  case '8':
		PACMAN_GAMEDATA.inputDirection = DOWN;
	    break;
	  case '4':
		PACMAN_GAMEDATA.inputDirection = LEFT;
	    break;
	  case '6':
		PACMAN_GAMEDATA.inputDirection = RIGHT;
	    break;

	  default:
	    // code block
	}

}
// Public Functions
void Pacman_gameloop(){


	// Handle input direction
	  if(Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, PACMAN_GAMEDATA.inputDirection)){
		  PACMAN_GAMEDATA.prevDirection = PACMAN_GAMEDATA.inputDirection;
	  } else {
		  // Go in orignial direction if cannot go in new directino
		  Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, PACMAN_GAMEDATA.prevDirection);
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


uint8_t Pacman_update(Pacman* pacman, char (*mazeData)[23], Direction direction) {
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
		return 1;
    }
    return 0;
}
