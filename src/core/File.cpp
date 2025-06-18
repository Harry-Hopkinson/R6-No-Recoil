#include "File.hpp"

const char* GetExecutableDir()
{
    static char exePath[MAX_PATH] = {};
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    return exePath;
}

const char* GetImagePath(const char* name)
{
    static char fullPath[MAX_PATH] = {};
    const char* exePath = GetExecutableDir();

    int lastSlash = -1;
    for (int i = 0; exePath[i] != '\0'; ++i)
    {
        if (exePath[i] == '\\' || exePath[i] == '/')
        {
            lastSlash = i;
        }
    }

    int j = 0;
    for (int i = 0; i <= lastSlash && j < MAX_PATH - 1; ++i)
    {
        fullPath[j++] = exePath[i];
    }

    // Append "\assets\build\"
    const char* assets = "\\assets\\operators\\";
    for (int i = 0; assets[i] != '\0' && j < MAX_PATH - 1; ++i)
    {
        fullPath[j++] = assets[i];
    }

    // Append name
    for (int i = 0; name[i] != '\0' && j < MAX_PATH - 1; ++i)
    {
        fullPath[j++] = name[i];
    }

    // Append ".bmp"
    const char* ext = ".bmp";
    for (int i = 0; ext[i] != '\0' && j < MAX_PATH - 1; ++i)
    {
        fullPath[j++] = ext[i];
    }

    fullPath[j] = '\0';
    return fullPath;
}

HBITMAP LoadBitmap(const char* path)
{
    wchar_t wpath[MAX_PATH] = {};
    int i = 0;
    for (; path[i] != '\0' && i < MAX_PATH - 1; ++i)
    {
        wpath[i] = (wchar_t)path[i];
    }
    wpath[i] = L'\0';

    HBITMAP bitmap = (HBITMAP)LoadImageW(
        NULL,
        wpath,
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION
    );

    if (!bitmap) MessageBoxA(NULL, "Failed to load bitmap", "Error", MB_OK);
    return bitmap;
}
