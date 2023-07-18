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
#pragma region SketchyShit
			inline static AActor* tempPlayer = nullptr;
			inline static bool playerSet = false; // Declaration
			inline static bool* isPlayerSet = &playerSet; // Assigning address
#pragma endregion

			static void OnActorBeginPlay(Unreal::AActor *Context);

			void SetPlayerObject(AActor* Player);

			AActor* GetPlayer();

			bool IsPlayerReady();

			void SetPlayerReadyState(bool isReady);

		private: 
			AActor* Player;
			bool isPlayerReady = false;
	};

}