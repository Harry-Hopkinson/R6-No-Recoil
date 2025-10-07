#pragma once

enum class SceneType
{
    MainMenu,
    OperatorSelection,
    WeaponDisplay,
    AttachmentDisplay
};

namespace Scenes
{

    SceneType GetCurrentScene();
    void ChangeCurrentScene(SceneType newScene);

} // namespace Scenes

extern int SelectedOperatorIndex;
