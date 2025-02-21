# PKT File Library

This project provides a library for working with `.pkt` files, a custom binary file format designed for lightweight packet storage.

## File Structure

The `.pkt` file format consists of a file header followed by multiple packet entries. The file header contains metadata about the file, while each packet entry contains information about an individual packet.

### File Header
| **Offset** | **Field**          | **Size**   |
|------------|--------------------|------------|
| 0          | Magic Number       | 4 bytes    |
| 4          | Version            | 1 byte     |
| 5          | Endian             | 1 byte     |
| 6          | Reserved           | 2 bytes    |
| 8          | Packet Count       | 4 bytes    |

### Packet Entry
| **Offset** | **Field**          | **Size**   |
|------------|--------------------|------------|
| 0          | Packet Type        | 2 bytes    |
| 2          | Timestamp          | 8 bytes    |
| 10         | Packet Length      | 4 bytes    |
| 14         | Packet Data        | Variable   |

## Usage

### Include the Library

To use the library, include the header file in your project:

```c
#include "libpkt.h"
```

### Create and Destroy Headers

```c
pkt_header_t *header = pkt_header_create();
// ... use the header ...
pkt_header_destroy(header);
```

### Create and Destroy Packets

```c
pkt_t *packet = pkt_create(length);
// ... use the packet ...
pkt_destroy(packet);
```

### Open and Close Files

```c
pkt_file_t *file = pkt_open("example.pkt");
// ... use the file ...
pkt_close(file);
```

### Write and Read Headers

```c
int result = pkt_write_header(file, header);
pkt_header_t *read_header = pkt_read_header(file);
```

### Append and Read Packets

```c
int result = pkt_append_packet(file, packet);
pkt_t *read_packet = pkt_read_packet(file);
pkt_t *read_packet_at_index = pkt_read_packet_at(file, index);
pkt_array_t all_packets = pkt_read_all_packets(file);
```

## Packet Type Numbers

- **ARP** → `0x00000001`

## License

This project is licensed under the MIT License.
