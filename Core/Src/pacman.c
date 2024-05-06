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
MultiplayerPacmanGameData MULTI_PACMAN_GAMEDATA = {.gameloopReady = 0, .playerNum = 1};
int isMulti = 2;
int ghostUpdate = 0;
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

void Pacman_handleInput(uint8_t player, uint8_t input){
	if(isMulti == 1){
		switch (input) {
		  case 0:
			  MULTI_PACMAN_GAMEDATA.pacmans[player].inputDirection = UP;
		    break;
		  case 1:
			  MULTI_PACMAN_GAMEDATA.pacmans[player].inputDirection  = DOWN;
		    break;
		  case 2:
			  MULTI_PACMAN_GAMEDATA.pacmans[player].inputDirection  = LEFT;
		    break;
		  case 3:
			  MULTI_PACMAN_GAMEDATA.pacmans[player].inputDirection  = RIGHT;
		    break;
		}
	} else {
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
	}
	}

}

void Pacman_handleKeypadInput(int timeout){

	if(isMulti == 1){
		switch (KeyPad_WaitForKeyGetChar(timeout)) {
		  case '2':
			 MULTI_PACMAN_GAMEDATA.pacmans[0].inputDirection = UP;
		    break;
		  case '8':
			  MULTI_PACMAN_GAMEDATA.pacmans[0].inputDirection  = DOWN;
		    break;
		  case '4':
			  MULTI_PACMAN_GAMEDATA.pacmans[0].inputDirection  = LEFT;
		    break;
		  case '6':
			  MULTI_PACMAN_GAMEDATA.pacmans[0].inputDirection  = RIGHT;
		    break;

		}
	} else {


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
		  break;
	}
	}
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


uint16_t buffTimer = 20;
uint16_t frightenedTimer = 20;
// Public Functions
void Pacman_handle_gameloop(){
	if(isMulti == 0){
		Pacman_gameloop();
	}
	else if(isMulti == 1){
		Pacman_gameloop_multi();
	}
}

void Pacman_gameloop(){
	uint16_t ghostColors[numGhost] = {RED, MAGENTA, CYAN, GREEN};
	if(PACMAN_GAMEDATA.gameloopReady != 1) return;

	if(checkWin(PACMAN_GAMEDATA.mazeData) == 1){
		LCD_DrawString_Color (0, 292, "YOU WIN!", BLACK, YELLOW);
		return;
	}
	if(PACMAN_GAMEDATA.pacman.health <= 0){
		LCD_DrawString_Color (0, 292, "YOU LOSE!", BLACK, YELLOW);
	return;
	}

	Position ghostPositions[numGhost];;
	getAllGhostsPos(PACMAN_GAMEDATA.ghosts, ghostPositions);

	// Handle input direction
	if(Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, PACMAN_GAMEDATA.inputDirection, ghostPositions)){
	  PACMAN_GAMEDATA.prevDirection = PACMAN_GAMEDATA.inputDirection;
	} else {
	  // Go in original direction if cannot go in new direction
	  Pacman_update(&PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, PACMAN_GAMEDATA.prevDirection, ghostPositions);
	}

	Position ghostRelativePositions[numGhost-1];
	if(ghostUpdate == 1){

	for(int i=0; i<numGhost; i++){
		Ghost* currentGhost = &(PACMAN_GAMEDATA.ghosts[i]);
		getRelativeGhostsPos(PACMAN_GAMEDATA.ghosts, ghostRelativePositions, currentGhost);
		Ghost_update(currentGhost, &PACMAN_GAMEDATA.pacman, PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
	}
		ghostUpdate = 0;
	} else {
		ghostUpdate = 1;
	}


	char healthDisplay[20];
	sprintf(healthDisplay, "Health: %d", PACMAN_GAMEDATA.pacman.health);
	LCD_DrawString_Color (0, 265, healthDisplay, BLACK, YELLOW);

	char scoreDisplay[20];
	sprintf(PACMAN_GAMEDATA.scoreString, "%d", PACMAN_GAMEDATA.pacman.score);
	strcpy(scoreDisplay, "Score: ");
	strcat(scoreDisplay, PACMAN_GAMEDATA.scoreString);
	LCD_DrawString_Color (0, 279, scoreDisplay, BLACK, YELLOW);
}

