#include "lcd.h"
#include "lcd_port.h"

// DEFINES (MAPEO)
// Bits de control
#define LCD_RS  (1 << 0)
#define LCD_RW  (1 << 1)
#define LCD_EN  (1 << 2)
#define LCD_BL  (1 << 3)

// Modos
#define LCD_COMMAND 0
#define LCD_DATA    1

// comandos y constantes
static const uint8_t INITIAL_CMD = 0x03;
static const uint8_t HOME_CMD = 0X02;
static const uint8_t FOUR_BITS_TWO_LINES = 0x28;
static const uint8_t DISPLAY_ON = 0x0C;
static const uint8_t CLEAR_CMD = 0x01;
static const uint8_t SET_POS_CMD = 0x80;
static const uint8_t ROW0_INIT_ADRS = 0x00;
static const uint8_t ROW1_INIT_ADRS = 0x40;
static const uint8_t ROW2_INIT_ADRS = 0x14;
static const uint8_t ROW3_INIT_ADRS = 0x54;
static uint8_t display_control = 0x0C;


// DEFINICION DE FUNCIONES SATATIC

/**
 * @brief funcion para envio de dato con pulso de enable.
 *realiza dos veces el envio del byte de dato o coamndo, el primero con mascada para EN=1 y
 *segundo con mascara de EN=0.
 * @param data, byte completo a enviar con el bit de EN en 0. ya debe tener las mascaras de los otros
 * bits de control establecida.
 */
static void LCD_EnablePulse(uint8_t data);

/**
 * @brief funcion para envio de dato en modo de 4 bits.
 *utilizada para enviar mitades del comando o dato que se le quiere enviar al LCD.
 * @param nibble,mitad del comando o dato a enviar (MSB, LSB).
 * @param mode, modo en el que se realiza el envio, si es un comando mode=0, si es un dato mode=1
 * la funcion realiza mascaras y desplazacmeintos con este valor para poder organizar el byte a enviar.
 */
static void LCD_Write4(uint8_t nibble, uint8_t mode);

/**
 * @brief funcion para envio de bytes(nibble) para la inicializacion del LCD.
 *la funcion solo es utilizaca en la inicializacion durante el envio de los comandos
 *que solicita el lcd para funcionar en modo de 4 bits.
 * @param nibble, comando de inicializacion a enviar (no es el byte completo, la funcion),
 * la funcion realiza mascaras y desplazacmeintos con este valor para poder organizar el byte a enviar.
 */
static void LCD_WriteInit(uint8_t nibble);

// ENABLE PULSE
static void LCD_EnablePulse(uint8_t data)
{
    LCD_Port_Write(data | LCD_EN);   // EN = 1
    LCD_Port_Delay(1);

    LCD_Port_Write(data & ~LCD_EN);  // EN = 0
    LCD_Port_Delay(1);
}

// FUNCIÓN BASE
static void LCD_Write4(uint8_t nibble, uint8_t mode)
{
    uint8_t data = 0;

    //Datos en bits 7–4
    data = (nibble << 4);

    // RS
    if (mode == LCD_DATA)
        data |= LCD_RS;

    // RW siempre 0 (escritura)
    // data |= 0;

    // Backlight ON
    data |= LCD_BL;

    LCD_Port_Write(data);
    LCD_EnablePulse(data);
}

// INIT LOW LEVEL
static void LCD_WriteInit(uint8_t nibble)
{
    uint8_t data = 0;

    // datos en bits altos
    data = (nibble << 4);

    // BL ON
    data |= LCD_BL;

    /* esta parte de escribir  el dato, luego enviar en pulso de enable
     * se realiza porque es equivalente a enviar un "ENTER", se escribe el dato en EN en 0,
     * luego se envia con el enable EN en 1 y finalmente se envia nuevamente con el  EN en 0.
     * esto para el LCD es como decirle "haz el cambio"
    */
    LCD_Port_Write(data);
    LCD_EnablePulse(data);
}

//---------------------------------------------------------------------
// FUNCIONES GLOBALES
//---------------------------------------------------------------------

//ENVÍO BYTE
void LCD_SendByte(uint8_t value, uint8_t mode)
{
    LCD_Write4((value >> 4) & 0x0F, mode); // MSB
    LCD_Write4(value & 0x0F, mode);        // LSB
}

//INIT
void LCD_Init(void)
{
    LCD_Port_Init();
    LCD_Port_Delay(50);

    // Secuencia especial
    LCD_WriteInit(INITIAL_CMD);
    LCD_Port_Delay(10);

    LCD_WriteInit(INITIAL_CMD);
    LCD_Port_Delay(1);

    LCD_WriteInit(INITIAL_CMD);
    LCD_Port_Delay(1);

    LCD_WriteInit(HOME_CMD); // modo 4 bits
    LCD_Port_Delay(1);

    // Configuración normal
    LCD_SendByte(FOUR_BITS_TWO_LINES, LCD_COMMAND); // 4 bits, 2 líneas
    LCD_SendByte(DISPLAY_ON, LCD_COMMAND); // display ON
    LCD_SendByte(CLEAR_CMD, LCD_COMMAND); // clear
    LCD_Port_Delay(2);
}

//CONTROL BÁSICO
void LCD_ClearLine(uint8_t row)
{
    LCD_SetCursor(row, 0);

    for (uint8_t i = 0; i < 16; i++)  // LCD 16x2
    {
        LCD_WriteChar(' ');
    }

    LCD_SetCursor(row, 0); // opcional: regresar cursor
}

void LCD_Clear(void)
{
    LCD_SendByte(CLEAR_CMD, LCD_COMMAND);
    LCD_Port_Delay(2);
}

void LCD_Home(void)
{
    LCD_SendByte(HOME_CMD, LCD_COMMAND);
    LCD_Port_Delay(2);
}

//CURSOR
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    switch (row)
    {
        case 0: address = ROW0_INIT_ADRS + col; break;
        case 1: address = ROW1_INIT_ADRS + col; break;
        case 2: address = ROW2_INIT_ADRS + col; break;
        case 3: address = ROW3_INIT_ADRS + col; break;
        default: address = ROW0_INIT_ADRS + col; break;
    }

    LCD_SendByte(SET_POS_CMD | address, LCD_COMMAND);
}

//ESCRITURA
void LCD_WriteChar(char ch)
{
    LCD_SendByte((uint8_t)ch, LCD_DATA);
}

void LCD_WriteString(char *str)
{
    while (*str)
    {
        LCD_WriteChar(*str++);
    }
}

//CONFIG DISPLAY
void LCD_SetDisplayState(bool state)
{
	display_control = DISPLAY_ON;
    if (state)
        display_control |= 0x04;
    else
        display_control &= ~0x04;

    LCD_SendByte(display_control, LCD_COMMAND);
}

void LCD_SetCursorState(bool state)
{
	display_control = DISPLAY_ON;
    if (state)
        display_control |= 0x02;
    else
        display_control &= ~0x02;

    LCD_SendByte(display_control, LCD_COMMAND);
}

void LCD_SetBlinkState(bool state)
{
	display_control = DISPLAY_ON;
    if (state)
        display_control |= 0x01;
    else
        display_control &= ~0x01;

    LCD_SendByte(display_control, LCD_COMMAND);
}
