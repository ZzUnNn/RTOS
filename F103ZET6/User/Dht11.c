#include "Dht11.h"
#include "./SYSTEM/delay/delay.h"
void Dht11_start(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	gpio_init_struct.Pin=Dht_PIN;
	gpio_init_struct.Mode=GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(Dht_port,&gpio_init_struct);
	
	HAL_GPIO_WritePin(Dht_port,Dht_PIN,GPIO_PIN_RESET);
	HAL_Delay(18);
	
	HAL_GPIO_WritePin(Dht_port,Dht_PIN,GPIO_PIN_SET);
	delay_us(30);
}

uint8_t Dht11_check()
{
	int check=0;
	GPIO_InitTypeDef gpio_init_struct1;
	gpio_init_struct1.Pin=Dht_PIN;
	gpio_init_struct1.Mode=GPIO_MODE_INPUT;
	gpio_init_struct1.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(Dht_port,&gpio_init_struct1);
	
	while(HAL_GPIO_ReadPin(Dht_port,Dht_PIN));
	while(!HAL_GPIO_ReadPin(Dht_port,Dht_PIN));
	
	if(HAL_GPIO_ReadPin(Dht_port,Dht_PIN))
	{
		check = 1;
	}
	return check;
}

uint8_t read_bit()
{
	uint8_t bit = 0;
	uint8_t tick = 0;
	while(HAL_GPIO_ReadPin(Dht_port,Dht_PIN));
	delay_us(40);
	
	while(HAL_GPIO_ReadPin(Dht_port,Dht_PIN))
	{
		delay_us(1);
		tick++;
	}
	if(25 < tick &&tick<30)
	{
		bit = 0;
	}
	else if(tick>60)
	{
		bit = 1;
	}
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

uint8_t read_data(int *tem,int *hum)
{
	uint8_t data[5];
	uint8_t i = 0;
	for(i=0;i<5;i++)
	{
		data[i]=read_byte();
	}
	if(data[0]+data[1]+data[2]+data[3]==data[4])
	{
		*tem = data[0];
		*hum = data[2];
	}
}