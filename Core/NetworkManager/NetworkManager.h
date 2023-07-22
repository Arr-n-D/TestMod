#pragma once 
#include <enet/enet.h>
#include "../GameMessage/GameMessages.h"

namespace ArrND::Core::Networking {
	class NetworkManager {
		public:
			NetworkManager();
			~NetworkManager();

			void OnUpdate(ENetHost* clientHost);
			void ConnectToServer();
			bool InitEnet();
			bool InitClient();
			void Run();

			void SendMessage(void* data, GameMessage gMessage, bool isReliable);
			void SendMovementMessage(ENetPacket* p);
			void SendGameMessage(ENetPacket* p, GameMessage gMessage, bool isReliable);


		private : 
			ENetHost* clientHost;
			ENetPeer* communicationPeer;
			bool isClientInitialized = false;
	};
}