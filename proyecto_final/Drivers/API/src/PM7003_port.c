#include "PM7003_port.h"
#include "PM7003.h"   // se necesita para poder llamar a PM7003_ProcessByte()


//variable global
uint8_t PM7003_rx_byte;
//Variables internas
UART_HandleTypeDef huart1;
static uint8_t is_initialized = 0;

// FUNCIONES STATIC
/**
 * @brief configuracion de GPIO
 * inicializacion de los pines Rx Y Tx por los que se maneja la comunicacion serial
 * @param None
 * @return None
 */
static void PM7003_GPIO_Init(void);
/**
 * @brief configuracion de UART
 * inicializacion y configuracion de UART segun el datasheet del dispositivo.
 * @param None
 * @return None
 */
static void PM7003_UART_Init(void);

//GPIO INIT
static void PM7003_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // TX -> PA9
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // RX -> PA10
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// UART INIT
static void PM7003_UART_Init(void)
{
    __HAL_RCC_USART1_CLK_ENABLE();

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_RX;   // solo recepción
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&huart1);

}


//---------------------------------------------------------------------
// FUNCIONES GLOBALES
//---------------------------------------------------------------------

// INIT COMPLETO
void PM7003_Port_Init(void)
{
    PM7003_GPIO_Init();
    PM7003_UART_Init();

    is_initialized = 1;

    // Iniciar recepción
    PM7003_Port_StartReception();
}

// START RECEPCIÓN
void PM7003_Port_StartReception(void)
{
    if (is_initialized)
    {
        HAL_UART_Receive_IT(&huart1, &PM7003_rx_byte, 1);
    }
}

// STOP RECEPCIÓN
void PM7003_Port_StopReception(void)
{
    if (is_initialized)
    {
        HAL_UART_AbortReceive_IT(&huart1);
    }
}

// CALLBACK PUENTE
void PM7003_Port_RxCallback(UART_HandleTypeDef *huart)
{

    if (!is_initialized)
        return;
    if (huart == &huart1)
    {
        // Enviar byte al driver
        PM7003_ProcessByte(PM7003_rx_byte);
        PM7003_Port_StartReception();
    }
}

//ESTADO
uint8_t PM7003_Port_IsInitialized(void)
{
    return is_initialized;
}

// FUNCION DE LA HAL QUE MANEJA LA RECEPCION
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//	if (huart == NULL) return;
//    if (huart->Instance == USART1) {
//           // procesamiento
//        	PM7003_Port_RxCallback(huart);
//        	HAL_UART_Receive_IT(&huart1, &PM7003_rx_byte, 1);
//            // Rehabilitar la interrupción esto lo hace la func anterior
//
//        }
//}
