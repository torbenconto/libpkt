#ifndef PKT_LIBRARY_H
#define PKT_LIBRARY_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// big-endian
#define PKT_MAGIC_NUMBER 0x504B5400

typedef enum {
    PKT_TYPE_ARP = 0x01
} pkt_type_t;

// This must be packed to 11 bytes to ensure correct header size, without this it will be 12 bytes.
typedef struct  __attribute__((packed)) {
    uint32_t magic_number;
    uint8_t version;
    uint16_t reserved;
    uint32_t length;
} pkt_header_t;

typedef struct {
    uint16_t type;
    int64_t timestamp;
    uint32_t length;
    uint8_t data[];
} pkt_t;

typedef struct {
    FILE *fp;
} pkt_file_t;

typedef struct {
    pkt_t **packets;
    size_t length;
} pkt_array_t;

pkt_header_t *pkt_header_create();
void pkt_header_destroy(pkt_header_t *header);
pkt_t *pkt_create(uint32_t length); 
void pkt_destroy(pkt_t *packet);
pkt_file_t *pkt_open(const char *filename);
void pkt_close(pkt_file_t *file);
int pkt_write_header(pkt_file_t *file, pkt_header_t *header);
pkt_header_t *pkt_read_header(pkt_file_t *file);
int pkt_append_packet(pkt_file_t *file, pkt_t *packet);
pkt_t *pkt_read_packet(pkt_file_t *file, size_t index);
pkt_array_t pkt_read_all_packets(pkt_file_t *file);

#endif //PKT_LIBRARY_H