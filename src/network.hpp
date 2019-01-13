#ifndef NETWORK_H
#define NETWORK_H

#include <string>

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
		~Network();
	private:
		int sfd = -1;
		int lfd = -1;

		void setupfd(int fd);
};

extern Network network;

#endif
