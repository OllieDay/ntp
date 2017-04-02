#include <inttypes.h>
#include <stdio.h>

#include "ntp.h"

int main(int argc, char *argv[]) {
	const char *server = NULL;

	if (argc > 1) {
		server = argv[1];
	}

	const char *port = NULL;

	if (argc > 2) {
		port = argv[2];
	}

	struct ntp_packet packet;

	enum ntp_result result = ntp_query(&packet, server, port);

	if (result != NTP_RESULT_OK) {
		perror(NULL);

		return result;
	}

	printf("flags: li = %d, vn = %d, mode = %d\n", NTP_FLAGS_LI(packet.flags), NTP_FLAGS_VN(packet.flags), NTP_FLAGS_MODE(packet.flags));
	printf("stratum: %u\n", packet.stratum);
	printf("poll: %d\n", packet.poll);
	printf("precision: %d\n", packet.precision);
	printf("root_delay: %" PRId32 "\n", packet.root_delay);
	printf("root_dispersion: %" PRId32 "\n", packet.root_dispersion);
	printf("reference_id: %" PRIu32 "\n", packet.reference_id);
	printf("reference_timestamp: %" PRIu32 ".%" PRIu32 "\n", packet.reference_timestamp.seconds, packet.reference_timestamp.fraction);
	printf("origin_timestamp: %" PRIu32 ".%" PRIu32 "\n", packet.origin_timestamp.seconds, packet.origin_timestamp.fraction);
	printf("receive_timestamp: %" PRIu32 ".%" PRIu32 "\n", packet.receive_timestamp.seconds, packet.receive_timestamp.fraction);
	printf("transmit_timestamp: %" PRIu32 ".%" PRIu32 "\n", packet.transmit_timestamp.seconds, packet.transmit_timestamp.fraction);
}
