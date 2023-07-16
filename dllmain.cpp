//#include <stdio.h>
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/Hooks.hpp>
#include "Core/Core.hpp"

using namespace RC;
using namespace RC::Unreal;
using namespace ArrND;

class TestMod : public RC::CppUserModBase
{
public:
    bool hasBeenSet = false;
    Core::Core coreInstance;

    TestMod() : CppUserModBase()
    {
        ModName = STR("TestMod");
        ModVersion = STR("1.0");
        ModDescription = STR("This is my awesome mod");
        ModAuthors = STR("UE4SS Team");
        // Do not change this unless you want to target a UE4SS version
        // other than the one you're currently building with somehow.
        //ModIntendedSDKVersion = STR("2.6");
        
        Output::send<LogLevel::Verbose>(STR("MyAwesomeMod says hello\n"));

        this->coreInstance = Core::Core();
    }

    ~TestMod()
    {
    }

    auto on_unreal_init() -> void override
    {
        this->coreInstance.OnUnrealInitialized();
 

    }

    /*void AActorBeginPlayHook(Unreal::AActor* Context) {
        if(Context->GetName().starts_with(STR("PlayerController"))) {
			Output::send<LogLevel::Verbose>(STR("PlayerController found\n"));
		}

		Output::send<LogLevel::Verbose>(STR("BeginPlayHook called\n"));
		Output::send<LogLevel::Verbose>(STR("Actor Name: {}\n"), Context->GetFullName());
	}*/

    auto on_update() -> void override
    {
        //generate a const std string
        //std::string_view player_controller_name = "PlayerController";

        if (!hasBeenSet) {
            Hook::RegisterBeginPlayPostCallback(AActorBeginPlayHook);
        }
    //        auto foundPlayer = UObjectGlobals::FindFirstOf(player_controller_name);
    //        //UObjectGlobals::FindAllOf("fsfs", objects);

    //        if (foundPlayer) {
    //            Output::send<LogLevel::Verbose>(STR("Player Controller Name: {}\n"), foundPlayer->GetFullName());

    //            if (foundPlayer->IsA(AActor::StaticClass())) {
    //                //temporarily cast to AActor to call GetWorld
    //                auto pos = static_cast<AActor*>(foundPlayer)->K2_GetActorLocation();
				//	Output::send<LogLevel::Verbose>(STR("Player Controller is an actor\n"));
    //                Output::send<LogLevel::Verbose>(STR("Player Controller Position: ({}, {}, {})\n"), pos.X(), pos.Y(), pos.Z());
				//}

    //            hasBeenSet = true;
    //        }
    //    }
   
    }

    auto GetPlayerController() -> auto
    {
		
	}
};

#define TEST_MOD_API __declspec(dllexport)
extern "C"
{
    TEST_MOD_API RC::CppUserModBase* start_mod()
    {
        return new TestMod();
    }

    TEST_MOD_API void uninstall_mod(RC::CppUserModBase* mod)
    {
        delete mod;
    }
}