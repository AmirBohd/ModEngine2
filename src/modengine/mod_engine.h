#pragma once

#include "modengine/extension.h"
#include "modengine/hook_set.h"
#include "modengine/game_info.h"
#include "modengine/patch.h"
#include "modengine/settings.h"

#include <map>

namespace modengine {

class ModEngine {
    friend class ModEngineExtension;

public:
    ModEngine(GameInfo game, Settings settings)
        : m_game(game)
        , m_hooks()
        , m_settings(settings)
    {
    }

    void attach();
    void detach();
    bool isAttached();

    void register_extension(std::unique_ptr<ModEngineExtension> extension);

    const GameInfo& game_info() const
    {
        return m_game;
    }

    const Settings& settings() const
    {
        return m_settings;
    }
private:
    Settings m_settings;
    GameInfo m_game;
    HookSet m_hooks;
    std::vector<std::unique_ptr<Patch>> m_patches;
    std::vector<std::unique_ptr<ModEngineExtension>> m_extensions;
    std::map<std::string, ExtensionInfo> m_extension_info;
    bool attached = false;
};

extern std::shared_ptr<ModEngine> mod_engine_global;

}
