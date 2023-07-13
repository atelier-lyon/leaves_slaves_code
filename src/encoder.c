#include "proto.h"

void encodeWithoutChecksum(uint16_t* msg, function_name_t function, uint16_t size, uint8_t* payload) {
    msg[0] = SOF;
    msg[1] = (uint8_t) (function >> 8);
    msg[2] = (uint16_t) (function >> 0);
    msg[3] = (uint16_t) (size >> 8);
    msg[4] = (uint8_t) (size >> 0);

    for (int i = 0; i < size; i++) {
        msg[9 + i] = payload[i];
    }
}

char* encoder(function_name_t fn_id, uint16_t id, uint16_t size, uint8_t* payload, uint8_t pin){
    uint32_t trame[10 + size];
    encodeWithoutChecksum(trame, fn_id, size, payload);
    uint32_t checksum = compute_checksum(trame, size);
    trame[10 + size] = checksum;
    return trame;
}
