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

void Ack(void){
    printf("ACK\n");
}

void Discovery(void){
    printf("Discovery\n");
}

void Heartbeat(void){
    printf("Heartbeat\n");
}

void Unreachable(void){
    printf("Unreachable\n");
}

void process_decoded_data(struct protoframe frame){
    switch(frame.function_name){
        case SETCOLORRGB:
            SetColorRGB();
            break;
        case SETCOLORHSV:
            SetColorHSV();
            break;
        case GETCOLORRGB:
            GetColorRGB();
            break;
        case FOCUSON:
            FocusOn();
            break;
        case FOCUSOFF:
            FocusOff();
            break;
        case ACK:
            Ack();
            break;
        case DISCOVERY:
            Discovery();
            break;
        case HEARTBEAT:
            Heartbeat();
            break;
        case UNREACHABLE:
            Unreachable();
            break;
        default:
            break;
    }
}
