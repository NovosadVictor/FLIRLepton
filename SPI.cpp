//
// Created by Victor Novosad on 11/01/2020.
//

#include <stdlib.h>

#include "SPI.hpp"


SPIConnection::SPIConnection(const char *device, const uint8_t mode, const int speed, uint8_t bits, bool print) {
    speed_ = speed;
    bits_ = bits;
    int ret;

    is_connected_ = false;

    /* open device */
    file_descriptor_ = open(device, O_RDWR);
    if (file_descriptor_ < 0) {
        perror("Error open device");
        return;
    }
    usleep(2000);

    /* set mode */
    ret = ioctl(file_descriptor_, SPI_IOC_WR_MODE, &mode);
    if (ret < 0) {
        perror("Error set SPI-Mode");
    }
    usleep(2000);

    /* check mode */
    ret = ioctl(file_descriptor_, SPI_IOC_RD_MODE, &mode);
    if (ret < 0) {
        perror("Error get SPI-Mode");
    }
    usleep(2000);

    /* set word length */
    ret = ioctl(file_descriptor_, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret < 0) {
        perror("Error set word length");
        return;
    }
    usleep(2000);

    /* check word length */
    ret = ioctl(file_descriptor_, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret < 0) {
        perror("Error get word length");
        return;
    }
    usleep(2000);

    /* set speed */
    ret = ioctl(file_descriptor_, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret < 0) {
        perror("Error set speed");
        return;
    }
    usleep(2000);

    /* check speed */
    ret = ioctl(file_descriptor_, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret < 0) {
        perror("Error get speed");
        return;
    }
    usleep(2000);

    is_connected_ = true;

    if (print) {
        printf("\n\nSPI-Device.....: %s\n", device);
        printf("SPI-Mode.......: %d\n", mode);
        printf("Bits per word.....: %d\n", bits);
        printf("Speed..........: %d Hz (%d kHz)\n", speed, speed / 1000);
        printf("File descriptor.....: %d\n\n\n", file_descriptor_);
    }
}

void SPIConnection::transfer(uint8_t *tx_buf, uint8_t *rx_buf, size_t length) {
    int ret;

    // clear memory
    memset(&spi_, 0, sizeof(spi_));

    // set spi_ioc_transfer parameters
    spi_.tx_buf = (unsigned long) tx_buf;
    spi_.rx_buf = (unsigned long) rx_buf;
    spi_.len = length;
    spi_.delay_usecs = (uint16_t) 6500;
    spi_.speed_hz = speed_;
    spi_.bits_per_word = bits_;
    spi_.cs_change = 0;

    ret = ioctl(file_descriptor_, SPI_IOC_MESSAGE(1), &spi_);

    if (ret < 1) {
        perror("Can't send / read spi data");
        std::abort();
    }
}

