#pragma once 
#include <enet/enet.h>
#include <Unreal/AActor.hpp>
#include "../GameMessage/network_enums.h"

using namespace RC;
using namespace RC::Unreal;

namespace ArrND::Core::Networking {
	class NetworkManager {
		public:
			NetworkManager(AActor *Player);
			~NetworkManager();

			void OnUpdate();
			void Run();

			void SendGameMessage(const char* data , size_t sizeOfMessage, GameMessage gMessage, bool isReliable);
			bool IsCommunicationEstablished() { return isCommunicationEstablished; }

			void ConnectToServer();

		private : 
			AActor* Player = NULL;
			ENetHost* clientHost = NULL;
			ENetPeer* communicationPeer = NULL;
			bool isCommunicationEstablished = false;
			bool InitEnet();
			bool InitClient();
			void SendMovementMessage(ENetPacket* p);
			void SendGameMessage(ENetPacket* p, GameMessage gMessage, bool isReliable);


#pragma region OnEvents
			void OnEvent(ENetEvent event);
			void OnMessagedReceived(ENetEvent event);
#pragma endregion
	};
}