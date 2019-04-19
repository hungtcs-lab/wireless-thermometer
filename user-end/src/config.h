#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <mcs51/8051.h>

// spi pins
#define SPI_CS          P1_1
#define SPI_SCK         P1_2
#define SPI_MOSI        P1_3
#define SPI_MISO        P1_4

// nRF24L01 pins
#define NRF24_CE        P1_0
#define NRF24_IRQ       P1_5

#define DHT22_PIN       P0_0

// OLED pins
// #define OLED_DC         P1_4
// #define OLED_RESET      P1_5

#endif
