#pragma once

#include <windows.h>

enum class SceneType
{
    LandingPage,
    OperatorSelection,
    WeaponDisplay,
    InfoScreen
};

namespace Scenes
{

    /**
     * @brief Paints landing page contents to the screen
     */
    void DrawLandingPage(HDC memDC, const RECT& rect);

    /**
     * @brief Paints operator selection contents to the screen
     */
    void DrawOperatorSelection(HDC memDC, const RECT& rect);

    /**
     * @brief Paints weapon selection contents to the screen
     */
    void DrawWeaponDisplay(HDC memDC, const RECT& rect);

    /**
     * @brief Paints the info screen page contents to the screen
     */
    void DrawInfoScreen(HDC memDC, const RECT& rect);

    SceneType GetCurrentScene();
    void ChangeCurrentScene(SceneType newScene);

} // namespace Scenes

extern int SelectedOperatorIndex;
