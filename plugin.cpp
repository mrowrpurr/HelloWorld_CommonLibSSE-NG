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

class DispatchMethodCallback : public RE::BSScript::IStackCallbackFunctor {
public:
    DispatchMethodCallback() = default;
    void operator()(RE::BSScript::Variable a_result) override {}
    void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>&) override {}
};

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

bool LOADED = false;

void BindAllTheScripts() {
    auto* vm = VirtualMachine::GetSingleton();
    auto* character = PlayerCharacter::GetSingleton();
    auto* handlePolicy = vm->GetObjectHandlePolicy();

    // Get a handle for the character
    VMHandle handle = handlePolicy->GetHandleForObject(character->GetFormType(), character);

    if (! LOADED) {
        LOADED = true;
        ConsoleLog::GetSingleton()->Print("Binding the BindMe script...");

        // Force load the BindMe script
        vm->linker.Process(BSFixedString("BindMe"));

        // Create an object for this script
        BSTSmartPointer<Object> objectPtr;
        vm->CreateObject("BindMe", objectPtr);

        // Bind it!
        auto* bindPolicy = vm->GetObjectBindPolicy();
        bindPolicy->BindObject(objectPtr, handle);
    } else {
        // Call a function on the new initialized object!
        ConsoleLog::GetSingleton()->Print("Calling a function on BindMe...");

        // Setup the arguments
//        auto args = RE::FunctionArguments<std::string>("");
        auto* args = RE::MakeFunctionArguments();

        // Required: fallback
        auto callback = new DispatchMethodCallback();
        auto callbackPtr = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>(callback);

        vm->DispatchMethodCall(handle, "BindMe", "CallMe", args, callbackPtr);

        // Find it!
//        auto& scriptsAttachedToCharacter = vm->attachedScripts.find(handle)->second;
//        for (auto& attachedScript : scriptsAttachedToCharacter) {
//            if (attachedScript->GetTypeInfo()->GetName() == "BindMe") {
//            }
//        }
    }
};

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
