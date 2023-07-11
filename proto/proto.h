#ifndef PROTO_H
#define PROTO_H
#include <stdio.h>
#include <err.h>
#include <inttypes.h>

#define MAX_PAYLOAD_SIZE 4096

typedef union {
    uint32_t raw;
    struct {
        uint8_t id;
        uint8_t payload[3];
    } content;
} function;

struct protoframe {
    function SetColorRGB;
    function SetColorHSV;
    // TODO: ...
    uint8_t FocusOn;
    // TODO: ...
    uint8_t discovery[];
} __attribute__((__packed__)); // evite le padding

// Fn must check the checksum which is 4 bytes after the payload
void decoder(uint8_t bufferbyte);

char* encoder(uint8_t fn_id, uint16_t id, uint16_t size,uint8_t* payload);

#endif
