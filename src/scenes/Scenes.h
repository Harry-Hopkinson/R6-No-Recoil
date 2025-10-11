#pragma once

enum class SceneType
{
    MainMenu,
    OperatorSelection,
    WeaponDisplay,
};

namespace Scenes
{

    SceneType GetCurrentScene();
    void ChangeCurrentScene(SceneType newScene);

} // namespace Scenes

extern int SelectedOperatorIndex;
