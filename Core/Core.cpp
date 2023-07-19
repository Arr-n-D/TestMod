
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
        //this->FetchPlayer();

    }

    void Core::FetchPlayer()
    {
        //if (!this->IsPlayerReady()) {
        //    //print we're running'
        //    Output::send<LogLevel::Verbose>(STR("We're running\n"));
        //    if (Core::tempPlayer != nullptr)
        //    {
        //        if (!Core::tempPlayer->IsUnreachable()) {
        //            auto pos = Core::tempPlayer->K2_GetActorLocation();
        //            std::string posx = std::to_string(pos.X());

        //            if (!posx.starts_with("0") && !posx.starts_with("-0.0000"))
        //            {
        //                //print we're in game'
        //                Output::send<LogLevel::Verbose>(STR("We're past the main menus and in-game\n"));
        //                this->SetPlayerObject(Core::tempPlayer);
        //            }
        //        }

        //    }
        //}
       
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

        //*Core::isPlayerSet = true;
        //Core::tempPlayer = nullptr;
        //foundPlayerHowManyTimes = 0;
	}

    //void Core::HandleLoadingScren(UObject* (*)(const FStaticConstructObjectParameters& Params, UObject* ConstructedObject) {

    //}

    void Core::OnUnrealInitialized()
    {
        //Unreal::Hook::RegisterStaticConstructObjectPostCallback(hook);

        auto posthook = [](Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
            auto Player = UObjectGlobals::FindFirstOf(STR("Biped_Player"));

            //if not null ptr 
            if (Player != nullptr && !Player->IsUnreachable()) {
                Output::send<LogLevel::Verbose>(STR("Player address: {}\n"), (uintptr_t)Player);

            }
        };

        UObjectGlobals::RegisterHook(STR("/Script/Engine.PlayerController:ClientRestart"), nullptr, posthook, nullptr);
    }
}