void Pacman_gameloop_multi(){
	uint16_t ghostColors[numGhost] = {RED, MAGENTA, CYAN, GREEN};
	if(MULTI_PACMAN_GAMEDATA.gameloopReady != 1) return;

	Pacman* Pacman1 = &(MULTI_PACMAN_GAMEDATA.pacmans[0]);
	Pacman* Pacman2 = &(MULTI_PACMAN_GAMEDATA.pacmans[1]);
	Pacman* Pacman3 = &(MULTI_PACMAN_GAMEDATA.pacmans[2]);
	Pacman* Pacman4 = &(MULTI_PACMAN_GAMEDATA.pacmans[3]);

	for(int i=0; i<numPacman; i++){
		Pacman* currentPacman = &(MULTI_PACMAN_GAMEDATA.pacmans[i]);
		if(currentPacman->joined){
			if(currentPacman->health <= 0){
				currentPacman->curX = 99;
				currentPacman->curY = 99;
				currentPacman->state = DEATH;
			}
		}
	}


	if(checkWin(MULTI_PACMAN_GAMEDATA.mazeData) == 1){
		stopGameloopTimer();
		int playerId = -1;
		int maxScore = -1;
		for(int i=0; i<numPacman; i++){
			if(maxScore < MULTI_PACMAN_GAMEDATA.pacmans[i].score){
				playerId = i;
				maxScore = MULTI_PACMAN_GAMEDATA.pacmans[i].score;
			}
		}
		char winDisplay[20];
		LCD_Clear(0,0,LCD_Default_Max_COLUMN, LCD_Default_Max_PAGE, BLACK);
		sprintf(winDisplay, "Pacman %d Won!", playerId + 1);
		LCD_DrawString_Color (100, 140, winDisplay, BLACK, YELLOW);
		char scoreDisplay[20];

		// Pacman1 Stats display
		if(Pacman1->joined){
		sprintf(scoreDisplay, "P1: %d", Pacman1->score);
		LCD_DrawString_Color (0, 265, scoreDisplay, BLACK, YELLOW);
		memset(scoreDisplay, 0, sizeof(scoreDisplay));

		}
		if(Pacman2->joined){
		sprintf(scoreDisplay, "P2: %d", Pacman2->score);
		LCD_DrawString_Color (150, 265, scoreDisplay, BLACK, YELLOW);
		memset(scoreDisplay, 0, sizeof(scoreDisplay));
		}
		if(Pacman3->joined){
		sprintf(scoreDisplay, "P3: %d", Pacman3->score);
		LCD_DrawString_Color (0, 295, scoreDisplay, BLACK, YELLOW);
		memset(scoreDisplay, 0, sizeof(scoreDisplay));
		}
		if(Pacman4->joined){
		sprintf(scoreDisplay, "P4: %d", Pacman4->score);
		LCD_DrawString_Color (150, 295, scoreDisplay, BLACK, YELLOW);
		memset(scoreDisplay, 0, sizeof(scoreDisplay));
		}
		return;
	}

	Position ghostPositions[numGhost];;
	getAllGhostsPos(MULTI_PACMAN_GAMEDATA.ghosts, ghostPositions);

	// Handle input direction
	for(int i=0; i<numPacman; i++){
		Pacman* currentPacman = &(MULTI_PACMAN_GAMEDATA.pacmans[i]);
		if(currentPacman->joined){
			if(Pacman_update(currentPacman, MULTI_PACMAN_GAMEDATA.mazeData, currentPacman->inputDirection, ghostPositions)){
				currentPacman->prevDirection = currentPacman->inputDirection;
			} else {
			  // Go in original direction if cannot go in new direction
			  Pacman_update(currentPacman, MULTI_PACMAN_GAMEDATA.mazeData, currentPacman->prevDirection, ghostPositions);
			}
		}
	}

	int numPacmanLeft = 0;
	int pacmanLeftIndex[numPacman];
	for(int i=0; i<numPacman; i++){
		Pacman* currentPacman = &(MULTI_PACMAN_GAMEDATA.pacmans[i]);
		if(currentPacman->health > 0){
			pacmanLeftIndex[numPacmanLeft] = i;
			numPacmanLeft++;
		}
	}

	if(ghostUpdate){
		ghostUpdate = 0;
		Position ghostRelativePositions[numGhost-1];
		if(numPacmanLeft == 1){
			Pacman* currentPacman = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[0]]);
			for(int i=0; i<numGhost; i++){
				Ghost* currentGhost = &(MULTI_PACMAN_GAMEDATA.ghosts[i]);
				getRelativeGhostsPos(MULTI_PACMAN_GAMEDATA.ghosts, ghostRelativePositions, currentGhost);
				Ghost_update(currentGhost, currentPacman, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
			}
		}
		else if(numPacmanLeft == 2){
			Pacman* currentPacman1 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[0]]);
			Pacman* currentPacman2 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[1]]);
			for(int i=0; i<numGhost; i++){
				Ghost* currentGhost = &(MULTI_PACMAN_GAMEDATA.ghosts[i]);
				getRelativeGhostsPos(MULTI_PACMAN_GAMEDATA.ghosts, ghostRelativePositions, currentGhost);
				if(i<2){
					Ghost_update(currentGhost, currentPacman1, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
				else{
					Ghost_update(currentGhost, currentPacman2, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
			}
		}
		else if(numPacmanLeft == 3){
			Pacman* currentPacman1 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[0]]);
			Pacman* currentPacman2 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[1]]);
			Pacman* currentPacman3 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[2]]);
			for(int i=0; i<numGhost; i++){
				Ghost* currentGhost = &(MULTI_PACMAN_GAMEDATA.ghosts[i]);
				getRelativeGhostsPos(MULTI_PACMAN_GAMEDATA.ghosts, ghostRelativePositions, currentGhost);
				if(i==1){
					Ghost_update(currentGhost, currentPacman1, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
				else if(i==2){
					Ghost_update(currentGhost, currentPacman2, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
				else{
					Ghost_update(currentGhost, currentPacman3, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
			}
		}
		else if(numPacmanLeft == 4){
			Pacman* currentPacman1 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[0]]);
			Pacman* currentPacman2 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[1]]);
			Pacman* currentPacman3 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[2]]);
			Pacman* currentPacman4 = &(MULTI_PACMAN_GAMEDATA.pacmans[pacmanLeftIndex[2]]);
			for(int i=0; i<numGhost; i++){
				Ghost* currentGhost = &(MULTI_PACMAN_GAMEDATA.ghosts[i]);
				getRelativeGhostsPos(MULTI_PACMAN_GAMEDATA.ghosts, ghostRelativePositions, currentGhost);
				if(i==1){
					Ghost_update(currentGhost, currentPacman1, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
				else if(i==2){
					Ghost_update(currentGhost, currentPacman2, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
				else if(i==3){
					Ghost_update(currentGhost, currentPacman3, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
				else{
					Ghost_update(currentGhost, currentPacman4, MULTI_PACMAN_GAMEDATA.mazeData, ghostRelativePositions, ghostColors[i]);
				}
			}
		}
	}
	else{
		ghostUpdate = 1;
	}

	char healthDisplay[20];
	char scoreDisplay[20];

	// Pacman1 Stats display
	if(Pacman1->joined){
	sprintf(scoreDisplay, "P1: %d", Pacman1->score);
	LCD_DrawString_Color (0, 265, scoreDisplay, BLACK, YELLOW);
	memset(scoreDisplay, 0, sizeof(scoreDisplay));

	}
	if(Pacman2->joined){
	sprintf(scoreDisplay, "P2: %d", Pacman2->score);
	LCD_DrawString_Color (150, 265, scoreDisplay, BLACK, YELLOW);
	memset(scoreDisplay, 0, sizeof(scoreDisplay));
	}
	if(Pacman3->joined){
	sprintf(scoreDisplay, "P3: %d", Pacman3->score);
	LCD_DrawString_Color (0, 295, scoreDisplay, BLACK, YELLOW);
	memset(scoreDisplay, 0, sizeof(scoreDisplay));
	}
	if(Pacman4->joined){
	sprintf(scoreDisplay, "P4: %d", Pacman4->score);
	LCD_DrawString_Color (150, 295, scoreDisplay, BLACK, YELLOW);
	memset(scoreDisplay, 0, sizeof(scoreDisplay));
	}
}

void Pacman_gamestart(const void * maze, int isMulti){
	// Make sure the gameloop timer is stopped
	stopGameloopTimer();
	// Starts the pacman game
	//Single player
	if(!isMulti){
		memcpy(PACMAN_GAMEDATA.mazeData, maze, sizeof (char) * MAZE_HEIGHT * MAZE_WIDTH);
		LCD_Clear(0, 0, 240, 320, BLACK);
		initSingleMaze(mazeStartX, mazeStartY, PACMAN_GAMEDATA.mazeData, &PACMAN_GAMEDATA.pacman, &PACMAN_GAMEDATA.ghosts);
		startGameloopTimer();
		PACMAN_GAMEDATA.gameloopReady = 1;
	}
	//Multi player
	else{
		memcpy(MULTI_PACMAN_GAMEDATA.mazeData, maze, sizeof (char) * MAZE_HEIGHT * MAZE_WIDTH);
		LCD_Clear(0, 0, 240, 320, BLACK);
		initMultiMaze(mazeStartX, mazeStartY, MULTI_PACMAN_GAMEDATA.mazeData, &MULTI_PACMAN_GAMEDATA.pacmans, &MULTI_PACMAN_GAMEDATA.ghosts);
		startGameloopTimer();
		MULTI_PACMAN_GAMEDATA.gameloopReady = 1;
	}

}


uint8_t Pacman_update(Pacman* pacman, char (*mazeData)[23], Direction direction, Position ghostPositions[]) {
	if(pacman->state == DEATH){
		return 0;
	}

	uint16_t pacmanColor = YELLOW;

	uint16_t curX = pacman->curX;
	uint16_t curY = pacman->curY;

//	mazeData[pacman->pastY ][pacman->pastX] = ' ';
	mazeData[curY][curX] = ' ';

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
        	else if(mazeChar != '#' && mazeChar != 'P'){
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
            else if(mazeChar != '#' && mazeChar != 'P'){
            	pacman->curX++;
            	pacman->direction = RIGHT;
			}
			else{
				pacman->direction = STOP;
			}
            break;
        case UP:
        	mazeChar = mazeData[curY - 1][curX];
        	if(mazeChar != '#' && mazeChar != 'P'){
        		pacman->curY--;
				pacman->direction = UP;
			}
			else{
				pacman->direction = STOP;
			}
            break;
        case DOWN:
        	mazeChar = mazeData[curY + 1][curX];
			if(mazeChar != '#' && mazeChar != 'P'){
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
    	mazeData[pacman->curY ][pacman->curX] = 'P';


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
    	for (int i=0; i<numGhost; i++){
    		Ghost* currentGhost = &(PACMAN_GAMEDATA.ghosts[i]);
    		currentGhost->state = FRIGHTENED;
    	}
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
    		pacman->state = NORMAL;
    		pacmanColor = YELLOW;
    		for (int i=0; i<numGhost; i++){
				Ghost* currentGhost = &(PACMAN_GAMEDATA.ghosts[i]);
				currentGhost->state = REGULAR;
			}
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

