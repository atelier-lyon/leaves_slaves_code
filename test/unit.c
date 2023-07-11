#include "unit.h"

void test_decode_ack_normal() {
    // ACK frame
    //             SOF    FN    ID    ID    LEN   Checksum
    char data[9] = {0x69, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x70};
    for (int i = 0; i < 9; i++) {
        decoder(data[i]);
        CU_ASSERT_EQUAL(frame.function_name, 0x06);
        CU_ASSERT_EQUAL(frame.id, 1);
        CU_ASSERT_EQUAL(frame.size, 0);
    }
}

void test_decode_ack_wrong_checksum() {
    // ACK frame
    //             SOF    FN    ID    ID    LEN   Checksum
    char data[9] = {0x69, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x70};
    for (int i = 0; i < 6; i++) {
        decoder(data[i]);
    }
    CU_ASSERT_EQUAL(frame.function_name, 0xff); // shows that the checksum is wrong
}




int main() {
    CU_initialize_registry();

    // Create a suite
    CU_pSuite suite_normal = CU_add_suite("Suite Correct", NULL, NULL);
    CU_pSuite suite_erroned = CU_add_suite("Suite Erroned", NULL, NULL);

    // Add the test case to the suite
    CU_add_test(suite_normal, "decode ack", test_decode_ack_normal);
    CU_add_test(suite_erroned, "decode ack", test_decode_ack_wrong_checksum);

    // Run the tests
    CU_basic_run_tests();

    // Clean up
    CU_cleanup_registry();

    return CU_get_error();
}
