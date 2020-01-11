//
// Created by Victor Novosad on 11/01/2020.
//

#ifndef LEPTON3_LEPTON_HPP
#define LEPTON3_LEPTON_HPP

#include "SPI.hpp"
#include <vector>


class Lepton {
public:
    Lepton(SPIConnection *spi_conn) : spi_conn_(spi_conn), image_((uint8_t *)malloc( size_t 4 * 60 * 164)) {};

    ~Lepton() {
        printf("\nLepton destructed\n");
    }

    uint8_t *getImage();

private:
    SPIConnection *spi_conn_;
    uint8_t *image_;
};


#endif //LEPTON3_LEPTON_HPP
