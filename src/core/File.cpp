#include "File.hpp"

std::string GetExecutableDir()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

std::wstring StringToWString(const std::string& str)
{
    if (str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    return wstr;
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
    std::wstring wpath = StringToWString(path);
    HBITMAP bitmap = (HBITMAP)LoadImageW(
        NULL,
        wpath.c_str(),
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION
    );
    if (!bitmap) MessageBox(NULL, "Cannot load bitmap", "Error", MB_OK);
    return bitmap;
}
