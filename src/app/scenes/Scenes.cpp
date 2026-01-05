#include "Scenes.h"

namespace
{
    SceneType CurrentScene = SceneType::OperatorSelection;
}

namespace Scenes
{

    SceneType GetCurrentScene()
    {
        return CurrentScene;
    }

    void ChangeCurrentScene(SceneType newScene)
    {
        CurrentScene = newScene;
    }

} // namespace Scenes

int SelectedOperatorIndex = -1;
