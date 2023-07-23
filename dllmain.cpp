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

class TestMod : public RC::CppUserModBase
{
public:
    Core::Core coreInstance = Core::Core();

    TestMod() : CppUserModBase()
    {
        ModName = STR("TestMod");
        ModVersion = STR("1.0");
        ModDescription = STR("This is my awesome mod");
        ModAuthors = STR("UE4SS Team");

        //this->coreInstance = Core::Core();
        //this->coreInstance.networkManagerInstance.Run();
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