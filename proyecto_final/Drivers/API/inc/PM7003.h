#ifndef PM7003_H
#define PM7003_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Constantes
#define PM7003_FRAME_SIZE 32
#define PM7003_FIRST_BIT 0X42
#define PM7003_SECOND_BIT 0X4D
#define FIRST_BIT_POS 0
#define SECOND_BIT_POS 1
#define MSB_CHECKSUM_POS 30
#define LSB_CHECKSUM_POS 31




// Estructura de datos
typedef struct
{
    uint16_t pm1_0;      // PM1.0 (atm)
    uint16_t pm2_5;      // PM2.5 (atm)
    uint16_t pm10;       // PM10  (atm)

    uint16_t pm1_0_cf1;  // PM1.0 (CF=1)
    uint16_t pm2_5_cf1;  // PM2.5 (CF=1)
    uint16_t pm10_cf1;   // PM10  (CF=1)

} PM7003_Data;



// Inicialización lógica

/**
 * @brief Inicializa variables internas del driver
 */
void PM7003_Init(void);


//Procesamiento de datos
/**
 * @brief Procesa un byte recibido desde UART
 * Implementa la máquina de estados para armar el frame
 *
 * @param byte: dato recibido
 */
void PM7003_ProcessByte(uint8_t byte);



// Estado del frame
/**
 * @brief Indica si hay un frame completo listo
 * @return 1 si hay datos disponibles, 0 en caso contrario
 */
uint8_t PM7003_IsFrameReady(void);


// Obtener datos
/**
 * @brief Copia los datos del último frame válido
 * @param data: puntero donde se guardarán los datos
 */
void PM7003_GetData(PM7003_Data *data);



//Parser de frame
/**
 * @brief Parsea un frame completo de 32 bytes
 * Verifica header y checksum
 * @param frame: buffer de 32 bytes
 * @param data: estructura destino
 *
 * @return 0 si OK, <0 si error
 */
int PM7003_ParseFrame(uint8_t *frame, PM7003_Data *data);



// Checksum
/**
 * @brief Calcula checksum del frame
 * (suma de bytes 0 a 29)
 * @param buffer: frame recibido
 * @return checksum calculado
 */
uint16_t PM7003_CalcChecksum(uint8_t *buffer);



//Control interno (no necesario)
/**
 * @brief Reinicia el parser (máquina de estados)
 */
void PM7003_Reset(void);

#endif
