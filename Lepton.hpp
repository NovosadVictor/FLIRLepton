//
// Created by Victor Novosad on 11/01/2020.
//

#ifndef LEPTON3_LEPTON_HPP
#define LEPTON3_LEPTON_HPP

#include <cstdlib>

#include "SPI.hpp"

class Lepton {
public:
    Lepton(SPIConnection *spi_conn) : PACKET_BUFFER_LEN(164), FRAME_BUFFER_LEN(3 * 4 * 60 * 164), spi_conn(spi_conn), rx_buf((uint8_t *) malloc (3 * 4 * 60 * 164)), image((uint8_t *) malloc (4 * 60 * 164)), copy_image((uint8_t *) malloc (4 * 60 * 164)) {};

    ~Lepton() {
        printf("\nLepton destructed\n");
    }

    void getImage();

private:
    SPIConnection *spi_conn;
    size_t PACKET_BUFFER_LEN;
    size_t FRAME_BUFFER_LEN;
    uint8_t *rx_buf;
    uint8_t *image;
    uint8_t *copy_image;
};


#endif //LEPTON3_LEPTON_HPP
