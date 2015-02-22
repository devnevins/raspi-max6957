#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "rasp-max6957.h"

#define FOREVER 1

void main(int argc, char** argv) {
    int status = initialize(SPI_CHANNEL_0);

    if(status != SPI_SUCCESS) {
      printf("SPI Initialization Error %d: %s\n", status, strerror(status));
      exit(EXIT_FAILURE);
    }

    // 0x0D00 Sets P22 to LED Driver
    status = write_raw_word(0x0D00);
    if(status != SPI_SUCCESS) {
      printf("SPI Write Error %d: %s\n", status, strerror(status));
      exit(EXIT_FAILURE);
    }

    // 0x36FF Turns on P22 bit
    status = write_raw_word(0x36FF);
    if(status != SPI_SUCCESS) {
      printf("SPI Write Error %d: %s\n", status, strerror(status));
      exit(EXIT_FAILURE);
    }

    // 0x36FF Set global LED current to
    status = write_raw_word(0x020A);
    if(status != SPI_SUCCESS) {
      printf("SPI Write Error %d: %s\n", status, strerror(status));
      exit(EXIT_FAILURE);
    }

    // 0x0401 Turns MAX 6957 device on from shutdown mode
    status = write_raw_word(0x0401);
    if(status != SPI_SUCCESS) {
      printf("SPI Write Error %d: %s\n", status, strerror(status));
      exit(EXIT_FAILURE);
    }

    while(FOREVER) {
      sleep(1);
      // 0x36FF Turns off P20 bit
      status = write_raw_word(0x3600);
      if(status != SPI_SUCCESS) {
        printf("SPI Write Error %d: %s\n", status, strerror(status));
        exit(EXIT_FAILURE);
      }
      sleep(1);
      // 0x36FF Turns on P20 bit
      status = write_raw_word(0x36FF);
      if(status != SPI_SUCCESS) {
        printf("SPI Write Error %d: %s\n", status, strerror(status));
        exit(EXIT_FAILURE);
      }
    }
}
