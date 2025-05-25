#ifndef __DHT11_H
#define __DHT11_H
#include "./SYSTEM/sys/sys.h"

#define Dht_PIN GPIO_PIN_2
#define Dht_port GPIOC
void Dht11_start(void);
uint8_t Dht11_check(void);
uint8_t read_bit(void);
uint8_t read_byte(void);
uint8_t read_data(int *tem, int *sen ,int *hum);

#endif
