#include "context-menu-manager.h"
#include "constants.h"
#include <Windows.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void Context_Menu_Manager::create_context_menu(HWND hwnd) {

    // create a context menu
    HMENU context_menu = CreatePopupMenu();
    AppendMenu(context_menu, MF_STRING, IDM_CONTEXT_EDIT_SETTINGS, "Edit Settings");
    AppendMenu(context_menu, MF_STRING, IDM_CONTEXT_HELP, "Help");
    AppendMenu(context_menu, MF_STRING, IDM_CONTEXT_EXIT, "Exit");

    /**
     * bring our window to the front, otherwise the context menu won't show correctly
     * @see
     * https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-trackpopupmenu#remarks
     */
    SetForegroundWindow(hwnd);

    // get the mouse position
    POINT pt;
    GetCursorPos(&pt);

    // display the context menu at the mouse position
    TrackPopupMenu(context_menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);

    // destroy the context menu object
    DestroyMenu(context_menu);
}

void Context_Menu_Manager::context_menu_item_click(HWND hwnd, WPARAM wParam) {
    if (wParam == IDM_CONTEXT_EDIT_SETTINGS) {
        // open the settings file
        WinExec(("notepad \"" + CONFIG_FILE_PATH + "\"").c_str(), SW_SHOWDEFAULT);
    }
    if (wParam == IDM_CONTEXT_HELP) {
        // open the github repo
        ShellExecute(hwnd, "open", "https://github.com/amrbashir/XBar/issues", nullptr, nullptr,
                     SW_SHOWDEFAULT);
    }
    if (wParam == IDM_CONTEXT_EXIT) {
        // exit the app
        DestroyWindow(hwnd);
    }
}
