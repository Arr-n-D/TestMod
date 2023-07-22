
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/Hooks.hpp>
#include <Unreal/World.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UFunction.hpp>
#include "Core.hpp"
#include <Core/NetworkManager/NetworkManager.h>

using namespace RC;
using namespace RC::Unreal;
using namespace ArrND::Core::NetworkManager;

namespace ArrND::Core
{
    int foundPlayerHowManyTimes = 0;

    Core::Core()
    {
        Output::send<LogLevel::Verbose>(STR("Core initialized"));
        this->networkManagerInstance = NetworkManager::NetworkManager();
    }

    Core::~Core()
    {
    }

    void Core::OnUpdate()
    {
        //this->FetchPlayer();

    }

    void Core::SetPlayerReadyState(bool isReady) {
        this->isPlayerReady = isReady;
        Output::send<LogLevel::Verbose>(STR("Player is set\n"));
    }

    bool Core::IsPlayerReady() {
        return this->isPlayerReady;
    }

    void Core::SetPlayer(AActor* Player)
    {
		this->Player = Player;
        this->SetPlayerReadyState(true);
	}

    AActor* Core::GetPlayer()
    {
        return nullptr;
    }

    void Core::OnUnrealInitialized()
    {
        auto prehook = [](Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
            //do nothing
        };

        auto posthook = [this](Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
            auto Player = UObjectGlobals::FindFirstOf(STR("Biped_Player"));

            //if not null ptr 
            if (Player != nullptr && !Player->IsUnreachable()) {
                //increase the number of times we found the player
                foundPlayerHowManyTimes++;

                if (foundPlayerHowManyTimes == 2) {
                    Output::send<LogLevel::Verbose>(STR("Player address: {}\n"), (uintptr_t)Player);

                    this->SetPlayer((AActor*)Player);

                    //found set back to 0
                    foundPlayerHowManyTimes = 0;
                }

            }
        };

        Unreal::UObjectGlobals::RegisterHook(STR("/Script/Engine.PlayerController:ClientRestart"), prehook, posthook, nullptr);
    }
}