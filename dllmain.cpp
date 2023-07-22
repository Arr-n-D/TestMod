#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UObject.hpp"
#include "Unreal/AActor.hpp"
#include "Unreal/Hooks.hpp"
#include "Core/Core.hpp"

using namespace RC;
using namespace RC::Unreal;
using namespace ArrND;
using namespace ENet;

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

        this->coreInstance = Core::Core();
    }

    ~TestMod()
    {
    }

    auto on_unreal_init() -> void override
    {
        this->coreInstance.OnUnrealInitialized();
    }

    auto on_update() -> void override
    {
        this->coreInstance.OnUpdate();
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
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/Hooks.hpp>
#include "Core/Core.hpp"

namespace ENet {
    #include <enet/enet.h>
}

using namespace RC;
using namespace RC::Unreal;
using namespace ArrND;
using namespace ENet;

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

        this->coreInstance = Core::Core();
    }

    ~TestMod()
    {
    }

    auto on_unreal_init() -> void override
    {
        this->coreInstance.OnUnrealInitialized();

        if (ENet::enet_initialize() != 0)
        {
            Output::send<LogLevel::Error>(STR("An error occurred while initializing ENet.\n"));
        }
        else {
			Output::send<LogLevel::Verbose>(STR("ENet initialized.\n"));
		}
    }

    auto on_update() -> void override
    {
        this->coreInstance.OnUpdate();
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