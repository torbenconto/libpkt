#include "libpkt.h"

#include <stdlib.h>
#include <string.h>

pkt_header_t *pkt_header_create() {
    pkt_header_t *header = (pkt_header_t *)malloc(sizeof(pkt_header_t));
    if (!header) return NULL;

    header->magic_number = PKT_MAGIC_NUMBER;
    header->version = 1;
    header->endian = 0;
    header->reserved = 0;
    header->length = 0;

    return header;
}

void pkt_header_destroy(pkt_header_t *header) {
    free(header);
}

pkt_t *pkt_create(uint32_t length) {
    pkt_t *packet = (pkt_t *)malloc(sizeof(pkt_t) + (length > 0 ? length : 0));
    if (!packet) return NULL;

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
    if (!pkt) return NULL;

    FILE *file = fopen(filename, "r+b");
    if (!file) file = fopen(filename, "w+b");

    pkt->fp = file;
    if (!pkt->fp) {
        free(pkt);
        return NULL;
    }

    return pkt;
}

void pkt_close(pkt_file_t *file) {
    if (file) {
        if (file->fp) fclose(file->fp);
        free(file);
    }
}

int pkt_write_header(pkt_file_t *file, pkt_header_t *header) {
    if (!file || !header || !file->fp) return -1;

    fseek(file->fp, 0, SEEK_SET);

    if (fwrite(header, sizeof(pkt_header_t), 1, file->fp) != 1) {
        return -1;
    }

    fflush(file->fp);
    return 0;
}

pkt_header_t *pkt_read_header(pkt_file_t *file) {
    if (!file || !file->fp) return NULL;

    fseek(file->fp, 0, SEEK_SET);

    pkt_header_t *header = pkt_header_create();
    if (!header) return NULL;

    if (fread(header, sizeof(pkt_header_t), 1, file->fp) != 1) {
        free(header);
        return NULL;
    }

    if (header->magic_number != PKT_MAGIC_NUMBER) {
        free(header);
        return NULL;
    }

    return header;
}

int pkt_append_packet(pkt_file_t *file, pkt_t *packet) {
    if (!file || !file->fp || !packet) return -1;


    pkt_header_t *header = pkt_read_header(file);
    if (!header) {
        fprintf(stderr, "Can't read header\n");
        return -1;
    }

    header->length++;

    fseek(file->fp, 0, SEEK_SET);

    if (pkt_write_header(file,header) != 0) {
        fprintf(stderr, "Can't write header\n");
        free(header);
        return -1;
    }   

    fseek(file->fp, 0, SEEK_END);

    if (fwrite(packet, sizeof(pkt_t), 1, file->fp) != 1) {
        return -1;
    }

    if (packet->length > 0) {
        if (fwrite(packet->data, 1, packet->length, file->fp) != packet->length) {
            return -1;
        }
    }

    fflush(file->fp);
    free(header);
    return 0;
}

pkt_t *pkt_read_packet(pkt_file_t *file) {
    if (!file || !file->fp) return NULL;

    // skip header
    fseek(file->fp, sizeof(pkt_header_t), SEEK_SET);

    pkt_t packet;
    if (fread(&packet, sizeof(pkt_t), 1, file->fp) != 1) return NULL;

    pkt_t *packet_ptr = pkt_create(packet.length);
    if (!packet_ptr) return NULL;

    *packet_ptr = packet;

    if (packet_ptr->length > 0) {
        if (fread(packet_ptr->data, 1, packet_ptr->length, file->fp) != packet_ptr->length) {
            free(packet_ptr);
            return NULL;
        }
    }

    return packet_ptr;
}

pkt_t *pkt_read_packet_at(pkt_file_t *file, size_t index) {
    if (!file || !file->fp) return NULL;

    pkt_header_t *header = pkt_read_header(file);
    if (!header) return NULL;
    
    if (index >= header->length) {
        return NULL;
    }

    long offset = sizeof(pkt_header_t);

    for (size_t i = 0; i < index; i++) {
        pkt_t packet;
        if (fread(&packet, sizeof(pkt_t), 1, file->fp) != 1) {
            return NULL;
        }
        offset += sizeof(pkt_t) + packet.length;
    }

    if (fseek(file->fp, offset, SEEK_SET) != 0) {
        return NULL;
    }
    
    pkt_t packet;
    if (fread(&packet, sizeof(pkt_t), 1, file->fp) != 1) return NULL;

    pkt_t *packet_ptr = pkt_create(packet.length);
    if (!packet_ptr) return NULL;

    *packet_ptr = packet;

    if (packet_ptr->length > 0) {
        if (fread(packet_ptr->data, 1, packet_ptr->length, file->fp) != packet_ptr->length) {
            free(packet_ptr);
            return NULL;
        }
    }

    return packet_ptr;
}



pkt_array_t pkt_read_all_packets(pkt_file_t *file) {
    pkt_array_t result = {NULL, 0};

    if (!file || !file->fp) return result;

    pkt_header_t *header = pkt_read_header(file);
    if (!header) return result;

    if (header->length == 0) {
        return result;
    }

    fseek(file->fp, sizeof(pkt_header_t), SEEK_SET);

    result.packets = malloc(header->length * sizeof(pkt_t *));
    if (!result.packets) return result;

    result.length = header->length;

    for (size_t i = 0; i < result.length; i++) {
        pkt_t packet;

        if (fread(&packet, sizeof(pkt_t), 1, file->fp) != 1) {
            for (size_t j = 0; j < i; j++) {
                free(result.packets[j]);
            }
            free(result.packets);
            result.packets = NULL;
            return result;
        }

        pkt_t *packet_ptr = pkt_create(packet.length);
        if (!packet_ptr) {
            for (size_t j = 0; j < i; j++) {
                free(result.packets[j]);
            }
            free(result.packets);
            result.packets = NULL;
            return result;
        }

        *packet_ptr = packet;

        if (packet_ptr->length > 0) {
            if (fread(packet_ptr->data, 1, packet_ptr->length, file->fp) != packet_ptr->length) {
                free(packet_ptr);
                for (size_t j = 0; j < i; j++) {
                    free(result.packets[j]);
                }
                free(result.packets);
                result.packets = NULL;
                return result;
            }
        }

        result.packets[i] = packet_ptr;
    }

    return result;
}