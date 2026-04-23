/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
#include "PM7003_port.h"
#include "PM7003.h"
#include "LM298N_port.h"
#include "LM298N.h"
#include "LCD_port.h"
#include "LCD.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
PM7003_Data PM7003_VALUES;
char buffer[16];

static uint16_t last_pm25 = 0;
static int8_t last_speed = -1;

/* Constantes PWM */
static const uint8_t MOTOR_POWER_OFF = 0;
static const uint8_t MOTOR_POWER_LOW = 40;
static const uint8_t MOTOR_POWER_HALF = 60;
static const uint8_t MOTOR_POWER_MEDIUM_HIGH = 80;
static const uint8_t MOTOR_POWER_MAX = 100;

/* Constantes calidad aire */
static const uint16_t GOOD_LIMIT = 12;
static const uint16_t OK_LIMIT = 35;
static const uint16_t WARNING_LIMIT = 55;
static const uint16_t BAD_LIMIT = 150;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
static const char* GetAirQuality(uint16_t pm25);
static uint8_t GetMotorSpeed(uint16_t pm25);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

  // Inicialización de módulos
  PM7003_Port_Init();
  PM7003_Init();

  Motor_Init();
  Motor_Forward();

  LCD_Init();
  LCD_Clear();

  LCD_SetCursor(0,0);
  LCD_WriteString("CALIDAD DE AIRE");

  HAL_Delay(2000);
  LCD_Clear();

  /* USER CODE END 2 */

  while (1)
      {
          if (PM7003_IsFrameReady())
          {
              PM7003_GetData(&PM7003_VALUES);

              //CONTROL MOTOR
              int8_t speed = GetMotorSpeed(PM7003_VALUES.pm2_5);

              if (speed != last_speed)
              {
                  last_speed = speed;
                  Motor_SetSpeed(speed); // implementar el arrancador
                  LCD_SetCursor(1,0);
                  LCD_ClearLine(1);
                  sprintf(buffer, "AQ:%s-%d%%",GetAirQuality(PM7003_VALUES.pm2_5),speed);
                  LCD_WriteString(buffer);
              }

              if (PM7003_VALUES.pm2_5 != last_pm25)
              {
                  last_pm25 = PM7003_VALUES.pm2_5;

                  LCD_SetCursor(0,0);
                  sprintf(buffer, "PM2.5:%4d ug/m3", PM7003_VALUES.pm2_5);
                  LCD_WriteString(buffer);
              }
          }
      }

    /* USER CODE END WHILE */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief categorizar la calidad del aire
  * @param pm25, valor de material particulado pm25 obtenido en el sensor
  * @return string que indica la calidad del aire
  */
static const char* GetAirQuality(uint16_t pm25)
{
    if (pm25 <= GOOD_LIMIT) return "BUENO";
    else if (pm25 <= OK_LIMIT) return "OK";
    else if (pm25 <= WARNING_LIMIT) return "ALERTA";
    else if (pm25 <= BAD_LIMIT) return "MALO";
    else return "CRITICO";
}
/**
  * @brief Calcular la velocidad del extractor en funciion de la concentracion pm25
  * @param pm25, valor de material particulado pm25 obtenido en el sensor
  * @return valor duty para el pwm que controla el motor
  */
static uint8_t GetMotorSpeed(uint16_t pm25)
{
    if (pm25 <= GOOD_LIMIT) return MOTOR_POWER_OFF;      // OFF
    else if (pm25 <= OK_LIMIT) return MOTOR_POWER_LOW ;
    else if (pm25 <= WARNING_LIMIT) return MOTOR_POWER_HALF;
    else if (pm25 <= BAD_LIMIT) return MOTOR_POWER_MEDIUM_HIGH;
    else return MOTOR_POWER_MAX;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
