----------------------------------------------------------------------------------------------------------
Práctica 2 – Retardos No Bloqueantes
Objetivo

Implementar un módulo de software en C que permita trabajar con retardos no bloqueantes utilizando una base de tiempo del sistema.
El objetivo es evitar el uso de retardos bloqueantes (como HAL_Delay) y permitir que el microcontrolador continúe ejecutando otras tareas mientras se mide el tiempo.

Punto 1 – Implementación del módulo de delay

Se implementó un módulo para manejar retardos no bloqueantes utilizando una estructura de control que guarda el estado del retardo.
Para que el código compile correctamente se deben incluir las siguientes bibliotecas estándar:
- stdint.h permite usar tipos de tamaño definido como uint32_t
- stdbool.h permite utilizar el tipo bool

Archivo main.h:
En este archivo se definen los typedef y los prototipos de funciones utilizados para manejar los retardos.
Archivo main.c:
Aquí se implementan las funciones del módulo.
La función HAL_GetTick() se utiliza como base de tiempo, ya que devuelve un contador que se incrementa cada 1 ms.

funciones:

delayInit():
  Inicializa la estructura del delay.
  Carga la duración del retardo
  Inicializa running en false
  No inicia el conteo del tiempo

delayRead():
  Verifica si el retardo se cumplió.

delayWrite():
  Permite cambiar la duración de un delay existente.

Punto 2 – Parpadeo del LED con retardo no bloqueante

Se implementó un programa que hace parpadear el LED de la placa de desarrollo con el siguiente patrón:

- 100 ms encendido
- 100 ms apagado
- Repetición continua
- Esto se logra utilizando la función delayRead() dentro del loop principal.

Punto 3 (Opcional) – Patrón de parpadeo

Se implementa un patrón de parpadeo utilizando un vector de períodos:
- 5 vces con período de 1 segundo
- 5 veces con período de 200 ms
- 5 veces con período de 100 ms

El programa cambia el tiempo del delay utilizando delayWrite() para modificar dinámicamente la duración del retardo.
Esto permite configurar el patrón de parpadeo fácilmente sin modificar la lógica del programa

NOTA: para poder observar el punto dos o tres , hace falta descomentar las lineas de codigo correspondiente en el programa (aparecen como "Còdigo punto 2" y "còdigo punto 3" respectivamente)
----------------------------------------------------------------------------------------------------------------------------------------------------------
