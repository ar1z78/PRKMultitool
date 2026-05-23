#include "shared.h"

// Helper function to apply custom font to a control safely
static void ApplyControlFont(HWND hCtrl) {
    if (hCtrl && hCustomFont) {
        SendMessageA(hCtrl, WM_SETFONT, (WPARAM)hCustomFont, TRUE);
    }
}

// GUI for main launcher window (from launcher.c)
void CreateMainGUI(HWND hwnd) {
    char lastM[64], lastA[64];
    hMainWnd = hwnd;

    // Initialize logic: Load strings
    GetPrivateProfileStringA("Settings", "LastMain", " ", lastM, 64, INI);
    GetPrivateProfileStringA("Settings", "LastAlt", " ", lastA, 64, INI);

    hCustomFont = CreateFontA(fontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName);
    hBtnBrush = CreateSolidBrush(btnColor);
    hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

    // Labels
    CreateWindowA("STATIC", "Primary:", WS_VISIBLE | WS_CHILD, 10, 20, 60, 20, hwnd, 0, 0, 0);

    // Account Name
    hCbMain = CreateWindowA("STATIC", lastM, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE | WS_BORDER, 75, 18, 125, 25, hwnd, (HMENU)ID_CB_MAIN, 0, 0);
    // Arrow Button
    HWND a1 = CreateWindowA("STATIC", "v", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE | WS_BORDER, 200, 18, 25, 25, hwnd, (HMENU)ID_BTN_ARROW_MAIN, 0, 0);

    // Go 1
    HWND b1 = CreateWindowA("STATIC", go1Lbl, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE | WS_BORDER, 235, 15, 80, 25, hwnd, (HMENU)ID_BTN_GO1, 0, 0);

    CreateWindowA("STATIC", "Alt:", WS_VISIBLE | WS_CHILD, 10, 60, 60, 20, hwnd, 0, 0, 0);

    // Account Name
    hCbAlt = CreateWindowA("STATIC", lastA, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE | WS_BORDER, 75, 58, 125, 25, hwnd, (HMENU)ID_CB_ALT, 0, 0);
    // Arrow Button
    HWND a2 = CreateWindowA("STATIC", "v", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE | WS_BORDER, 200, 58, 25, 25, hwnd, (HMENU)ID_BTN_ARROW_ALT, 0, 0);

    // Go 2
    HWND b2 = CreateWindowA("STATIC", go2Lbl, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE | WS_BORDER, 235, 55, 80, 25, hwnd, (HMENU)ID_BTN_GO2, 0, 0);

    // Launch Both
    HWND bB = CreateWindowA("STATIC", bothLbl, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE | WS_BORDER, 10, 100, 305, 35, hwnd, (HMENU)ID_BTN_BOTH, 0, 0);

    // Tools
    for (int i = 0; i < 8; i++) {
        char n[64], k[32]; 
        sprintf(k, "T%d_Name", i);
        GetPrivateProfileStringA("Tools", k, "Empty", n, 64, INI);
        HWND tb = CreateWindowA("STATIC", n, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE | WS_BORDER, 10 + (i % 2) * 165, 145 + (i / 2) * 35, 150, 30, hwnd, (HMENU)(UINT_PTR)(ID_BASE_TOOL + i), 0, 0);
        ApplyControlFont(tb);
    }

    // Apply fonts to remaining layout fields
    ApplyControlFont(hCbMain);
    ApplyControlFont(hCbAlt);
    ApplyControlFont(a1);
    ApplyControlFont(a2);
    ApplyControlFont(b1);
    ApplyControlFont(b2);
    ApplyControlFont(bB);
}

