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

void main()
{
  uint8_t address[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  uint8_t data[32] = { 0x01, 'A' };
  DHT22MeasurementResult result;

  SPI_CS = 1;
  NRF24_CE = 0;
  SPI_SCK = 0;

  serial_init();

  while(1)
  {
    dht22_measuring(&result);
    data[1] = (uint8_t)result.temperature;
    
    nrf24_transmit(address, data);
    delay_ms(3000);
  }
}
