#include <windows.h>

#include "../ui/Bitmap.h"

namespace Drawing
{

    void DrawAttachmentDisplay(HDC memDC, int right, int bottom)
    {
        SetBkMode(memDC, TRANSPARENT);

        HBITMAP HoloSight = Bitmap::LoadBitmap("assets/attachments/holo.bmp");
        HBITMAP MagnifiedSight = Bitmap::LoadBitmap("assets/attachments/magnified.bmp");

        // Back button at the bottom
        RECT backBtn = { 30, bottom - 80, 130, bottom - 30 };
        DrawText(memDC, "Back", -1, &backBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
    }

} // namespace Drawing
