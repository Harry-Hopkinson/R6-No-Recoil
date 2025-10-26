#pragma once

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

    __forceinline SceneType GetCurrentScene()
    {
        return CurrentScene;
    }

    __forceinline void ChangeCurrentScene(SceneType newScene)
    {
        CurrentScene = newScene;
    }

} // namespace Scenes

extern int SelectedOperatorIndex;
