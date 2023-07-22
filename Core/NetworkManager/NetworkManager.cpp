#include "NetworkManager.h"
#include <DynamicOutput/DynamicOutput.hpp>

using namespace RC;

namespace ArrND::Core::NetworkManager {
	NetworkManager::NetworkManager() {
		//do nothing
		Output::send<LogLevel::Verbose>(STR("NetworkManager initialized"));
	}

	NetworkManager::~NetworkManager() {
		//do nothing
	}

	void NetworkManager::OnUpdate()
	{

	}

	bool NetworkManager::ConnectToServer()
	{
		return false;
	}
}