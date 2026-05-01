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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//#define FALSH_BASE_ADDR 0x08000000//+ 0x00019000
#define APP_FLASH_ADDR 0x08019000 //APP程序的启动地址-MSP
typedef void (*pFunc)(void);      //pFunc 是变量名'，类型是 void (*)(void)。
pFunc Jump2Application;//函数指针类型--变量 
                        //完全等价与 void (*Jump2Application)(void) 
uint32_t JumpAddress;
 void delay(volatile uint32_t count)
{
	while(count--)
	{
		//延时1个时钟周期
		//空操作
	}
}
void delay_seconds(uint32_t seconds)
{
	uint32_t count = 100000000;
	for(uint32_t i = 0;i < seconds;i++)
	{
		delay(count);//ַԃғʱ
	}
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/*define func pointer*/
/**关闭外设函数 */
void DisablePeriphClock_irq(void){
/**  //串口关闭
  // HAL_UART_DeInit(&huart1);
  //关闭所有外设时钟/
  __HAL_RCC_RTC_DISABLE();
  ///中断禁用
  __disable_irq(); */
	  // HAL_UART_DeInit(&huart1);
  //关闭所有外设时钟/
  __HAL_RCC_RTC_DISABLE();
  ///中断禁用
  __disable_irq();
//    HAL_DeInit();
//    HAL_RCC_DeInit();

//    // 关闭所有中断
//    __set_PRIMASK(1);
//      // 2. 彻底关闭 SysTick 并且清除所有NVIC中断（极其关键！）
//    SysTick->CTRL = 0;
//    SysTick->LOAD = 0;
//    SysTick->VAL = 0;
//    for (int i = 0; i < 8; i++) {
//        NVIC->ICER[i] = 0xFFFFFFFF; // 关闭全部中断
//        NVIC->ICPR[i] = 0xFFFFFFFF; // 清除所有挂起的中断
//    }
}
/**跳转函数 */
void Jump2App(void){

//  uint32_t jumpAddr,,i;
	uint32_t armAddr;
	armAddr = *(__IO uint32_t*)APP_FLASH_ADDR; //读取app程序的初始堆栈地址
	printf("app stack :0x%08X\r\n",armAddr);

  if(((*(__IO uint32_t*)APP_FLASH_ADDR) & 0x2FFE0000) == 0x20000000){ //判断地址是否合法//0x2FFE0000
    printf("jump addr start\r\n");
    
DisablePeriphClock_irq();
      /**取出app加载地址0x00019000-》将数字转成指针（地址）-》
       * 将falsh中的地址解引用查到地址中的存好的值-》判断这个值是否是RAM地址-128kb
       * */
    // 1. 从Flash读取应用程序的复位向量地址（PC指针）----这里是ram地址????
    JumpAddress = *(__IO uint32_t*)(APP_FLASH_ADDR + 4);
    
    // 2. 把地址转成函数指针类型，赋值给变量
    Jump2Application = (pFunc)JumpAddress;//类型是 void (*)(void)
    // 3. 设置主堆栈指针（MSP）为应用程序的初始堆栈地址
    __set_MSP(*(__IO uint32_t*)APP_FLASH_ADDR);/////////将栈顶指针手动设置
    // 4. 跳转到应用程序的复位向量地址，开始执行应用程序
    Jump2Application();

  }else{
    printf("jump app addr error\r\n");
  }
}


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
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
  SCB->VTOR = 0x08000000 | 0x0; //设置向量表偏移地址为0x08000000
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  // SystemInit();////////////////////////
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  printf("start bootloader main running\r\n");
   /* USER CODE BEGIN 2 */DisablePeriphClock_irq();
   Jump2App();
	
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
