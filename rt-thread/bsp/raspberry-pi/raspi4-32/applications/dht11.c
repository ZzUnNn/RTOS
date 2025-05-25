#include <rtdevice.h>
#include <dht11.h>

void Dht11_GPIO_OUT()
{
    /* 设定 dht 引脚为推挽输出模式 */
    rt_pin_mode(Dht_PIN, PIN_MODE_OUTPUT);
}
void Dht11_GPIO_IN()
{
    /* 设定 dht 引脚为输入模式 */
    rt_pin_mode(Dht_PIN, PIN_MODE_INPUT);
}
void Dht11_HIGH()
{
    /* 拉高 dht 引脚 */
    rt_pin_write(Dht_PIN,PIN_HIGH);
}
void Dht11_LOW()
{
    /* 拉低 dht 引脚 */
    rt_pin_write(Dht_PIN,PIN_LOW);
}
void Dht11_start(void)
{
    /* 拉低20ms再拉高30us启动通信 */
	Dht11_GPIO_OUT();
	Dht11_LOW();
	rt_thread_mdelay(20);
	Dht11_HIGH();
	rt_hw_us_delay(30);
	Dht11_GPIO_IN();
}

uint8_t read_bit()
{
    /* 轮询处理，高电平持续60us则读取1 */
	uint8_t bit = 0;
	while(rt_pin_read(Dht_PIN) == 0);
	rt_hw_us_delay(60);
	if(rt_pin_read(Dht_PIN) == 1)
	{
		bit = 1;
	}
	else bit = 0;
    /* 轮询处理，等待高电平结束开始下一次通信 */
	while(rt_pin_read(Dht_PIN) == 1);
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
struct data_struct read_data(int *tem,int *ten_xiaoshu,int *hum,int *hum_xiaoshu)
{	

	uint8_t data[5];
	uint8_t i = 0;
	Dht11_start();
    struct data_struct raw_data;
    raw_data.temperature = *tem;
    raw_data.temperature_xiaoshu = *ten_xiaoshu;
    raw_data.humidity = *hum;
    raw_data.humidity_xiaoshu = *hum_xiaoshu;
	if(rt_pin_read(Dht_PIN) == 0)
	{	
        /* 延时160us等待通信稳定 */
		rt_hw_us_delay(160);
		for(i=0;i<5;i++)
		{
			data[i]=read_byte();
		}
		Dht11_LOW();
		rt_hw_us_delay(55);
		Dht11_HIGH();
		rt_kprintf("Raw data: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
		if(data[0]+data[1]+data[2]+data[3] == data[4])
		{
			*tem = data[2];
			*ten_xiaoshu = data[3];
			*hum = data[0];
            *hum_xiaoshu = data[1];
			return raw_data;
		}
		else
		{
			rt_kprintf("read_data failed\r\n");
		}
	}
}