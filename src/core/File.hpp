#pragma once

#include <windows.h>

#include <string>

std::string GetExecutableDir();
std::string GetImagePath(const std::string& name);

HBITMAP LoadBitmap(const std::string& path);
