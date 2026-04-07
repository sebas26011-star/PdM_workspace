/*
 * API_uart.c
 *
 *  Created on: Apr 02, 2026
 *      Author: sebastian pinto
 */
#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

/**
 * @brief Inicializa el periférico UART.
 *
 * Configura el módulo USART2 con los parámetros definidos en la implementación,
 * incluyendo baudrate, longitud de palabra, paridad y bits de parada.
 * Además, realiza la configuración de los pines GPIO asociados.
 *
 * @return true  Si la inicialización fue exitosa.
 * @return false Si ocurrió un error durante la inicialización.
 */
bool uartInit(void);

/**
 * @brief Envía una cadena de caracteres por UART.
 *
 * Transmite un string completo terminado en carácter nulo ('\0').
 * La función calcula automáticamente la longitud del string.
 *
 * @param pstring Puntero al string a transmitir.
 */
void uartSendString(const char * pstring);

/**
 * @brief Envía una cantidad específica de datos por UART.
 *
 * Transmite un número fijo de bytes a través de la UART.
 * No requiere que el buffer esté terminado en '\0'.
 *
 * @param pstring Puntero al buffer de datos a transmitir.
 * @param size Cantidad de bytes a enviar.
 */
void uartSendStringSize(const char * pstring, uint16_t size);

/**
 * @brief Recibe una cantidad específica de datos por UART.
 *
 * Realiza una lectura bloqueante de la UART hasta recibir la cantidad
 * de bytes indicada o hasta que ocurra un timeout interno.
 *
 * @param pstring Puntero al buffer donde se almacenarán los datos recibidos.
 * @param size Cantidad de bytes a recibir.
 *
 * @return true  Si la recepción fue exitosa.
 * @return false Si ocurrió un error o timeout.
 */
bool uartReceiveStringSize(uint8_t * pstring, uint16_t size);

#endif /* API_INC_API_UART_H_ */
