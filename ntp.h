#ifndef NTP_H
#define NTP_H

#include <stdint.h>

#define NTP_DEFAULT_SERVER      "pool.ntp.org"
#define NTP_DEFAULT_PORT        "123"

// li = 0, vn = 3, mode = 3
#define NTP_FLAGS               0x1B

#define NTP_FLAGS_LI(value)     ((value) >> 6)
#define NTP_FLAGS_VN(value)     (((value) & 0x3F) >> 3)
#define NTP_FLAGS_MODE(value)   ((value) & 0x07)

#define NTP_TIMESTAMP_OFFSET    2208988800UL

enum ntp_result {
	NTP_RESULT_OK,
	NTP_RESULT_INVALID_ARGUMENT,
	NTP_RESULT_DNS_ERROR,
	NTP_RESULT_SOCKET_ERROR
};

struct ntp_timestamp {
	uint32_t seconds;
	uint32_t fraction;
};

struct ntp_packet {
	uint8_t flags;
	uint8_t stratum;
	int8_t poll;
	int8_t precision;
	int32_t root_delay;
	int32_t root_dispersion;
	uint32_t reference_id;
	struct ntp_timestamp reference_timestamp;
	struct ntp_timestamp origin_timestamp;
	struct ntp_timestamp receive_timestamp;
	struct ntp_timestamp transmit_timestamp;
};

enum ntp_result ntp_query(struct ntp_packet *packet, const char *server, const char *port);

#endif
