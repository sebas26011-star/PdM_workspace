#ifndef PM7003_PORT_H
#define PM7003_PORT_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//Inicialización del puerto
/**
 * @brief Inicializa el UART usado por el PM7003
 * Configura:
 *  - GPIO (TX/RX)
 *  - UART (9600, 8N1)
 *  - Interrupción RX
 */
void PM7003_Port_Init(void);


// Callback de recepción UART
/**
 * @brief Callback que debe llamarse desde HAL_UART_RxCpltCallback
 * Maneja:
 *  - Lectura de byte recibido
 *  - Envío al driver (PM7003_ProcessByte)
 *  - Rearme de la interrupción
 *
 * @param huart: UART que generó la interrupción
 */
void PM7003_Port_RxCallback(UART_HandleTypeDef *huart);


// Control de recepción

/**
 * @brief Inicia la recepción por interrupciones
 */
void PM7003_Port_StartReception(void);

/**
 * @brief Detiene la recepción UART
 */
void PM7003_Port_StopReception(void);

// Estado (opcional pero útil)
/**
 * @brief Indica si el puerto está inicializado
 * @return 1 = listo, 0 = no inicializado
 */
uint8_t PM7003_Port_IsInitialized(void);


#endif
