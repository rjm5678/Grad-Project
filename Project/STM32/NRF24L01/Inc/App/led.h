#ifndef __LED_H
#define __LED_H

#include "main.h"

#define LED1_RESET 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)
#define LED1_SET 		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)

void LED_Init(void);

#endif
