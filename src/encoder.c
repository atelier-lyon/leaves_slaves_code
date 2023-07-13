#include "proto.h"

void encodeWithoutChecksum(uint8_t* msg, function_name_t function, uint16_t id, uint16_t size, uint8_t* payload) {
    msg[0] = (uint8_t) SOF;
    msg[1] = (uint8_t) function;
    msg[2] = (uint8_t) (id >> 8);
    msg[3] = (uint8_t) (id >> 0);
    msg[4] = (uint8_t) (size >> 8);
    msg[5] = (uint8_t) (size >> 0);

    for (int i = 0; i < size; i++) {
        msg[6 + i] = payload[i];
    }
}

//trame size must be 10 + size
uint8_t* encoder(uint8_t* trame, function_name_t fn_id, uint16_t id, uint16_t size, uint8_t* payload, uint8_t pin){
    encodeWithoutChecksum(trame, fn_id, id, size, payload);
    uint32_t checksum = compute_checksum(trame, size);
    trame[10 + size] = checksum;
    (void) pin; //to avoid unused parameter error
    return trame;
}
