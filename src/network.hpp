#ifndef NETWORK_H
#define NETWORK_H

#include <string>

#include "trl.hpp"
#include "piece.hpp"

enum msg_type {CON = 0x00, DATA = 0x01, DISCON = 0x03};

static const std::string HOST_ARG = "-h";
static const std::string CLIENT_ARG = "-c";

class Network {
	public:
		int host(std::string port);
		int conn(std::string hostname, std::string port);
		void update(TileMap tileMap, std::shared_ptr<GamePiece> active);
	private:
		int sfd = -1;
		int lfd = -1;
};

extern Network network;

#endif
