#include "pkt.h"

#include <stdlib.h>
#include <string.h>

uint16_t swap16(const uint16_t val) {
    return (val >> 8) | (val << 8);
}

uint32_t swap32(const uint32_t val) {
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

pkt_header_t *pkt_header_create() {
    pkt_header_t *header = (pkt_header_t *)malloc(sizeof(pkt_header_t));
    if (header == NULL) {
        return NULL;
    }

    // Initialize the header fields
    header->magic_number = PKT_MAGIC_NUMBER;
    header->version = 1;
    header->endian = 1;
    header->reserved = 0;
    header->length = 0;

    return header;
}


void pkt_header_destroy(pkt_header_t *header) {
    free(header);
}

pkt_file_t *pkt_open(const char *filename, const char *mode) {
    pkt_file_t *pkt = malloc(sizeof(pkt_file_t));
    if (!pkt) {
        return NULL;
    }

    pkt->fp = fopen(filename, mode);
    if (!pkt->fp) {
        free(pkt);
        return NULL;
    }

    return pkt;
}

void pkt_close(pkt_file_t *file) {
    if (file) {
        if (file->fp) {
            fclose(file->fp);
        }
        free(file);
    }
}

int pkt_write_header(pkt_file_t *file, pkt_header_t *header) {
    if (!file || !header || !file->fp) {
        return -1;
    }
    unsigned char *buf = (unsigned char *)malloc(sizeof(pkt_header_t));
    if (!buf) {
        return -1;
    }

    pkt_header_t converted_header = *header;

    if (header->endian == 1) {
        converted_header.magic_number = swap32(header->magic_number);
        converted_header.version = swap32(header->version);
        converted_header.length = swap32(header->length);
    }

    memcpy(buf, &converted_header, sizeof(pkt_header_t));

    if (fwrite(buf, 1, sizeof(pkt_header_t), file->fp) != sizeof(pkt_header_t)) {
        free(buf);
        return -1;
    }

    free(buf);

    return 0;
}

pkt_header_t *pkt_read_header(pkt_file_t *file) {
    if (!file || !file->fp) {
        return NULL;
    }

    pkt_header_t *header = pkt_header_create();
    if (!header) {
        return NULL;
    }

    if (fread(header, 1, sizeof(pkt_header_t), file->fp) != sizeof(pkt_header_t)) {
        fprintf(stderr, "Failed to read header\n");
        free(header);
        return NULL;
    }

    // Magic number is big-endian
    if (swap32(header->magic_number) != PKT_MAGIC_NUMBER) {
        fprintf(stderr, "Invalid magic number\n");
        free(header);
        return NULL;
    }

    if (header->endian == 1) {
        header->magic_number = swap32(header->magic_number);
        header->length = swap32(header->length);
    }

    return header;
}
