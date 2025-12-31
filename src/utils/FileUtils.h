#pragma once

#include <windows.h>
#include <cstdio>

namespace FileUtils
{

    /**
     * @brief Reads entire file into memory
     * @param filename Path to file
     * @param outSize Output parameter for file size
     * @return Pointer to file contents in memory
     */
    inline char* ReadFileToMemory(
        const char* filename, DWORD* outSize = nullptr)
    {
        if (!filename)
            return nullptr;

        HANDLE file = CreateFileA(
            filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);

        if (file == INVALID_HANDLE_VALUE)
            return nullptr;

        DWORD fileSize = GetFileSize(file, NULL);
        if (fileSize == INVALID_FILE_SIZE || fileSize == 0)
        {
            CloseHandle(file);
            return nullptr;
        }

        char* buffer = new char[fileSize + 1];
        DWORD bytesRead;

        if (!ReadFile(file, buffer, fileSize, &bytesRead, NULL))
        {
            delete[] buffer;
            CloseHandle(file);
            return nullptr;
        }

        buffer[bytesRead] = '\0';
        CloseHandle(file);

        if (outSize)
            *outSize = bytesRead;

        return buffer;
    }

    /**
     * @brief Writes buffer to file
     * @param filename Path to file
     * @param buffer Data to write
     * @param length Length of data
     */
    inline void WriteFileFromMemory(
        const char* filename, const char* buffer, DWORD length)
    {
        if (!filename || !buffer || length == 0)
            return;

        HANDLE file = CreateFileA(
            filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, NULL);

        if (file == INVALID_HANDLE_VALUE)
            return;

        DWORD written;
        WriteFile(file, buffer, length, &written, NULL);
        CloseHandle(file);
    }

    /**
     * @brief Gets the directory of the current executable
     * @return Pointer to static buffer containing executable directory path
     */
    inline const char* GetExecutableDir()
    {
        static char exePath[MAX_PATH] = {};
        if (exePath[0] == '\0')
            GetModuleFileNameA(NULL, exePath, MAX_PATH);
        return exePath;
    }

} // namespace FileUtils
