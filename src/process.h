#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdio.h>

#include "proto.h"

void process_decoded_data(struct protoframe frame, uint8_t *args);
#endif
