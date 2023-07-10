


void test_decode_ack_normal() {
    // ACK frame
    //             SOF    FN    ID    ID    LEN   Checksum
    char data[6] = {0x69, 0x06, 0x00, 0x01, 0x00, 0x70};
    struct protoframe frame = decoder(data);
    CU_ASSERT_EQUAL(frame.sof, 0x69);
    CU_ASSERT_EQUAL(frame.fn_id, 0x06);
    CU_ASSERT_EQUAL((frame.id, 1);
    CU_ASSERT_EQUAL(frame.size, 0x00);
}

void test_decode_ack_erroned(){
    char data[6] = {0x69, 0x06, 0x00, 0x01, 0x00, 0x71};
    struct protoframe frame = decoder(data);
    CU_ASSERT_EQUAL(frame, NULL);
}
