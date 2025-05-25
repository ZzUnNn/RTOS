#ifndef __DHT11_H
#define __DHT11_H
#include "./SYSTEM/sys/sys.h"

#define Dht_PIN GPIO_PIN_0
#define Dht_port GPIOD
#define DHT11_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define DHT11_TIMEOUT_US 1000
#define ReadPinState HAL_GPIO_ReadPin(Dht_port,Dht_PIN)
void Dht11_start(void);
uint8_t Dht11_check(void);
uint8_t read_bit(void);
uint8_t read_byte(void);
uint8_t read_data(int *tem, int *sen ,int *hum);

#endif
