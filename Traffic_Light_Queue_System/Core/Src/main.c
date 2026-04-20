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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define QUEUE_CAPACITY 10
#define ITEM_SIZE sizeof(Colors)

#define TIMEOUT_ROSSO 5000
#define TIMEOUT_GIALLO 2000
#define TIMEOUT_VERDE 1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
queue_t miaCoda;
uint8_t buffer_memoria[QUEUE_CAPACITY * ITEM_SIZE];

Colors currentState = ROSSO;
uint32_t lastTick = 0;
uint32_t currentTimeout = TIMEOUT_ROSSO;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Set_Traffic_Light(Colors color);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  queue_init(&miaCoda, buffer_memoria, ITEM_SIZE, QUEUE_CAPACITY);
  lastTick = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // 1. Set LEDs
	  Set_Traffic_Light(currentState);

	  // 2. Check Queue for override
	  Colors nextRequested;
	  if(queue_extract(&miaCoda, &nextRequested) == QUEUE_OK){
		  currentState = nextRequested;
		  lastTick = HAL_GetTick();

		  // Update timeout for the new state
		  switch(currentState){
		  	  case ROSSO: currentTimeout = TIMEOUT_ROSSO; break;
		  	  case GIALLO: currentTimeout = TIMEOUT_GIALLO; break;
		  	  case VERDE: currentTimeout = TIMEOUT_VERDE; break;
		  }
	  }

	  // 3. Check Timeout for automatic transition
	  if(HAL_GetTick() - lastTick >= currentTimeout){
		  switch(currentState){
		  	  case ROSSO:
		  		  currentState = GIALLO;
		  		  currentTimeout = TIMEOUT_GIALLO;
		  		  break;
		  	  case GIALLO:
		  		  currentState = VERDE;
		  		  currentTimeout = TIMEOUT_VERDE;
		  		  break;
		  	  case VERDE:
		  		  currentState = ROSSO;
		  		  currentTimeout = TIMEOUT_ROSSO;
		  		  break;
		  }
		  lastTick = HAL_GetTick();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Set_Traffic_Light(Colors color){
	// Reset all LEDs
	HAL_GPIO_WritePin(GPIOA, Led_rosso_Pin | Led_giallo_Pin | Led_verde_Pin, GPIO_PIN_RESET);

	// Set requested LED
	switch(color){
		case ROSSO:
			HAL_GPIO_WritePin(Led_rosso_GPIO_Port, Led_rosso_Pin, GPIO_PIN_SET);
			break;
		case GIALLO:
			HAL_GPIO_WritePin(Led_giallo_GPIO_Port, Led_giallo_Pin, GPIO_PIN_SET);
			break;
		case VERDE:
			HAL_GPIO_WritePin(Led_verde_GPIO_Port, Led_verde_Pin, GPIO_PIN_SET);
			break;
	}
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
