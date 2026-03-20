/*
 * API_delay.h
 *
 *  Created on: Mar 20, 2026
 *      Author: seabstian pinto
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

void delayInit(delay_t *delay, tick_t duration);
bool_t delayRead(delay_t *delay);
void delayWrite(delay_t *delay, tick_t duration);
//void sequence(uint32_t times,uint32_t period,uint16_t duty);
bool_t delaysIsRunning(delay_t * delay);

#endif /* API_INC_API_DELAY_H_ */
