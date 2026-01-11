#include <iostream>
#include <fstream>

#include "webview/webview.h"
#include "index_html.h"

#include "resource.h"

#ifdef _WIN32
    #include <windows.h>
    #include <commdlg.h>
    #include <shlobj.h>
#endif

std::string configFilePath = "Config.toml";

std::string ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Write file contents
bool WriteFile(const std::string& path, const std::string& content)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        return false;
    }
    file << content;
    file.close();
    return true;
}

// Escape string for JavaScript
std::string EscapeStringJS(const std::string& str)
{
    std::string result;
    for (char c : str)
    {
        switch (c)
        {
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            case '\\':
                result += "\\\\";
                break;
            case '\'':
                result += "\\'";
                break;
            case '\"':
                result += "\\\"";
                break;
            default:
                result += c;
        }
    }
    return result;
}

#ifdef _WIN32
std::string OpenFileDialog(HWND hwnd, bool save = false)
{
    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "TOML Files (*.toml)\0*.toml\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = save ? "Save Config File" : "Open Config File";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (save)
    {
        ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
        ofn.lpstrDefExt = "toml";
        if (GetSaveFileNameA(&ofn))
        {
            return std::string(filename);
        }
    }
    else
    {
        if (GetOpenFileNameA(&ofn))
        {
            return std::string(filename);
        }
    }

    return "";
}
#endif

#ifdef _WIN32
int WINAPI
    WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
#else
int main()
{
#endif
    try
    {
        webview::webview main_window(false, nullptr);
        main_window.set_title("R6 No Recoil - Config Editor");
        main_window.set_size(1280, 720, WEBVIEW_HINT_NONE);

        // Bind loadConfig function
        main_window.bind("loadConfig", [&](std::string) -> std::string {
#ifdef _WIN32
            std::string path = OpenFileDialog(nullptr, false);
            if (!path.empty())
            {
                configFilePath = path;
                std::string content = ReadFile(path);
                if (!content.empty())
                {
                    std::string escapedContent = escapeForJS(content);
                    main_window.eval(
                        "window.loadConfigData('" + escapedContent + "')");
                    return "success";
                }
            }
#else
            std::string content = ReadFile(configFilePath);
            if (!content.empty()) {
                std::string escapedContent = EscapeStringJS(content);
                main_window.eval("window.loadConfigData('" + escapedContent + "')");
                return "success";
            }
#endif
            return "failed";
        });

        // Bind saveConfig function
        main_window.bind("saveConfig", [&](std::string content) -> std::string {
#ifdef _WIN32
            std::string path = OpenFileDialog(nullptr, true);
            if (!path.empty())
            {
                configFilePath = path;
                if (writeFile(path, content))
                {
                    main_window.eval(
                        "window.showStatus ? window.showStatus('Configuration "
                        "saved successfully!') : null");
                    return "success";
                }
            }
#else
            // For non-Windows, save to default path
            if (WriteFile(configFilePath, content))
            {
                main_window.eval("window.showStatus ? window.showStatus('Configuration saved successfully!') : null");
                return "success";
            }
#endif
            main_window.eval("window.showStatus ? window.showStatus('Failed to "
                             "save configuration') : null");
            return "failed";
        });

        main_window.set_html(INDEX_HTML);

        // Auto-load config if it exists
        std::string initialConfig = ReadFile(configFilePath);
        if (!initialConfig.empty())
        {
            std::string escapedContent = EscapeStringJS(initialConfig);
            main_window.init("window.loadConfigData('" + escapedContent + "')");
        }

        main_window.run();
    }
    catch (const webview::exception& e)
    {
        std::cerr << "Webview error: " << e.what() << '\n';
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
