-------------------------------------------------------------------------------------------------------------
# Práctica 4 – MEF

## Descripción

Implementar una Máquina de Estados Finitos (MEF) para trabajar con anti-rebotes por software.

El módulo se integra en una aplicación que controla el parpadeo de un LED de la placa NUCLEO-F4xx con tiempos variables 
mediante un pulsador.

---

## Objetivo

Implementar una Máquina de Estados Finitos (MEF) para trabajar con anti-rebotes por software.

El módulo se integra en una aplicación que controla el parpadeo de un LED de la placa NUCLEO-F4xx con tiempos variables 
mediante un pulsador.

---

## Estructura del Proyecto
```c
Drivers/
└── API/
├── Inc/
│ ├── API_delay.h
│ └── API_debounce.h

└── Src/
   ├── API_delay.c
   └── API_debounce.c

Core/
├── Src/
│ └── main.c
└── Inc/
   └── main.h
```
---

## API de Anti-rebote (Debounce)

### typedef

```c
typedef enum{
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RAISING,
} debounceState_t;
```
---
### Funciones
```c
void debounceFSM_init();
void debounceFSM_update();

/*
 * Lee el estado de la tecla.
 * Retorna true si fue presionada.
 * Luego resetea el estado a false.
 */
bool_t readKey();
---
```
### Funcionamiento del Programa

El programa principal controla el parpadeo de un LED con dos frecuencias:

- 100 ms
- 500 ms
#### Lógica de operación

Se utiliza:

- Módulo API_debounce
- Módulo API_delay

Cada vez que readKey() retorna true:

- Se cambia la frecuencia de parpadeo del LED

El parpadeo se implementa con retardos no bloqueantes:

- delayRead() controla el tiempo
- Se conmuta el estado del LED periódicamente
  
#### Características
- Implementación de anti-rebote por software mediante MEF
- Uso de retardos no bloqueantes
- Detección de flancos (presión del botón)
- Código modular y reutilizable
- Separación clara entre lógica de aplicación y drivers
#### Notas de Implementación
- Agregar /drivers/API/inc al include path
- debounceFSM_update() debe llamarse dentro del loop principal
- El sistema depende de HAL_GetTick() como base temporal
- Validar punteros en las funciones de la API
  
---
### Conclusión

Se implementó una Máquina de Estados Finitos para el manejo de anti-rebote por software, permitiendo detectar de forma confiable las pulsaciones de un botón. Este enfoque mejora la robustez del sistema y es ampliamente utilizado en sistemas embebidos para evitar errores por ruido mecánico en entradas digitales.
