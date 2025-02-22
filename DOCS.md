# PKT File Library Documentation

This document provides detailed information about the functions, structs, and constants available in the PKT File Library.

## Constants

### `PKT_MAGIC_NUMBER`
- **Type**: `uint32_t`
- **Value**: `0x504B5400`
- **Description**: The magic number used to identify `.pkt` files.

## Enumerations

### `pkt_type_t`
- **Description**: Enumeration of packet types.
- **Values**:
  - `PKT_TYPE_ARP` (`0x01`): ARP packet type.

## Structs

### `pkt_header_t`
- **Description**: Represents the header of a `.pkt` file.
- **Fields**:
  - `uint32_t magic_number`: The magic number identifying the file format.
  - `uint8_t version`: The version of the `.pkt` format.
  - `uint16_t reserved`: Reserved space for future use.
  - `uint32_t length`: The number of packets in the file.

### `pkt_t`
- **Description**: Represents a packet entry in a `.pkt` file.
- **Fields**:
  - `uint16_t type`: The type of the packet.
  - `int64_t timestamp`: The timestamp of the packet.
  - `uint32_t length`: The length of the packet data.
  - `uint8_t data[]`: The packet data.

### `pkt_file_t`
- **Description**: Represents a `.pkt` file.
- **Fields**:
  - `FILE *fp`: The file pointer to the `.pkt` file.

### `pkt_array_t`
- **Description**: Represents an array of packets.
- **Fields**:
  - `pkt_t **packets`: The array of packet pointers.
  - `size_t length`: The number of packets in the array.

## Functions

### `pkt_header_t *pkt_header_create()`
- **Description**: Creates a new `pkt_header_t` instance.
- **Returns**: A pointer to the newly created `pkt_header_t` instance, or `NULL` on failure.

### `void pkt_header_destroy(pkt_header_t *header)`
- **Description**: Destroys a `pkt_header_t` instance.
- **Parameters**:
  - `pkt_header_t *header`: The header to destroy.

### `pkt_t *pkt_create(uint32_t length)`
- **Description**: Creates a new `pkt_t` instance with the specified data length.
- **Parameters**:
  - `uint32_t length`: The length of the packet data.
- **Returns**: A pointer to the newly created `pkt_t` instance, or `NULL` on failure.

### `void pkt_destroy(pkt_t *packet)`
- **Description**: Destroys a `pkt_t` instance.
- **Parameters**:
  - `pkt_t *packet`: The packet to destroy.

### `pkt_file_t *pkt_open(const char *filename)`
- **Description**: Opens a `.pkt` file.
- **Parameters**:
  - `const char *filename`: The name of the file to open.
- **Returns**: A pointer to the newly opened `pkt_file_t` instance, or `NULL` on failure.

### `void pkt_close(pkt_file_t *file)`
- **Description**: Closes a `.pkt` file.
- **Parameters**:
  - `pkt_file_t *file`: The file to close.

### `int pkt_write_header(pkt_file_t *file, pkt_header_t *header)`
- **Description**: Writes a header to a `.pkt` file.
- **Parameters**:
  - `pkt_file_t *file`: The file to write to.
  - `pkt_header_t *header`: The header to write.
- **Returns**: `0` on success, `-1` on failure.

### `pkt_header_t *pkt_read_header(pkt_file_t *file)`
- **Description**: Reads the header from a `.pkt` file.
- **Parameters**:
  - `pkt_file_t *file`: The file to read from.
- **Returns**: A pointer to the read `pkt_header_t` instance, or `NULL` on failure.

### `int pkt_append_packet(pkt_file_t *file, pkt_t *packet)`
- **Description**: Appends a packet to a `.pkt` file.
- **Parameters**:
  - `pkt_file_t *file`: The file to append to.
  - `pkt_t *packet`: The packet to append.
- **Returns**: `0` on success, `-1` on failure.

### `pkt_t *pkt_read_packet(pkt_file_t *file, size_t index)`
- **Description**: Reads a packet from a `.pkt` file at the specified index.
- **Parameters**:
  - `pkt_file_t *file`: The file to read from.
  - `size_t index`: The index of the packet to read.
- **Returns**: A pointer to the read `pkt_t` instance, or `NULL` on failure.

### `pkt_array_t pkt_read_all_packets(pkt_file_t *file)`
- **Description**: Reads all packets from a `.pkt` file.
- **Parameters**:
  - `pkt_file_t *file`: The file to read from.
- **Returns**: A `pkt_array_t` instance containing all the packets read from the file.
