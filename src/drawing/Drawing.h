#pragma once

namespace Drawing
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

} // namespace Drawing
