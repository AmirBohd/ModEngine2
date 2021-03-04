#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

namespace modengine::ext {

class RuntimeScriptingExtension : public ModEngineExtension {
public:
    RuntimeScriptingExtension(const std::shared_ptr<ModEngine>& instance)
        : ModEngineExtension(instance)
    {
    }
private:
    void on_attach() override;
    void on_detach() override;

    std::string id() override
    {
        return "runtime_scripting";
    }

    static void load();

    static void unload();

    static void loaderThreadProc();

    static WNDPROC previousWindowProc;
    static HMODULE ds3runtimeLibrary;
    static HANDLE loaderThreadId;
};

}
