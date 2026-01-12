#pragma once
#include <windows.h>

namespace LayoutUtils
{

    struct OperatorGridLayout
    {
        static constexpr int COLUMNS = 6;

        static constexpr float GRID_START_X_PERCENT = 0.025f;
        static constexpr float GRID_START_Y_PERCENT = 0.054f;
        static constexpr float CELL_SIZE_PERCENT = 0.092f;
        static constexpr float CELL_SPACING_PERCENT = 0.015f;

        static constexpr int MIN_CELL_SIZE = 80;
        static constexpr int MIN_CELL_SPACING = 8;

        static inline int GetCellSize(int windowWidth, int windowHeight)
        {
            int sizeW = static_cast<int>(windowWidth * CELL_SIZE_PERCENT);
            int sizeH = static_cast<int>(windowHeight * 0.12f);
            return (std::max)((std::min)(sizeW, sizeH), MIN_CELL_SIZE);
        }

        static inline int GetCellSpacing(int windowWidth)
        {
            return (
                std::max)(static_cast<int>(windowWidth * CELL_SPACING_PERCENT),
                          MIN_CELL_SPACING);
        }

        static inline int GetGridStartX(int windowWidth)
        {
            return static_cast<int>(windowWidth * GRID_START_X_PERCENT);
        }

        static inline void GetCellPosition(
            size_t index, int windowWidth, int windowHeight, int& outX,
            int& outY)
        {
            int cellSize = GetCellSize(windowWidth, windowHeight);
            int spacing = GetCellSpacing(windowWidth);
            int stride = cellSize + spacing;

            int startX = GetGridStartX(windowWidth);
            int startY = static_cast<int>(windowHeight * GRID_START_Y_PERCENT);

            outX = startX + int(index % COLUMNS) * stride;
            outY = startY + int(index / COLUMNS) * stride;
        }

        static inline RECT GetCellRect(
            size_t index, int windowWidth, int windowHeight)
        {
            int x, y;
            GetCellPosition(index, windowWidth, windowHeight, x, y);
            int size = GetCellSize(windowWidth, windowHeight);
            return { x, y, x + size, y + size };
        }
    };

    struct WeaponDisplayLayout
    {
        static constexpr float WEAPON_WIDTH_PERCENT = 0.333f;
        static constexpr float WEAPON_HEIGHT_PERCENT = 0.162f;
        static constexpr float WEAPON_SPACING_PERCENT = 0.008f;

        static constexpr int MIN_WEAPON_WIDTH = 300;
        static constexpr int MIN_WEAPON_HEIGHT = 120;
        static constexpr int MIN_WEAPON_SPACING = 8;

        static inline int GetWeaponWidth(int windowWidth)
        {
            return (
                std::max)(static_cast<int>(windowWidth * WEAPON_WIDTH_PERCENT),
                          MIN_WEAPON_WIDTH);
        }

        static inline int GetWeaponHeight(int windowHeight)
        {
            return (std::max)(static_cast<int>(
                                  windowHeight * WEAPON_HEIGHT_PERCENT),
                              MIN_WEAPON_HEIGHT);
        }

        static inline int GetWeaponSpacing(int windowWidth)
        {
            return (std::max)(static_cast<int>(
                                  windowWidth * WEAPON_SPACING_PERCENT),
                              MIN_WEAPON_SPACING);
        }

        static inline void GetWeaponStartPosition(
            int weaponCount, int windowWidth, int windowHeight, int& outStartX,
            int& outStartY)
        {
            int w = GetWeaponWidth(windowWidth);
            int s = GetWeaponSpacing(windowWidth);

            int totalWidth = weaponCount * w + (weaponCount - 1) * s;
            outStartX = (windowWidth - totalWidth) / 2;
            outStartY = static_cast<int>(windowHeight * 0.25f);
        }

        static inline RECT GetWeaponRect(
            int weaponIndex, int windowWidth, int windowHeight, int startX,
            int startY)
        {
            int w = GetWeaponWidth(windowWidth);
            int h = GetWeaponHeight(windowHeight);
            int s = GetWeaponSpacing(windowWidth);

            int x = startX + weaponIndex * (w + s);
            return { x, startY, x + w, startY + h + 45 };
        }

        static inline RECT GetToggleButtonRect(
            int weaponX, int weaponY, int weaponWidth, int weaponHeight,
            int windowHeight)
        {
            int buttonHeight = static_cast<int>(windowHeight * 0.049f);
            int buttonSpacing = static_cast<int>(windowHeight * 0.011f);
            int keyWidth = static_cast<int>(weaponWidth * 0.25f);
            int keyHeight = static_cast<int>(windowHeight * 0.032f);

            int nameOffsetY = static_cast<int>(weaponHeight * 0.1f);
            int nameHeight = static_cast<int>(windowHeight * 0.049f);

            int buttonStartY = weaponY + weaponHeight + nameOffsetY + nameHeight
                + static_cast<int>(windowHeight * 0.016f);

            int keyStartX = weaponX + (weaponWidth - keyWidth) / 2;
            int keyStartY = buttonStartY + 3 * (buttonHeight + buttonSpacing)
                + static_cast<int>(windowHeight * 0.032f);

            int toggleOffsetY = static_cast<int>(keyHeight * 0.67f);

            return { keyStartX, keyStartY + keyHeight + toggleOffsetY,
                     keyStartX + keyWidth,
                     keyStartY + keyHeight + toggleOffsetY + keyHeight };
        }
    };

    inline bool IsPointInRect(const RECT& r, int x, int y)
    {
        return x >= r.left && x <= r.right && y >= r.top && y <= r.bottom;
    }

} // namespace LayoutUtils
