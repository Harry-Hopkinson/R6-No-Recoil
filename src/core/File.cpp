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
    HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return hBmp;
}
