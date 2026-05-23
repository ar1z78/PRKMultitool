#include "shared.h"

// Define the globals
char go1Lbl[64], go2Lbl[64], bothLbl[64], aoParams[128], fontName[32];
int pressEnter, waitTitle, waitTab, typeDelay, fontSize;
COLORREF formColor, btnColor, txtColor;
char keyGo1, keyGo2, keyBoth, keyTools[8];

// Helper to write defaults if they don't exist
void DEF_SET(const char* k, const char* v) {
    char buf[MAX_PATH];
    if (GetPrivateProfileStringA("Settings", k, "", buf, MAX_PATH, INI) == 0)
        WritePrivateProfileStringA("Settings", k, v, INI);
}

void InitDefaults() {
    DEF_SET("Go1Label", "Go 1");
    DEF_SET("Go2Label", "Go 2");
    DEF_SET("BothLabel", "Launch Both");
    DEF_SET("PressEnter", "1");
    DEF_SET("AoParams", "IA 2642997703 IP 7000 UI");
    DEF_SET("WaitTitle", "5000");
    DEF_SET("WaitTab", "300");
    DEF_SET("TypeDelay", "40");
    DEF_SET("FontSize", "18");
    DEF_SET("FontName", "Segoe UI");
    DEF_SET("FormColor", "0xF0F0F0");
    DEF_SET("BtnColor", "0xE1E1E1");
    DEF_SET("TxtColor", "0x000000");
	DEF_SET("KeyGo1", "1");
    DEF_SET("KeyGo2", "2");
    DEF_SET("KeyBoth", "0");
    DEF_SET("KeyTool0", "a");
    DEF_SET("KeyTool1", "s");
    DEF_SET("KeyTool2", "3");
    DEF_SET("KeyTool3", "4");
    DEF_SET("KeyTool4", "5");
    DEF_SET("KeyTool5", "6");
    DEF_SET("KeyTool6", "7");
    DEF_SET("KeyTool7", "8");
}

void LoadAllSettings() {
    GetPrivateProfileStringA("Settings", "Go1Label", "Go 1", go1Lbl, 64, INI);
    GetPrivateProfileStringA("Settings", "Go2Label", "Go 2", go2Lbl, 64, INI);
    GetPrivateProfileStringA("Settings", "BothLabel", "Launch Both", bothLbl, 64, INI);
    GetPrivateProfileStringA("Settings", "AoParams", "IA 2642997703 IP 7000 UI", aoParams, 128, INI);
    GetPrivateProfileStringA("Settings", "FontName", "Segoe UI", fontName, 32, INI);

    pressEnter = GetInt("PressEnter", 1);
    waitTitle = GetInt("WaitTitle", 5000);
    waitTab = GetInt("WaitTab", 300);
    typeDelay = GetInt("TypeDelay", 40);
    fontSize = GetInt("FontSize", 18);
    
    formColor = (COLORREF)GetInt("FormColor", 0xF0F0F0);
    btnColor = (COLORREF)GetInt("BtnColor", 0xE1E1E1);
    txtColor = (COLORREF)GetInt("TxtColor", 0x000000);
	
	char buf[8];
    GetPrivateProfileStringA("Settings", "KeyGo1", "1", buf, 8, INI);   keyGo1 = buf[0];
    GetPrivateProfileStringA("Settings", "KeyGo2", "2", buf, 8, INI);   keyGo2 = buf[0];
    GetPrivateProfileStringA("Settings", "KeyBoth", "0", buf, 8, INI);  keyBoth = buf[0];
    GetPrivateProfileStringA("Settings", "KeyTool0", "a", buf, 8, INI); keyTools[0] = buf[0];
    GetPrivateProfileStringA("Settings", "KeyTool1", "s", buf, 8, INI); keyTools[1] = buf[0];
    
    for(int i = 2; i < 8; i++) {
        char k[32], d[2];
        sprintf(k, "KeyTool%d", i);
        sprintf(d, "%d", i + 1); // 3, 4, 5, 6, 7, 8
        GetPrivateProfileStringA("Settings", k, d, buf, 8, INI);
        keyTools[i] = buf[0];
	}
	    // --- AUTOMATIC COMPANION TOOL AUTO-BINDING ---
    char checkCmd[MAX_PATH];
    
    // If Tool 0 (Slot 0) has no saved path, map it to accounts.exe
    GetPrivateProfileStringA("Tools", "T0_Cmd", "", checkCmd, MAX_PATH, INI);
    if (strlen(checkCmd) == 0) {
        WritePrivateProfileStringA("Tools", "T0_Cmd", "accounts.exe", INI);
        WritePrivateProfileStringA("Tools", "T0_Name", "Accounts Manager", INI);
    }

    // If Tool 1 (Slot 1) has no saved path, map it to settings.exe
    GetPrivateProfileStringA("Tools", "T1_Cmd", "", checkCmd, MAX_PATH, INI);
    if (strlen(checkCmd) == 0) {
        WritePrivateProfileStringA("Tools", "T1_Cmd", "settings.exe", INI);
        WritePrivateProfileStringA("Tools", "T1_Name", "Settings Manager", INI);
    }
}

void LoadAccounts(HWND hTarget) {
    if (!hTarget) return;
    char list[1024];
    SendMessage(hTarget, CB_RESETCONTENT, 0, 0);
    GetPrivateProfileStringA("Settings", "AccountList", "", list, sizeof(list), INI);
    char* t = strtok(list, ",");
    while(t) {
        SendMessage(hTarget, CB_ADDSTRING, 0, (LPARAM)t);
        t = strtok(NULL, ",");
    }
}

