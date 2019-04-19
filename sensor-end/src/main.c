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

void *memcpy (void *dest, const void *src, size_t len)
{
  char *d = dest;
  const char *s = src;
  while (len--)
    *d++ = *s++;
  return dest;
}

void main()
{
  uint8_t data[9];
  uint8_t address[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  DHT22MeasurementResult result;

  io_init();
  serial_init();

  while(1)
  {
    dht22_measuring(&result);
    memcpy(data, &result, sizeof(result));
    nrf24_transmit(address, data);
    delay_ms(3000);
  }
}