// GUI for settings manager window (from settings.c)
void CreateSettingsGUI(HWND hwnd) {
    hMain = hwnd; // Assign standard handle link

    int y = 10;
    CreateWindowA("STATIC", "Button Names", WS_VISIBLE | WS_CHILD, 10, y, 110, 20, hwnd, 0, 0, 0);
    CreateWindowA("STATIC", "Delays", WS_VISIBLE | WS_CHILD, 220, y, 80, 20, hwnd, 0, 0, 0); y += 20;

    hGo1 = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, y, 110, 25, hwnd, 0, 0, 0);
    CreateWindowA("STATIC", "Title:", WS_VISIBLE | WS_CHILD, 160, y + 2, 60, 20, hwnd, 0, 0, 0);
    hWaitTitle = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, y, 80, 25, hwnd, 0, 0, 0); y += 30;

    hGo2 = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, y, 110, 25, hwnd, 0, 0, 0);
    CreateWindowA("STATIC", "Tab:", WS_VISIBLE | WS_CHILD, 160, y + 2, 60, 20, hwnd, 0, 0, 0);
    hWaitTab = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, y, 80, 25, hwnd, 0, 0, 0); y += 30;

    hBoth = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, y, 110, 25, hwnd, 0, 0, 0);
    CreateWindowA("STATIC", "Typing:", WS_VISIBLE | WS_CHILD, 160, y + 2, 60, 20, hwnd, 0, 0, 0);
    hTypeDelay = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, y, 80, 25, hwnd, 0, 0, 0); y += 35;

    CreateWindowA("STATIC", "PRK Params:", WS_VISIBLE | WS_CHILD, 10, y + 2, 100, 20, hwnd, 0, 0, 0);
    hParams = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 200, 25, hwnd, 0, 0, 0); y += 30;
    hPress = CreateWindowA("BUTTON", "Press Enter after login", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, y, 200, 25, hwnd, 0, 0, 0); y += 35;

    CreateWindowA("STATIC", "--- Tools Config ---", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, y, 300, 20, hwnd, 0, 0, 0); y += 20;
    
    // 1. Shrunk Slot ComboBox (Now a stable dropdown list for numbers only)
    hSlotCb = CreateWindowA("COMBOBOX", "", WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST, 10, y, 70, 200, hwnd, (HMENU)ID_SET_CB_SLOT, 0, 0);
    
    // 2. New Dedicated Tool Name Edit Box (Sits to the right of the ComboBox)
    hTName = CreateWindowA("EDIT", "", WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL, 85, y, 145, 24, hwnd, 0, 0, 0);
    
    // 3. Clear Button
    CreateWindowA("BUTTON", "Clear", WS_VISIBLE|WS_CHILD|BS_OWNERDRAW, 235, y-1, 65, 26, hwnd, (HMENU)ID_SET_BTN_CLEAR, 0, 0); y += 30;
    
    // 4. Executable Command Path Edit Box & Browse Button
    hTCmd = CreateWindowA("EDIT", "", WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL, 10, y, 220, 25, hwnd, 0, 0, 0);
    CreateWindowA("BUTTON", "Browse", WS_VISIBLE|WS_CHILD|BS_OWNERDRAW, 235, y-1, 65, 26, hwnd, (HMENU)ID_SET_BTN_BROWSE, 0, 0); y += 40;

    // Populate the dropdown with simple Slot strings
    for (int i = 1; i <= 8; i++) {
        char slotNum[16];
        sprintf(slotNum, "Slot %d", i);
        SendMessageA(hSlotCb, CB_ADDSTRING, 0, (LPARAM)slotNum);
    }
    SendMessageA(hSlotCb, CB_SETCURSEL, 0, 0);

    CreateWindowA("BUTTON", "Window Color", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 10, y, 145, 30, hwnd, (HMENU)ID_SET_BTN_FCLR, 0, 0);
    CreateWindowA("BUTTON", "Button Color", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 160, y, 145, 30, hwnd, (HMENU)ID_SET_BTN_BCLR, 0, 0); y += 34;
    CreateWindowA("BUTTON", "Font Settings", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 10, y, 145, 30, hwnd, (HMENU)ID_SET_BTN_FONT, 0, 0);
    CreateWindowA("BUTTON", "BackGrd Image", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 160, y, 145, 30, hwnd, (HMENU)ID_SET_BTN_BGIMG, 0, 0); y += 34;
    CreateWindowA("BUTTON", "SAVE SETTINGS", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 10, y, 295, 34, hwnd, (HMENU)ID_SET_BTN_SAVE, 0, 0);

    LoadToolList();
    LoadUIValues();
}

// GUI for account credentials manager window (from accounts.c)
void CreateAccountsGUI(HWND hwnd) {
    // Create global typography configuration
    hCustomFont = CreateFontA(fontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName);

    // Labels
    HWND lbl1 = CreateWindowA("STATIC", "Account:", WS_VISIBLE | WS_CHILD, 10, 22, 70, 20, hwnd, 0, 0, 0);
    HWND lbl2 = CreateWindowA("STATIC", "Password:", WS_VISIBLE | WS_CHILD, 10, 62, 70, 20, hwnd, 0, 0, 0);

    hMgrCb = CreateWindowA("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_AUTOHSCROLL, 80, 20, 150, 200, hwnd, (HMENU)ID_MGR_CB, 0, 0);
    hMgrPass = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 80, 60, 150, 20, hwnd, (HMENU)ID_MGR_PASS, 0, 0);

    // Buttons
    CreateWindowA("BUTTON", "Save", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 10, 100, 115, 30, hwnd, (HMENU)ID_MGR_SAVE, 0, 0);
    CreateWindowA("BUTTON", "Delete", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 135, 100, 115, 30, hwnd, (HMENU)ID_MGR_DEL, 0, 0);

    hStatus = CreateWindowA("STATIC", "Ready", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 140, 240, 20, hwnd, (HMENU)ID_MGR_STATUS, 0, 0);

    // Standard control structural typography pass down mappings
    ApplyControlFont(lbl1);
    ApplyControlFont(lbl2);
    ApplyControlFont(hStatus);
    ApplyControlFont(hMgrCb);
    ApplyControlFont(hMgrPass);

    LoadAccounts(hMgrCb);
}
