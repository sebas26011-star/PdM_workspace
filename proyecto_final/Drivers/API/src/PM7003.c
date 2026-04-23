#include "PM7003.h"

//Variables internas
static uint8_t frame[PM7003_FRAME_SIZE];
static uint8_t index = 0;
static uint8_t state = 0;
static uint8_t frame_ready = 0;
static uint8_t RESET = 0;
static uint8_t SET = 1;

static PM7003_Data current_data; // estructura para guardar los valores del sensor

// FUNCIONES STATIC

//Parser de frame
/**
 * @brief Parsea un frame completo de 32 bytes
 * Verifica header y checksum
 * @param frame: buffer de 32 bytes
 * @param data: estructura destino
 *
 * @return 0 si OK, <0 si error
 */
static int PM7003_ParseFrame(uint8_t *frame, PM7003_Data *data);

// Checksum
/**
 * @brief Calcula checksum del frame
 * (suma de bytes 0 a 29)
 * @param buffer: frame recibido
 * @return checksum calculado
 */
static uint16_t PM7003_CalcChecksum(uint8_t *buffer);

// Parser
static int PM7003_ParseFrame(uint8_t *buf, PM7003_Data *data)
{
    // Validar header
    if (buf[0] != 0x42 || buf[1] != 0x4D)
        return -1;

    // Validar checksum
    uint16_t sum = PM7003_CalcChecksum(buf);
    uint16_t received = (buf[30] << 8) | buf[31];

    if (sum != received)
        return -2;

    // Extraer datos CF=1
    data->pm1_0_cf1 = (buf[4] << 8) | buf[5];
    data->pm2_5_cf1 = (buf[6] << 8) | buf[7];
    data->pm10_cf1  = (buf[8] << 8) | buf[9];

    // Extraer datos ATM (los importantes)
    data->pm1_0 = (buf[10] << 8) | buf[11];
    data->pm2_5 = (buf[12] << 8) | buf[13];
    data->pm10  = (buf[14] << 8) | buf[15];

    return 0;
}

// Checksum
static uint16_t PM7003_CalcChecksum(uint8_t *buffer)
{
    uint16_t sum = RESET;

    for (int i = 0; i < 30; i++)
    {
        sum += buffer[i];
    }

    return sum;
}


typedef enum {
    INIT,        ///< Esperando inicio de comando
    FILLING,   ///< Recibiendo caracteres
    VALIDATION,     ///< Procesando comando recibido
    EXTRACTION,        ///< Ejecutando comando válido
} frame_state_t;


//---------------------------------------------------------------------
// FUNCIONES GLOBALES
//---------------------------------------------------------------------

//Inicialización
void PM7003_Init(void)
{
    index = RESET;
    state = INIT;
    frame_ready = RESET;
}


// Máquina de estados
void PM7003_ProcessByte(uint8_t byte)
{
	static frame_state_t state = INIT;
    switch (state)
    {
        case INIT:
            if (index == RESET)
            {
                if (byte == PM7003_FIRST_BIT)
                {
                    frame[FIRST_BIT_POS] = byte;
                    index = SET;
                }
            }
            else if (index == SET)
            {
                if (byte == PM7003_SECOND_BIT)
                {
                    frame[SECOND_BIT_POS] = byte;
                    index++;
                    state = FILLING;
                }
                else
                {
                    index = RESET; // reiniciar
                }
            }
            break;

        case FILLING:
            frame[index++] = byte;
            if (index >= PM7003_FRAME_SIZE)
            {
                state = VALIDATION;
            }
            break;

        case VALIDATION:
        {
            uint16_t checksum = PM7003_CalcChecksum(frame);
            uint16_t received = (frame[MSB_CHECKSUM_POS] << 8) | frame[ LSB_CHECKSUM_POS];

            if (checksum == received)
            {
                state = EXTRACTION;
            }
            else
            {
                index = RESET;
                state = INIT;
            }
        }
        break;

        case EXTRACTION:
            PM7003_ParseFrame(frame, &current_data);
            frame_ready = SET;
            // volver a empezar
            index = RESET;
            state = INIT;
            break;

        default:
            state = INIT;
            index = RESET;
            break;
    }
}



//Frame listo
uint8_t PM7003_IsFrameReady(void)
{
    return frame_ready;
}



//Obtener datos
void PM7003_GetData(PM7003_Data *data)
{
    if (data == NULL) return;

    *data = current_data;
    frame_ready = RESET;
}


// Reset
void PM7003_Reset(void)
{
    index = RESET;
    state = INIT;
    frame_ready = RESET;
}
