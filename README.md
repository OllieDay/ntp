# ntp
Library for querying NTP servers.

## Usage
```c
#include "ntp.h"

struct ntp_packet packet;

// Use default server and port
ntp_query(&packet, NULL, NULL);

// Use specified server and port
ntp_query(&packet, "pool.ntp.org", "123");
```

## Example
See [main.c](main.c) for an example.
