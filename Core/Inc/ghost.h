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

typedef struct {
    uint16_t curX;
    uint16_t curY;
    uint16_t pastX;
    uint16_t pastY;
    Direction direction;
} Ghost;

//void Ghost_update(Ghost* ghost, Pacman* pacman, char (*mazeData)[23]);

#endif /* INC_GHOST_H_ */
