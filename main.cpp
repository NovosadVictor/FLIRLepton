//
// Created by Victor Novosad on 11/01/2020.
//

#include <iostream>

#include "Lepton.hpp"


int main(int argc, char const *argv[]) {
    SPIConnection spi_conn;

    if (spi_conn.is_connected()) {
        Lepton camera(&spi_conn);

        for (int s = 0; s < 1000; s++) {
            camera.getImage();
        }

        return 0;
    }
    else {
        perror("Couldn't connect");
        return -1;
    }
}

