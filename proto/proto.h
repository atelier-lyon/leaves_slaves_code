#ifndef PROTO_H
#define PROTO_H
#include <stdio.h>
#include <err.h>
#include <inttypes.h>

#define MAX_PAYLOAD_SIZE 4096

struct protoframe {
    uint8_t function_name;
    uint16_t id;
    uint16_t size;
    uint8_t payload[MAX_PAYLOAD_SIZE];
};


// Fn must check the checksum which is 4 bytes after the payload
void decoder(uint8_t bufferbyte);

char* encoder(uint8_t fn_id, uint16_t id, uint16_t size,uint8_t* payload);

#endif