void UpdateAccountList(const char* user, int remove) {
    char list[1024] = {0}, newList[1024] = {0};
    GetPrivateProfileStringA("Settings", "AccountList", "", list, 1024, INI);
    char* t = strtok(list, ",");
    while(t) {
        if (strcmp(t, user) != 0) {
            strcat(newList, t); strcat(newList, ",");
        }
        t = strtok(NULL, ",");
    }
    if (!remove) { strcat(newList, user); strcat(newList, ","); }
    WritePrivateProfileStringA("Settings", "AccountList", newList, INI);
}

void ShowAccountMenu(HWND hwnd, HWND hTarget, const char* settingsKey) {
    HMENU hMenu = CreatePopupMenu();
    char list[1024];
    GetPrivateProfileStringA("Settings", "AccountList", "", list, sizeof(list), INI);
    
    char* t = strtok(list, ",");
    int i = 1;
    while(t) {
        AppendMenuA(hMenu, MF_STRING, 2000 + i, t);
        t = strtok(NULL, ",");
        i++;
    }

    POINT pt; GetCursorPos(&pt);
    int sel = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL);
    
    if (sel > 2000) {
        MENUITEMINFOA mii = { sizeof(mii), MIIM_STRING };
        char result[64];
        mii.dwTypeData = result; mii.cch = 64;
        GetMenuItemInfoA(hMenu, sel, FALSE, &mii);
        
        SetWindowTextA(hTarget, result); // No 'v' added here
        WritePrivateProfileStringA("Settings", settingsKey, result, INI);

        // Ghosting Fix
        RECT rc; GetWindowRect(hTarget, &rc);
        MapWindowPoints(NULL, hwnd, (LPPOINT)&rc, 2);
        ShowWindow(hTarget, SW_HIDE);
        InvalidateRect(hwnd, &rc, TRUE);
        UpdateWindow(hwnd);
        ShowWindow(hTarget, SW_SHOW);
    }
    DestroyMenu(hMenu);
}

void SetStr(const char* k, HWND h, const char* sec) { 
    char b[512]; 
    GetWindowTextA(h, b, 512); 
    WritePrivateProfileStringA(sec, k, b, INI); 
}
void LoadToolData() {
    int idx = SendMessageA(hSlotCb, CB_GETCURSEL, 0, 0);
    if (idx < 0 || idx > 7) return;

    char name[64], cmd[MAX_PATH], kn[32], kc[32];
    sprintf(kn, "T%d_Name", idx); sprintf(kc, "T%d_Cmd", idx);
    GetPrivateProfileStringA("Tools", kn, "Empty", name, 64, INI);
    GetPrivateProfileStringA("Tools", kc, "", cmd, MAX_PATH, INI);
    
    SetWindowTextA(hTName, name);
    SetWindowTextA(hTCmd, cmd);
}

// Helper to save a single tool slot frame directly from the text boxes to disk
void DirectSaveTool(int slot) {
    if (slot < 0 || slot > 7) return;
    char name[64], cmd[MAX_PATH], kn[32], kc[32];
    GetWindowTextA(hTName, name, 64);
    GetWindowTextA(hTCmd, cmd, MAX_PATH);
    
    sprintf(kn, "T%d_Name", slot); sprintf(kc, "T%d_Cmd", slot);
    WritePrivateProfileStringA("Tools", kn, name, INI);
    WritePrivateProfileStringA("Tools", kc, cmd, INI);
}


void LoadUIValues() {
    SetWindowTextA(hGo1, go1Lbl); SetWindowTextA(hGo2, go2Lbl);
    SetWindowTextA(hBoth, bothLbl); SetWindowTextA(hParams, aoParams);
    char b[32];
    sprintf(b, "%d", waitTitle); SetWindowTextA(hWaitTitle, b);
    sprintf(b, "%d", waitTab); SetWindowTextA(hWaitTab, b);
    sprintf(b, "%d", typeDelay); SetWindowTextA(hTypeDelay, b);
    SendMessage(hPress, BM_SETCHECK, pressEnter ? BST_CHECKED : BST_UNCHECKED, 0);
    LoadToolData();
}

void SaveAllToINI() {
    SetStr("Go1Label", hGo1, "Settings");
    SetStr("Go2Label", hGo2, "Settings");
    SetStr("BothLabel", hBoth, "Settings");
    SetStr("AoParams", hParams, "Settings");
    SetStr("WaitTitle", hWaitTitle, "Settings");
    SetStr("WaitTab", hWaitTab, "Settings");
    SetStr("TypeDelay", hTypeDelay, "Settings");
    
    // Save Checkbox
    WritePrivateProfileStringA("Settings", "PressEnter", 
        (SendMessage(hPress, BM_GETCHECK, 0, 0) == BST_CHECKED ? "1" : "0"), INI);
    
    // Save the currently visible/edited text block to the selected index first
    int currentIdx = SendMessage(hSlotCb, CB_GETCURSEL, 0, 0);
    if (currentIdx >= 0 && currentIdx < 8) {
        char editedName[64], editedCmd[MAX_PATH];
        GetWindowTextA(hTName, editedName, 64);
        GetWindowTextA(hTCmd, editedCmd, MAX_PATH);
        
        char kn[32], kc[32];
        sprintf(kn, "T%d_Name", currentIdx);
        sprintf(kc, "T%d_Cmd", currentIdx);
        WritePrivateProfileStringA("Tools", kn, editedName, INI);
        WritePrivateProfileStringA("Tools", kc, editedCmd, INI);
    }
}
