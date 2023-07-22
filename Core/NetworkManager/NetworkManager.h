#pragma once 
#include <enet/enet.h>

namespace ArrND::Core::NetworkManager {
	class NetworkManager {
		public:
			NetworkManager();
			~NetworkManager();

			void OnUpdate(ENetHost* client);
			bool ConnectToServer();
			bool InitEnet();
			bool InitClient();
			void Run();


		private : 
			ENetHost* client;
	};
}