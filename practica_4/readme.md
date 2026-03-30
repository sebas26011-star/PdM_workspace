-------------------------------------------------------------------------------------------------------------
# Práctica 4 – MEF

## Descripción

En esta práctica se implementa un módulo de software para el manejo de retardos no bloqueantes en sistemas embebidos utilizando la biblioteca HAL de STM32. Se desarrolla una API denominada `API_delay` que permite gestionar temporizaciones sin detener la ejecución del programa principal.

El módulo se integra en una aplicación que controla el parpadeo de un LED de la placa NUCLEO-F4xx con tiempos variables.

---

## Objetivo

Implementar un módulo reutilizable para retardos no bloqueantes y aplicarlo en un programa que genere una secuencia de parpadeo en un LED.

---

## Estructura del Proyecto
```c
Drivers/
└── API/
├── Inc/
│ └── API_delay.h
└── Src/
└── API_delay.c

Core/
└── Src/
 └── main.c
├── Inc/
 └── main.h
```
---

## API de Retardos

### typedef

```c
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct {
    tick_t startTime;
    tick_t duration;
    bool_t running;
} delay_t;

Librerías necesarias:

#include <stdint.h>
#include <stdbool.h>
```
---
### Funciones
#### delayInit
```c
//Inicializa la estructura del retardo.
void delayInit(delay_t *delay, tick_t duration);
```
#### delayRead
```c
//Verifica si el tiempo del retardo se ha cumplido.
bool_t delayRead(delay_t *delay);
//Comportamiento:
//Si el retardo no está corriendo, lo inicia.
//Si está corriendo y el tiempo se cumple, retorna true.
//No bloquea la ejecución del programa.
```
#### delayWrite
```c
//Modifica la duración del retardo.
void delayWrite(delay_t *delay, tick_t duration);
```

#### delayIsRunning
```c
//Devuelve el estado del retardo.
bool_t delayIsRunning(delay_t *delay);
```
---
### Funcionamiento del Programa

El programa principal implementa el parpadeo de un LED utilizando un vector de tiempos:
```c
const uint32_t timesVector[4] = {500, 100, 100, 1000};
```
#### Lógica de operación
Se utiliza una única instancia de delay_t.

Cada vez que delayRead() retorna true:
- Se conmuta el estado del LED.
- Se incrementa un contador.
  
Cada dos cambios de estado (equivalente a un ciclo completo con duty cycle 50%):
- Se selecciona el siguiente valor del vector.
- Se actualiza la duración del retardo con delayWrite.
- Se verifica previamente que el retardo no esté en ejecución.
#### Características
- Implementación de retardos no bloqueantes.
- Uso de HAL_GetTick() como base temporal.
- Control de LED con duty cycle del 50%.
- Cambio dinámico de frecuencia de parpadeo.
- Código modular y reutilizable.
#### Notas de Implementación
- Es necesario agregar la carpeta API/Inc al include path del proyecto.
- El funcionamiento depende del temporizador del sistema (HAL_GetTick()).
- Se recomienda validar punteros en las funciones de la API.
  
---
### Conclusión

Se desarrolló un módulo funcional para la gestión de retardos no bloqueantes, permitiendo la ejecución concurrente de tareas simples sin bloquear el flujo principal del programa. Este enfoque resulta fundamental en el desarrollo de sistemas embebidos eficientes y escalables.