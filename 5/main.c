#include "stm32f4xx.h"                  // Device header
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "queue.h"                      // ARM.FreeRTOS::RTOS:Core
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "inttypes.h"



#define t 1

void int_config();
void init();
void EXTI15_10_IRQHandler();
void IST(void *pvParams);
void sos_blink();
TaskHandle_t blink_, sos_blink_;
SemaphoreHandle_t bin;
xQueueHandle x_queue;


int main()
{
	init();
	bin = xSemaphoreCreateBinary();
	if(bin == NULL) return 1;
	x_queue = xQueueCreate(16, sizeof(int));
	if (x_queue == NULL) return 1;
	xTaskCreate(IST, "interrupt", configMINIMAL_STACK_SIZE, NULL, 4, NULL);
	xTaskCreate(sos_blink, "sos_blink", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	vTaskStartScheduler();

	while(1);
}
void init()
{
	int_config();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //Enable Port A 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; //Enable Port C
	GPIOA->MODER |= GPIO_MODER_MODER5_1; //00 - input, 01 - out, 10 - alternative, 11 - analog
	GPIOC->MODER &= (1 << 21) & ~(1 << 22); //PC10 to input
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}
void int_config()
{
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC; //Chose PC13 as signal of interrupt
  EXTI->RTSR = EXTI_RTSR_TR13;// Rising edge as an event                                      
  EXTI->IMR = EXTI_IMR_MR13;                  
  NVIC_EnableIRQ(EXTI15_10_IRQn);              
  NVIC_SetPriority(EXTI15_10_IRQn,5);// Allow NVIC interruption
  __enable_irq(); // Allow NVIC to work

//	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
//	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PC; //Chose PC10 as signal of interrupt
//	EXTI->IMR |= EXTI_IMR_IM10; 
//	EXTI->RTSR |= EXTI_RTSR_TR10; // Rising edge as an event

}


void EXTI15_10_IRQHandler(){
	BaseType_t needCS = pdFALSE;
	EXTI->PR = EXTI_PR_PR13; // Reset interrupt flag
	xSemaphoreGiveFromISR(bin, &needCS);
	if(needCS == pdTRUE)
	{
		portYIELD_FROM_ISR(needCS);
	}

}


// task interrupt routine
void IST(void *pvParams)
{
	int regime = 0;
	while(1)
	{
		//
		if((xSemaphoreTake(bin, portMAX_DELAY)) == pdPASS)
			{
				xQueueReceive(x_queue, &regime, 0);
				  regime=!regime;
				  xQueueSendToFrontFromISR(x_queue,&regime,0);
			}
	}


}
void sos_blink()
{
	int regime = 0;
	while(1)
	{
		
		xQueueReceive(x_queue, &regime, 0);
      
				for(uint8_t i = 0; (i < 3)&& regime; i++)
				{
					GPIOA->ODR |= GPIO_ODR_ODR_5; //LED ON
					vTaskDelay(t*1000);
					GPIOA->ODR &= ~GPIO_ODR_ODR_5; //LED OFF	 
					vTaskDelay(t*1000);
					xQueueReceive(x_queue, &regime, 0);
						
				}
				for(uint8_t i = 0; (i < 3)&& regime; i++)
				{
					GPIOA->ODR |= GPIO_ODR_ODR_5; //LED ON
					vTaskDelay(t*1000*3);
					GPIOA->ODR &= ~GPIO_ODR_ODR_5; //LED OFF	 
					vTaskDelay(t*1000*3);
					xQueueReceive(x_queue, &regime, 0);
						
				}
				for(uint8_t i = 0; (i < 3)&& regime; i++)
				{
					GPIOA->ODR |= GPIO_ODR_ODR_5; //LED ON
					vTaskDelay(t*1000);
					GPIOA->ODR &= ~GPIO_ODR_ODR_5; //LED OFF
					vTaskDelay(t*1000);
					xQueueReceive(x_queue, &regime, 0);
				}
			
		if(regime != 1){
		
			GPIOA->ODR &= ~GPIO_ODR_ODR_5;
			
		}		
	}
}





