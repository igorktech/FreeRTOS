#include "stm32f4xx.h"                  // Device header
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "FreeRTOSConfig.h"             // ARM.FreeRTOS::RTOS:Config
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "queue.h"                      // ARM.FreeRTOS::RTOS:Core
#include "inttypes.h"

#define t 0.2
#define m 3
void blink();
void move_blink();
TaskHandle_t blink_, move_blink_;
void init(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //Enable Port A 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; //Enable Port B
	
	GPIOA->MODER |= GPIO_MODER_MODER5_1; //00 - input, 01 - out, 10 - alternative, 11 - analog
	
}
int main()
{
	init();

	xTaskCreate(blink, "blink", configMINIMAL_STACK_SIZE, NULL, 2, &blink_);
	xTaskCreate(move_blink, "move_blink", configMINIMAL_STACK_SIZE, NULL, 3, &move_blink_);
	vTaskStartScheduler();
	
	while(1);
}

void blink()
{
	while(1)
	{
			vTaskSuspend(move_blink_);
			for(uint8_t i = 0; i < 2*m; i++){
				GPIOA->ODR |= GPIO_ODR_ODR_5; //LED ON
				vTaskDelay(1000*t);
				GPIOA->ODR &= ~GPIO_ODR_ODR_5; //LED OFF
				vTaskDelay(1000*t);
			}
			vTaskResume(move_blink_);
			vTaskDelay(2000);
	}
}

void move_blink()
{
	while(1)
	{
			for(uint8_t j = 0; j < m; j++){
				for(uint8_t i = 0; i < 8; i++)
				{
					GPIOB->ODR = (1 << (7-i)) | (1 << (i+8));
					vTaskDelay(1000);
				}
			}
			vTaskDelay(5);
	}
}
	