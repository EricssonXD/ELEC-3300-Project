#ifndef	_KEYPADCONFIG_H
#define	_KEYPADCONFIG_H
#include "gpio.h"

#define           _KEYPAD_DEBOUNCE_TIME_MS        20
#define           _KEYPAD_USE_FREERTOS            0

// Copied from main.h
#define Col2_Pin GPIO_PIN_6
#define Col2_GPIO_Port GPIOC
#define Col1_Pin GPIO_PIN_7
#define Col1_GPIO_Port GPIOC
#define Row4_Pin GPIO_PIN_8
#define Row4_GPIO_Port GPIOC
#define Row3_Pin GPIO_PIN_9
#define Row3_GPIO_Port GPIOC
#define Row2_Pin GPIO_PIN_10
#define Row2_GPIO_Port GPIOC
#define Row1_Pin GPIO_PIN_11
#define Row1_GPIO_Port GPIOC
#define Col3_Pin GPIO_PIN_12
#define Col3_GPIO_Port GPIOC
#define Col4_Pin GPIO_PIN_2
#define Col4_GPIO_Port GPIOD

const GPIO_TypeDef* _KEYPAD_COLUMN_GPIO_PORT[] =
{
  Col1_GPIO_Port,
  Col2_GPIO_Port,
  Col3_GPIO_Port,
  Col4_GPIO_Port
};

const uint16_t _KEYPAD_COLUMN_GPIO_PIN[] =
{
	Col1_Pin,
	Col2_Pin,
	Col3_Pin,
	Col4_Pin
};

const GPIO_TypeDef* _KEYPAD_ROW_GPIO_PORT[] =
{
	Row1_GPIO_Port,
	Row2_GPIO_Port,
	Row3_GPIO_Port,
	Row4_GPIO_Port
};

const uint16_t _KEYPAD_ROW_GPIO_PIN[] =
{
	Row1_Pin,
	Row2_Pin,
	Row3_Pin,
	Row4_Pin
};

#endif

