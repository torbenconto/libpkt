# PKT File Library

The **PKT File Library** is a high-performance C library designed for efficient handling and manipulation of `.pkt` files, a custom binary file format intended for lightweight packet storage. This library provides a robust API for creating, reading, and modifying `.pkt` files, making it ideal for applications such as packet capture, storage, and analysis.

## Features

- **Custom File Format**: Supports the `.pkt` file format for storing packets with metadata.
- **Efficient Data Handling**: Optimized for fast packet storage and retrieval.
- **Flexible API**: Provides a clean and easy-to-use interface for file and packet management.
- **Cross-platform**: Compatible with a wide range of systems and compilers.

## Project Overview

This project allows for the creation, manipulation, and reading of `.pkt` files, each containing a sequence of packets along with a header that stores essential file metadata. The format is designed to be lightweight and easy to use, making it suitable for network tools, data analysis applications, and custom networking protocols.

## File Structure

The `.pkt` file format is structured into a **file header** and **multiple packet entries**, each with its own metadata. Below is a breakdown of the structure:

### File Header

The header contains critical information about the file:

| **Offset** | **Field**          | **Size**   |
|------------|--------------------|------------|
| 0          | Magic Number       | 4 bytes    |
| 4          | Version            | 1 byte     |
| 5          | Reserved           | 2 bytes    |
| 8          | Packet Count       | 4 bytes    |

- **Magic Number**: A unique identifier for the file format.
- **Version**: Version of the `.pkt` format.
- **Reserved**: Reserved space for future use.
- **Packet Count**: The number of packets in the file.

### Packet Entry

Each packet entry contains information about an individual packet:

| **Offset** | **Field**          | **Size**   |
|------------|--------------------|------------|
| 0          | Packet Type        | 2 bytes    |
| 2          | Timestamp          | 8 bytes    |
| 10         | Packet Length      | 4 bytes    |
| 14         | Packet Data        | Variable   |

- **Packet Type**: Type of the packet (e.g., ARP, IP, etc.).
- **Timestamp**: Time at which the packet was captured or created.
- **Packet Length**: Length of the packet data.
- **Packet Data**: The actual data of the packet, length determined by the `Packet Length`.

## Installation

### Prerequisites

- A C compiler (e.g., GCC or Clang).
- CMake (for building the project).
- A POSIX-compliant operating system (Linux, macOS, or Windows with Cygwin/MSYS2).

### Building the Project

To build the project, clone the repository and use CMake to generate the build files.

1. Clone the repository:

    ```bash
    git clone https://github.com/torbenconto/libpkt.git
    cd libpkt
    ```

2. Create the build directory and generate the Makefiles using CMake:

    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3. Build the project:

    ```bash
    make
    ```

4. Install the library:

    ```bash
    sudo make install
    ```

### Cross-compiling the Project

To cross-compile the project for different platforms, use the provided toolchain files.

#### Cross-compiling for ARM

```bash
mkdir build-arm
cd build-arm
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/arm.cmake ..
make
```

#### Cross-compiling for macOS

```bash
mkdir build-mac
cd build-mac
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/mac.cmake ..
make
```

#### Cross-compiling for Linux

```bash
mkdir build-linux
cd build-linux
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/linux.cmake ..
make
```

### Using the Library with CMake

To use the PKT File Library in your own CMake project, you can use the `find_package` command:

```cmake
find_package(pkt REQUIRED)
target_link_libraries(your_target PRIVATE pkt)
```

## Usage

To use the PKT File Library, include the header file in your project:

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
pkt_t *read_packet_at_index = pkt_read_packet(file, index);
pkt_array_t all_packets = pkt_read_all_packets(file);
```

## Packet Type Numbers

- **ARP** → `0x00000001`

## Contributing

We welcome contributions to the PKT File Library! Whether it’s bug fixes, feature requests, or improvements, feel free to fork the repository and submit a pull request.

### How to Contribute:

1. Fork the repository.
2. Create a new branch for your changes.
3. Make your modifications and commit them.
4. Push your changes to your fork.
5. Submit a pull request describing your changes.

Please ensure that you write unit tests for new features or bug fixes, and ensure all existing tests pass.

## License

This project is licensed under the **MIT License**.