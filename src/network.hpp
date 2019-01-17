#ifndef NETWORK_H
#define NETWORK_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef __linux__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#elif _WIN32

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define ERRMSG_LEN 500

static char ERRMSG_BUFF[ERRMSG_LEN];
static int wsaCount = 0;

#endif

#include <errno.h>


#include "trl.hpp"
#include "piece.hpp"

extern const std::string HOST_ARG;
extern const std::string CLIENT_ARG;

class Network {
	public:
		int host(std::string port);
		int conn(std::string hostname, std::string port);
		void send_state(std::shared_ptr<GameState> state);
		std::shared_ptr<GameState> rec();
	private:
		int misses = 0;
		int sfd = -1;
		int lfd = -1;

		void setupfd(int fd);
};

extern Network network;

#endif
