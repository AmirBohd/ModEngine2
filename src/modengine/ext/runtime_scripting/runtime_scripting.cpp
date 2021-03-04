#pragma once

#include "runtime_scripting.h"

namespace modengine::ext {

void RuntimeScriptingExtension::on_attach()
{
    info("Creating runtime_scripting loader thread");
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)loaderThreadProc, NULL, 0, NULL);
}

void RuntimeScriptingExtension::on_detach()
{
}

void RuntimeScriptingExtension::load() {
    if (ds3runtimeLibrary) {
        info("You have already loaded DS3RuntimeScripting.dll");
        return;
    }

    info("Loading DS3RuntimeScripting.dll...");
    ds3runtimeLibrary = LoadLibraryW(L"DS3RuntimeScripting.dll");
    if (!ds3runtimeLibrary) error("Unable to load DS3RuntimeScripting.dll");
}

void RuntimeScriptingExtension::unload() {
    if (ds3runtimeLibrary) {
        info("Unloading DS3RuntimeScripting.dll..");
        if (!FreeLibrary(ds3runtimeLibrary)) error("Unable to unload DS3RuntimeScripting.dll");
        else ds3runtimeLibrary = nullptr;
    }
    else {
        warn("DS3RuntimeScripting.dll is not loaded.");
    }
}

void RuntimeScriptingExtension::loaderThreadProc()
{
    info("Thread created! Starting load check loop");

    while (mod_engine_global->isAttached()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::ifstream file("ds3runtimeload.txt");
        if (!file.good()) continue;
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string fileContent(buffer.str());
        static std::string lastFileContent = "";

        if (lastFileContent.compare(fileContent) != 0 && !ds3runtimeLibrary && fileContent.compare("load") == 0) {
            load();
        }
        else if (lastFileContent.compare(fileContent) != 0 && ds3runtimeLibrary && fileContent.compare("unload") == 0) {
            unload();
        }

        lastFileContent = fileContent;
    }

    info("Runtime_scripting thread terminated.");
}

WNDPROC RuntimeScriptingExtension::previousWindowProc;
HMODULE RuntimeScriptingExtension::ds3runtimeLibrary = nullptr;
HANDLE RuntimeScriptingExtension::loaderThreadId;

}