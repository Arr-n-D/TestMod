#pragma once
namespace ArrND::Core {
	class Core {
	public: 
		Core();
		~Core();

		void OnUnrealInitialized();

		void OnUnrealShutdown();

		void OnUpdate();
		void OnUpdate(float DeltaTime);
	};
}