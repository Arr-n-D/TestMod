#pragma once

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
	};
}