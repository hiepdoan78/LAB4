/*
 * task.c
 *
 *  Created on: Nov 22, 2023
 *      Author: Hiepm
 */

#include "task.h"

void turnOffAllLed(){
	HAL_GPIO_WritePin(GPIOA, LED_RED_Pin, SET);
	HAL_GPIO_WritePin(GPIOA, LED_GREEN_Pin, SET);
	HAL_GPIO_WritePin(GPIOA, LED_YELLOW_Pin, SET);
	HAL_GPIO_WritePin(GPIOA, LED_BLUE_Pin, SET);
}
void oneShotTask(){
	HAL_GPIO_TogglePin(GPIOA, LED_RED_Pin | LED_GREEN_Pin | LED_YELLOW_Pin | LED_BLUE_Pin);
}
void task1(){
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
void task2(){
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}
void task3(){
	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
}
void task4(){
	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
}
