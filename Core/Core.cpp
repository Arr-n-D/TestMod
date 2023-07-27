
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
#include <Input/Handler.hpp>
#include <thread>
#include "msgpack.hpp"
#include "sentry.h"

using namespace RC;
using namespace RC::Unreal;
using namespace ArrND::Core::Networking;
using namespace RC::Input;

namespace ArrND::Core
{
    int foundPlayerHowManyTimes = 0;

    Core::Core()
    {
        Output::send<LogLevel::Verbose>(STR("Core initialized"));
        //this->networkManagerInstance = NetworkManager::NetworkManager();
    }

    Core::~Core()
    {
    }

    void Core::OnUpdate()
    {
        if (this->networkManagerInstance.IsCommunicationEstablished()) {
            this->networkManagerInstance.OnUpdate();
        }
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


#pragma region DebugKeyBinds
//#if _DEBUG
    void Core::RegisterKeyBinds() {
        RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, VK_F10); // Send message to server
        RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, VK_F11); // Set Player
        RegisterHotKey(NULL, 3, MOD_ALT | MOD_NOREPEAT, VK_F12); // Reconnect to server
        RegisterHotKey(NULL, 4, MOD_ALT | MOD_NOREPEAT, VK_F5); // Teleport to Azkaban

        MSG msg = { 0 };
        while (GetMessage(&msg, NULL, 0, 0) != 0)
        {
            if (msg.message == WM_HOTKEY)
            {
                // wParam maps to the id of the hotkey
                switch (msg.wParam) {
                    case 1:
                        //print current position of player
                        if (this->Player != nullptr && !this->Player->IsUnreachable()) {
                            auto pos = this->Player->K2_GetActorLocation();
                            Output::send<LogLevel::Verbose>(STR("Player position: {} {} {}\n"), pos.X(), pos.Y(), pos.Z());
                            //generate a base C++ struct with the position
                            PlayerMove position = { pos.X(), pos.Y(), pos.Z() };
                      
                            msgpack::sbuffer buffer;
                            msgpack::pack(buffer, position);

                            const char* data = buffer.data();
                            this->networkManagerInstance.SendGameMessage(data, buffer.size(), GameMessage::MOVE, false);
                        }
                        break;
                    case 2:
                    {
                        auto Player = UObjectGlobals::FindFirstOf(STR("Biped_Player"));
                        if (Player != nullptr && !Player->IsUnreachable()) {
                            this->SetPlayer((AActor*)Player);
                        }
                        break;
                    }
                    

                    case 3:
                        if (!this->networkManagerInstance.IsCommunicationEstablished()) {
                            this->networkManagerInstance.ConnectToServer();
                        }
                        break;
                    case 4:
                    {
                        if (this->Player && !this->Player->IsUnreachable()) {
                            //generate a FVector and a FRotator with the position
                            FVector position = { 429823.69, -380638.44, -82295.96 };
                            FRotator rotation = { 0, 0, 0 };
                            this->Player->K2_TeleportTo(position, rotation);
                        }
                    }
                }
                

            }
        }

    }
#pragma endregion
//#endif

#pragma region Core Initialization

    void Core::OnUnrealInitialized()
    {
        this->InitializeSentry();
        this->GetPlayerInstance();
        this->networkManagerInstance = NetworkManager::NetworkManager(this->GetPlayer());
        this->networkManagerInstance.Run();

        //this->RegisterKeyBinds();
        std::thread RegisterKeyBinds(&Core::RegisterKeyBinds, this);
        RegisterKeyBinds.detach();

    }

    void Core::GetPlayerInstance() {
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
                    this->SetPlayer((AActor*)Player);
                    foundPlayerHowManyTimes = 0;
                }

            }
        };

        Unreal::UObjectGlobals::RegisterHook(STR("/Script/Engine.PlayerController:ClientRestart"), prehook, posthook, nullptr);
    }

    void Core::InitializeSentry() {
        sentry_options_t* options = sentry_options_new();
        sentry_options_set_dsn(options, "https://f663e887625c4fffb61dcef1a80e42f5@o4505591808196608.ingest.sentry.io/4505592369184768");
        // This is also the default-path. For further information and recommendations:
        // https://docs.sentry.io/platforms/native/configuration/options/#database-path
        sentry_options_set_database_path(options, ".sentry-native");
        sentry_options_set_release(options, "Hogwarts Legacy Together Client@0.1.0");
        //#if _DEBUG // #ifdef _DEBUG for Production/Development mode //For later use
        //        sentry_options_set_environment(options, "development");
        //#else
        //        sentry_options_set_environment(options, "production");
        //#endif

        sentry_options_set_environment(options, "development");

        sentry_options_set_handler_path(options, "crashpad_handler.exe");
        sentry_options_set_debug(options, 1);
        sentry_init(options);
    }
#pragma endregion
}
