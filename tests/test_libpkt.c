#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/libpkt.h"

#define RUN_TEST(test) \
    printf("Running %s...\n", #test); \
    test(); \
    printf("%s passed.\n", #test);

void test_pkt_header_create() {
    pkt_header_t *header = pkt_header_create();
    assert(header != NULL);
    assert(header->magic_number == PKT_MAGIC_NUMBER);
    assert(header->version == 1);
    assert(header->reserved == 0);
    assert(header->length == 0);
    pkt_header_destroy(header);
}

void test_pkt_create() {
    uint32_t length = 100;
    pkt_t *packet = pkt_create(length);
    assert(packet != NULL);
    assert(packet->type == 0);
    assert(packet->timestamp == 0);
    assert(packet->length == length);
    pkt_destroy(packet);
}

void test_pkt_open_close() {
    const char *filename = "test.pkt";
    pkt_file_t *file = pkt_open(filename);
    assert(file != NULL);
    pkt_close(file);
}

void test_pkt_write_read_header() {
    const char *filename = "test.pkt";
    pkt_file_t *file = pkt_open(filename);
    assert(file != NULL);

    pkt_header_t *header = pkt_header_create();
    assert(header != NULL);
    assert(pkt_write_header(file, header) == 0);

    pkt_header_t *read_header = pkt_read_header(file);
    assert(read_header != NULL);
    assert(read_header->magic_number == PKT_MAGIC_NUMBER);
    assert(read_header->version == 1);
    assert(read_header->reserved == 0);
    assert(read_header->length == 0);

    pkt_header_destroy(header);
    pkt_header_destroy(read_header);
    pkt_close(file);
}

void test_pkt_append_read_packet() {
    const char *filename = "test.pkt";
    pkt_file_t *file = pkt_open(filename);
    assert(file != NULL);

    pkt_header_t *header = pkt_header_create();
    assert(header != NULL);
    pkt_write_header(file, header);

    uint32_t length = 100;
    pkt_t *packet = pkt_create(length);
    assert(packet != NULL);
    pkt_append_packet(file, packet);

    pkt_t *read_packet = pkt_read_packet(file, 0);
    assert(read_packet != NULL);
    assert(read_packet->type == 0);
    assert(read_packet->timestamp == 0);
    assert(read_packet->length == length);

    pkt_destroy(packet);
    pkt_destroy(read_packet);
    pkt_header_destroy(header);
    pkt_close(file);
}

int main() {
    RUN_TEST(test_pkt_header_create);
    RUN_TEST(test_pkt_create);
    RUN_TEST(test_pkt_open_close);
    RUN_TEST(test_pkt_write_read_header);
    RUN_TEST(test_pkt_append_read_packet);

    printf("All tests passed!\n");
    return 0;
}
