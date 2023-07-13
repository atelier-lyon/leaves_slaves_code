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


typedef enum {
    SETCOLORRGB = 0x01,
    SETCOLORHSV = 0x02,
    GETCOLORRGB = 0x03,
    FOCUSON = 0x11,
    FOCUSOFF = 0x10,
    ACK = 0x0A,
    DISCOVERY = 0xAA,
    HEARTBEAT = 0x64,
    UNREACHABLE = 0xFF,
    ERRORSYNTAX = 0xEE
} function_name_t;

// Frame structure
struct protoframe {
    function_name_t function_name;
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

uint32_t compute_checksum(const uint8_t* data, size_t size);

//TODO
char* encoder(function_name_t fn_id, uint16_t id, uint16_t size, uint8_t* payload, uint8_t pin);

#endif
