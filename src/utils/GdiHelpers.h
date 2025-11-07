#pragma once

#include <windows.h>

namespace GdiHelpers
{

    /**
     * @brief RAII wrapper for selecting GDI objects
     */
    class ScopedSelectObject
    {
    public:
        ScopedSelectObject(HDC hdc, HGDIOBJ obj)
            : hdc(hdc)
            , oldObj(SelectObject(hdc, obj))
        {
        }
        ~ScopedSelectObject()
        {
            if (oldObj) SelectObject(hdc, oldObj);
        }

    private:
        HDC hdc;
        HGDIOBJ oldObj;
        ScopedSelectObject(const ScopedSelectObject&) = delete;
        ScopedSelectObject& operator=(const ScopedSelectObject&) = delete;
    };

} // namespace GdiHelpers
