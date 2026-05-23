#include "shared.h"

HWND hMain, hGo1, hGo2, hBoth, hParams, hPress, hSlotCb, hTCmd;
HWND hWaitTitle, hWaitTab, hTypeDelay;

LRESULT CALLBACK MgrProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    char user[64], pass[64], obf[128];
    switch(msg) {
        case WM_CREATE: {
			CreateAccountsGUI(hwnd);
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wp;
            SetTextColor(hdcStatic, txtColor);
            SetBkColor(hdcStatic, formColor);
            static HBRUSH hBrForm;
            if (!hBrForm) hBrForm = CreateSolidBrush(formColor);
            return (LRESULT)hBrForm;
        }

        case WM_DRAWITEM:
            DrawCustomBtn((LPDRAWITEMSTRUCT)lp);
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wp) == ID_MGR_CB && HIWORD(wp) == CBN_SELCHANGE) {
                GetWindowTextA(hMgrCb, user, 64);
                GetPrivateProfileStringA(user, "Pass", "", obf, 128, INI);
                Xcrypt(obf, pass, 1, user);
                SetWindowTextA(hMgrPass, pass);
            }
            if (LOWORD(wp) == ID_MGR_SAVE) {
                GetWindowTextA(hMgrCb, user, 64); GetWindowTextA(hMgrPass, pass, 64);
                if(strlen(user) > 0) {
                    Xcrypt(pass, obf, 0, user); WritePrivateProfileStringA(user, "Pass", obf, INI);
                    UpdateAccountList(user, 0);
                    SetWindowTextA(hStatus, "Saved!");
                    LoadAccounts(hMgrCb);
                }
            }
            if (LOWORD(wp) == ID_MGR_DEL) {
                GetWindowTextA(hMgrCb, user, 64);
                UpdateAccountList(user, 1); WritePrivateProfileStringA(user, NULL, NULL, INI);
                SetWindowTextA(hStatus, "Deleted!");
                LoadAccounts(hMgrCb);
            }
            break;

        case WM_DESTROY: 
            if(hCustomFont) DeleteObject(hCustomFont);
            PostQuitMessage(0); 
            break;
    }
    return DefWindowProcA(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lp, int nS) {
    GetModuleFileNameA(NULL, INI, MAX_PATH); 
    char* s = strrchr(INI, '\\'); if(s) strcpy(s+1, "settings.ini");
    LoadAllSettings();
	// Position window based on launcher coordinates
    int wx = GetInt("WinX", CW_USEDEFAULT);
    int wy = GetInt("WinY", CW_USEDEFAULT);
	
    WNDCLASSA mc = {0}; mc.lpfnWndProc = MgrProc; mc.hInstance = hI; mc.lpszClassName = "AccountMgr"; 
    mc.hbrBackground = CreateSolidBrush(formColor);
    RegisterClassA(&mc);
    
    HWND hwnd = CreateWindowA("AccountMgr", "Accounts Manager", WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_VISIBLE, wx, wy, 280, 200, 0, 0, hI, 0);
    MSG m; while (GetMessageA(&m, 0, 0, 0)) { TranslateMessage(&m); DispatchMessageA(&m); }
    return 0;
}
