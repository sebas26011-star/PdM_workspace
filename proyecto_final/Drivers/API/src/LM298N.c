#include <LM298N_port.h>
#include "LM298N.h"


static const uint8_t DUTY_MIN = 25; // valor de duty que ni aun dandole un asistente de arranque se logra que el motor gire.
static const uint8_t DUTY_MIN_NO_KICK = 40; // minimo valor de duty para que el motor funcione sin asistencia de arranque.

static const uint32_t DELAY = 200;

static void kick_start(void);

/**
 * @brief patada de arranque al motor.
 * si el duty no es lo sufucientemente alto para arrancar el motor, este envia una velocidad mayor
 * por un timepo definido y luego la cambia a la velocidad establecida por el usuario
 * pdt. tener cuidado con su uso porque aunque da arranque al motor a bajas velocidades, es posible que el motor se detenga en cualquier
 * momento.
 *
 */
static void kick_start(void){
	Motor_Port_SetSpeed(DUTY_MIN_NO_KICK);
	Motor_port_delay(DELAY);
}

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
    if (speed > 100){
        speed = 100;}
    else if(DUTY_MIN_NO_KICK >= speed >= DUTY_MIN){
    	kick_start();
    }
    else if (speed < DUTY_MIN){
    	speed = 0;
    }

    Motor_Port_SetSpeed(speed);
}
