#include "context-menu-manager.h"
#include "constants.h"
#include "tray-icon-manager.h"
#include <Windows.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void Context_Menu_Manager::create_context_menu(HWND hwnd) {

    HMENU context_menu = CreatePopupMenu();
    AppendMenu(context_menu, MF_STRING, IDM_CONTEXT_EDIT_SETTINGS, "Edit Settings");
    AppendMenu(context_menu, MF_STRING, IDM_CONTEXT_HELP, "Help");
    AppendMenu(context_menu, MF_STRING, IDM_CONTEXT_EXIT, "Exit");

    // bring our hidden window to the front, otherwise the context menu won't show correctly
    SetForegroundWindow(hwnd);

    // get the mouse position
    POINT pt;
    GetCursorPos(&pt);

    // display the context menu at the mouse position
    TrackPopupMenu(context_menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);

    // destroy the context menu object
    DestroyMenu(context_menu);
}

void Context_Menu_Manager::context_menu_item_click(HWND hwnd, WPARAM wParam) {
    if (wParam == IDM_CONTEXT_EDIT_SETTINGS) {
        WinExec(("notepad \"" + CONFIG_FILE_PATH + "\"").c_str(), SW_SHOWDEFAULT);
    }
    if (wParam == IDM_CONTEXT_HELP) {
        ShellExecute(hwnd, "open", "https://github.com/amrbashir/XBar", NULL, NULL, SW_SHOWDEFAULT);
    }
    if (wParam == IDM_CONTEXT_EXIT) {
        Tray_Icon_Manager::unregister_tray_icon(hwnd);
        ExitProcess(0);
    }
}
