//
// Created by Victor Novosad on 11/01/2020.
//
#ifndef LEPTON3_SPI_HPP
#define LEPTON3_SPI_HPP

#include <fcntl.h>     // Needed for SPI port
#include <sys/ioctl.h> // Needed for SPI port
#include <errno.h>     // linux errors
#include <unistd.h>    // sleep()
#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>    // for  uint8_t,  uint16_t etc
#include <linux/spi/spidev.h> // fpr spi_ioc_transfer
#include <cstring>


class SPIConnection {
public:
    SPIConnection(
            const char *device = "/dev/spidev0.0", // device path
            const uint8_t mode = SPI_CPOL | SPI_CPHA, // spi mode (Clock polarity and phase) default=CPOL|CPHA (3)
            const int speed = 16000000, // spi speed (Hz)
            uint8_t bits = 8, // bits per word, default=8
            bool print = true // print connection details
    );

    ~SPIConnection() {
        if (file_descriptor_) {
            close(file_descriptor_);
        };
        printf("\nSPI destructed\n");
    };

    bool is_connected() {
        return is_connected_;
    }

    void transfer(uint8_t *tx_buf, uint8_t *rx_buf, size_t length);

private:
    int file_descriptor_; // device file descriptor
    int speed_;
    uint8_t bits_;
    struct spi_ioc_transfer spi_;

    bool is_connected_;
};


#endif //LEPTON3_SPI_HPP
