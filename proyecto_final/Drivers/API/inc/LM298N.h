#ifndef LM298N_H
#define LM298N_H

#include <stdint.h>


//  Inicialización
/**
 * @brief Inicializa el módulo del motor
 * hace llamado a la funcion que inicializa el modulo pwm, los GPIO y el timer,
 * necesarios para hacer el control del motor
 */
void Motor_Init(void);


// Control de movimiento

/**
 * @brief Giro Forwar
 * hace llamado a la funcion que controla el giro del motor y lo establece en sentido Forwar
 */
void Motor_Forward(void);

/**
 * @brief Giro Backwar
 * hace llamado a la funcion que controla el giro del motor y lo establece en sentido Backward
 */
void Motor_Backward(void);

/**
 * @brief Detener motor
 * hace llamado a la funcion que ajusta los pines de salida necesarios para detener el motor
 */
void Motor_Stop(void);



// Control de velocidad
/**
 * @brief Ajuste de velocidad del motor
 * @param speed: porcentaje (0–100)
 */
void Motor_SetSpeed(uint8_t speed);
#endif
