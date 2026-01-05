#include <iostream>

#include "webview/webview.h"
#include "index_html.h"

#ifdef _WIN32
int WINAPI WinMain(
    HINSTANCE /*hInst*/, HINSTANCE /*hPrevInst*/, LPSTR /*lpCmdLine*/,
    int /*nCmdShow*/)
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

        main_window.bind(
            "ping", [&](const std::string& args_str) -> std::string {
                std::cout << "Ping from UI: " << args_str << std::endl;
                return R"({"code": 200})";
            });

        main_window.set_html(INDEX_HTML);
        main_window.run();
    }
    catch (const webview::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
