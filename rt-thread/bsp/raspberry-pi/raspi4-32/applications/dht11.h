#ifndef __DHT11_H
#define __DHT11_H
#include <rtdevice.h>
#include <drv_gpio.h>
#include <delay_us.h>
#define Dht_PIN GPIO_PIN_2
/**/
struct data_struct
{
    int temperature;
    int temperature_xiaoshu;
    int humidity;
    int humidity_xiaoshu;
};

void Dht11_start(void);
uint8_t read_bit(void);
void Dht11_GPIO_OUT();
void Dht11_GPIO_IN();
void Dht11_HIGH();
void Dht11_LOW();
uint8_t read_byte(void);
struct data_struct read_data(int *tem,int *ten_xiaoshu,int *hum,int *hum_xiaoshu);

#endif