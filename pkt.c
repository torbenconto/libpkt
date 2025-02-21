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

uint64_t swap_int64(uint64_t val) {
    return ((val >> 56) & 0xFF) |
           ((val >> 40) & 0xFF00) |
           ((val >> 24) & 0xFF0000) |
           ((val >> 8)  & 0xFF000000) |
           ((val << 8)  & 0xFF00000000) |
           ((val << 24) & 0xFF0000000000) |
           ((val << 40) & 0xFF000000000000) |
           ((val << 56) & 0xFF00000000000000);
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


pkt_t *pkt_create(uint32_t length) {
    pkt_t *packet = (pkt_t *)malloc(sizeof(pkt_t) + (length > 0 ? length : 0));
    if (packet == NULL) {
        return NULL;
    }

    // Initialize the packet fields
    packet->type = 0;
    packet->timestamp = 0;
    packet->length = length;    

    return packet;
}

void pkt_destroy(pkt_t *packet) {
    free(packet);
}

pkt_file_t *pkt_open(const char *filename) {
    pkt_file_t *pkt = malloc(sizeof(pkt_file_t));
    if (!pkt) {
        return NULL;
    }
    pkt->fp = fopen(filename, "w+b");
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
        converted_header.length = swap32(header->length);
    }

    memcpy(buf, &converted_header, sizeof(pkt_header_t));

    if (fwrite(buf, 1, sizeof(pkt_header_t), file->fp) != sizeof(pkt_header_t)) {
        free(buf);
        return -1;
    }

    fflush(file->fp);

    fseek(file->fp, 0, SEEK_SET);

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

int pkt_append_packet(pkt_file_t *file, pkt_t *packet) {
    if (!file || !file->fp || !packet) {
        fprintf(stderr, "Invalid arguments\n");
        return -1;
    }

    // Read the existing header from the file
    pkt_header_t *header = pkt_read_header(file);
    if (!header) {
        fprintf(stderr, "Cant read header\n");
        return -1;
    }

    // Calculate the new packet length
    uint32_t new_length = header->length + 1;

    // Update the length in the header
    header->length = new_length;

    // Allocate buffer to write the packet
    unsigned char *buf = (unsigned char *)malloc(sizeof(pkt_t) + packet->length);
    if (!buf) {
        fprintf(stderr, "Failed to allocate buffer\n");
        free(header);
        return -1;
    }

    // Convert packet to the correct endian format if needed
    pkt_t converted_packet = *packet;
    if (header->endian == 1) {
        converted_packet.type = swap16(packet->type);
        converted_packet.timestamp = swap_int64(packet->timestamp);
        converted_packet.length = swap32(packet->length);
    }


    // Copy the packet data into the buffer
    memcpy(buf, &converted_packet, sizeof(pkt_t));
    memcpy(buf + sizeof(pkt_t), packet->data, packet->length);

    // Move file pointer back to the start of the file to overwrite the header
    fseek(file->fp, 0, SEEK_SET);

    // Write the updated header to the file
    if (pkt_write_header(file, header) != 0) {
        free(buf);
        free(header);
        return -1;
    }

    // Move the file pointer back to the end where the packet will be written
    fseek(file->fp, 0, SEEK_END);

    // Then write the packet data to the file
    if (fwrite(buf, 1, sizeof(pkt_t) + packet->length, file->fp) != sizeof(pkt_t) + packet->length) {
        free(buf);
        free(header);
        return -1;
    }


    fflush(file->fp);
    fseek(file->fp, 0, SEEK_SET);

    free(buf);
    free(header);

    return 0;
}

pkt_t *pkt_read_packet(pkt_file_t *file) {
    if (!file || !file->fp) {
        return NULL;
    }

    pkt_header_t *header = pkt_read_header(file);
    if (!header) {
        return NULL;
    }

    pkt_t packet;
    if (fread(&packet, 1, sizeof(pkt_t), file->fp) != sizeof(pkt_t)) {
        fprintf(stderr, "Error reading fixed part of packet\n");
        free(header);
        return NULL;
    }

    pkt_t *packet_ptr = pkt_create(packet.length);
    if (!packet_ptr) {
        fprintf(stderr, "Error allocating memory for packet\n");
        free(header);
        return NULL;
    }

    *packet_ptr = packet;

    if (header->endian == 1) {
        packet_ptr->type = swap16(packet_ptr->type);
        packet_ptr->timestamp = swap_int64(packet_ptr->timestamp);
        packet_ptr->length = swap32(packet_ptr->length);
    }

    size_t bytes_read = fread(packet_ptr->data, 1, packet_ptr->length, file->fp);
    if (bytes_read != packet_ptr->length) {
        fprintf(stderr, "Error reading packet data: expected %u bytes, but got %zu\n", packet_ptr->length, bytes_read);
        free(header);
        free(packet_ptr);
        return NULL;
    }

    free(header);
    return packet_ptr;
}

