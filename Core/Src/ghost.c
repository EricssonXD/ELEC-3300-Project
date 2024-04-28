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

void Ghost_update(Ghost* ghost, Pacman* pacman, char (*mazeData)[23], uint16_t color) {
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
            validDirections[numValidDirections] = dir;
//            numValidDirections++;

            float distance = sqrtf(powf(nextX - pacX, 2) + powf(nextY - pacY, 2));
            distances[numValidDirections] = distance;
            numValidDirections++;

        }
    }

    // Calculate the distance to Pacman for each valid direction
//    float distances[4] = {100, 100, 100, 100};
//    for (int i = 0; i < numValidDirections; i++) {
//    	if()
//        float distance = sqrtf(powf(nextX - pacX, 2) + powf(nextY - pacY, 2));
//        distances[i] = distance;
//    }

    // Find the index of the shortest distance
    int shortestIndex = 0;
    float shortestDistance = distances[0];
    for (int i = 1; i < numValidDirections; i++) {
        if (distances[i] < shortestDistance) {
            shortestIndex = i;
            shortestDistance = distances[i];
        }
    }

    //Update the ghost variables
    Direction optimalDir = validDirections[shortestIndex];
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

    // Update ghost display
    uint16_t borderStartX = mazeStartX + ghost->curX * gamePixelSize;
	uint16_t borderStartY = mazeStartY + ghost->curY * gamePixelSize;
	LCD_DrawGhost(ghost, borderStartX, borderStartY, 9, color);

	borderStartX = mazeStartX + ghost->pastX * gamePixelSize;
	borderStartY = mazeStartY + ghost->pastY * gamePixelSize;
	LCD_DrawPixel(borderStartX, borderStartY, gamePixelSize, BLACK);
}
