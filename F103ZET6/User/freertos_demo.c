
#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Dht11.h"
#include "./SYSTEM/sys/sys.h"
#include "./BSP/KEY/key.h"
#include "queue.h"
#include "event_groups.h" 
#define EVENT_bits0 1 << 0
#define EVENT_bits1 1 << 1
int tem = 1;
int hum = 1;
int max = 0;
int sen = 0;
uint8_t data_send[3] = {NULL,NULL};
uint8_t data_receive[3] = {NULL,NULL};

void start_task(void* pvParameters);
void task_sensor(void* pvParameters);
void task_handle(void* pvParameters);
void task_led(void* pvParameters);
void task_usart(void* pvParameters);
void task_button(void* pvParameters);

EventGroupHandle_t buttongroup_handle;

QueueHandle_t data_queue;
QueueHandle_t err_queue;
QueueHandle_t usart_queue;

TaskHandle_t start_task_handler;
TaskHandle_t task_sensor_handler;
TaskHandle_t task_handle_handler;
TaskHandle_t task_led_handler;
TaskHandle_t task_usart_handler;
TaskHandle_t task_button_handler;
void start_task(void* pvParameters)
{
	taskENTER_CRITICAL();
	buttongroup_handle = xEventGroupCreate();
	data_queue = xQueueCreate(3,3*sizeof(uint8_t));
	err_queue = xQueueCreate(1,sizeof(uint8_t));
	usart_queue = xQueueCreate(3,3*sizeof(uint8_t));
	xTaskCreate(
                           (TaskFunction_t) task_sensor,
                           (char *)"task_sensor",
                           (configSTACK_DEPTH_TYPE) 128,
                           (void *)NULL,
                           (UBaseType_t) 6,
                           (TaskHandle_t *)&task_sensor_handler
                        );
	xTaskCreate(
                           (TaskFunction_t) task_handle,
                           (char *)"task_handle",
                           (configSTACK_DEPTH_TYPE) 128,
                           (void *)NULL,
                           (UBaseType_t) 5,
                           (TaskHandle_t *)&task_handle_handler
                        );	
	xTaskCreate(
                           (TaskFunction_t) task_led,
                           (char *)"task_led",
                           (configSTACK_DEPTH_TYPE) 128,
                           (void *)NULL,
                           (UBaseType_t) 4,
                           (TaskHandle_t *)&task_led_handler
                        );
	xTaskCreate(
                           (TaskFunction_t) task_usart,
                           (char *)"task_usart",
                           (configSTACK_DEPTH_TYPE) 128,
                           (void *)NULL,
                           (UBaseType_t) 3,
                           (TaskHandle_t *)&task_usart_handler
                        );
	xTaskCreate(
                           (TaskFunction_t) task_button,
                           (char *)"task_button",
                           (configSTACK_DEPTH_TYPE) 128,
                           (void *)NULL,
                           (UBaseType_t) 2,
                           (TaskHandle_t *)&task_button_handler
                        );

	taskEXIT_CRITICAL();
	vTaskDelete(NULL);
}

