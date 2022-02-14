#include "stm32f4xx.h"                  // Device header
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "FreeRTOSConfig.h"             // ARM.FreeRTOS::RTOS:Config
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "queue.h"                      // ARM.FreeRTOS::RTOS:Core


void vTask1Function(void* pvParameters)
{
	while(1)
	{
		//do something
		//vTaskDelay(100);
		GPIOA->ODR |= GPIO_ODR_ODR_5;	//led on
		vTaskDelay(100);
	  GPIOA->ODR &= ~GPIO_ODR_ODR_5;// led off
		vTaskDelay(100);
	}
}
void InitHardware()
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //port A
   	GPIOA->MODER |= GPIO_MODER_MODER5_0;//4 0-input 1-out 2-aim  func 3- analog

	}
int main()
{
	InitHardware();
	
	xTaskCreate(vTask1Function, "task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	//xTaskCreate(vTask2Function, "task2", configMINI-MAL_STACK_SIZE, NULL, 4, NULL);
	
	vTaskStartScheduler();
	
	while(1)
	{
	}	
}

//int main()
//{
////A5
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //port A
//	GPIOA->MODER |= GPIO_MODER_MODER5_0;//4 0-input 1-out 2-aim  func 3- analog
//	while(1)
//	{
//	   GPIOA->ODR |= GPIO_ODR_ODR_5;	//led on
//		 for(int i = 0; i <100000; i++);
//		 GPIOA->ODR &= ~GPIO_ODR_ODR_5;// led off
//		 for(int i = 0; i < 100000; i++);
//	}
//}