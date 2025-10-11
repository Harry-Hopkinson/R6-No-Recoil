#pragma once

#include "../../Globals.h"
#include <vector>

typedef struct HWND__* HWND;
typedef struct HMENU__* HMENU;
typedef struct HINSTANCE__* HINSTANCE;

class Button
{
public:
    Button(HWND parent, int x, int y, int width, int height, const char* text, int id);

    HWND GetHWND() const;

private:
    HWND hwndButton;
    int id;
};

namespace Buttons
{
    std::vector<Button> GetButtons();
    void ClearButtons();

    void CreateMainMenuButtons(HWND hwnd);
    void CreateOperatorSelectionButtons(HWND hwnd);
}
