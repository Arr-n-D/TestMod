#pragma once 
#include <enet/enet.h>
#include "../GameMessage/network_enums.h"

namespace ArrND::Core::Networking {
	class NetworkManager {
		public:
			NetworkManager();
			~NetworkManager();

			void OnUpdate();
			void ConnectToServer();
			bool InitEnet();
			bool InitClient();
			void Run();

			void SendMovementMessage(ENetPacket* p);
			void SendGameMessage(const char* data, GameMessage gMessage, bool isReliable);
			void SendGameMessage(ENetPacket* p, GameMessage gMessage, bool isReliable);
			bool IsCommunicationEstablished() { return isCommunicationEstablished; }


		private : 
			ENetHost* clientHost = NULL;
			ENetPeer* communicationPeer = NULL;
			bool isCommunicationEstablished = false;
	};
}