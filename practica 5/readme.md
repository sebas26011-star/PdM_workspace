-------------------------------------------------------------------------------------------------------------
# Práctica 5 – UART y Parser de Comandos (MEF)

## Descripción

En esta práctica se implementa un sistema de comunicación serie utilizando la biblioteca HAL de STM32, junto con una Máquina de Estados Finitos (MEF) para el procesamiento de comandos recibidos por UART.

Se desarrollan dos módulos principales:

- `API_uart`: encargado de la inicialización y manejo de la comunicación UART.
- `API_cmdparser`: encargado de interpretar y ejecutar comandos ingresados por el usuario.

El sistema permite controlar el LED de la placa NUCLEO-F4xx mediante comandos enviados por terminal serie.

---

## Objetivo

- Implementar una API de comunicación UART.
- Diseñar e implementar una MEF para el procesamiento de comandos.
- Integrar ambos módulos en una aplicación funcional.
- Controlar periféricos (GPIO) a partir de comandos recibidos por UART.

---

## Estructura del Proyecto

```c
Drivers/
└── API/
    ├── Inc/
    │   ├── API_uart.h
    │   └── API_cmdparser.h
    └── Src/
        ├── API_uart.c
        └── API_cmdparser.c

Core/
└── Src/
    └── main.c
├── Inc/
    └── main.h
```

---

## API UART

### Funciones

#### uartInit

```c
bool uartInit(void);
```

Inicializa el periférico USART2, configurando:

- Baudrate: 115200
- Word length: 8 bits
- Paridad: ninguna
- Stop bits: 1
- Modo: TX/RX

---

#### uartSendString

```c
void uartSendString(const char * pstring);
```

Envía un string completo terminado en `\0`.

---

#### uartSendStringSize

```c
void uartSendStringSize(const char * pstring, uint16_t size);
```

Envía una cantidad específica de bytes.

---

#### uartReceiveStringSize

```c
bool uartReceiveStringSize(uint8_t * pstring, uint16_t size);
```

Recibe datos por UART de manera bloqueante con timeout.

---

### Características

- Uso de HAL UART
- Validación de parámetros
- Encapsulamiento del periférico
- Manejo de strings y buffers

---

## API Parser de Comandos

### Máquina de Estados

```c
typedef enum {
    CMD_IDLE,
    CMD_RECEIVING,
    CMD_PROCESS,
    CMD_EXEC,
    CMD_ERROR
} cmd_state_t;
```

---

### Estados

- **CMD_IDLE**: espera inicio de comando  
- **CMD_RECEIVING**: almacena caracteres en buffer  
- **CMD_PROCESS**: valida el comando recibido  
- **CMD_EXEC**: ejecuta la acción asociada  
- **CMD_ERROR**: maneja errores  

---

### Funciones

#### cmdInit

```c
void cmdInit(void);
```

Inicializa el parser y la MEF.

---

#### cmdPoll

```c
void cmdPoll(void);
```

Debe ejecutarse periódicamente. Se encarga de:

- Leer datos desde UART
- Construir comandos
- Procesarlos
- Ejecutarlos

---

## Comandos Implementados

| Comando       | Descripción                      |
|--------------|---------------------------------|
| `HELP`        | Lista comandos disponibles       |
| `LED ON`      | Enciende el LED                 |
| `LED OFF`     | Apaga el LED                   |
| `LED TOGGLE`  | Alterna el estado del LED      |
| `STATUS`      | Muestra el estado del LED      |

---

## Manejo de Strings

- Conversión a mayúsculas (`toupper`)
- Comparación con `strcmp`
- Terminación con `\0`
- Buffer de tamaño fijo

---

## Funcionamiento del Programa

### Flujo General

1. El usuario envía un comando por terminal.
2. Se reciben caracteres uno a uno.
3. Se construye el comando en un buffer.
4. Al recibir `\r`, se procesa el comando.
5. Si es válido:
   - Se ejecuta la acción.
6. Si es inválido:
   - Se envía mensaje de error.

---

### Control de LED

El LED se controla mediante:

```c
HAL_GPIO_WritePin(...)
HAL_GPIO_TogglePin(...)
HAL_GPIO_ReadPin(...)
```

El comando `STATUS` lee directamente el estado del GPIO.

---

## Características

- Comunicación UART funcional
- MEF implementada correctamente
- Procesamiento de comandos en tiempo real
- Uso de buffers y validación de entrada
- Código modular y reutilizable
- Separación clara entre:
  - Comunicación
  - Lógica de aplicación

---

## Notas de Implementación

- Es necesario agregar `API/Inc` al include path.
- El sistema depende del clock correctamente inicializado.
- La UART debe configurarse después de `SystemClock_Config()`.
- Se recomienda usar terminales como:
  - PuTTY
  - TeraTerm
- Configuración UART:
  - 115200 baudios
  - 8N1
  - Sin control de flujo

---

## Conclusión

Se desarrolló un sistema completo de comunicación serie con interpretación de comandos basado en una Máquina de Estados Finitos. Este enfoque permite desacoplar la lógica de entrada, procesamiento y ejecución, facilitando el desarrollo de sistemas embebidos escalables, robustos y reutilizables.

La práctica integra conceptos fundamentales como:

- Manejo de periféricos (UART, GPIO)
- Diseño de APIs
- Procesamiento de strings
- Máquinas de estados

Constituyendo una base sólida para aplicaciones embebidas más complejas.
