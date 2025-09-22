#pragma once

namespace Scenes
{

    /**
     * @brief Paints landing page contents to the screen
     */
    void DrawLandingPage();

    /**
     * @brief Paints operator selection contents to the screen
     */
    void DrawOperatorSelection();

    /**
     * @brief Paints weapon selection contents to the screen
     */
    void DrawWeaponDisplay();

    /**
     * @brief Paints the info screen page contents to the screen
     */
    void DrawInfoScreen();

} // namespace Scenes

enum class SceneType
{
    LandingPage,
    OperatorSelection,
    WeaponDisplay,
    InfoScreen
};

SceneType CurrentScene;

int SelectedOperatorIndex = -1;
