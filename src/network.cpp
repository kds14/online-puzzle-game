#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include "network.hpp"

static const int BACKLOG = 500;
static const int NET_BUFF_SIZE = MAP_WIDTH * MAP_HEIGHT + 6;

const std::string HOST_ARG = "-h";
const std::string CLIENT_ARG = "-c";

Network network;

Network::~Network() {
	if (sfd >= 0) {
		close(sfd);
	}
	if (lfd >= 0) {
		close(lfd);
	}
}

static uint8_t* serialize(std::shared_ptr<GameState> state) {
	uint8_t *buff = new uint8_t[NET_BUFF_SIZE];
	GamePiecePtr g = state->active;
	if (g) {
		buff[0] = g->x;
		buff[1] = g->y;
		buff[2] = g->rot;
		buff[3] = g->map.size() == 4 ? 1 : 0;	// need to know if I piece or not
		// store piece map array in 2 bytes
		uint16_t map = 0;
		uint16_t bit = 0x1;
		for (std::size_t i = 0; i < g->map.size(); ++i) {
			for (std::size_t j = 0; j < g->map.size(); ++j) {
				if (g->map[i][j]) {
					map |= bit;
				}
				bit <<= 1;
			}
		}
		map = htons(map);
		memcpy(&buff[4], &map, 2);
	} else {
		memset(buff, 0xFF, 6); // All 0xFF means NULL
	}
	buff[6] = state->hp;
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i) {
		buff[7+i] = state->tileMap[i];
	}
	return buff;
}

static std::shared_ptr<GameState> deserialize(uint8_t* buff) {
	std::shared_ptr<GameState> state =  std::make_shared<GameState>();
	GamePiecePtr g = NULL;
	if ((buff[2] & buff[3]) != 0xFF) {
		g = std::make_shared<GamePiece>();
		g->x = buff[0];
		g->y = buff[1];
		g->rot = buff[2];
		uint8_t iflag = buff[3];
		uint16_t map = 0;
		memcpy(&map, &buff[4], 2);
		map = ntohs(map);
		int ms = 3 + iflag;
		PieceMap pm(ms);
		for (int i = 0; i < ms; ++i) {
			std::vector<bool> v(ms);
			for (int j = 0; j < ms; ++j) {
				v[j] = map & 0x1;
				map >>= 1;
			}
			pm[i] = v;
		}
		g->map = pm;
	}
	state->active = g;
	state->hp = buff[6];
	TileMap tm(MAP_WIDTH * MAP_HEIGHT);
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i) {
		tm[i] = buff[7+i];
	}
	state->tileMap = tm;
	return state;
}

std::shared_ptr<GameState> Network::rec() {
	std::shared_ptr<GameState> ptr = NULL;

	fd_set rfds;
	struct timeval tv;
	FD_ZERO(&rfds);
	FD_SET(sfd, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (select(sfd + 1, &rfds, NULL, NULL, &tv) <= 0) {
		return ptr;
	}
	if (sfd >= 0) {
		uint8_t *buff = new uint8_t[NET_BUFF_SIZE];
		int res = read(sfd, buff, NET_BUFF_SIZE);
		if (res > 0) {
			ptr = deserialize(buff);
		}
		delete buff;
	}
	return ptr;
}

void Network::send_state(std::shared_ptr<GameState> state) {
	if (sfd >= 0) {
		uint8_t *buff = serialize(state);
		send(sfd, buff, NET_BUFF_SIZE, 0);
		delete buff;
	}
}

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
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));

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
