#include "stm32f4xx_hal.h"
#include "PM7003_port.h" // se necesita para obtener la funcion que gestiona la recepcion.

// extern
extern UART_HandleTypeDef huart1; // UART (definido en el archivo port de PM7003)

// CALLBACK GLOBAL
/*aqui redefinimos la funcion que maneja la recepcion por interrupcion de los puertos UART,
 * en caso de necesitar utilizar otro modulo UART, se puede gestionar su interrupcion en este mismo
 * archivo
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == NULL) return;
    if (huart->Instance == USART1)
    {
        // enviar byte al módulo correspondiente
    	PM7003_Port_RxCallback(huart);
    }
}
