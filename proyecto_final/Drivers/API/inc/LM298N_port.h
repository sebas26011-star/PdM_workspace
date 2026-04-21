#ifndef LM298N_PORT_H
#define LM298N_PORT_H

#include <stdint.h>


// Inicialización

/**
 * @brief Inicializa del modulo PWM y GPIO para el control de LM298N
 * Configura el periférico según los parámetros del datasheet
 */
void Motor_Port_Init(void);

// Dirección
/**
 * @brief configuracion de pines de giro de motor
 * encargada de ajustar los pines de salida de tal forma que el motor gire como ventilador
 * @param None
 * @return None
 */
void Motor_Port_SetForward(void);

/**
 * @brief configuracion de pines de giro de motor
 * encargada de ajustar los pines de salida de tal forma que el motor gire como extractor
 * @param None
 * @return None
 */
void Motor_Port_SetBackward(void);

/**
 * @brief Detener giro del motor
 * encargada de resetear los pines de salida para detener el motor
 * @param None
 * @return None
 */
void Motor_Port_Stop(void);

// Velocidad (0–100%)
/**
 * @brief ajuste de velocidad del motor
 * Configura el ciclo de dureza del pwm ajustando asi la velocidad del motor
 * @param duty, valor del ciclo de dureza (0-100)
 * @return None
 */
void Motor_Port_SetSpeed(uint8_t duty);

#endif
