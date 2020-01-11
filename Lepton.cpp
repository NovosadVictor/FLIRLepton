//
// Created by Victor Novosad on 11/01/2020.
//

#include "Lepton.hpp"


uint8_t *Lepton::getImage() {
    size_t length = 164;
    size_t full_image_length = 4 * 60 * length;

    uint8_t rx_buf[length];

    unsigned int segment_number = 0;
    bool is_right_segment_number = false;

    uint8_t *prev_segment = &image_[0];
    uint8_t *packet_pos = &image_[0];

    while (packet_pos <= image_ + full_image_length - length) {
        spi_conn_->transfer(NULL, rx_buf, length);

        if ((rx_buf[0] & 0x0F) == 0x0F) {         // check if packet is broken
            is_right_segment_number = false;
            packet_pos = prev_segment;
            continue;
        }

        if (!(is_right_segment_number) && (rx_buf[1] == 20) && ((rx_buf[0] >> 4) == segment_number)) {      // check if segment is valid
            is_right_segment_number = true;
        }

        // if packet is valid - concatenate
        memcpy(packet_pos, rx_buf, sizeof(uint8_t) * length);
        packet_pos += length;

        // if it's the last packet of current segment and the segment is valid - concatenate
        if ((is_right_segment_number) && (rx_buf[1] == 59)) {
            prev_segment = packet_pos;

            is_right_segment_number = false;
            segment_number += 1;
        }
    }

    return image_;
}