#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ntp.h"

enum ntp_result ntp_query(struct ntp_packet *packet, const char *server, const char *port) {
	if (packet == NULL) {
		return NTP_RESULT_INVALID_ARGUMENT;
	}

	if (server == NULL) {
		server = NTP_DEFAULT_SERVER;
	}

	if (port == NULL) {
		port = NTP_DEFAULT_PORT;
	}

	struct addrinfo hints = {0};
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo *res;

	if (getaddrinfo(server, port, &hints, &res) != 0) {
		return NTP_RESULT_DNS_ERROR;
	}

	struct addrinfo *ai = res;
	int sock;

	while (ai != NULL) {
		sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

		if (sock != -1) {
			break;
		}

		ai = ai->ai_next;
	}

	if (ai == NULL) {
		goto error1;
	}

	size_t len = sizeof *packet;

	memset(packet, 0, len);
	packet->flags = NTP_FLAGS;

	ssize_t sent = sendto(sock, packet, len, 0, ai->ai_addr, ai->ai_addrlen);

	if (sent == -1) {
		goto error2;
	}

	ssize_t received = recvfrom(sock, packet, len, 0, ai->ai_addr, &ai->ai_addrlen);

	if (received == -1) {
		goto error2;
	}

	close(sock);
	freeaddrinfo(res);

	packet->root_delay = ntohl(packet->root_delay);
	packet->root_dispersion = ntohl(packet->root_dispersion);
	packet->reference_id = ntohl(packet->reference_id);
	packet->reference_timestamp.seconds = ntohl(packet->reference_timestamp.seconds);
	packet->reference_timestamp.fraction = ntohl(packet->reference_timestamp.fraction);
	packet->origin_timestamp.seconds = ntohl(packet->origin_timestamp.seconds);
	packet->origin_timestamp.fraction = ntohl(packet->origin_timestamp.fraction);
	packet->receive_timestamp.seconds = ntohl(packet->receive_timestamp.seconds);
	packet->receive_timestamp.fraction = ntohl(packet->receive_timestamp.fraction);
	packet->transmit_timestamp.seconds = ntohl(packet->transmit_timestamp.seconds);
	packet->transmit_timestamp.fraction = ntohl(packet->transmit_timestamp.fraction);

	return NTP_RESULT_OK;

error2:
	close(sock);

error1:
	freeaddrinfo(res);

	return NTP_RESULT_SOCKET_ERROR;
}
