#include "unit.h"

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

Test(goodInput, decodeAck)
{
    // ACK frame
    //                  SOF   FN    ID    ID    LEN         Checksum
    uint8_t data[10] = {0x69, 0x0A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // setup checksum
    uint32_t checkss = compute_checksum(data, 5);
    for (int i = 0; i < 4; i++)
    {
        data[6 + i] = (checkss >> (8 * (3 - i))) & 0xFF;
    }

    for (int i = 0; i < 10; i++)
    {
        decoder(data[i]);
    }

    // TEST:
    print_protoframe(frame);
    cr_assert_eq(frame.function_name, ACK);
    cr_assert_eq(frame.id, 1);
    cr_assert_eq(frame.size, 0);
}

Test(badInput, decodeBadChecksumAck)
{
    // ACK frame
    //                  SOF   FN    ID    ID    LEN         Checksum
    uint8_t data[10] = {0x69, 0x0A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // setup checksum
    uint32_t checkss = compute_checksum(data, 5) + 1;
    for (int i = 0; i < 4; i++)
    {
        data[6 + i] = (checkss >> (8 * (3 - i))) & 0xFF;
    }
    for (int i = 0; i < 10; i++)
    {
        decoder(data[i]);
    }
    // TEST:
    print_protoframe(frame);
    cr_assert_eq(frame.function_name, 0x42);
    cr_assert_eq(frame.id, 1);
    cr_assert_eq(frame.size, 0);
}

Test(basicEncoding, basicTrame)
{
    uint8_t trame[10 + 0x01];
    uint8_t frame_expected[] = {0x69, 0x01, 0x00, 0x01, 0x00, 0x03};
    uint32_t expected = compute_checksum(frame_expected, sizeof(frame_expected) - 4);
    uint8_t* payload = {0x00};
    uint8_t* res = encoder(trame, 0x01, 0x00, 0x01, payload, 0x03);
    cr_assert_eq(res, frame_expected);
}
