#include "process.h"

void SetColorRGB(uint8_t r, uint8_t g, uint8_t b){
    printf("SetColorRGB: R=%d G=%d B=%d\n", r, g, b);
}

void SetColorHSV(uint8_t h, uint8_t s, uint8_t v){
    printf("SetColorHSV: H=%d S=%d V=%d\n", h, s, v);
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

void syntaxError(void){
    printf("SyntaxError\n");
}

void process_decoded_data(struct protoframe frame, uint8_t* args){
    switch(frame.function_name){
        case SETCOLORRGB:
            SetColorRGB(args[0], args[1], args[2]);
            break;
        case SETCOLORHSV:
            SetColorHSV(args[0], args[1], args[2]);
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
        case ERRORSYNTAX:
            syntaxError();
            break;
        default:
            break;
    }
}
