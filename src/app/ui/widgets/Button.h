#pragma once

#include <windows.h>

#include <vector>

struct Button
{
    int id;
    int x, y, width, height;
    const char* text;

    RECT GetRect() const
    {
        return { x, y, x + width, y + height };
    }

    bool Contains(int px, int py) const
    {
        return px >= x && px < x + width && py >= y && py < y + height;
    }
};

namespace Buttons
{

    std::vector<Button>& GetButtons();

    void CreateOperatorSelectionButtons();

    void ClearButtons();

} // namespace Buttons
