#pragma once

typedef struct HDC__* HDC;

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

    /**
     * @brief Paints landing page contents to the screen
     */
    void DrawLandingPage(HDC memDC, int right, int bottom);

    /**
     * @brief Paints operator selection contents to the screen
     */
    void DrawOperatorSelection(HDC memDC, int right, int bottom);

    /**
     * @brief Paints weapon selection contents to the screen
     */
    void DrawWeaponDisplay(HDC memDC, int right, int bottom);
	
	/**
     * @brief Paints attachment selection contents to the screen
     */
    void DrawAttachmentDisplay(HDC memDC, int right, int bottom);

    /**
     * @brief Paints the info screen page contents to the screen
     */
    void DrawInfoScreen(HDC memDC, int right);

    SceneType GetCurrentScene();
    void ChangeCurrentScene(SceneType newScene);

} // namespace Scenes

extern int SelectedOperatorIndex;
