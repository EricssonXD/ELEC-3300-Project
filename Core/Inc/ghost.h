/*
 * ghost.h
 *
 *  Created on: Apr 27, 2024
 *      Author: Darryl
 */

#ifndef INC_GHOST_H_
#define INC_GHOST_H_

#include <stdint.h>
#include <math.h>

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
	STOP
} Direction;

typedef enum {
    REGULAR,
	FRIGHTENED,
} GhostState;

typedef struct {
    uint8_t x;
    uint8_t y;
} Position;

typedef struct {
    uint16_t curX;
    uint16_t curY;
    uint16_t pastX;
    uint16_t pastY;
    Direction direction;
    uint16_t speed;
    GhostState state;
} Ghost;

//void Ghost_update(Ghost* ghost, Pacman* pacman, char (*mazeData)[23]);

void getRelativeGhostsPos(Ghost ghosts[], Position* ghostPositions, Ghost* currentGhost);
void getAllGhostsPos(Ghost ghosts[], Position* ghostPositions);
void ghostReset(Ghost* ghost, uint16_t color);

#endif /* INC_GHOST_H_ */
