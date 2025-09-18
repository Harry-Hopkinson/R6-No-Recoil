#include "Bitmap.h"

#include "../Globals.h"
#include "../core/String.h"
#include "../files/Files.h"

namespace Bitmap
{

    std::vector<HBITMAP> AttackerBitmaps;
    std::vector<HBITMAP> DefenderBitmaps;

    static std::vector<WeaponBitmapEntry> WeaponBitmaps;

    std::vector<HBITMAP>& GetCurrentBitmapList()
    {
        return IsAttackerView ? AttackerBitmaps : DefenderBitmaps;
    }

    HBITMAP LoadBitmap(const char* path)
    {
        if (!path)
            return nullptr;

        return (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    }

    HBITMAP LoadWeaponBitmap(const char* weaponName)
    {
        char* cleanName = String::RemoveSpaces(weaponName);
        if (!cleanName)
            return nullptr;

        char* path = String::BuildPath("assets/weapons/", cleanName);
        HBITMAP bitmap = LoadBitmap(path);

        if (!bitmap)
            return bitmap;

        // Free the allocated strings
        delete[] cleanName;
        delete[] path;

        return bitmap;
    }

    HBITMAP GetWeaponBitmap(const char* weaponName)
    {
        if (!weaponName)
            return nullptr;

        // Search the cache first
        for (const auto& entry : WeaponBitmaps)
        {
            if (strcmp(entry.name, weaponName) == 0)
                return entry.bitmap;
        }

        // Not found in cache, load the bitmap
        HBITMAP bmp = LoadWeaponBitmap(weaponName);
        if (bmp)
        {
            // Make a copy of the name to store in our cache
            char* nameCopy = String::CreateStringCopy(weaponName);
            if (nameCopy)
            {
                WeaponBitmapEntry entry;
                entry.name = nameCopy;
                entry.bitmap = bmp;
                WeaponBitmaps.push_back(entry);
            }
        }
        return bmp;
    }

    std::vector<HBITMAP> LoadOperatorBitmaps(const std::vector<const char*>& names)
    {
        std::vector<HBITMAP> bitmaps;

        bitmaps.reserve(names.size());
        for (const auto& name : names)
        {
            const char* path = Files::GetImagePath(name);
            HBITMAP bitmap = LoadBitmap(path);
            bitmaps.push_back(bitmap);

            if (!bitmap)
                return bitmaps;
        }
        return bitmaps;
    }

    void CleanupWeaponBitmaps()
    {
        for (auto& entry : WeaponBitmaps)
        {
            if (entry.bitmap)
                DeleteObject(entry.bitmap);
            if (entry.name)
                delete[] entry.name;
        }
        WeaponBitmaps.clear();
    }

    void CleanupBitmaps(std::vector<HBITMAP>& bitmaps)
    {
        for (HBITMAP bmp : bitmaps)
        {
            if (bmp)
                DeleteObject(bmp);
        }
        bitmaps.clear();
    }

    bool DrawBitmap(HDC hdc, HBITMAP bitmap, int x, int y, int width, int height, bool showPlaceholder)
    {
        if (!hdc || !bitmap)
        {
            if (showPlaceholder)
                DrawPlaceholder(hdc, x, y, width, height);
            return false;
        }

        HDC hdcMem = CreateCompatibleDC(hdc);
        if (!hdcMem)
        {
            if (showPlaceholder)
                DrawPlaceholder(hdc, x, y, width, height);
            return false;
        }

        HGDIOBJ oldBmp = SelectObject(hdcMem, bitmap);
        BITMAP bm;
        GetObject(bitmap, sizeof(bm), &bm);

        // Draw a border around the image area
        RECT imgRect = { x, y, x + width, y + height };
        FrameRect(hdc, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Set up for better quality scaling
        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, NULL);

        // Draw the bitmap
        BOOL result = StretchBlt(hdc, x, y, width, height, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

        SelectObject(hdcMem, oldBmp);
        DeleteDC(hdcMem);

        if (!result && showPlaceholder)
        {
            DrawPlaceholder(hdc, x, y, width, height);
            return false;
        }

        return result != FALSE;
    }

    void DrawPlaceholder(HDC hdc, int x, int y, int width, int height, const char* text)
    {
        if (!hdc)
            return;

        RECT imgRect = { x, y, x + width, y + height };

        // Draw border and fill with gray
        FrameRect(hdc, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        FillRect(hdc, &imgRect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

        // Draw text if provided
        if (text)
        {
            int oldBkMode = SetBkMode(hdc, TRANSPARENT);
            DrawTextA(hdc, text, -1, &imgRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SetBkMode(hdc, oldBkMode);
        }
    }

} // namespace Bitmap
