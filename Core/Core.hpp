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
			AActor* GetPlayer();



		private: 
			Networking::NetworkManager networkManagerInstance = NULL;
			AActor* Player = NULL;
			bool isPlayerReady = false;

			void SetNetworkManagerInstance(Networking::NetworkManager networkManagerInstance);
			void SetPlayerReadyState(bool isReady);
			bool IsPlayerReady();
			void InitializeSentry();
			void FetchPlayerSteamId();
			void SetPlayer(AActor* Player);
			void RegisterKeyBinds();
			void GetPlayerInstance();
	};

}