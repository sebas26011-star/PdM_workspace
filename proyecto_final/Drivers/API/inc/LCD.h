#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

// Inicialización
/**
 * @brief Inicializa el LCD en modo 4 bits vía I2C
 *
 * @param hi2c: puntero al handler I2C
 */
void LCD_Init(void);


//Control básico
/**
 * @brief Limpia la pantalla
 */
void LCD_Clear(void);

/**
 * @brief Retorna el cursor al inicio
 */
void LCD_Home(void);

/**
 * @brief liampia una linea completa
 *
 * @param row, numero de la linea que se quiere limpiar
 */
void LCD_ClearLine(uint8_t row);

// Posicionamiento
/**
 * @brief Posiciona el cursor
 *
 * @param row: fila
 * @param col: columna
 */
void LCD_SetCursor(uint8_t row, uint8_t col);



// Escritura
/**
 * @brief Escribe un caracter ASCII
 *
 * @param ch: caracter a escribir
 */
void LCD_WriteChar(char ch);


/**
 * @brief Escribe un string completo
 *
 * @param str: puntero al string
 */
void LCD_WriteString(char *str);

// Configuración display
/**
 * @brief Enciende o apaga el display
 */
void LCD_SetDisplayState(bool state);

/**
 * @brief Activa o desactiva el cursor
 */
void LCD_SetCursorState(bool state);

/**
 * @brief Activa o desactiva el parpadeo del cursor
 */
void LCD_SetBlinkState(bool state);

/**
 * @brief Envía un byte completo al LCD
 *
 * @param value: dato a enviar
 * @param mode: 0 = comando, 1 = dato
 */
void LCD_SendByte(uint8_t value, uint8_t mode);

#endif /* LCD_H */
