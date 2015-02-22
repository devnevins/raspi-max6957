#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "rasp-max6957.h"

// Borrowed from spidev_test.c
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

const char *channel0_device = "/dev/spidev0.0";

static int SPI_fd = -1;

/*
Opens the SPI device and saves the file descriptor.
*/
int initialize(enum SPI_CHANNEL channel) {
  if(channel == SPI_CHANNEL_0) {
    SPI_fd = open("/dev/spidev0.0", O_RDWR);
  }  else if(channel == SPI_CHANNEL_1) {
    SPI_fd = open("/dev/spidev0.1", O_RDWR);
  } else {
    // We "borrow" an error from errno.h since all other errors come from
    // this same source.
    return ENXIO;
  }

  int local_errno;

  // If we get an error make it visible to the calling function.
  if(SPI_fd == -1) {
    return errno;
  }

  return SPI_SUCCESS;
}

int write_raw_word(uint16_t raw_word) {
  if(SPI_fd == -1) {
    // Returns file descriptor in bad state since we don't have
    return EBADFD;
  }
  uint16_t tx_buf = 0;
  uint16_t rx_buf = 0;

  tx_buf = raw_word;

  // Swap the bytes so that the MSB of the lead byte comes out first.
  tx_buf = ((tx_buf >> 8) & 0x00FF) | ((tx_buf << 8) & 0xFF00);

  struct spi_ioc_transfer msg;
  memset(&msg, 0, sizeof(struct spi_ioc_transfer));

  msg.tx_buf = (int)&tx_buf;
  msg.rx_buf = (int)&rx_buf;
  msg.len = 2;

  int status = ioctl(SPI_fd, SPI_IOC_MESSAGE(1), &msg);

  // If we get an error make it visible to the calling function.
  if(status == -1) {
    return errno;
  }

  return SPI_SUCCESS;
}
