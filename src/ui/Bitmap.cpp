#include "Bitmap.h"

#include <windows.h>

#include "../Globals.h"
#include "../core/String.h"
#include "../files/Files.h"

namespace Bitmap
{

    static std::vector<HBITMAP> AttackerBitmaps;
    static std::vector<HBITMAP> DefenderBitmaps;
    static std::vector<WeaponBitmapEntry> WeaponBitmaps;

    std::vector<HBITMAP>& GetCurrentBitmapList()
    {
        return IsAttackerView ? AttackerBitmaps : DefenderBitmaps;
    }

    void InitialiseOperatorBitmaps(const std::vector<const char*>& attackerNames, const std::vector<const char*>& defenderNames)
    {
        AttackerBitmaps = LoadOperatorBitmaps(attackerNames);
        DefenderBitmaps = LoadOperatorBitmaps(defenderNames);
    }

    void CleanupOperatorBitmaps()
    {
        CleanupBitmaps(AttackerBitmaps);
        CleanupBitmaps(DefenderBitmaps);
    }

    static HBITMAP LoadBitmap(const char* path)
    {
        if (!path) return nullptr;
        return (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    }

    HBITMAP LoadWeaponBitmap(const char* weaponName)
    {
        if (!weaponName) return nullptr;

        char* path = String::BuildPath("assets/weapons/", weaponName);
        HBITMAP bitmap = LoadBitmap(path);
        delete[] path;
        return bitmap;
    }

    HBITMAP GetWeaponBitmap(const char* weaponName)
    {
        if (!weaponName) return nullptr;

        // Search cache
        for (const auto& entry : WeaponBitmaps)
            if (strcmp(entry.name, weaponName) == 0) return entry.bitmap;

        // Not cached, load
        HBITMAP bmp = LoadWeaponBitmap(weaponName);
        if (bmp) WeaponBitmaps.push_back({ String::CreateStringCopy(weaponName), bmp });

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
            if (!bitmap) return {}; // Return empty if any bitmap fails
            bitmaps.push_back(bitmap);
        }

        return bitmaps;
    }

    void CleanupWeaponBitmaps()
    {
        for (auto& entry : WeaponBitmaps)
        {
            if (entry.bitmap) DeleteObject(entry.bitmap);
            if (entry.name) delete[] entry.name;
        }
        WeaponBitmaps.clear();
    }

    void CleanupBitmaps(std::vector<HBITMAP>& bitmaps)
    {
        for (HBITMAP bmp : bitmaps)
            if (bmp) DeleteObject(bmp);
        bitmaps.clear();
    }

    void DrawOperatorBitmap(HDC hdc, HBITMAP bitmap, int x, int y, int width, int height)
    {
        if (!hdc || !bitmap) return;

        HDC memDC = CreateCompatibleDC(hdc);
        HGDIOBJ oldBmp = SelectObject(memDC, bitmap);

        BITMAP bm{};
        GetObject(bitmap, sizeof(bm), &bm);

        const int cropMargin = 45;

        int cropLeft = cropMargin;
        int cropTop = cropMargin;
        int cropWidth = bm.bmWidth - (cropMargin * 2);
        int cropHeight = bm.bmHeight - (cropMargin * 2);

        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, NULL);

        StretchBlt(hdc, x, y, width, height, memDC, cropLeft, cropTop, cropWidth, cropHeight, SRCCOPY);

        SelectObject(memDC, oldBmp);
        DeleteDC(memDC);
    }

    void DrawWeaponBitmap(HDC hdc, HBITMAP bitmap, int x, int y, int width, int height)
    {
        if (!hdc || !bitmap) return;

        HDC memDC = CreateCompatibleDC(hdc);
        HGDIOBJ oldBmp = SelectObject(memDC, bitmap);

        BITMAP bm{};
        GetObject(bitmap, sizeof(bm), &bm);

        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, NULL);

        StretchBlt(hdc, x, y, width, height, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

        SelectObject(memDC, oldBmp);
        DeleteDC(memDC);
    }

} // namespace Bitmap
