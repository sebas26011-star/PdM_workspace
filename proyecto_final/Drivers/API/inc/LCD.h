#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

// Inicialización
/**
 * @brief Inicializacion del lcd
 * envia los comandos para que el lcd trabaje en modo de 4 bits y 2 lineas
 * @param hi2c: puntero al handler I2C
 */
void LCD_Init(void);


//Control básico
/**
 * @brief Limpia la pantalla
 * borra todo lo que este escrito en la pantalla
 */
void LCD_Clear(void);

/**
 * @brief Retornar el cursor al inicio
 * retorna el cursor a la posicion 0,0
 */
void LCD_Home(void);

/**
 * @brief limpia una linea completa del LCD
 * escribe un caracter "" en cada espacio de la linea seleccionada
 * @param row, numero de la linea que se quiere limpiar
 */
void LCD_ClearLine(uint8_t row);

// Posicionamiento
/**
 * @brief Posicionamiento del cursor
 * espablece el cursor en un espacio especifico del LCD
 * @param row: fila
 * @param col: columna
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

// Escritura
/**
 * @brief Escribe un caracter ASCII
 * @param ch: caracter a escribir
 */
void LCD_WriteChar(char ch);

/**
 * @brief Escribe un string completo
 * @param str: puntero al string
 */
void LCD_WriteString(char *str);

// Configuración display
/**
 * @brief Enciende o apaga el display
 * @param state, 1=dispaly ON, 0=dispaly OFF
 */
void LCD_SetDisplayState(bool state);

/**
 * @brief Activa o desactiva el cursor
 */
void LCD_SetCursorState(bool state);

/**
 * @brief Parpadeo del cursor
 * activa o desactiva parpadeo del cursor
 * @param state, 1= parpadeo activo, 0=parpadeo inactivo
 */
void LCD_SetBlinkState(bool state);

/**
 * @brief Envía un byte completo al LCD
 * @param value: dato a enviar
 * @param mode: 0 = comando, 1 = dato
 */
void LCD_SendByte(uint8_t value, uint8_t mode);

#endif /* LCD_H */
