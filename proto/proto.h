#ifndef PROTO_H
#define PROTO_H
#include <stdio.h>
#include <err.h>

struct protoframe {
    char sof;
    char fn_id;
    char id[2];
    char size[2];
    char* payload;
};

// Fn must check the checksum which is 4 bytes after the payload
struct protoframe decoder(char* buffer);

char* encoder(char fn_id, char id[2], int size,char payload);


#endif
