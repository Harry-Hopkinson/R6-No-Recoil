#pragma once

enum class SceneType
{
    OperatorSelection,
    WeaponDisplay,
};

namespace Scenes
{

    SceneType GetCurrentScene();
    void ChangeCurrentScene(SceneType newScene);

} // namespace Scenes

extern int SelectedOperatorIndex;
