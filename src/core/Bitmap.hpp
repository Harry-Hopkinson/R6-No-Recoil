#pragma once

#include <windows.h>
#include <vector>

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
     * @brief Loads a bitmap from file path
     * @param path The file path to the bitmap
     * @return Handle to the loaded bitmap, or nullptr if failed
     */
    HBITMAP LoadBitmap(const char* path);

    /**
     * @brief Loads a weapon bitmap by name, with caching
     * @param weaponName The name of the weapon
     * @return Handle to the loaded bitmap, or nullptr if failed
     */
    HBITMAP GetWeaponBitmap(const char* weaponName);

    /**
     * @brief Loads all operator bitmaps for a given list of names
     * @param names Vector of operator names
     * @param getImagePathFunc Function to get image path from name
     * @return Vector of loaded bitmap handles
     */
    std::vector<HBITMAP> LoadOperatorBitmaps(const std::vector<const char*>& names,
                                           const char* (*getImagePathFunc)(const char*));

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
     * @brief Draws a bitmap with proper scaling and error handling
     * @param hdc Device context to draw on
     * @param bitmap Bitmap to draw
     * @param x X position
     * @param y Y position
     * @param width Width to draw
     * @param height Height to draw
     * @param showPlaceholder Whether to show placeholder on failure
     * @return True if bitmap was drawn successfully
     */
    bool DrawBitmap(HDC hdc, HBITMAP bitmap, int x, int y, int width, int height, bool showPlaceholder = true);

    /**
     * @brief Draws a placeholder rectangle with optional text
     * @param hdc Device context to draw on
     * @param x X position
     * @param y Y position
     * @param width Width of placeholder
     * @param height Height of placeholder
     * @param text Optional text to display (default: "No Image")
     */
    void DrawPlaceholder(HDC hdc, int x, int y, int width, int height, const char* text = "No Image");

    extern std::vector<HBITMAP> AttackerBitmaps;
    extern std::vector<HBITMAP> DefenderBitmaps;

    std::vector<HBITMAP>& GetCurrentBitmapList();
}
