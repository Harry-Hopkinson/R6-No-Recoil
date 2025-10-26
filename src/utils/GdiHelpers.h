#pragma once

#include <windows.h>

namespace GdiHelpers
{

    /**
     * @brief RAII wrapper for GDI brush objects
     */
    class ScopedBrush
    {
    public:
        explicit ScopedBrush(COLORREF color) : brush(CreateSolidBrush(color)) {}
        ~ScopedBrush() { if (brush) DeleteObject(brush); }
        
        __forceinline operator HBRUSH() const { return brush; }
        __forceinline HBRUSH Get() const { return brush; }
        
    private:
        HBRUSH brush;
        ScopedBrush(const ScopedBrush&) = delete;
        ScopedBrush& operator=(const ScopedBrush&) = delete;
    };

    /**
     * @brief RAII wrapper for GDI pen objects
     */
    class ScopedPen
    {
    public:
        ScopedPen(int style, int width, COLORREF color) : pen(CreatePen(style, width, color)) {}
        ~ScopedPen() { if (pen) DeleteObject(pen); }
        
        __forceinline operator HPEN() const { return pen; }
        __forceinline HPEN Get() const { return pen; }
        
    private:
        HPEN pen;
        ScopedPen(const ScopedPen&) = delete;
        ScopedPen& operator=(const ScopedPen&) = delete;
    };

    /**
     * @brief RAII wrapper for selecting GDI objects
     */
    class ScopedSelectObject
    {
    public:
        ScopedSelectObject(HDC hdc, HGDIOBJ obj) : hdc(hdc), oldObj(SelectObject(hdc, obj)) {}
        ~ScopedSelectObject() { if (oldObj) SelectObject(hdc, oldObj); }
        
    private:
        HDC hdc;
        HGDIOBJ oldObj;
        ScopedSelectObject(const ScopedSelectObject&) = delete;
        ScopedSelectObject& operator=(const ScopedSelectObject&) = delete;
    };

    /**
     * @brief Creates and fills a rectangle with a solid color brush
     * @param hdc Device context
     * @param rect Rectangle to fill
     * @param color Fill color
     */
    __forceinline void FillRectColor(HDC hdc, const RECT& rect, COLORREF color)
    {
        ScopedBrush brush(color);
        FillRect(hdc, &rect, brush);
    }

    /**
     * @brief Draws a rectangle outline with specified pen
     * @param hdc Device context
     * @param rect Rectangle to draw
     * @param color Border color
     * @param width Border width
     */
    __forceinline void DrawRectBorder(HDC hdc, const RECT& rect, COLORREF color, int width = 1)
    {
        ScopedPen pen(PS_SOLID, width, color);
        ScopedSelectObject select(hdc, pen);
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    }

} // namespace GdiHelpers
