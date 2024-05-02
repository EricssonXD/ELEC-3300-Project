/*
 * pacman.h
 *
 *  Created on: Apr 15, 2024
 *      Author: Darryl
 */

#ifndef INC_PACMAN_H_
#define INC_PACMAN_H_

#include <stdint.h>
#include <math.h>
#include "ghost.h"

#define gamePixelSize	10
#define mazeStartX	5
#define mazeStartY	5
#define numGhost	4
#define numPacman	2
#define MAZE_WIDTH 23
#define MAZE_HEIGHT 26

extern int isMulti;

typedef enum {
    NORMAL,
    BUFF,
} State;

typedef struct {
    uint16_t curX;
    uint16_t curY;
    uint16_t pastX;
    uint16_t pastY;
    Direction direction;
    uint16_t score;
    State state;
    uint16_t speed;
    uint16_t health;
} Pacman;

typedef struct
{
	uint8_t gameloopReady; // Realized that this is useless after figuring out the timer, but I'll leave it here just in case
	Pacman pacman;
	Ghost ghosts[numGhost];
	Direction prevDirection;
	Direction inputDirection;
	char scoreString[10];
	char mazeData[MAZE_HEIGHT][MAZE_WIDTH];

} PacmanGameData;

typedef struct
{
	uint8_t gameloopReady; // Realized that this is useless after figuring out the timer, but I'll leave it here just in case
	Pacman pacmans[numPacman];
	Ghost ghosts[numGhost];
	Direction prevDirection;
	Direction inputDirection;
	char scoreString[10];
	char mazeData[MAZE_HEIGHT][MAZE_WIDTH];

} MultiplayerPacmanGameData;


uint8_t Pacman_update(Pacman* pacman, char (*mazeData)[23], Direction direction, Position ghostPositions[]);
void Pacman_gamestart();
void Pacman_gameloop();
void Pacman_gameloop_multi();
void Pacman_handleInput(uint8_t input);
void Pacman_handleKeypadInput(int timeout);

#endif /* INC_PACMAN_H_ */
