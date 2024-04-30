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
#include "tim.h"


PacmanGameData PACMAN_GAMEDATA = {.gameloopReady = 0};

// Private functions

/*
Start the gameloop timer
The timer runs in interrupt, so if the lcd display is too slow it will interrupt the lcd update
Therefore, if there are anything that takes a long time to run and should not be interrupted by game loop, please use stopGameLoopTimer()
*/
void startGameloopTimer(){
    HAL_TIM_Base_Start_IT(&htim6);
}

// See startGameLoopTimer()
void stopGameloopTimer(){
    HAL_TIM_Base_Stop_IT(&htim6);

}
void setMaze(const void * maze){
	memcpy(PACMAN_GAMEDATA.mazeData, maze, sizeof (char) * MAZE_HEIGHT * MAZE_WIDTH);
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

uint16_t buffTimer = 20;
// Public Functions
void Pacman_gameloop(){
	uint16_t ghostColors[numGhost] = {RED, MAGENTA, CYAN, GREEN};
	if(PACMAN_GAMEDATA.gameloopReady != 1) return;

	if(checkWin(PACMAN_GAMEDATA.mazeData) == 1){
		LCD_DrawString_Color (0, 300, "YOU WIN!", BLACK, YELLOW);
		return;
	}

	Position ghostPositions[numGhost];
	for(int i=0; i<numGhost; i++){
		getAllGhostsPos(PACMAN_GAMEDATA.ghosts, ghostPositions);
	}

	// Handle input direction
	if(Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, PACMAN_GAMEDATA.inputDirection, ghostPositions)){
	  PACMAN_GAMEDATA.prevDirection = PACMAN_GAMEDATA.inputDirection;
	} else {
	  // Go in original direction if cannot go in new direction
	  Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, PACMAN_GAMEDATA.prevDirection, ghostPositions);
	}

	for(int i=0; i<numGhost; i++){
		Ghost* currentGhost = &(PACMAN_GAMEDATA.ghosts[i]);
		Ghost_update(currentGhost, &PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, ghostPositions, ghostColors[i]);
	}

	char scoreDisplay[20];
	sprintf(PACMAN_GAMEDATA.scoreString, "%d", PACMAN_GAMEDATA.pacman.score);
	strcpy(scoreDisplay, "Score: ");
	strcat(scoreDisplay, PACMAN_GAMEDATA.scoreString);
	LCD_DrawString_Color (0, 280, scoreDisplay, BLACK, YELLOW);
}

void Pacman_gamestart(){
	// Make sure the gameloop timer is stopped
	stopGameloopTimer();
	// Starts the pacman game
	setMaze(MAZE1);
	LCD_Clear(0, 0, 240, 320, BLACK);
  
	initMaze(mazeStartX, mazeStartY, PACMAN_GAMEDATA.mazeData, &PACMAN_GAMEDATA.pacman, &PACMAN_GAMEDATA.ghosts);

	// Tells the code the the game is initialized and gameloop will start to be called
	startGameloopTimer();
	PACMAN_GAMEDATA.gameloopReady = 1;
}


uint8_t Pacman_update(Pacman* pacman, char (*mazeData)[23], Direction direction, Position ghostPositions[]) {
	uint16_t pacmanColor = YELLOW;

	uint16_t curX = pacman->curX;
	uint16_t curY = pacman->curY;

	pacman->pastX = pacman->curX;
	pacman->pastY = pacman->curY;

	char mazeChar;

    switch (direction) {
        case LEFT:
        	mazeChar = mazeData[curY][curX - 1];
        	if(mazeChar == 'L'){
        		pacman->curX = mazeTunnelRightX;
        		pacman->curY = mazeTunnelRightY;
        	}
        	else if(mazeChar != '#'){
        		pacman->curX--;
        		pacman->direction = LEFT;
        	}
        	else{
        		pacman->direction = STOP;
        	}
            break;
        case RIGHT:
            mazeChar = mazeData[curY][curX + 1];
            if(mazeChar == 'R'){
				pacman->curX = mazeTunnelLeftX;
				pacman->curY = mazeTunnelLeftY;
			}
            else if(mazeChar != '#' ){
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
    	pacman->state = BUFF;
    	mazeData[pacman->curY][pacman->curX] = ' ';
    	buffTimer = 20;
    }

    if(pacman->state == BUFF){
    	if(buffTimer > 0){
			pacmanColor = WHITE;

			uint16_t ghostColors[numGhost] = {RED, MAGENTA, CYAN, GREEN};
			for (int i=0; i<numGhost; i++){
				if(pacman->curX == ghostPositions[i].x && pacman->curY == ghostPositions[i].y){
					pacman->score += 100;
					Ghost* currentGhost = &(PACMAN_GAMEDATA.ghosts[i]);
					ghostReset(currentGhost, ghostColors[i]);
				}
			}
			buffTimer--;
    	}
    	else{
    		pacman->state == NORMAL;
    		pacmanColor = YELLOW;
    	}
    }

    if(pacman->direction != STOP){
    	uint16_t borderStartX = mazeStartX + pacman->curX * gamePixelSize;
		uint16_t borderStartY = mazeStartY + pacman->curY * gamePixelSize;
		LCD_DrawPacman(pacman, borderStartX, borderStartY, 9, pacmanColor);

    	borderStartX = mazeStartX + pacman->pastX * gamePixelSize;
		borderStartY = mazeStartY + pacman->pastY * gamePixelSize;
		LCD_DrawPixel(borderStartX, borderStartY, gamePixelSize, BLACK);
		return 1;
    }
    return 0;
}

int checkWin(char (*mazeData)[23]){
	int mazeWidth = 23;
	int mazeHeight = 26;
	int x, y;
	int isWin = 1;
	for (y = 0; y < mazeHeight; y++) {
		for (x = 0; x < mazeWidth; x++) {
			char mazeChar = mazeData[y][x];
			if(mazeChar == '*'){
				isWin = 0;
				break;
			}
		}
	}
	return isWin;
}
