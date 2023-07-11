#include "proto.h"

#define SOF 0x69

typedef enum {
    S_WAITING_SOF,
    S_FUNCTION_NAME,
    S_ID,
    S_ID2,
    S_SIZE,
    S_SIZE2,
    S_PAYLOAD,
    S_CHECKSUM,
} State;

State state = S_WAITING_SOF;
int payload_counter = 0;
int checksum_counter = 0;
uint32_t checksum = 0;

struct protoframe frame = {
    .function_name = 0,
    .id = 0,
    .size = 0,
    .payload = {0},
};

uint32_t compute_checksum(const uint8_t* data, size_t size) {
    uint32_t sum = 0;
    
    for (size_t i = 0; i < size; i++) {
        sum += data[i];
    }
    
    return sum;
}

void decoder(uint8_t bufferbyte){
    switch(state){
        case S_WAITING_SOF:
            if(bufferbyte == SOF){
                state = S_FUNCTION_NAME;
            }
            break;
        case S_FUNCTION_NAME:
            frame.function_name = bufferbyte;
            state = S_ID;
            break;
        case S_ID:
            frame.id = bufferbyte;
            state = S_ID2;
            break;
        case S_ID2:
            frame.id = frame.id | (bufferbyte << 8);
            state = S_SIZE;
            break;
        case S_SIZE:
            frame.size = bufferbyte;
            state = S_SIZE2;
            break;
        case S_SIZE2:
            frame.size = frame.size | (bufferbyte << 8);
            state = S_PAYLOAD;
            break;
        case S_PAYLOAD:
            if(frame.size > MAX_PAYLOAD_SIZE){
                state = S_CHECKSUM;
                break;
            }
            frame.payload[payload_counter] = bufferbyte;
            payload_counter++;
            if(payload_counter == frame.size){
                state = S_CHECKSUM;
            }
            break;
        case S_CHECKSUM:
            checksum = checksum << 8 | bufferbyte;
            checksum_counter++;
            if(checksum_counter == 4){
                if(checksum != compute_checksum((uint8_t*)&frame, sizeof(frame) - 4)){
                    state = S_WAITING_SOF;
                }else {
                    // TODO: call process function
                }
            }
            payload_counter = 0;
            checksum_counter = 0;
            break;
    }
