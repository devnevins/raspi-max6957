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

// Borrowed from spidev_test.c
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))


void main(int argc, char** argv) {
    // Need to get a file descriptor first
    int spi_channel_0 = open("/dev/spidev0.0", O_RDWR);
    int local_errno;

    if(spi_channel_0 == -1) {
        local_errno = errno; // Need to save it if making any library calls.
        printf("SPI device open error %d: %s\n", local_errno, strerror(local_errno));
        exit(EXIT_FAILURE);
    }

    uint16_t tx_buf[10];
    uint16_t rx_buf[10];

    memset(tx_buf, 0, ARRAY_SIZE(tx_buf));
    memset(rx_buf, 0, ARRAY_SIZE(rx_buf));

    tx_buf[0] = 0xF5A5;

    printf("Message to be transmitted: \"%x\"\n", tx_buf[0]);

    struct spi_ioc_transfer msg;
    memset(&msg, 0, sizeof(struct spi_ioc_transfer));

    msg.tx_buf = (int)tx_buf;
    msg.rx_buf = (int)rx_buf;
    msg.len = 2;
    // msg.bits_per_word = 8;
    // msg.cs_change = 0;
    // msg.speed_hz = 0;
    // msg.delay_usecs = 0;
    // msg.cs_change = 0;
    // msg.pad = 0;

    int status = ioctl(spi_channel_0, SPI_IOC_MESSAGE(1), &msg);

    if(status == -1) {
        local_errno = errno; // Need to save it if making any library calls.
        printf("Post Transmit Error %d: %s\n", local_errno, strerror(local_errno));
        printf("No Message received.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Message received: \"%x\"\n", rx_buf[0]);
    }

}
