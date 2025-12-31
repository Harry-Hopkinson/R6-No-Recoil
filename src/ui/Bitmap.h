#pragma once

#include <vector>

typedef struct HBITMAP__* HBITMAP;
typedef struct HDC__* HDC;

namespace Bitmap
{

    /**
     * @brief Entry structure for caching weapon bitmaps
     */
    struct WeaponBitmapEntry
    {
        const char* name;
        HBITMAP bitmap;
    };

    /**
     * @brief Loads a weapon bitmap by name, with caching
     * @param weaponName The name of the weapon
     * @return Handle to the loaded bitmap, or nullptr if failed
     */
    HBITMAP GetWeaponBitmap(const char* weaponName);

    /**
     * @brief Loads all operator bitmaps for a given list of names
     * @param names Vector of operator names
     * @return Vector of loaded bitmap handles
     */
    std::vector<HBITMAP> LoadOperatorBitmaps(
        const std::vector<const char*>& names);

    /**
     * @brief Cleans up all cached weapon bitmaps
     */
    void CleanupWeaponBitmaps();

    /**
     * @brief Cleans up a vector of bitmap handles
     * @param bitmaps Vector of bitmap handles to clean up
     */
    void CleanupBitmaps(std::vector<HBITMAP>& bitmaps);

    /**
     * @brief Draws a bitmap onto a device context with optional cropping
     * @param hdc Device context to draw on
     * @param bitmap Bitmap to draw
     * @param x X position
     * @param y Y position
     * @param width Width to draw
     * @param height Height to draw
     * @param cropMargin Margin to crop from the bitmap
     * @param useTransparency Whether to use transparency when drawing
     */
    void DrawBitmap(
        HDC hdc, HBITMAP bitmap, int x, int y, int width, int height,
        int cropMargin = 0, bool useTransparency = false);

    /**
     * @brief Initialises operator bitmaps for attackers and defenders
     * @param attackerNames Vector of attacker names
     * @param defenderNames Vector of defender names
     */
    void InitialiseOperatorBitmaps(
        const std::vector<const char*>& attackerNames,
        const std::vector<const char*>& defenderNames);

    /**
     * @brief Cleans up all operator bitmaps
     */
    void CleanupOperatorBitmaps();

    std::vector<HBITMAP>& GetCurrentBitmapList();

} // namespace Bitmap
