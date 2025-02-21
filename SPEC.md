# Specifications of `.pkt` File Type

## File Structure
| **Offset** | **Octet** | **Bit** | 0–3                          | 4–7                              | 8–11                     | 12–15                       | 16–19  | 20–23  | 24–27  | 28–31  |
|-----------|----------|---------|------------------------------|--------------------------------|-------------------------|---------------------------|--------|--------|--------|--------|
| 0         | 0        | 0       | **Magic (0x504B5400)**       |                                |                         |                           |        |        |        |        |
| 4         | 4        | 32      | **Version (1 byte)**         | **Endian (1 byte)**           | **Reserved (2 bytes)**  |                           |        |        |        |        |
| 8         | 8        | 64      | **Packet Count (4 bytes)**   |                                |                         |                           |        |        |        |        |
| 12        | 12       | 96      | **Packet Type (2 bytes)**    | **Unused (2 bytes)**          |                         |                           |        |        |        |        |
| 16        | 16       | 128     | **Timestamp (8 bytes)**      |                                |                         |                           |        |        |        |        |
| 24        | 24       | 192     | **Packet Length (4 bytes)**  |                                |                         |                           |        |        |        |        |
| 28        | 28       | 224     | **Packet Data (Variable)**   |                                |                         |                           |        |        |        |        |

The file header only takes up 12 bytes and the heaader on each packet 16 bytes. This makes the files incredibly lightweight with no bloat.


## Magic Number
- `.pkt` files use the magic number: 
  **`0x504B5400`** (`PKT` in ASCII with a null terminator).

## Packet Type Numbers
- **ARP** → `0x00000001`