void task_sensor(void* pvParameters)
{
	
	uint8_t *buf = &data_send[0];
	BaseType_t check;
	BaseType_t usart_handle;
	TickType_t pxPreviousWakeTime;
	uint8_t key = 0;
	
	/*while(!Dht11_check())
	{
		printf("initialize failed\n");
		vTaskDelay(3000);
	}*/
	pxPreviousWakeTime = xTaskGetTickCount();
	while(1)
	{
		
		vTaskDelayUntil(&pxPreviousWakeTime,500);	
		printf("Temperature£∫%d.%d°„C£¨\r\nHumidity£∫%d%% \n",tem,sen,hum);
		if(read_data(&tem,&sen,&hum)) 
		{
			data_send[0] = hum;
			data_send[1] = tem;
			data_send[2] = sen;
			check = xQueueSend( data_queue, buf, portMAX_DELAY );
			usart_handle = xQueueSend( usart_queue, buf, portMAX_DELAY );
		}
		if(check == pdTRUE )
		{
			printf("dataQueueSend succeeded\r\n");
			
		}else printf("dataQueueSend failed\r\n");
		if(usart_handle == pdTRUE)
		{
			printf("usartQueueSend succeeded\r\n");
		}
		else printf("usartQueueSend failed\r\n");
		
		
		

		
		key = key_scan(0);
		if(key == KEY0_PRES)
		{
			xEventGroupSetBits( buttongroup_handle,EVENT_bits0);
		}
		else if(key == KEY1_PRES)
		{
			xEventGroupSetBits( buttongroup_handle,EVENT_bits1);
		}
		
		
	}

}
void task_handle(void* pvParameters)
{	
	BaseType_t check;
	uint8_t *buf = &data_receive[0];
	uint8_t err = NULL;
	TickType_t pxPreviousWakeTime;
	pxPreviousWakeTime = xTaskGetTickCount();
	while(1)
	{
		
		
		vTaskDelayUntil(&pxPreviousWakeTime,500);
		
		
		check = xQueueReceive( data_queue,buf,100);
		if(check == pdTRUE)
		{
			printf("dataQueueReceive succeeded\r\n");
		}else printf("dataQueueReceive failed\r\n");
		
		
		err = data_receive[1];
		if(err != NULL) 
		{
			check = xQueueSend( err_queue, &err, 10 );
		}
		if(check == pdTRUE)
		{
			printf("errQueueSend succeeded\r\n");
		}else printf("errQueueSend failed\r\n");
	}		
	
}
void task_led(void* pvParameters)
{
	led_init();
	BaseType_t check;
	uint8_t err;
	while(1)
	{	
		check = xQueueReceive( err_queue,&err,100);
		if(check == pdTRUE)
		{
			printf("errQueueReceive succeeded\r\n");
		}else printf("errQueueReceive failed\r\n");
		while(err > max)
		{
			LED0_TOGGLE();
			vTaskDelay(200);
			LED0_TOGGLE();	
			vTaskDelay(200);
			
		}
	}
}
	
void task_usart(void* pvParameters)
{
	usart_init(115200);
	uint8_t rx_buf[1];
    uint8_t tx_buf[64];
	HAL_UART_Receive_IT(&g_uart1_handle, rx_buf, 1);
	uint8_t sensor_data[2];
	BaseType_t check;
	while(1)
	{
		
		check = xQueueReceive( usart_queue,sensor_data,100);
		if(check == pdTRUE)
		{
			printf("usartQueueReceive succeeded\r\n");
		}else printf("usartQueueReceive failed\r\n");

		snprintf((char*)tx_buf, sizeof(tx_buf),  "Temperature: %d°„C, Humidity: %d%%\r\n", sensor_data[1], sensor_data[0]);
		
		HAL_UART_Transmit(&g_uart1_handle, tx_buf, sizeof((char*)tx_buf), HAL_MAX_DELAY);
		
	}
}
void task_button(void* pvParameters)
{	
	EventBits_t event_bit;
	while(1)
	{
		event_bit = xEventGroupWaitBits( buttongroup_handle,EVENT_bits0,pdTRUE,pdTRUE,100);
		if(event_bit == (EventBits_t) EVENT_bits0)
		{
			//vTaskSuspendAll();
		}else if(event_bit == (EventBits_t) EVENT_bits1)
		{
			//xTaskResumeAll();
		}
	}
		
}

void freertos_demo(void)
{
	
	xTaskCreate(
                           (TaskFunction_t) start_task,
                           (char *)"start_task",
                           (configSTACK_DEPTH_TYPE) 128,
                           (void *)NULL,
                           (UBaseType_t) 1,
                           (TaskHandle_t *)&start_task_handler
						   );
	vTaskStartScheduler();
}

