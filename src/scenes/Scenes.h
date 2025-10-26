#pragma once

#include "../core/CompilerUtils.h"

enum class SceneType
{
    MainMenu,
    OperatorSelection,
    WeaponDisplay,
};

namespace
{
    SceneType CurrentScene = SceneType::MainMenu;
}

namespace Scenes
{

    FORCE_INLINE SceneType GetCurrentScene()
    {
        return CurrentScene;
    }

    FORCE_INLINE void ChangeCurrentScene(SceneType newScene)
    {
        CurrentScene = newScene;
    }

} // namespace Scenes

extern int SelectedOperatorIndex;
