#pragma once

void DebugMessageInt(const char* label, int value)
{
    char buf[256];
    sprintf(buf, "%s: %d", label, value);
    MessageBoxA(NULL, buf, "Debug Message", MB_OK | MB_ICONINFORMATION);
}

void DebugMessageString(const char* label, const char* value)
{
    char buf[256];
    sprintf(buf, "%s: %s", label, value);
    MessageBoxA(NULL, buf, "Debug Message", MB_OK | MB_ICONINFORMATION);
}
