#pragma once

typedef struct HWND__* HWND;

namespace WindowUtils
{

    /**
     * @brief Invalidates and redraws the window without erasing background
     * @param hwnd Window handle to redraw
     */
    inline void InvalidateWindow(HWND hwnd)
    {
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
    }

    /**
     * @brief Invalidates and redraws the window without erasing background or children
     * @param hwnd Window handle to redraw
     */
    inline void InvalidateWindowNoChildren(HWND hwnd)
    {
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE | RDW_NOCHILDREN);
    }

} // namespace WindowUtils
