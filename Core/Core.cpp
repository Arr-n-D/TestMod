
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
        this->networkManagerInstance = NetworkManager::NetworkManager();
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

//#if _DEBUG
    void Core::RegisterKeyBinds() {
        RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, VK_F10);
        RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, VK_F11);
        RegisterHotKey(NULL, 3, MOD_ALT | MOD_NOREPEAT, VK_F12);

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

                        //print size of data
                        Output::send<LogLevel::Verbose>(STR("Size of data: {}\n"), buffer.size());
                        //print size of data strlen
                        Output::send<LogLevel::Verbose>(STR("Size of data strlen: {}\n"), strlen(data));

                        this->networkManagerInstance.SendGameMessage(data, buffer.size(), GameMessage::MOVE, false);
                    }
                    break;
                case 2:
                {
                    auto Player = UObjectGlobals::FindFirstOf(STR("Biped_Player"));
                    if (Player != nullptr && !Player->IsUnreachable()) {
                        this->SetPlayer((AActor*)Player);
                    }
                }
                    

                case 3:
                    if (!this->networkManagerInstance.IsCommunicationEstablished()) {
                        this->networkManagerInstance.ConnectToServer();
                    }
                }
            }
        }

    }
//#endif

    void Core::OnUnrealInitialized()
    {
        this->networkManagerInstance.Run();

        //this->RegisterKeyBinds();
        std::thread RegisterKeyBinds(&Core::RegisterKeyBinds, this);
        RegisterKeyBinds.detach();

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

        //#if !_DEBUG 
        Unreal::UObjectGlobals::RegisterHook(STR("/Script/Engine.PlayerController:ClientRestart"), prehook, posthook, nullptr);
        //#endif
    }
}