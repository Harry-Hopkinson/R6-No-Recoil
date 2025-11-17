#include "Scenes.h"

namespace
{
    SceneType CurrentScene = SceneType::OperatorSelection;
    int SelectedOperatorIndex = -1;
} // namespace

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

    int GetSelectedOperatorIndex()
    {
        return SelectedOperatorIndex;
    }

    void SetSelectedOperatorIndex(int index)
    {
        SelectedOperatorIndex = index;
    }

} // namespace Scenes
