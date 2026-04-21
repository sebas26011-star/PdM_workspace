#include <LM298N_port.h>
#include "LM298N.h"

//---------------------------------------------------------------------
// FUNCIONES GLOBALES
//---------------------------------------------------------------------
//Inicialización
void Motor_Init(void)
{
    Motor_Port_Init();
}

//Control de movimiento

void Motor_Forward(void)
{
    Motor_Port_SetForward();
}

void Motor_Backward(void)
{
    Motor_Port_SetBackward();
}

void Motor_Stop(void)
{
    Motor_Port_Stop();
}

// Control de velocidad
void Motor_SetSpeed(uint8_t speed)
{
    if (speed > 100)
        speed = 100;

    Motor_Port_SetSpeed(speed);
}
