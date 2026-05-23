#include "shared.h"

void LaunchGame(HWND hCombo, const char* label, const char* pidKey) {
    DWORD pid = (DWORD)GetInt(pidKey, 0);
    
    if (IsProcessRunning(pid)) {
        HWND existing = FindWindowByPID(pid);
        if (existing) {
            if (IsIconic(existing)) ShowWindow(existing, SW_RESTORE);
            SetForegroundWindow(existing);
            return;
        }
    }

    char user[64], obf[128], pass[64], cmd[MAX_PATH+150];
    GetWindowTextA(hCombo, user, 64); 
    if (strlen(user) == 0) return;

    WritePrivateProfileStringA("Settings", (hCombo == hCbMain ? "LastMain" : "LastAlt"), user, INI);
    GetPrivateProfileStringA(user, "Pass", "", obf, 128, INI); 
    Xcrypt(obf, pass, 1, user);

    STARTUPINFOA si = {sizeof(si)}; PROCESS_INFORMATION pi;
    sprintf(cmd, "\"%s\" %s", GAME_EXE, aoParams);
    
    if (CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, GAME_DIR, &si, &pi)) {
        SetInt(pidKey, (int)pi.dwProcessId);
        Sleep(waitTitle);
        
        HWND g = FindWindowA(NULL, "Anarchy Online");
        if(g) SetWindowTextA(g, label);
        
        keybd_event(VK_TAB, 0, 0, 0); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); 
        Sleep(waitTab);
        for(int i=0; i<15; i++) { 
            keybd_event(VK_DELETE, 0, 0, 0); 
            keybd_event(VK_DELETE, 0, KEYEVENTF_KEYUP, 0); 
            Sleep(typeDelay); 
        }
        
        SendText(user); 
        keybd_event(VK_TAB, 0, 0, 0); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); 
        Sleep(waitTab);
        
        SendText(pass); 
        keybd_event(VK_RETURN, 0, 0, 0); keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
        
        if (pressEnter) {
            Sleep(waitTitle);
            keybd_event(VK_RETURN, 0, 0, 0); keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
        }
        CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    }
}
