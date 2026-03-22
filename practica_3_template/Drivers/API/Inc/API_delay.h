/*
 * API_delay.h
 *
 *  Created on: Mar 20, 2026
 *      Author: sebastian pinto
 */

/**
 * @file API_delay.h
 * @brief functions for handling non-blocking delays.
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include <stdint.h>
#include <stdbool.h>



typedef uint32_t tick_t;
typedef bool bool_t;


typedef struct {
	tick_t startTime;
	tick_t duration;
	bool_t running;
} delay_t;

/**
 * @brief defines the first initialization parameters for the delay structure.
 * @param delay , structure type delay_t.
 * @param duration , value for delay duration, type uint32.
 * @return void.
 */
void delayInit(delay_t *delay, tick_t duration);


/**
 * @brief Check the flag running status and verify if the delay time was met.
 * @param delay , structure type delay_t.
 * @return bool, indicate if time was met or no.
 */
bool_t delayRead(delay_t *delay);


/**
 * @brief modify the duration value in the delay structure.
 * @param delay , structure type delay_t.
 * @param duration , value for delay duration, type uint32.
 * @return void.
 */
void delayWrite(delay_t *delay, tick_t duration);

/**
 * @brief return the running flag state in the delay structure.
 * @param delay , structure type delay_t.
 * @return bool , indicate the state of the delay, if is running or no .
 */
bool_t delaysIsRunning(delay_t * delay);

#endif /* API_INC_API_DELAY_H_ */
