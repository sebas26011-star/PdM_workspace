#  Sistema de Monitoreo de Calidad del Aire con Control de Extractor

## Descripción

Este proyecto implementa un sistema embebido basado en STM32 que:

- Mide material particulado (PM1.0, PM2.5, PM10) usando el sensor PM7003
- Muestra la información en un LCD 16x2 vía I2C (PCF8574)
- Controla un motor DC mediante L298N como extractor de aire
- Ajusta automáticamente la velocidad del motor según la calidad del aire

---

## Objetivo

Desarrollar un sistema capaz de:

Medir → Procesar → Decidir → Actuar → Informar

---

## Hardware Utilizado

- STM32F446RE
- Sensor PM7003
- LCD 16x2 con módulo I2C (PCF8574)
- Driver LM298N
- Motor DC
- Fuente de alimentacion de 12V (para manejo de LM298N)

---

## Estructura del Proyecto
```bash
Core/
├── Src/
│ ├── main.c
│ ├── stm32f4xx_it.c
│ └── ...
│
├── Inc/
│ └── main.h

Drivers/
└── API/
├── inc/
│ ├── PM7003.h
│ ├── PM7003_port.h
│ ├── LCD.h
│ ├── LCD_port.h
│ ├── LM298N.h
│ └── LM298N_port.h
│
└── src/
├── PM7003.c
├── PM7003_port.c
├── LCD.c
├── LCD_port.c
├── LM298N.c
├── LM298N_port.c
└── UART_handler.c
```

---

## Arquitectura del Software

Aplicación (main.c)  
↓  
Drivers (API)  
↓  
HAL (STM32)  
↓  
Hardware  

---

## Módulos Implementados

### PM7003

- Comunicación UART por interrupción
- Parsing de frames
- Lectura de PM1.0, PM2.5 (ATM), PM10

---

### LCD (I2C - PCF8574)

- Modo 4 bits
- Comunicación por I2C
- Funciones de escritura, cursor y limpieza por línea

Mapa de bits utilizado:

D7 D6 D5 D4 BL EN RW RS

---

### Motor (L298N)

- Control de dirección (IN1, IN2)
- PWM mediante TIM2
- Ajuste dinámico de velocidad

---

## Conexiones

### LCD (I2C)

- SDA → PB9  
- SCL → PB8  
- VCC → 5V  
- GND → GND  

---

### PM7003 (UART)

- TX → RX del STM32  
- VCC → 5V  
- GND → GND  

---

### L298N
- IN1 → PB1  
- IN2 → PB2  
- ENA → PA5 (PWM - TIM2)  

---

## Lógica del Sistema

1. El PM7003 envía datos por UART  
2. Se detecta un frame válido  
3. Se obtiene el valor de PM2.5 (ATM)  
4. Se clasifica la calidad del aire  
5. Se ajusta la velocidad del motor  
6. Se actualiza el LCD  

---

## Escala de Calidad del Aire y relacion con velocidad de motor

| PM2.5 (μg/m³) | Estado  | Acción          |
|--------------|--------|----------------|
| 0 – 12       | BUENO  | Motor OFF      |
| 13 – 35      | OK     | Baja velocidad |
| 36 – 55      | ALERTA | Media velocidad|
| 56 – 150     | MALO   | Alta  velocidad|
| >150         | CRÍTICO| Máxima velocidad|
