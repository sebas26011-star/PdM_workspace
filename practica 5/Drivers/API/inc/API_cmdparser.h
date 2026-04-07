/*
 * API_cmdparser.h
 *
 *  Created on: Apr 06, 2026
 *      Author: sebastian pinto
 */
#ifndef API_CMDPARSER_H
#define API_CMDPARSER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file API_cmdparser.h
 * @brief Interfaz del módulo parser de comandos por UART.
 *
 * Este módulo implementa una máquina de estados finitos (MEF)
 * para la recepción, procesamiento y ejecución de comandos
 * ingresados por una interfaz serie.
 */

//================== API PÚBLICA ==================//

/**
 * @brief Inicializa el parser de comandos.
 *
 * Configura las variables internas y el estado inicial
 * de la máquina de estados.
 */
void cmdInit(void);

/**
 * @brief Ejecuta el parser de comandos.
 *
 * Debe llamarse periódicamente (por ejemplo, dentro del while(1)).
 * Se encarga de:
 *  - Leer datos desde la UART
 *  - Construir comandos
 *  - Procesarlos y ejecutarlos
 */
void cmdPoll(void);

#endif /* API_CMDPARSER_H */
