#pragma once

#include <Unreal/AActor.hpp>
#include <Core/NetworkManager/NetworkManager.h>

using namespace RC;
using namespace RC::Unreal;
using namespace ArrND::Core::Networking;

namespace ArrND::Core {
	class Core {
		public:
			Core();
			~Core();

			void OnUnrealInitialized();

			void OnUpdate();

			void SetPlayer(AActor* Player);

			AActor* GetPlayer();

			bool IsPlayerReady();

			void SetPlayerReadyState(bool isReady);

			Networking::NetworkManager networkManagerInstance;

			void RegisterKeyBinds();

		private: 
			AActor* Player = NULL;
			bool isPlayerReady = false;
	};

}