#include "proto.h"
#include <stdint.h>


/**
 * @brief decode the frame given in parameter and fill the protoframe structure given in parameter
 *
 * @param frame the frame buffer to decode
 * @param buffer the (preferably empty) protoframe structure to fill
 */
struct protoframe *decoder(const uint8_t* buffer) {
    // Decode fn_id
    uint8_t fn_id = *(buffer);
    uint16_t id = *(buffer + 1);
    uint16_t size = *(buffer + 3);
    uint8_t *payload = NULL;
    uint8_t checksum = *(buffer + 4 + size);

    // Checksum
    uint32_t calculated_checksum = 0;
    for (uint16_t i = 0; i < size; i++) {
        calculated_checksum += payload[i];
    }
     if (calculated_checksum != checksum) return NULL;

    struct protoframe frame = {
        .fn_id = fn_id,
        .id = id,
        .size = size,
        .payload = payload,
        .checksum = checksum
    };
    return &frame;
}
