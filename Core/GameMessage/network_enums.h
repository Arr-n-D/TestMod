#pragma once
#include "msgpack.hpp"

namespace ArrND::Core::Networking {

	enum GameMessage {
		MOVE = 0,
	};

	enum GameMessageReliability {
		UNRELIABLE = 0,
		RELIABLE = 1,
	};

	enum Channels {
		MOVEMENT = 0,

	};

	struct PlayerMove {
		float x;
		float y;
		float z;
		MSGPACK_DEFINE(x, y, z);
	};

	struct Packet {
		int packetType;
		size_t sizeOfMessage;
		std::string data;
		MSGPACK_DEFINE(packetType, sizeOfMessage, data);
	};
}