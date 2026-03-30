/*
 * API_debounce.h
 *
 *  Created on: Mar 26, 2026
 *      Author: sebastian pinto
 */

#include "API_delay.h"

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

/**
 * @brief private function to initialize the debouncher.
 * @param void.
 * @return void .
 */
void debounceFSM_init();	// debe cargar el estado inicial

/**
 * @brief private function to read the button value and solve the "state machine" logic.
 * @param void.
 * @return void .
 */
void debounceFSM_update();	// debe leer las entradas, resolver la lógica de
							// transición de estados y actualizar las salidas

/**
 * @brief function to read the case in the state machine.
 * @param void.
 * @return case; return the number of actual case in the state machine .
 */
debounceState_t debounceFSM_getState();

/**
 * @brief function to return a flag in case that the button was pressed.
 * @param void.
 * @return flanco: flag to advice a flank .
 */
bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */
