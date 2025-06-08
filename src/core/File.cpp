#include "File.hpp"

#include "String.hpp"

std::string GetExecutableDir()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

std::string GetImagePath(const std::string& name)
{
    std::string fullPath = GetExecutableDir();
    size_t pos = fullPath.find_last_of("\\/");
    if (pos != std::string::npos)
        fullPath = fullPath.substr(0, pos);
    fullPath += std::string("\\") + name;
    return fullPath.c_str();
}

HBITMAP LoadBitmap(const std::string& path)
{
    std::wstring wpath = StringToWString("D:\\Programming\\R6-No-Recoil\\assets\\kaid.bmp");
    HBITMAP bitmap = (HBITMAP)LoadImageW(NULL, wpath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!bitmap)
    {
        MessageBox(NULL, "Cannot load bitmap", "Error", MB_OK);
    }
    return bitmap;
}
