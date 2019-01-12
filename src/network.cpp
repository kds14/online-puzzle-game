#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include "network.hpp"

static const int BACKLOG = 500;

Network network;

int Network::conn(std::string hostname, std::string port) {
	struct addrinfo hints, *res, *rptr;
	while (sfd < 0) {
		memset(&hints, 0, sizeof(struct addrinfo));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = 0;
		hints.ai_flags = AI_NUMERICSERV;

		int r = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &res);
		if (r != 0) {
			fprintf(stderr, "getaddrinfo info failed: %s\n", gai_strerror(r));
			return -1;
		}

		int fd;
		for (rptr = res; rptr != NULL; rptr = rptr->ai_next) {
			fd = socket(rptr->ai_family, rptr->ai_socktype, rptr->ai_protocol);

			if (fd == -1)
				continue;

			if (connect(fd, rptr->ai_addr, rptr->ai_addrlen) == 0) {
				sfd = fd;
				break;
			}

			close(fd);
		}

		freeaddrinfo(res);
	}
	return 0;
}

int Network::host(std::string port) {
	lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1) {
		fprintf(stderr, "Failed to create server socket %s\n", strerror(errno));
		return -1;
	}

	int so_reuseaddr = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof so_reuseaddr);

	struct sockaddr_in saddr;
	struct sockaddr_in caddr;
	memset(&saddr, 0, sizeof(struct sockaddr_in));
	memset(&caddr, 0, sizeof(struct sockaddr_in));

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(stoi(port));

	if (bind(lfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) == -1) {
		fprintf(stderr, "Failed to bind: %s\n", strerror(errno));
		return -1;
	}

	if (listen(lfd, BACKLOG) == -1) {
		fprintf(stderr, "Failed to listen: %s\n", strerror(errno));
		return -1;
	}

	socklen_t caddr_len = sizeof(struct sockaddr_in);
	sfd = accept(lfd, (struct sockaddr *)&caddr, &caddr_len);
	if (sfd == -1) {
		fprintf(stderr, "Failed to accept connection: %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

void Network::update(TileMap tileMap, std::shared_ptr<GamePiece> active) {
}
