#pragma once

#include <windows.h>

namespace LayoutUtils
{
    /**
     * @brief Layout constants for operator selection grid
     */
    struct OperatorGridLayout
    {
        static constexpr int GRID_START_X = 30;
        static constexpr int GRID_START_Y = 50;
        static constexpr int COLUMNS = 6;
        static constexpr int CELL_SIZE = 110;
        static constexpr int CELL_SPACING = 10;
        static constexpr int CELL_STRIDE = CELL_SIZE + CELL_SPACING;

        /**
         * @brief Calculate grid cell position
         * @param index Cell index
         * @param outX Output X position
         * @param outY Output Y position
         */
        static inline void GetCellPosition(size_t index, int& outX, int& outY)
        {
            outX = GRID_START_X + static_cast<int>(index % COLUMNS) * CELL_STRIDE;
            outY = GRID_START_Y + static_cast<int>(index / COLUMNS) * CELL_STRIDE;
        }

        /**
         * @brief Get cell bounds as RECT
         * @param index Cell index
         * @return RECT with cell bounds
         */
        static inline RECT GetCellRect(size_t index)
        {
            int x, y;
            GetCellPosition(index, x, y);
            return { x, y, x + CELL_SIZE, y + CELL_SIZE };
        }
    };

    /**
     * @brief Layout constants for weapon display
     */
    struct WeaponDisplayLayout
    {
        static constexpr int WEAPON_WIDTH = 400;
        static constexpr int WEAPON_HEIGHT = 150;
        static constexpr int WEAPON_SPACING = 10;
        static constexpr int WEAPON_NAME_HEIGHT = 30;

        static constexpr int SECTION_OFFSET_FROM_BOTTOM = 330;

        /**
         * @brief Calculate centered weapon positions
         * @param weaponCount Number of weapons
         * @param screenWidth Screen width
         * @param screenHeight Screen height
         * @param outStartX Output start X position
         * @param outStartY Output start Y position
         */
        static inline void GetWeaponStartPosition(
            int weaponCount, int screenWidth, int screenHeight, int& outStartX, int& outStartY)
        {
            int totalWidth = weaponCount * WEAPON_WIDTH + (weaponCount - 1) * WEAPON_SPACING;
            outStartX = (screenWidth - totalWidth) / 2;
            outStartY = 40 + (screenHeight - 120 - (WEAPON_HEIGHT + 50)) / 2;
        }

        /**
         * @brief Get weapon click area rect
         * @param weaponIndex Weapon index
         * @param startX Start X position from GetWeaponStartPosition
         * @param startY Start Y position from GetWeaponStartPosition
         * @return RECT with weapon clickable area
         */
        static inline RECT GetWeaponRect(int weaponIndex, int startX, int startY)
        {
            int x = startX + weaponIndex * (WEAPON_WIDTH + WEAPON_SPACING);
            return { x, startY, x + WEAPON_WIDTH, startY + WEAPON_HEIGHT + 45 };
        }
    };

    /**
     * @brief Check if a point is inside a rectangle
     * @param rect Rectangle to test
     * @param x Point X coordinate
     * @param y Point Y coordinate
     * @return true if point is inside rectangle
     */
    inline bool IsPointInRect(const RECT& rect, int x, int y)
    {
        return x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom;
    }

} // namespace LayoutUtils
