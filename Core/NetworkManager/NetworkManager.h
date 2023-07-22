#pragma once 
#include <enet/enet.h>

namespace ArrND::Core::NetworkManager {
	class NetworkManager {
		public:
			NetworkManager();
			~NetworkManager();

			void OnUpdate();
			bool ConnectToServer();


		private : 
			ENetHost* client = NULL;
	};
}