#pragma once

#include "../../Globals.h"
#include "../../core/CompilerUtils.h"
#include <vector>

typedef struct HWND__* HWND;
typedef struct HMENU__* HMENU;
typedef struct HINSTANCE__* HINSTANCE;

class Button
{
public:
    Button(HWND parent, int x, int y, int width, int height, const char* text, int id);

    FORCE_INLINE HWND GetHWND() const { return hwndButton; }

private:
    HWND hwndButton;
    int id;
};

namespace Buttons
{
    extern std::vector<Button> ButtonsVector;

    FORCE_INLINE const std::vector<Button>& GetButtons()
    {
        return ButtonsVector;
    }

    void ClearButtons();

    void CreateMainMenuButtons(HWND hwnd);
    void CreateOperatorSelectionButtons(HWND hwnd);
}
