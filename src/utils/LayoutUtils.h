#pragma once

#include <windows.h>

namespace LayoutUtils
{

    /**
     * @brief Layout constants for operator selection grid
     */
    struct OperatorGridLayout
    {
        static constexpr int COLUMNS = 6;
        static constexpr float GRID_START_X_PERCENT = 0.025f;
        static constexpr float GRID_START_Y_PERCENT = 0.054f;
        static constexpr float CELL_SIZE_PERCENT = 0.092f;
        static constexpr float CELL_SPACING_PERCENT = 0.008f;

        // Minimum sizes to prevent UI from becoming unusable
        static constexpr int MIN_CELL_SIZE = 80;
        static constexpr int MIN_CELL_SPACING = 8;

        /**
         * @brief Get cell size based on window width
         * @param windowWidth Current window width
         * @return Calculated cell size
         */
        static inline int GetCellSize(int windowWidth)
        {
            int size = static_cast<int>(windowWidth * CELL_SIZE_PERCENT);
            return (std::max)(size, MIN_CELL_SIZE);
        }

        /**
         * @brief Get cell spacing based on window width
         * @param windowWidth Current window width
         * @return Calculated cell spacing
         */
        static inline int GetCellSpacing(int windowWidth)
        {
            int spacing = static_cast<int>(windowWidth * CELL_SPACING_PERCENT);
            return (std::max)(spacing, MIN_CELL_SPACING);
        }

        /**
         * @brief Calculate grid cell position
         * @param index Cell index
         * @param windowWidth Current window width
         * @param windowHeight Current window height
         * @param outX Output X position
         * @param outY Output Y position
         */
        static inline void GetCellPosition(size_t index, int windowWidth, int windowHeight, int& outX, int& outY)
        {
            int gridStartX = static_cast<int>(windowWidth * GRID_START_X_PERCENT);
            int gridStartY = static_cast<int>(windowHeight * GRID_START_Y_PERCENT);
            int cellSize = GetCellSize(windowWidth);
            int cellSpacing = GetCellSpacing(windowWidth);
            int cellStride = cellSize + cellSpacing;

            outX = gridStartX + static_cast<int>(index % COLUMNS) * cellStride;
            outY = gridStartY + static_cast<int>(index / COLUMNS) * cellStride;
        }

        /**
         * @brief Get cell bounds as RECT
         * @param index Cell index
         * @param windowWidth Current window width
         * @param windowHeight Current window height
         * @return RECT with cell bounds
         */
        static inline RECT GetCellRect(size_t index, int windowWidth, int windowHeight)
        {
            int x, y;
            GetCellPosition(index, windowWidth, windowHeight, x, y);
            int cellSize = GetCellSize(windowWidth);
            return { x, y, x + cellSize, y + cellSize };
        }
    };

    /**
     * @brief Layout constants for weapon display
     */
    struct WeaponDisplayLayout
    {
        static constexpr float WEAPON_WIDTH_PERCENT = 0.333f;   // 33.3% of width per weapon
        static constexpr float WEAPON_HEIGHT_PERCENT = 0.162f;  // 16.2% of height
        static constexpr float WEAPON_SPACING_PERCENT = 0.008f; // 0.8% of width

        // Minimum sizes
        static constexpr int MIN_WEAPON_WIDTH = 300;
        static constexpr int MIN_WEAPON_HEIGHT = 120;
        static constexpr int MIN_WEAPON_SPACING = 8;

        static constexpr int WEAPON_NAME_HEIGHT = 30;
        static constexpr float SECTION_OFFSET_PERCENT = 0.357f; // 35.7% from bottom

        /**
         * @brief Get weapon width
         * @param windowWidth Current window width
         * @return Calculated weapon width
         */
        static inline int GetWeaponWidth(int windowWidth)
        {
            int width = static_cast<int>(windowWidth * WEAPON_WIDTH_PERCENT);
            return (std::max)(width, MIN_WEAPON_WIDTH);
        }

        /**
         * @brief Get weapon height
         * @param windowHeight Current window height
         * @return Calculated weapon height
         */
        static inline int GetWeaponHeight(int windowHeight)
        {
            int height = static_cast<int>(windowHeight * WEAPON_HEIGHT_PERCENT);
            return (std::max)(height, MIN_WEAPON_HEIGHT);
        }

        /**
         * @brief Get weapon spacing
         * @param windowWidth Current window width
         * @return Calculated weapon spacing
         */
        static inline int GetWeaponSpacing(int windowWidth)
        {
            int spacing = static_cast<int>(windowWidth * WEAPON_SPACING_PERCENT);
            return (std::max)(spacing, MIN_WEAPON_SPACING);
        }

        /**
         * @brief Calculate centered weapon positions
         * @param weaponCount Number of weapons
         * @param windowWidth Current window width
         * @param windowHeight Current window height
         * @param outStartX Output start X position
         * @param outStartY Output start Y position
         */
        static inline void GetWeaponStartPosition(
            int weaponCount, int windowWidth, int windowHeight, int& outStartX, int& outStartY)
        {
            int weaponWidth = GetWeaponWidth(windowWidth);
            int weaponHeight = GetWeaponHeight(windowHeight);
            int spacing = GetWeaponSpacing(windowWidth);

            int totalWidth = weaponCount * weaponWidth + (weaponCount - 1) * spacing;
            outStartX = (windowWidth - totalWidth) / 2;
            outStartY = static_cast<int>(windowHeight * 0.25f); // 25% from top
        }

        /**
         * @brief Get weapon click area rect
         * @param weaponIndex Weapon index
         * @param windowWidth Current window width
         * @param windowHeight Current window height
         * @param startX Start X position from GetWeaponStartPosition
         * @param startY Start Y position from GetWeaponStartPosition
         * @return RECT with weapon clickable area
         */
        static inline RECT GetWeaponRect(int weaponIndex, int windowWidth, int windowHeight, int startX, int startY)
        {
            int weaponWidth = GetWeaponWidth(windowWidth);
            int weaponHeight = GetWeaponHeight(windowHeight);
            int spacing = GetWeaponSpacing(windowWidth);

            int x = startX + weaponIndex * (weaponWidth + spacing);
            return { x, startY, x + weaponWidth, startY + weaponHeight + 45 };
        }

        /**
         * @brief Get section offset from bottom (for notes, etc.)
         * @param windowHeight Current window height
         * @return Calculated offset
         */
        static inline int GetSectionOffset(int windowHeight)
        {
            return static_cast<int>(windowHeight * SECTION_OFFSET_PERCENT);
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
