#include "process.h"

void SetColorRGB(void){
    printf("SetColorRGB\n");
}

void SetColorHSV(void){
    printf("SetColorHSV\n");
}

void GetColorRGB(void){
    printf("GetColorRGB\n");
}

void FocusOn(void){
    printf("FocusOn\n");
}

void FocusOff(void){
    printf("FocusOff\n");
}

void ACK(void){
    printf("ACK\n");
}

void Discovery(void){
    printf("Discovery\n");
}

void Heartbeat(void){
    printf("Heartbeat\n");
}

void process_decoded_data(struct protoframe frame){
    switch(frame.function_name){
        case 0x01:
            SetColorRGB();
            break;
        case 0x02:
            SetColorHSV();
            break;
        case 0x03:
            GetColorRGB();
            break;
        case 0x11:
            FocusOn();
            break;
        case 0x10:
            FocusOff();
            break;
        case 0xf0:
            ACK();
            break;
        case 0xff:
            Discovery();
            break;
        case 0x0f:
            Heartbeat();
            break;
        default:
            break;
    }
}
