#include "stm32f4xx_hal.h"
#include "motor_port.h"
#include "main.h"


//VARIABLES PRIVADAS
TIM_HandleTypeDef htim2;

//DEFINICIÓN DE PINES
// IN1 -> PA0
#define IN1_GPIO_Port GPIOB
#define IN1_Pin GPIO_PIN_1

// IN2 -> PA1
#define IN2_GPIO_Port GPIOB
#define IN2_Pin GPIO_PIN_2

// ENA (PWM) -> PA5 (TIM2_CH1)
#define PWM_GPIO_Port GPIOA
#define PWM_Pin GPIO_PIN_5
#define PWM_AF GPIO_AF1_TIM2
#define PWM_CHANNEL TIM_CHANNEL_1


// GPIO INIT
static void Motor_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // IN1 y IN2 como salida
    GPIO_InitStruct.Pin = IN1_Pin | IN2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // PWM pin (AF)
    GPIO_InitStruct.Pin = PWM_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = PWM_AF;
    HAL_GPIO_Init(PWM_GPIO_Port, &GPIO_InitStruct);
}

// TIMER PWM INIT
static void Motor_TIM_Init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Configuración base del timer
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 84 - 1;   // 84 MHz / 84 = 1 MHz
    htim2.Init.Period = 100 - 1;    // 1 kHz PWM
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_PWM_Init(&htim2);

    // Configuración canal PWM
    TIM_OC_InitTypeDef sConfig = {0};
    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.Pulse = 0; // duty inicial 0%
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfig, PWM_CHANNEL);

    // Iniciar PWM
    HAL_TIM_PWM_Start(&htim2, PWM_CHANNEL);
}


// INIT GENERAL
void Motor_Port_Init(void)
{
    Motor_GPIO_Init();
    Motor_TIM_Init();
    Motor_Port_Stop();
}

// CONTROL DIRECCIÓN
void Motor_Port_SetForward(void)
{
    HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
}

void Motor_Port_SetBackward(void)
{
    HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_SET);
}

void Motor_Port_Stop(void)
{
    HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
}

// CONTROL VELOCIDAD
void Motor_Port_SetSpeed(uint8_t duty)
{
    if (duty > 100) duty = 100;

    uint32_t pwm = (htim2.Init.Period * duty) / 100;

    __HAL_TIM_SET_COMPARE(&htim2, PWM_CHANNEL, pwm);
}
