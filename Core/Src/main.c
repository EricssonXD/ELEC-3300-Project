/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pacman.h"
#include "keypad.h"
#include "maze.h"
#include "speaker.h"
#include "main_menu.h"
//#include "pacman_death_sound.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
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
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_I2S2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
//  USART3_IRQHandler();
//  WM8978_Init();
  LCD_INIT();

  // Init WIFI
  LCD_DrawString(0, 0, "Initalizing Wifi");
  wifiInit(SERVER);
  LCD_DrawString(0, 0, "Wifi Initialized");

  // Get IP
  getIP();
  LCD_DrawString(0, 20, "Server IP:");
  LCD_DrawString(11*8, 20, ipAddr);

  // Init Keypad
  LCD_DrawString(0, 40, "Initalizing Keypad");
  KeyPad_Init();
  LCD_DrawString(0, 40, "Keypad Initialized");

//  WM8978_PlayChar(Pacman_death_sound_wav);
  LCD_DrawString(40, 240, "Press K1 to continue");

  int number = 0;
  char numberString[4];
  char keypadResult;

  GPIO_PinState k1 = GPIO_PIN_RESET;
//  GPIO_PinState k2 = GPIO_PIN_RESET;
  int displayReady = 2;


//  getIP();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  k1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
//	  k2 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	  if(k1 == GPIO_PIN_SET){
		  displayReady = -1;
		  HAL_TIM_Base_Stop_IT(&htim6);
		  ShowMainMenu();
		  while(1){
			  k1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
			  keypadResult = KeyPad_WaitForKeyGetChar(100);
			  if(k1 == GPIO_PIN_SET) keypadResult = 'S';
			  if(MenuHandleInput(keypadResult) == 1){
				  if(MULTI_PACMAN_GAMEDATA.playerNum > 1){
					  isMulti = 1;
				  } else {
					  isMulti = 0;
				  }
				  displayReady = 0;
				  break;
			  }
		  }
	  }

	  // inital debugging screen
	  if(displayReady == 2) {
	   continue;
	   keypadResult = KeyPad_WaitForKeyGetChar(100);
	   if(keypadResult == '1'){

		   LCD_DrawChar(0, 100, 'G');

	   }


	   LCD_DrawChar(0, 80, keypadResult);

	   HAL_Delay(1000);
	   sprintf(numberString, "%d", number);
	   LCD_DrawChar(0, 80, *numberString);
	   number++;
	   if(number==10)number=0;
	  }


	  if(displayReady == 0){
		  if(!isMulti){
			  Pacman_gamestart(MAZE1, isMulti);
		  }
		  else{
			  Pacman_gamestart(MAZE4, isMulti);
		  }

		  displayReady = 1;
	  }

	  if(displayReady == 1){
		  Pacman_handleKeypadInput(200);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2S2;
  PeriphClkInit.I2s2ClockSelection = RCC_I2S2CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	static uint8_t count = 0;
	if(htim == (&htim2)){
		count++;
		if(count >= 10){
			recDataHandle();
			count = 0;
		}
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

#ifdef  USE_FULL_ASSERT
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
