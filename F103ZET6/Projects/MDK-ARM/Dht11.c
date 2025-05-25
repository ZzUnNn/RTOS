#include "Dht11.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"


void Dht11_GPIO_OUT(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_struct;
	gpio_init_struct.Pin = Dht_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(Dht_port,&gpio_init_struct);
}
void Dht11_GPIO_IN(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_struct;
	gpio_init_struct.Pin = Dht_PIN;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;
	gpio_init_struct.Pull = GPIO_PULLUP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(Dht_port,&gpio_init_struct);
}
void Dht11_HIGH(void)
{
	HAL_GPIO_WritePin(Dht_port,Dht_PIN,GPIO_PIN_SET);
}
void Dht11_LOW(void)
{
	HAL_GPIO_WritePin(Dht_port,Dht_PIN,GPIO_PIN_RESET);
}
void Dht11_start(void)
{
	Dht11_GPIO_OUT();
	Dht11_LOW();
	delay_ms(20);
	Dht11_HIGH();
	delay_us(30);
	Dht11_GPIO_IN();
}
/*
uint8_t Dht11_check()
{
	uint32_t timeout = 0;
    uint8_t check = 0;
    
    
    GPIO_InitTypeDef gpio_init_struct = {
        .Pin = Dht_PIN,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(Dht_port, &gpio_init_struct);
    timeout = DHT11_TIMEOUT_US;
    while(HAL_GPIO_ReadPin(Dht_port, Dht_PIN) && (timeout-- > 0)) {
        delay_us(1);
    }
    if(timeout == 0) {
        printf("[DHT11] 错误: 等待初始低电平超时\r\n");
        return 0;
    }

    timeout = DHT11_TIMEOUT_US;
    while(!HAL_GPIO_ReadPin(Dht_port, Dht_PIN) && (timeout-- > 0)) {
        delay_us(1);
    }
    if(timeout == 0) {
        printf("[DHT11] 错误: 等待初始高电平超时\r\n");
        return 0;
    }

    if(HAL_GPIO_ReadPin(Dht_port, Dht_PIN)) {
        check = 1;
        printf("[DHT11] 初始化成功\r\n");
    } else {
        printf("[DHT11] 错误: 设备未保持准备状态\r\n");
    }

    return check;
}
*/
uint8_t read_bit()
{
	uint8_t bit = 0;
	uint8_t tick = 0;
	while(HAL_GPIO_ReadPin(Dht_port,Dht_PIN) == 0);
	delay_us(60);
	if(HAL_GPIO_ReadPin(Dht_port,Dht_PIN) == 1)
	{
		bit = 1;
	}
	else bit = 0;
	while(HAL_GPIO_ReadPin(Dht_port,Dht_PIN) == 1);
	
	
	return bit;
	
}

uint8_t read_byte()
{
	uint8_t i = 0;
	uint8_t byte = 0;
	for(i=0;i<8;i++)
	{
		byte<<=1;
		byte |= read_bit();
	}
	return byte;
}

uint8_t read_data(int *tem,int *sen,int *hum)
{	

	uint8_t data[5];
	uint8_t i = 0;
	Dht11_start();
	
	if(HAL_GPIO_ReadPin(Dht_port,Dht_PIN) == 0)
	{	
		delay_us(160);
		for(i=0;i<5;i++)
		{
			data[i]=read_byte();
		}
		Dht11_LOW();
		delay_us(55);
		Dht11_HIGH();
		printf("Raw data: %d %d %d %d %d\r\n", data[0], data[1], data[2], data[3], data[4]);
		if(data[0]+data[1]+data[2]+data[3]==data[4])
		{
			*tem = data[2];
			*sen = data[3];
			*hum = data[0];
			return 1;
		}
		else
		{
			printf("read_data failed");
		}
	}
	return 0;
}
