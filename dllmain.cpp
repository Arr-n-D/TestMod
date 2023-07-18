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
        static const double TARGET_TPS = 20.0;
        static const double MS_PER_TICK = 1000.0 / TARGET_TPS;

        static std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();

        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

        // Calculate the time elapsed since the last update in milliseconds
        std::chrono::duration<double, std::milli> elapsedTime = currentTime - lastUpdateTime;
        double elapsedMs = elapsedTime.count();

        if (elapsedMs >= MS_PER_TICK)
        {
            this->coreInstance.OnUpdate();
            lastUpdateTime = currentTime;
        }
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