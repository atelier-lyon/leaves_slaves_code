#ifndef PROTO_H
#define PROTO_H
#include <stdio.h>
#include <err.h>
#include <inttypes.h>

// Macros
#define MAX_PAYLOAD_SIZE 4096
#define SOF 0x69

// ######################################################

typedef union {
    uint32_t raw;
    struct {
        uint8_t id;
        uint8_t payload[3];
    } content;
} function;

struct protoframe_v2 {
    function SetColorRGB;
    function SetColorHSV;
    // TODO: ...
    uint8_t FocusOn;
    // TODO: ...
    uint8_t discovery[];
} __attribute__((__packed__)); // evite le padding


// ######################################################

// Frame structure
struct protoframe {
    uint8_t function_name;
    uint16_t id;
    uint16_t size;
    uint8_t payload[MAX_PAYLOAD_SIZE];
    uint32_t checksum;
} __attribute__((__packed__)); // evite le padding

// IMPORTANT for the testsuite
// Global variable to be accessed by the testsuite
extern struct protoframe frame;

// Fn must check the checksum which is 4 bytes after the payload
void decoder(uint8_t bufferbyte);

//TODO
char* encoder(uint8_t fn_id, uint16_t id, uint16_t size,uint8_t* payload);

#endif
