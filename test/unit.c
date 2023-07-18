#include "unit.h"
#include "criterion/criterion.h"
#include <criterion/redirect.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// UTILS
////////////////////////////////////////////////////////////////////////////////////////////////////

void print_protoframe(struct protoframe frame) {
    printf("Function Name: %02X\n", frame.function_name);
    printf("ID: %u\n", frame.id);
    printf("Size: %u\n", frame.size);
    printf("Payload: ");
    for (int i = 0; i < frame.size; i++) {
        printf("%02X ", frame.payload[i]);
    }
    printf("\n");
    printf("Checksum: %u\n", frame.checksum);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// DECODER
////////////////////////////////////////////////////////////////////////////////////////////////////

Test(goodInput, decodeAck)
{
    // ACK frame
    //                  SOF   FN    ID    ID    LEN   LEN   Checksum
    uint8_t data[10] = {0x69, 0x0A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // setup checksum
    int length = sizeof(data) - 4;
    uint32_t checkss = compute_checksum(data, length);
    for (int i = 0; i < 4; i++)
        data[6 + i] = (checkss >> (8 * (3 - i))) & 0xFF;

    for (int i = 0; i < 10; i++)
        decoder(data[i]);

    // TEST:
    // print_protoframe(frame);
    cr_assert_eq(frame.function_name, ACK);
    cr_assert_eq(frame.id, 1);
    cr_assert_eq(frame.size, 0);
}

Test(goodInput, decodeRGBreq){
    // RGBreq frame
    //                  SOF   FN    ID    ID    LEN   LEN   PAYLOAD           Checksum
    uint8_t data[13] = {0x69, 0x01, 0x00, 0x01, 0x00, 0x03, 0x0A, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00};
    // setup checksum
    int length = sizeof(data) - 4;
    uint32_t checkss = compute_checksum(data, length); // not length -1 cuz strict inferior
    for (int i = 0; i < 4; i++)
        data[length + i] = (checkss >> (8 * (3 - i))) & 0xFF;

    for (int i = 0; i < 13; i++)
        decoder(data[i]);

    // TEST:
    // print_protoframe(frame);
    cr_assert_eq(frame.function_name, SETCOLORRGB);
    cr_assert_eq(frame.id, 1);
    cr_assert_eq(frame.size, 3);
    cr_assert_eq(frame.payload[0], 10);
    cr_assert_eq(frame.payload[1], 11);
    cr_assert_eq(frame.payload[2], 12);
    // check stdout for the printf of the setcolorrgb function
    // cr_assert_stdout_eq_str("SetColorRGB: R=10 G=11 B=12\n");
}

Test(badInput, decodeBadChecksumAck)
{
    // ACK frame
    //                  SOF   FN    ID    ID    LEN         Checksum
    uint8_t data[10] = {0x69, 0x0A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // setup checksum
    int length = sizeof(data) - 4;
    uint32_t checkss = compute_checksum(data, length) + 1;
    for (int i = 0; i < 4; i++)
        data[6 + i] = (checkss >> (8 * (3 - i))) & 0xFF;
    for (int i = 0; i < 10; i++)
        decoder(data[i]);
    // TEST:
    // print_protoframe(frame);
    cr_assert_eq(frame.function_name, ERRORSYNTAX);
    cr_assert_eq(frame.id, 1);
    cr_assert_eq(frame.size, 0);
}

Test(badInput, decodeRgbWrongPayload){
    // RGBreq frame
    //                  SOF   FN    ID    ID    LEN   LEN   PAYLOAD           Checksum
    uint8_t data[13] = {0x69, 0x01, 0x00, 0x01, 0x00, 0x03, 0x0A, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00};
    // setup checksum
    int length = sizeof(data) - 4;
    uint32_t checkss = compute_checksum(data, length); // not length -1 cuz strict inferior
    for (int i = 0; i < 4; i++)
        data[length + i] = (checkss >> (8 * (3 - i))) & 0xFF;

    // adding the error
    data[7] = 0x08;
    for (int i = 0; i < 13; i++)
        decoder(data[i]);

    // TEST:
    // print_protoframe(frame);
    cr_assert_eq(frame.function_name, ERRORSYNTAX);
    cr_assert_eq(frame.id, 1);
    cr_assert_eq(frame.size, 3);
    // check stdout for the printf of the setcolorrgb function
    //cr_assert_stdout_eq_str("SetColorRGB: R=10 G=11 B=12\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ENCODER
////////////////////////////////////////////////////////////////////////////////////////////////////

/* Test(basicEncoding, basicTrame) */
/* { */
    /* uint8_t trame[10 + 0x01]; */
    /* uint8_t frame_expected[] = {0x69, 0x01, 0x00, 0x01, 0x00, 0x03}; */
    /* uint32_t expected_checksum = compute_checksum(frame_expected, sizeof(frame_expected) - 4); */
    /* uint8_t* payload = {0x00}; //BUG: not needed for some function */
    /* uint8_t* res = encoder(trame, 0x01, 0x00, 0x01, payload, 0x03); */
    /* cr_assert_eq(res, frame_expected); */
/* } */
