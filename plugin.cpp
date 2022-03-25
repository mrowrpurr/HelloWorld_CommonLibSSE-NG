#pragma warning(push)

#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>
#include <RE/C/ConsoleLog.h>
#include <RE/T/TESDataHandler.h>
#include <RE/P/PlayerCharacter.h>

using namespace RE;
using namespace RE::BSScript;
using namespace RE::BSScript::Internal;

// Logic From FDGE to load all pex files.
void ForceLoadAllScripts() {
    auto* vm = VirtualMachine::GetSingleton();
    std::filesystem::directory_iterator scriptsDir("Data\\Scripts");
    for (auto& file : scriptsDir) {
        if (file.path().string().ends_with(".pex")) {
            vm->linker.Process(BSFixedString(file.path().stem().string().c_str()));
        }
    }
}

void BindAllTheScripts() {
    ConsoleLog::GetSingleton()->Print("Binding the BindMe script...");

    auto* character = PlayerCharacter::GetSingleton();
    auto* vm = VirtualMachine::GetSingleton();
    auto* handlePolicy = vm->GetObjectHandlePolicy();
    auto* bindPolicy = vm->GetObjectBindPolicy();

    // Force load the BindMe script
    vm->linker.Process(BSFixedString("BindMe"));

    // Get a handle for the character
    auto handle = handlePolicy->GetHandleForObject(character->GetFormType(), character);

    // Create an object for this script
    BSTSmartPointer<Object> objectPtr;
    vm->CreateObject("BindMe", objectPtr);

    // Bind it!
    bindPolicy->BindObject(objectPtr, handle);
}

void BindScriptsPapyrusFunction(StaticFunctionTag*) {
    BindAllTheScripts();
}

bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("BindAll", "BindScripts", BindScriptsPapyrusFunction);
    return true;
}

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SKSE::GetPapyrusInterface()->Register(RegisterPapyrusFunctions);
    return true;
}

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* info) {
    info->infoVersion = SKSE::PluginInfo::kVersion;
    info->name = "HelloSkse";
    info->version = 1;
    return true;
}

extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = [](){
    SKSE::PluginVersionData version;
    version.PluginName("HelloSkse");
    version.PluginVersion({ 0, 0, 1 });
    version.CompatibleVersions({ SKSE::RUNTIME_LATEST });
    return version;
}();








//            vm->objectTypeToTypeID()

//            policy->GetHandleForObject(character);

// Find Gold! Then bind a Papyrus script to it!
//            auto* dataHandler = RE::TESDataHandler::GetSingleton();
//            auto* gold = dataHandler->LookupForm(0xf, "Skyrim.esm");
//            gold->
