#include "unit.h"
#include "criterion/assert.h"


Test(goodInput, decodeAck)
{
    // ACK frame
    //             SOF    FN    ID    ID    LEN   Checksum
    char data[9] = {0x69, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x70};
    for (int i = 0; i < 9; i++)
    {
        decoder(data[i]);
    }
    cr_assert_eq(frame.function_name, ACK);
    cr_assert_eq(frame.id, 1);
    cr_assert_eq(frame.size, 0);
}

Test(badInput, decodeBadChecksumAck)
{
    // ACK frame
    //             SOF    FN    ID    ID    LEN   Checksum
    char data[9] = {0x69, 0x15, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x70};
    for (int i = 0; i < 9; i++)
    {
        decoder(data[i]);
    }
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
    uint8_t* res = encoder(trame, 0x01, 0x00, 0x01, payload, 0x03)
    cr_assert_eq(res, expected);
}
