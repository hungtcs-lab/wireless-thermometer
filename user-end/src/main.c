#include "delay.h"
#include "stdio.h"
#include "nRF24.h"
#include "dht22.h"
#include "serial.h"
#include <mcs51/8051.h>

int putchar(int c)
{
	while(!(SCON & 0x02));
	SCON &= ~0x02;
	SBUF = (char)c;
  return c;
}

void io_init()
{
  SPI_CS = 1;
  NRF24_CE = 0;
  SPI_SCK = 0;
}

void main()
{
  DHT22MeasurementResult result;
  uint8_t address[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

  io_init();
  serial_init();
  nrf24_listen(address);

  while(1)
  {
    if(nrf24_available())
    {
      nrf24_read_received_data((uint8_t *)&result);
      printf("humidity: %d.%d%%\n", (int)result.humidity, (int)((result.humidity - ((int)result.humidity)) * 10));
      printf("temperature: %d.%d`C\n", (int)result.temperature, (int)((result.temperature - ((int)result.temperature)) * 10));
    }
  }
}
