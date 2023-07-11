#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

#include "proto.h"

void SetColorRGB(void);

void SetColorHSV(void);

void GetColorRGB(void);

void FocusOn(void);

void FocusOff(void);

void ACK(void);

void Discovery(void);

void Heartbeat(void);

void process_decoded_data(struct protoframe frame);
#endif
