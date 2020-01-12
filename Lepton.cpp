//
// Created by Victor Novosad on 11/01/2020.
//
#include <boost/thread.hpp>  // async

#include "Lepton.hpp"

static unsigned int img_index = 0;

void saveImage(uint8_t *image_buf, unsigned int index) {
    char file_name[124];
    snprintf(file_name, sizeof(file_name), "%d.txt", index);

    FILE *f = fopen(file_name, "w");

    for (int i = 0; i < 240 * 164; i++) {
        fprintf(f, "%d ", image_buf[i]);
    }

    fclose(f);
}


void Lepton::getImage() {
    uint8_t status_bits = 0;

    int row;
    int packet;
    int packet_number, segment_number;
    int current_segment_number;
    bool valid_segment = false;

    while (status_bits != 0x0F) {
        spi_conn->transfer(NULL, rx_buf, FRAME_BUFFER_LEN);

        for (int packet_index = 0; packet_index < (FRAME_BUFFER_LEN / PACKET_BUFFER_LEN); packet_index++) {
            packet = packet_index * PACKET_BUFFER_LEN;

            if ((rx_buf[packet] & 0x0F) == 0x0F) {
                valid_segment = false;
                continue;
            }

            packet_number = rx_buf[packet + 1];

            if ((packet_number > 0) && !(valid_segment)) {
                continue;
            }

            if (valid_segment && (packet_number == 0)) {
                valid_segment = false;
            }

            // found start of the new segment
            if (!(valid_segment) && (packet_number == 0) && ((packet + 60 * PACKET_BUFFER_LEN) < FRAME_BUFFER_LEN)) {
                segment_number = rx_buf[packet + 20 * PACKET_BUFFER_LEN] >> 4;
                if ((segment_number > 0) && (segment_number < 5) && (rx_buf[packet + 20 * PACKET_BUFFER_LEN + 1] == 20)) {
                    valid_segment = true;
                    current_segment_number = segment_number;
                }
            }

            if (!valid_segment) {
                continue;
            }

            row = packet_number + (segment_number - 1) * 60;
            memcpy(image + row * PACKET_BUFFER_LEN, rx_buf + packet, PACKET_BUFFER_LEN);

            if (packet_number == 59) {
                status_bits |= (0x01 << (current_segment_number - 1));
            }
        }
    }

    img_index += 1;

    memcpy(copy_image, image, (4 * 60 * 164) * sizeof(uint8_t));
    boost::thread t{saveImage, copy_image, img_index};
    t.join();

    return;
}