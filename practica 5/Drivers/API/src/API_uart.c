/*
 * API_uart.c
 *
 *  Created on: Apr 02, 2026
 *      Author: sebastian pinto
 */
#include "API_uart.h"

/**
 * @brief tamano maximo del string.
 */
#define UART_MAX_SIZE 256
/**
 * @brief Handle privado de la UART utilizada (USART2).
 */
static UART_HandleTypeDef huart2;

/**
 * @brief Calcula la longitud de un string.
 *
 * Recorre el string hasta encontrar el carácter nulo ('\0')
 * o hasta alcanzar un tamaño máximo de seguridad.
 *
 * @param pstring Puntero al string.
 * @return Longitud del string.
 */
static uint16_t uartStringLength(const char * pstring);


bool uartInit(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart2.Instance = USART2;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.BaudRate = 115200;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    return (HAL_UART_Init(&huart2) == HAL_OK);
}


/**
 * @brief Envía un string completo por UART.
 */
void uartSendString(const char * pstring)
{
    if (pstring == NULL) return;

    uint16_t len = uartStringLength(pstring);

    if(len == 0 || len > UART_MAX_SIZE) return;

    HAL_UART_Transmit(&huart2, (uint8_t*)pstring, len, HAL_MAX_DELAY);
}


/**
 * @brief Envía una cantidad fija de bytes por UART.
 */
void uartSendStringSize(const char * pstring, uint16_t size)
{
    if(pstring == NULL) return;
    if(size == 0 || size > UART_MAX_SIZE) return;

    HAL_UART_Transmit(&huart2, (uint8_t*)pstring, size, HAL_MAX_DELAY);
}


/**
 * @brief Recibe datos por UART.
 */
bool uartReceiveStringSize(uint8_t *pstring, uint16_t size)
{
    if(pstring == NULL) return false;
    if(size == 0 || size > UART_MAX_SIZE) return false;

    return (HAL_UART_Receive(&huart2, pstring, size, 10) == HAL_OK);
}


/**
 * @brief Calcula la longitud de un string (máx 256).
 */
static uint16_t uartStringLength(const char * pstring)
{
    uint16_t len = 0;

    while(pstring[len] != '\0' && len < UART_MAX_SIZE)
    {
        len++;
    }

    return len;
}
