
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
        if (Core::tempPlayer != nullptr && !Core::tempPlayer->IsUnreachable())
        {
            auto pos = Core::tempPlayer->K2_GetActorLocation();
            std::string posx = std::to_string(pos.X());

            if (!posx.starts_with("0") && !posx.starts_with("-0.0000"))
            {
                Output::send<LogLevel::Verbose>(STR("Player is set\n"));
                this->SetPlayer(Core::tempPlayer);
                delete Core::tempPlayer;
            }
        }
    }

    void Core::OnUnrealInitialized()
    {
        Unreal::Hook::RegisterBeginPlayPostCallback(Core::OnActorBeginPlay);
    }

    void Core::OnActorBeginPlay(Unreal::AActor *Context)
    {
        if (Context->GetName().starts_with(STR("BP_Biped_Player_C")))
        {
            Output::send<LogLevel::Verbose>(STR("PlayerController found\n"));
            Core::tempPlayer = Context;
        }
    }
}