#include "proto.h"
#include "process.h"

struct protoframe frame = {
    .function_name = 0,
    .id = 0,
    .size = 0,
    .payload = {0},
};

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
            // if the size is bigger than the maximum payload size
            // then we make it fail by going directly to the checksum state
            if(frame.size > MAX_PAYLOAD_SIZE){
                state = S_CHECKSUM;
                break;
            }
            state = S_PAYLOAD;
            break;
        case S_PAYLOAD:
            if(frame.size > MAX_PAYLOAD_SIZE){
                state = S_CHECKSUM;
                break;
            } else if(payload_counter == frame.size){
                state = S_CHECKSUM;
            }
            frame.payload[payload_counter] = bufferbyte;
            payload_counter++; 
            break;

        case S_CHECKSUM:
            checksum = checksum << 8 | bufferbyte;
            checksum_counter++;
            // if the checksum is incorrect, the process function will not be called
            // but the valuues will still be there for test purposes
            if(checksum_counter == 4){
                if(checksum == compute_checksum((uint8_t*)&frame, sizeof(frame) - 4)) process_decoded_data(frame);
            }else {
                // for test purposes to see if the checksum is correctly tested
                frame.function_name = 0xff;
            }
            // resetting every parameter to be able to read another frame
            state = S_WAITING_SOF;
            payload_counter = 0;
            checksum_counter = 0;
            break;
    }
}
