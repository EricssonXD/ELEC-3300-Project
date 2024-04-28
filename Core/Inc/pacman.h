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

typedef enum {
    normal,
    buff,
} State;

typedef struct {
    uint16_t curX;
    uint16_t curY;
    uint16_t pastX;
    uint16_t pastY;
    Direction direction;
    uint16_t score;
    State state;
} Pacman;

typedef struct
{
	Pacman pacman;
	Ghost ghosts[numGhost];
	Direction prevDirection;
	Direction inputDirection;
	char scoreString[10];
	char mazeData[26][23];

} PacmanGameData;


uint8_t Pacman_update(Pacman* pacman, char (*mazeData)[23], Direction direction);
void Pacman_gamestart();
void Pacman_gameloop();
void Pacman_handleInput(uint8_t input);
void Pacman_handleKeypadInput(int timeout);

#endif /* INC_PACMAN_H_ */
