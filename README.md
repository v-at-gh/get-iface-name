# get-iface-name

`get-iface-name` is a lightweight C program that maps an IP address to the corresponding network interface on the system. The program continuously checks for the interface name associated with the given IP address until it finds a match or is interrupted.

## Usage

### Command-Line Syntax
```
./get-iface-name [-a|--address <IP_ADDRESS>]
```

### Arguments
- `-a`, `--address` `<IP_ADDRESS>`: Specify the IP address for which the corresponding network interface name will be resolved.

### Example
```bash
./get-iface-name --address 192.168.1.10
```

This command will continuously check for the network interface associated with the IP address `192.168.1.10` and print the interface name once found.

### Interrupting the Program
Press `Ctrl+C` to safely terminate the program. The program will handle the signal and exit gracefully.

## Build Instructions
To compile the program, use the following command:
```bash
cc -o get-iface-name get-iface-name.c
```

## Dependencies
The program relies on the following system libraries:
- `<stdio.h>`: For basic input and output.
- `<stdlib.h>`: For memory allocation and deallocation.
- `<string.h>`: For string manipulation.
- `<ifaddrs.h>`: To fetch network interface details.
- `<arpa/inet.h>`: For working with IP addresses.
- `<signal.h>`: For signal handling.
- `<getopt.h>`: For parsing command-line options.
- `<unistd.h>`: For usleep function.

Ensure your development environment includes these headers.

## Notes
- The program uses a polling interval of `50ms` (defined by `SLEEP_INTERVAL`) to minimize resource usage while continuously checking for the interface.
- The `find_iface_name` function relies on IPv4 addresses. IPv6 support is not implemented.

## License
This program is licensed under the MIT License. See `LICENSE` for more details.
