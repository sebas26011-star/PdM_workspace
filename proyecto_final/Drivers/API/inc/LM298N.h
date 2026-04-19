#ifndef LM298N_H
#define LM298N_H

#include <stdint.h>


//  Inicialización
/**
 * @brief Inicializa el módulo del motor
 * llama internamente al port
 */
void Motor_Init(void);


// Control de movimiento

/**
 * @brief Hace girar el motor hacia adelante
 */
void Motor_Forward(void);

/**
 * @brief Hace girar el motor hacia atrás
 */
void Motor_Backward(void);

/**
 * @brief Detiene el motor
 */
void Motor_Stop(void);



// Control de velocidad
/**
 * @brief Ajusta la velocidad del motor
 * @param speed: porcentaje (0–100)
 */
void Motor_SetSpeed(uint8_t speed);
#endif
