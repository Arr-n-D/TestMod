
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/Hooks.hpp>
#include <Unreal/World.hpp>
#include "Core.hpp"
#include <Unreal/UObjectArray.hpp>

using namespace RC;
using namespace RC::Unreal;

namespace ArrND::Core
{
    int foundPlayerHowManyTimes = 0;

    Core::Core()
    {
        Output::send<LogLevel::Verbose>(STR("Core initialized"));
    }

    Core::~Core()
    {
    }

    void Core::OnUpdate()
    {
        this->FetchPlayer();

    }

    void Core::FetchPlayer()
    {
        if (!this->IsPlayerReady()) {
            //print we're running'
            Output::send<LogLevel::Verbose>(STR("We're running\n"));
            if (Core::tempPlayer != nullptr)
            {
                if (!Core::tempPlayer->IsUnreachable()) {
                    auto pos = Core::tempPlayer->K2_GetActorLocation();
                    std::string posx = std::to_string(pos.X());

                    if (!posx.starts_with("0") && !posx.starts_with("-0.0000"))
                    {
                        //print we're in game'
                        Output::send<LogLevel::Verbose>(STR("We're past the main menus and in-game\n"));
                        this->SetPlayerObject(Core::tempPlayer);
                    }
                }

            }
        }
       
    }

    void Core::SetPlayerReadyState(bool isReady) {
        this->isPlayerReady = isReady;
        Output::send<LogLevel::Verbose>(STR("Player is set\n"));
    }

    bool Core::IsPlayerReady() {
        return this->isPlayerReady && *Core::isPlayerSet;
    }

    void Core::SetPlayerObject(AActor* Player)
    {
		this->Player = Player;
        this->SetPlayerReadyState(true);

        *Core::isPlayerSet = true;
        Core::tempPlayer = nullptr;
        foundPlayerHowManyTimes = 0;
	}

    void Core::OnUnrealInitialized()
    {
        Unreal::Hook::RegisterBeginPlayPostCallback(Core::OnActorBeginPlay);
    }

    void Core::OnActorBeginPlay(Unreal::AActor *Context)
    {
        if (Context->GetName().starts_with(STR("BP_Biped_Player_C")))
        {
            
            foundPlayerHowManyTimes++; 

            if (foundPlayerHowManyTimes == 2) {
                Output::send<LogLevel::Verbose>(STR("PlayerController found\n"));
                Core::tempPlayer = Context;
            }

            if (*Core::isPlayerSet != false) {
                *Core::isPlayerSet = false;
                Core::tempPlayer = Context;
            }
        }
    }
}