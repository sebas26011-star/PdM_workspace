#ifndef LCD_PORT_H
#define LCD_PORT_H

#include <stdint.h>

// =====================
// Inicialización
// =====================

/**
 * @brief Inicializa el módulo I2C para el LCD
 * Configura el periférico según los parámetros del datasheet
 */
void LCD_Port_Init(void);


// =====================
// Escritura I2C
// =====================

/**
 * @brief Envía un byte al LCD por I2C
 *
 * @param data: byte a enviar (incluye datos + control bits)
 */
void LCD_Port_Write(uint8_t data);


// =====================
// Delay
// =====================

/**
 * @brief Delay en milisegundos
 *
 * @param ms: tiempo de espera
 */
void LCD_Port_Delay(uint32_t ms);



#endif /* LCD_PORT_H */
