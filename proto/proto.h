#ifndef PROTO_H
#define PROTO_H
#include <stdio.h>
#include <err.h>
#include <inttypes.h>

struct protoframe {
    uint8_t fn_id;
    uint16_t id;
    uint16_t size;
    uint8_t* payload;
    uint32_t checksum;
};

// Fn must check the checksum which is 4 bytes after the payload
struct protoframe *decoder(const uint8_t* buffer);

char* encoder(uint8_t fn_id, uint16_t id, uint16_t size,uint8_t* payload);

#endif
