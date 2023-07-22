#pragma once

#include <Unreal/AActor.hpp>
#include <Core/NetworkManager/NetworkManager.h>

using namespace RC;
using namespace RC::Unreal;
using namespace ArrND::Core::NetworkManager;

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

			NetworkManager::NetworkManager networkManagerInstance;

		private: 
			AActor* Player = NULL;
			bool isPlayerReady = false;
	};

}