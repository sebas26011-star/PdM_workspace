/*
 * API_cmdparser.c
 *
 *  Created on: Apr 06, 2026
 *      Author: sebastian pinto
 */
#include "API_cmdparser.h"
#include "API_uart.h"
#include <string.h>
#include <ctype.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

/**
 * @file API_cmdparser.c
 * @brief Implementación de un parser de comandos basado en una MEF.
 *
 * Este módulo permite recibir comandos por UART, procesarlos y ejecutar
 * acciones asociadas, como control de GPIO (LED).
 */

//================== DEFINES ==================//

#define CMD_BUFFER_SIZE 256      ///< Tamaño máximo del buffer de comandos
#define CHAR_CR '\r'             ///< Carriage Return (fin de comando)
#define CHAR_LF '\n'             ///< Line Feed (ignorado)
#define RESET_VALUE 0            ///< Valor de reinicio para variables



//================== CONSTANTES PRIVADAS ==================//

/**
 * @brief Estructura que agrupa los mensajes de salida del sistema.
 */
typedef struct {
    const char* help;
    const char* led_on;
    const char* led_off;
    const char* led_toggle;
    const char* status_on;
    const char* status_off;
    const char* error;
} cmdMessages_t;

/**
 * @brief Comandos válidos del sistema.
 */
static const char* CMD_HELP       = "HELP";
static const char* CMD_LED_ON     = "LED ON";
static const char* CMD_LED_OFF    = "LED OFF";
static const char* CMD_STATUS     = "STATUS";
static const char* CMD_LED_TOGGLE = "LED TOGGLE";

/**
 * @brief Instancia de mensajes asociados a cada comando.
 */
static cmdMessages_t messages = {
    .help       = "Commands: HELP, LED ON, LED OFF, LED TOGGLE, STATUS\r\n",
    .led_on     = "LED ON\r\n",
    .led_off    = "LED OFF\r\n",
    .led_toggle = "LED TOGGLE\r\n",
    .status_on  = "LED is ON\r\n",
    .status_off = "LED is OFF\r\n",
    .error      = "ERROR: unknown command\r\n"
};

//================== TIPOS ==================//

/**
 * @brief Estados de la máquina de estados del parser.
 */
typedef enum {
    CMD_IDLE,        ///< Esperando inicio de comando
    CMD_RECEIVING,   ///< Recibiendo caracteres
    CMD_PROCESS,     ///< Procesando comando recibido
    CMD_EXEC,        ///< Ejecutando comando válido
    CMD_ERROR        ///< Estado de error
} cmd_state_t;

//================== VARIABLES PRIVADAS ==================//

static cmd_state_t state = CMD_IDLE;            ///< Estado actual de la MEF
static uint8_t buffer[CMD_BUFFER_SIZE];         ///< Buffer de recepción de comandos
static uint16_t indx = RESET_VALUE;             ///< Índice del buffer

//================== PROTOTIPOS PRIVADOS ==================//

static uint8_t cmdProcessLine(void);
static void cmdExecute(void);
static void toUpperCase(char *str);

//================== API ==================//

/**
 * @brief Inicializa el parser de comandos.
 */
void cmdInit(void)
{
    state = CMD_IDLE;
    indx = RESET_VALUE;
}

//------------------------------------------------------

/**
 * @brief Ejecuta la máquina de estados del parser.
 *
 * Debe llamarse periódicamente. Se encarga de:
 * - Leer datos desde UART
 * - Construir el comando en un buffer
 * - Procesar y ejecutar comandos válidos
 */
void cmdPoll(void)
{
    uint8_t rx_value = RESET_VALUE;

    // Lectura de un byte desde UART
    if(uartReceiveStringSize(&rx_value, 1))
    {
        switch(state)
        {
            case CMD_IDLE:

                // Ignorar caracteres de fin de línea
                if(rx_value != CHAR_CR && rx_value != CHAR_LF)
                {
                    indx = RESET_VALUE;
                    buffer[indx++] = rx_value;
                    state = CMD_RECEIVING;
                }
                break;

            //--------------------------------------------------

            case CMD_RECEIVING:

                // Fin de comando
                if(rx_value == CHAR_CR)
                {
                    buffer[indx] = '\0';   // finalizar string
                    state = CMD_PROCESS;
                }
                // Agregar carácter al buffer
                else if(rx_value != CHAR_CR)
                {
                    if(indx < CMD_BUFFER_SIZE - 1)
                    {
                        buffer[indx++] = rx_value;
                    }
                    else
                    {
                        state = CMD_ERROR; // overflow
                    }
                }
                break;

            //--------------------------------------------------

            default:
                break;
        }
    }

    // Procesamiento del comando
    if(state == CMD_PROCESS)
    {
        if(cmdProcessLine())
            state = CMD_EXEC;
        else
            state = CMD_ERROR;
    }

    // Ejecución del comando
    if(state == CMD_EXEC)
    {
        cmdExecute();
        indx = RESET_VALUE;
        memset(buffer, RESET_VALUE, CMD_BUFFER_SIZE);
        state = CMD_IDLE;
    }

    // Manejo de error
    if(state == CMD_ERROR)
    {
        uartSendString(messages.error);
        indx = RESET_VALUE;
        memset(buffer, RESET_VALUE, CMD_BUFFER_SIZE);
        state = CMD_IDLE;
    }
}



//================== FUNCIONES PRIVADAS ==================//

/**
 * @brief Valida el comando recibido.
 *
 * Convierte el string a mayúsculas y verifica si coincide con
 * alguno de los comandos definidos.
 *
 * @return 1 si el comando es válido, 0 en caso contrario.
 */
static uint8_t cmdProcessLine(void)
{
    toUpperCase((char*)buffer);

    if(strcmp((char*)buffer, CMD_HELP) == 0) return 1;
    if(strcmp((char*)buffer, CMD_LED_ON) == 0) return 1;
    if(strcmp((char*)buffer, CMD_LED_OFF) == 0) return 1;
    if(strcmp((char*)buffer, CMD_LED_TOGGLE) == 0) return 1;
    if(strcmp((char*)buffer, CMD_STATUS) == 0) return 1;

    return 0;
}

//------------------------------------------------------

/**
 * @brief Ejecuta la acción asociada al comando recibido.
 */
static void cmdExecute(void)
{
    if(strcmp((char*)buffer, CMD_HELP) == 0)
    {
        uartSendString(messages.help);
    }
    else if(strcmp((char*)buffer, CMD_LED_ON) == 0)
    {
        uartSendString(messages.led_on);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    }
    else if(strcmp((char*)buffer, CMD_LED_OFF) == 0)
    {
        uartSendString(messages.led_off);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }
    else if(strcmp((char*)buffer, CMD_LED_TOGGLE) == 0)
    {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        uartSendString(messages.led_toggle);
    }
    else if(strcmp((char*)buffer, CMD_STATUS) == 0)
    {
        GPIO_PinState state = HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin);

        if(state == GPIO_PIN_SET)
        {
            uartSendString(messages.status_on);
        }
        else
        {
            uartSendString(messages.status_off);
        }
    }
}

//------------------------------------------------------

/**
 * @brief Convierte un string a mayúsculas.
 *
 * @param str Puntero al string a modificar.
 */
static void toUpperCase(char *str)
{
    while(*str)
    {
        *str = toupper((unsigned char)*str);
        str++;
    }
}
