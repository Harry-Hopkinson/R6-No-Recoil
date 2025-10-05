#pragma once

enum class SceneType
{
    LandingPage,
    OperatorSelection,
    WeaponDisplay,
    AttachmentDisplay,
    InfoScreen
};

namespace Scenes
{

    SceneType GetCurrentScene();
    void ChangeCurrentScene(SceneType newScene);

} // namespace Scenes

extern int SelectedOperatorIndex;
