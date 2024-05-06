/*
 * speaker.h
 *
 *  Created on: May 3, 2024
 *      Author: ho
 */

#ifndef INC_SPEAKER_H_
#define INC_SPEAKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "i2s.h"
#include "i2c.h"

void WM8978_Init(void);

void WM8978_Play(uint16_t *data);

#endif /* INC_SPEAKER_H_ */
