#include "proto.h"
#include "process.h"
#include <stdint.h>
#include <stdio.h>

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

uint32_t compute_checksum(uint8_t* data, size_t size) {
    uint32_t sum = 0;
    
    for (size_t i = 0; i < size; i++) {
        sum += data[i];
    }
    
    return sum ;//% UINT32_MAX; // cutting the excedent bits
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
            frame.id = (frame.id << 8) | bufferbyte;
            state = S_SIZE;
            break;
        case S_SIZE:
            frame.size = bufferbyte;
            state = S_SIZE2;
            break;
        case S_SIZE2:
            frame.size = (frame.size << 8) | bufferbyte;
            // if the size is bigger than the maximum payload size
            // then we make it fail by going directly to the checksum state
            if(frame.size > MAX_PAYLOAD_SIZE || frame.size == 0){
                state = S_CHECKSUM;
                break;
            }
            state = S_PAYLOAD;
            break;
        case S_PAYLOAD:
            frame.payload[payload_counter] = bufferbyte;
            if(payload_counter == frame.size - 1){ // range 0..size-1 = size iterations
                state = S_CHECKSUM;
                break;
            }
            payload_counter++; 
            break;

        case S_CHECKSUM:
            frame.checksum = (checksum << 8) | bufferbyte;
            // if the checksum is incorrect, the process function will not be called
            // but the values will still be there for test purposes
            if(checksum_counter == 3){ // 0..3 = 4 iterations
                uint32_t computed = compute_checksum((uint8_t*)&frame, sizeof(frame) - 4) + 0x69;
                // if the checksum is correct, then the process function is called
                if(frame.checksum == computed) {
                    process_decoded_data(frame, frame.payload);
                }else {
                    // for test purposes to see if the checksum is correctly tested
                    frame.function_name = ERRORSYNTAX;
                }// resetting every parameter to be able to read another frame
                state = S_WAITING_SOF;
                payload_counter = 0;
                checksum_counter = 0;
                break;
            }            
            checksum_counter++;
            break;
    }
}

/* void print_protoframe(struct protoframe frame) { */
    /* printf("Function Name: %02X\n", frame.function_name); */
    /* printf("ID: %u\n", frame.id); */
    /* printf("Size: %u\n", frame.size); */
    /* printf("Payload: "); */
    /* for (int i = 0; i < frame.size; i++) { */
        /* printf("%02X ", frame.payload[i]); */
    /* } */
    /* printf("\n"); */
    /* printf("Checksum: %u\n", frame.checksum); */
/* } */

/* int main(){ */
    /* // RGBreq frame */
    /* //                  SOF   FN    ID    ID    LEN   LEN   PAYLOAD           Checksum */
    /* uint8_t data[13] = {0x69, 0x01, 0x00, 0x01, 0x00, 0x03, 0x0A, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00}; */
    /* // setup checksum */
    /* int length = sizeof(data) - 4; */
    /* uint32_t checkss = compute_checksum(data, length); // not length -1 cuz strict inferior */
    /* for (int i = 0; i < 4; i++) */
        /* data[length + i] = (checkss >> (8 * (3 - i))) & 0xFF; */

    /* for (int i = 0; i < 13; i++) */
        /* decoder(data[i]); */

    /* // TEST: */
    /* // print_protoframe(frame); */
/* } */
