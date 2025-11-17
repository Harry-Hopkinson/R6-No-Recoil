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

    int GetSelectedOperatorIndex();
    void SetSelectedOperatorIndex(int index);

} // namespace Scenes
