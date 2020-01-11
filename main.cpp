//
// Created by Victor Novosad on 11/01/2020.
//

#include <iostream>

#include "Lepton.hpp"


int main(int argc, char const *argv[]) {
    SPIConnection spi_conn;

    if (spi_conn.is_connected()) {
        uint8_t *image;

        Lepton camera(&spi_conn);

        image = camera.getImage();

        FILE *f = fopen("output.txt", "w");

        for (int i = 0; i < 4 * 60 * 164; i++) {
            fprintf(f, "%d ", image[i]);
        }

        fclose(f);

        return 0;
    }
    else {
        perror("Couldn't connect");
        return -1;
    }
}

