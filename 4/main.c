#include "stm32f4xx.h"                  // Device header
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "FreeRTOSConfig.h"             // ARM.FreeRTOS::RTOS:Config
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "queue.h"                      // ARM.FreeRTOS::RTOS:Core
#include "inttypes.h"
#include "semphr.h"


#define t 1
#define m 3
void blink();
void sos_blink();
TaskHandle_t blink_, sos_blink_;
SemaphoreHandle_t mutex;
void init(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //Enable Port A 
	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; //Enable Port B
	
	GPIOA->MODER |= GPIO_MODER_MODER5_1; //00 - input, 01 - out, 10 - alternative, 11 - analog
	
}
int main()
{
	mutex = xSemaphoreCreateMutex();
	init();

	xTaskCreate(blink, "blink", configMINIMAL_STACK_SIZE, NULL, 2, &blink_);
	xTaskCreate(sos_blink, "sos_blink", configMINIMAL_STACK_SIZE, NULL, 2, &sos_blink_);
	vTaskStartScheduler();
	
	while(1);
}

void blink()
{
	while(1)
	{
		if(xSemaphoreTake(mutex,((TickType_t)50000))==pdPASS){
			for(uint8_t i = 0; i < 5*m; i++){
			
					GPIOA->ODR |= GPIO_ODR_ODR_5; //LED ON
					vTaskDelay(t*1000);
					GPIOA->ODR &= ~GPIO_ODR_ODR_5; //LED OFF		 
					vTaskDelay(t*1000);
			}
		}	
		xSemaphoreGive(mutex);
		vTaskDelay(50); 
	}
}

void sos_blink()
{
	while(1)
	{
			if(xSemaphoreTake(mutex,((TickType_t)50000))==pdPASS){
				for(uint8_t i = 0; i < 3; i++)
				{
					GPIOA->ODR |= GPIO_ODR_ODR_5; //LED ON
					vTaskDelay(t*1000);
					GPIOA->ODR &= ~GPIO_ODR_ODR_5; //LED OFF	 
					vTaskDelay(t*1000);
						
				}
				for(uint8_t i = 0; i < 3; i++)
				{
					GPIOA->ODR |= GPIO_ODR_ODR_5; //LED ON
					vTaskDelay(t*1000*3);
					GPIOA->ODR &= ~GPIO_ODR_ODR_5; //LED OFF	 
					vTaskDelay(t*1000*3);
						
				}
				for(uint8_t i = 0; i < 3; i++)
				{
					GPIOA->ODR |= GPIO_ODR_ODR_5; //LED ON
					vTaskDelay(t*1000);
					GPIOA->ODR &= ~GPIO_ODR_ODR_5; //LED OFF
					vTaskDelay(t*1000);
						
				}
			}
		xSemaphoreGive(mutex);
		vTaskDelay(50); 
	}
}
