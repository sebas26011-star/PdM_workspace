#ifndef MOTOR_PORT_H
#define MOTOR_PORT_H

#include <stdint.h>


// Inicialización
void Motor_Port_Init(void);

// Dirección
void Motor_Port_SetForward(void);
void Motor_Port_SetBackward(void);
void Motor_Port_Stop(void);

// Velocidad (0–100%)
void Motor_Port_SetSpeed(uint8_t duty);

#endif
