#include "proto.h"
#include "process.h"
#include <stdint.h>

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
            if(frame.size > MAX_PAYLOAD_SIZE){
                state = S_CHECKSUM;
                break;
            }else if (frame.size == 0) {
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
            frame.checksum = (checksum << 8) | bufferbyte;
            checksum_counter++;
            // if the checksum is incorrect, the process function will not be called
            // but the values will still be there for test purposes
            if(checksum_counter == 4){
                uint32_t computed = compute_checksum((uint8_t*)&frame, sizeof(frame) - 4) + 0x69; // adding the SOF
                if(frame.checksum == computed) process_decoded_data(frame, frame.payload);
                else {
                    // for test purposes to see if the checksum is correctly tested
                    frame.function_name = 0x42;
                }// resetting every parameter to be able to read another frame
                state = S_WAITING_SOF;
                payload_counter = 0;
                checksum_counter = 0;
                break;
            }            
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
    /* uint8_t data[10] = {0x69, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; */
    /* // just setting the checksum correctly */
    /* uint32_t checkss = compute_checksum(data, 5); */
    /* for (int i = 0; i < 4; i++) */
    /* { */
        /* data[6 + i] = (checkss >> (8 * (3 - i))) & 0xFF; */
    /* } */
    /* printf("Checksum: %d\n", *(data + 5)); */
    /* for (int i = 0; i < 10; i++) */
    /* { */
        /* decoder(data[i]); */
    /* } */
    /* print_protoframe(frame); */
/* } */
