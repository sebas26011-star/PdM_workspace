/*
 * API_dealy.c
 *
 *  Created on: Mar 20, 2026
 *      Author: sebastian pinto
 */

/**
 * @file API_delay.c
 * @brief Implementation of functions for handling non-blocking delays.
 */

/////

#include "API_delay.h"
#include <stddef.h>
#include "stm32f4xx_hal.h"


/////

void delayInit(delay_t* delay, tick_t duration)
{
  if(delay == NULL) { return; }
  delay->duration = duration;
  delay->startTime = 0;
  delay->running = false;
}

/////

bool_t delayRead(delay_t *delay)
{
 /// verify that pointer to delay is valid
  if(delay == NULL ) { return false; }
  if(delay->running) {
    if(HAL_GetTick() - delay->startTime >= delay->duration) {
      delay->running = false;
      return true;
    }
  } else {
    delay->startTime = HAL_GetTick();
    delay->running = true;
  }
  return false;
}

/////
void delayWrite(delay_t* delay, tick_t duration)
{
  if(delay == NULL ) { return;}
  delay->duration = duration;
}

/////
bool_t delaysIsRunning(delay_t * delay)
{
	 if(delay == NULL ) { return;}
	return delay->running;

}


