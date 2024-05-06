/*
 * ghost.c
 *
 *  Created on: Apr 27, 2024
 *      Author: Darryl
 */

#include "ghost.h"
#include "lcd.h"
#include "maze.h"
#include <stdio.h>
#include <string.h>

uint16_t stunTimer = 20;
int isStun = 0;
void Ghost_update(Ghost* ghost, Pacman* pacman, char (*mazeData)[23], Position ghostPositions[],  uint16_t color, uint8_t move) {
	if(!move){
		for(int i=0; i<numPacman; i++){
			Pacman* currentPacman = &(MULTI_PACMAN_GAMEDATA.pacmans[i]);
			if(currentPacman->joined){
				if (ghost->curX == currentPacman->curX && ghost->curY == currentPacman->curY){
					pacman->health--;
					pacman->state = DEATH;
					MULTI_PACMAN_GAMEDATA.mazeData[currentPacman->curY][currentPacman->curX] = ' ';
					stunTimer = 10;
					isStun = 1;
				}
			}
		}
	}


	if(isStun == 1){
		if(stunTimer > 0){
			stunTimer--;
			return;
		}
		else{
			isStun = 0;
		}
	}
	if(!move)return;

    int curX = ghost->curX;
    int curY = ghost->curY;
    int pacX = pacman->curX;
    int pacY = pacman->curY;
    int nextX;
	int nextY;
	float distances[4] = {100, 100, 100, 100};

    // List of valid directions
    Direction listDirections[4] = {LEFT, RIGHT, UP, DOWN};
    Direction validDirections[4] = {STOP, STOP, STOP, STOP};
    int numValidDirections = 0;

    // Check each direction
    for (int i = 0; i < 4; i++) {
        Direction dir = listDirections[i];
        nextX = curX;
        nextY = curY;
        if(dir == LEFT){
        	nextX--;
        }
        else if(dir == RIGHT){
        	nextX++;
        }
        else if(dir == UP){
			nextY--;
		}
        else if(dir == DOWN){
			nextY++;
		}

        // Check if the next position is a valid move
        if (mazeData[nextY][nextX] != '#') {
        	//check if next position has a ghost
        	int ghostValid = 1;
        	for (int i=0; i<numGhost-1; i++){
        		int ghostX = ghostPositions[i].x;
        		int ghostY = ghostPositions[i].y;

        		if(ghostX == curX && ghostY == curY){
        			continue;
        		}
        		if(nextX == ghostPositions[i].x && nextY == ghostPositions[i].y){
        			ghostValid = 0;
        		}
        	}

        	if(ghostValid){
				validDirections[numValidDirections] = dir;
				float distance = sqrtf(powf(nextX - pacX, 2) + powf(nextY - pacY, 2));
				distances[numValidDirections] = distance;
				numValidDirections++;
        	}
        }
    }

    // Find the index of the shortest distance
    int index = 0;
    if(ghost->state != FRIGHTENED){
		float shortestDistance = distances[0];
		for (int i = 1; i < numValidDirections; i++) {
			if (distances[i] < shortestDistance) {
				index = i;
				shortestDistance = distances[i];
			}
		}
    }
    else if (ghost->state == FRIGHTENED){
    	float longestDistance = distances[0];
		for (int i = 1; i < numValidDirections; i++) {
			if (distances[i] > longestDistance) {
				index = i;
				longestDistance = distances[i];
			}
		}
    }

    //Update the ghost variables
    Direction optimalDir = validDirections[index];
	for(int i=0; i<numPacman; i++){
		Pacman* currentPacman = &(MULTI_PACMAN_GAMEDATA.pacmans[i]);
		if(currentPacman->joined){
			if (ghost->pastX == currentPacman->curX && ghost->pastY == currentPacman->curY){
				pacman->health--;
				pacman->state = DEATH;
				MULTI_PACMAN_GAMEDATA.mazeData[currentPacman->curY][currentPacman->curX] = ' ';

				stunTimer = 10;
				isStun = 1;
			}
		}
	}
    ghost->pastX = ghost->curX;
    ghost->pastY = ghost->curY;
    ghost->direction = optimalDir;
    if(optimalDir == LEFT){
    	ghost->curX--;
	}
	else if(optimalDir == RIGHT){
		ghost->curX++;
	}
	else if(optimalDir == UP){
		ghost->curY--;
	}
	else if(optimalDir == DOWN){
		ghost->curY++;
	}

    if (curX == mazeTunnelLeftX && curY == mazeTunnelLeftY) {
		ghost->curX = mazeTunnelRightX;
		ghost->curY = mazeTunnelRightY;
    }

	// Check if the ghost is at the right tunnel
	if (curX == mazeTunnelRightX && curY == mazeTunnelRightY) {
		ghost->curX = mazeTunnelLeftX;
		ghost->curY = mazeTunnelLeftY;
	}

	for(int i=0; i<numPacman; i++){
		Pacman* currentPacman = &(MULTI_PACMAN_GAMEDATA.pacmans[i]);
		if(currentPacman->joined){
			if (curX == currentPacman->curX && curY == currentPacman->curY){
				pacman->health--;
				pacman->state = DEATH;
				MULTI_PACMAN_GAMEDATA.mazeData[currentPacman->curY][currentPacman->curX] = ' ';

				stunTimer = 10;
				isStun = 1;
			}
		}
	}

    // Update ghost display
    uint16_t borderStartX = mazeStartX + ghost->curX * gamePixelSize;
	uint16_t borderStartY = mazeStartY + ghost->curY * gamePixelSize;
	LCD_DrawGhost(ghost, borderStartX, borderStartY, 9, color);

	char mazeChar = mazeData[ghost->pastY][ghost->pastX];
	borderStartX = mazeStartX + ghost->pastX * gamePixelSize;
	borderStartY = mazeStartY + ghost->pastY * gamePixelSize;
	LCD_DrawPixel(borderStartX, borderStartY, gamePixelSize, BLACK);
	if(mazeChar == '*'){
		LCD_DrawFood(borderStartX, borderStartY, 3, gamePixelSize, GREEN);
	}
	else if(mazeChar == '@'){
		LCD_DrawBuff(borderStartX, borderStartY, 6, gamePixelSize, YELLOW);
	}
}

void getRelativeGhostsPos(Ghost ghosts[], Position* ghostPositions, Ghost* currentGhost) {
    for (int i = 0; i < numGhost-1; i++) {
        ghostPositions[i].x = ghosts[i].curX;
        ghostPositions[i].y = ghosts[i].curY;
    }
}

void getAllGhostsPos(Ghost ghosts[], Position* ghostPositions) {
	for (int i = 0; i < numGhost; i++) {
		ghostPositions[i].x = ghosts[i].curX;
		ghostPositions[i].y = ghosts[i].curY;
	}
}

void ghostReset(Ghost* ghost, uint16_t color){
	ghost->pastX = ghost->curX;
	ghost->pastY = ghost->curY;
	ghost->curX = respawnX;
	ghost->curY = respawnY;

	uint16_t borderStartX = mazeStartX + ghost->curX * gamePixelSize;
	uint16_t borderStartY = mazeStartY + ghost->curY * gamePixelSize;
	LCD_DrawGhost(ghost, borderStartX, borderStartY, 9, color);

	borderStartX = mazeStartX + ghost->pastX * gamePixelSize;
	borderStartY = mazeStartY + ghost->pastY * gamePixelSize;
	LCD_DrawPixel(borderStartX, borderStartY, gamePixelSize, BLACK);
}
