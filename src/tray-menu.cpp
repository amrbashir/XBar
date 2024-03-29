#include "tray-menu.h"
#include "config-manager.h"
#include "constants.h"
#include "types.h"
#include <Windows.h>
#include <filesystem>
#include <nlohmann/json.hpp>

using namespace std;
namespace fs = std::filesystem;

void TrayMenu::show(HWND hwnd) {
    HMENU context_menu = CreatePopupMenu();
    AppendMenu(context_menu, MF_STRING, MENU_ITEM_EDIT_SETTINGS, "Edit Settings");
    AppendMenu(context_menu, MF_STRING, MENU_ITEM_RELOAD_CONFIG, "Reload Config");
    AppendMenu(context_menu, MF_STRING, MENU_ITEM_HELP, "Help");
    AppendMenu(context_menu, MF_STRING, MENU_ITEM_EXIT, "Exit");

    /**
     * bring our window to the front, otherwise the context menu won't show correctly
     * @see
     * https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-trackpopupmenu#remarks
     */
    SetForegroundWindow(hwnd);
    POINT pt;
    GetCursorPos(&pt);
    TrackPopupMenu(context_menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);

    DestroyMenu(context_menu);
}

void TrayMenu::on_menu_item_click(HWND hwnd, WPARAM wParam, types::RunData &run_data) {
    switch (wParam) {
        case MENU_ITEM_EDIT_SETTINGS:
            WinExec(("notepad \"" + CONFIG_FILE_PATH + "\"").c_str(), SW_SHOWDEFAULT);
            break;

        case MENU_ITEM_RELOAD_CONFIG:
            run_data.config = ConfigManager::parse_config_file();
            break;

        case MENU_ITEM_HELP:
            ShellExecute(hwnd, "open", "https://github.com/amrbashir/XBar/issues", nullptr, nullptr,
                         SW_SHOWDEFAULT);
            break;

        case MENU_ITEM_EXIT:
            PostQuitMessage(0);
            break;
    }
}
