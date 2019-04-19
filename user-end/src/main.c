#include <mcs51/8052.h>
#include "oled.h"
#include "delay.h"
#include "stdio.h"
#include "nRF24.h"
#include "dht22.h"
#include "serial.h"

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
  uint8_t i;
  uint8_t data[9];
  uint8_t address[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  const char celsius_symbol[] = { 95 + 32, 'C', '\0' };
  DHT22MeasurementResult result;

  io_init();
  oled_init();
  oled_clear();
  serial_init();
  nrf24_listen(address);

  while(1)
  {
    if(nrf24_available())
    {
      nrf24_read_received_data(data);
      memcpy(&result, data, sizeof(result));
      if(result.state == 0)
      {
        oled_clear();

        i = 0;
        oled_put_chars_8x16(0, 0, "Temperature:");
        if(result.temperature < 0)
        {
          oled_put_chars_8x16(i++, 1, "-");
          result.temperature = -result.temperature;
        }
        oled_put_char_8x16(i++, 1, (result.temperature / 10) + 48);
        oled_put_char_8x16(i++, 1, (((int)result.temperature) % 10) + 48);
        oled_put_char_8x16(i++, 1, '.');
        oled_put_char_8x16(i++, 1, ((int)((result.temperature - ((int)result.temperature)) * 10)) + 48);
        oled_put_chars_8x16(i, 1, celsius_symbol);

        i=0;
        oled_put_chars_8x16(0, 2, "Humidity:");
        oled_put_char_8x16(i++, 3, (result.humidity / 10) + 48);
        oled_put_char_8x16(i++, 3, (((int)result.humidity) % 10) + 48);
        oled_put_char_8x16(i++, 3, '.');
        oled_put_char_8x16(i++, 3, ((int)((result.humidity - ((int)result.humidity)) * 10)) + 48);
        oled_put_chars_8x16(i, 3, "%");
      }
      else
      {
        oled_clear();
        oled_put_chars_8x16(0, 0, "Error:");
        if(result.state == 1) {
          oled_put_chars_8x16(0, 1, "No response");
        }
        else if(result.state == 1)
        {
          oled_put_chars_8x16(0, 1, "Checksum error");
        }
        oled_put_chars_8x16(0, 3, "  Retrying...");
      }
    }
  }
}
