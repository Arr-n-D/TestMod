#pragma once

#include <Unreal/AActor.hpp>

using namespace RC;
using namespace RC::Unreal;

namespace ArrND::Core {
	class Core {
		public:
			Core();
			~Core();

			void OnUnrealInitialized();

			//void OnUnrealShutdown();

			void OnUpdate();

			void FetchPlayer();			

			inline static AActor* tempPlayer = nullptr;

			static void OnActorBeginPlay(Unreal::AActor *Context);

			void SetPlayer(AActor* Player);

			AActor* GetPlayer();

			bool IsPlayerReady();

			void SetPlayerReadyState(bool isReady);

		private: 
			AActor* Player;
			bool isPlayerReady = false;
	};

}