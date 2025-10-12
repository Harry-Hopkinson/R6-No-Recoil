#pragma once

namespace Drawing
{

    /**
     * @brief Paints main menu contents to the screen
     */
    void DrawMainMenu(HDC memDC, int right, int bottom);

    /**
     * @brief Paints operator selection contents to the screen
     */
    void DrawOperatorSelection(HDC memDC, int right, int bottom);

    /**
     * @brief Paints weapon selection contents to the screen
     */
    void DrawWeaponDisplay(HDC memDC, int right, int bottom);

} // namespace Drawing
