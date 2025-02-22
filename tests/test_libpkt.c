#include <check.h>
#include "../libpkt.h"

START_TEST(test_pkt_header_create) {
    pkt_header_t *header = pkt_header_create();
    ck_assert_ptr_nonnull(header);
    ck_assert_uint_eq(header->magic_number, PKT_MAGIC_NUMBER);
    ck_assert_uint_eq(header->version, 1);
    ck_assert_uint_eq(header->reserved, 0);
    ck_assert_uint_eq(header->length, 0);
    pkt_header_destroy(header);
}
END_TEST

START_TEST(test_pkt_create) {
    uint32_t length = 100;
    pkt_t *packet = pkt_create(length);
    ck_assert_ptr_nonnull(packet);
    ck_assert_uint_eq(packet->type, 0);
    ck_assert_int_eq(packet->timestamp, 0);
    ck_assert_uint_eq(packet->length, length);
    pkt_destroy(packet);
}
END_TEST

START_TEST(test_pkt_open_close) {
    const char *filename = "test.pkt";
    pkt_file_t *file = pkt_open(filename);
    ck_assert_ptr_nonnull(file);
    pkt_close(file);
}
END_TEST

START_TEST(test_pkt_write_read_header) {
    const char *filename = "test.pkt";
    pkt_file_t *file = pkt_open(filename);
    pkt_header_t *header = pkt_header_create();
    int result = pkt_write_header(file, header);
    ck_assert_int_eq(result, 0);

    pkt_header_t *read_header = pkt_read_header(file);
    ck_assert_ptr_nonnull(read_header);
    ck_assert_uint_eq(read_header->magic_number, PKT_MAGIC_NUMBER);
    ck_assert_uint_eq(read_header->version, 1);
    ck_assert_uint_eq(read_header->reserved, 0);
    ck_assert_uint_eq(read_header->length, 0);

    pkt_header_destroy(header);
    pkt_header_destroy(read_header);
    pkt_close(file);
}
END_TEST

START_TEST(test_pkt_append_read_packet) {
    const char *filename = "test.pkt";
    pkt_file_t *file = pkt_open(filename);
    pkt_header_t *header = pkt_header_create();
    pkt_write_header(file, header);

    uint32_t length = 100;
    pkt_t *packet = pkt_create(length);
    pkt_append_packet(file, packet);

    pkt_t *read_packet = pkt_read_packet(file, 0);
    ck_assert_ptr_nonnull(read_packet);
    ck_assert_uint_eq(read_packet->type, 0);
    ck_assert_int_eq(read_packet->timestamp, 0);
    ck_assert_uint_eq(read_packet->length, length);

    pkt_destroy(packet);
    pkt_destroy(read_packet);
    pkt_header_destroy(header);
    pkt_close(file);
}
END_TEST

Suite *pkt_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("PKT");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_pkt_header_create);
    tcase_add_test(tc_core, test_pkt_create);
    tcase_add_test(tc_core, test_pkt_open_close);
    tcase_add_test(tc_core, test_pkt_write_read_header);
    tcase_add_test(tc_core, test_pkt_append_read_packet);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = pkt_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}
