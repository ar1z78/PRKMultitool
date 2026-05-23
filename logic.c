#include "shared.h"

// Define globals
HWND hCbMain, hCbAlt, hMainWnd;
char INI[MAX_PATH], GAME_EXE[MAX_PATH], GAME_DIR[MAX_PATH], BG_PATH[MAX_PATH];
HBITMAP hBgBmp = NULL;
HFONT hCustomFont = NULL;
HBRUSH hBtnBrush = NULL, hNullBrush = NULL;
// Add these to the top of logic.c along with your other global variables:
HWND hMgrCb = NULL;
HWND hMgrPass = NULL;
HWND hStatus = NULL;
HWND hTName = NULL;
typedef struct { HWND hwnd; DWORD pid; } handle_data;

BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lParam) {
    handle_data* d = (handle_data*)lParam;
    DWORD pid = 0; GetWindowThreadProcessId(hwnd, &pid);
    if (pid == d->pid && GetWindow(hwnd, GW_OWNER) == NULL && IsWindowVisible(hwnd)) {
        d->hwnd = hwnd; return FALSE;
    }
    return TRUE;
}

HWND FindWindowByPID(DWORD pid) {
    if (pid == 0) return NULL;
    handle_data data = { NULL, pid };
    EnumWindows(EnumProc, (LPARAM)&data);
    return data.hwnd;
}

void LoadToolList() {
    if (!hSlotCb) return;
    // The combo box only holds static slot strings now, so no INI loading is needed here!
}


BOOL IsProcessRunning(DWORD pid) {
    if (pid == 0) return FALSE;
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (h) {
        DWORD exitCode;
        GetExitCodeProcess(h, &exitCode);
        CloseHandle(h);
        return (exitCode == STILL_ACTIVE);
    }
    return FALSE;
}

int GetInt(const char* k, int d) {
    char v[32]; GetPrivateProfileStringA("Settings", k, "", v, 32, INI);
    return (strlen(v) > 0) ? (int)strtol(v, NULL, 0) : d;
}

void SetInt(const char* k, int v) {
    char s[32]; sprintf(s, "%d", v);
    WritePrivateProfileStringA("Settings", k, s, INI);
}

void Xcrypt(const char* in, char* out, int dec, const char* user) {
    // Structural Guard: Check for the 5-byte prefix header "_ENC_"
    if (dec && strncmp(in, "_ENC_", 5) != 0) { strcpy(out, in); return; }
    if (!dec) { strcpy(out, "_ENC_"); out += 5; } else { in += 5; }

    const char* k = "AnarchyOnlineSalt";
    int kl = strlen(k), ul = (user && *user) ? strlen(user) : 1;
    int il = dec ? strlen(in)/2 : strlen(in);

    for (int i=0; i<il; i++) {
        unsigned char c;
        if (dec) { unsigned int h; sscanf(in+(i*2), "%02x", &h); c = (unsigned char)h; }
        else c = (unsigned char)in[i];

        // Unique Account Salting mixed right into your original compact math
        c ^= (k[i%kl] + i + (user ? user[i%ul] : 0));

        if (dec) out[i] = c; else sprintf(out+(i*2), "%02x", c);
    }
    out[dec?il:il*2] = '\0';
}

void SendText(const char* text) {
    int delay = GetInt("TypeDelay", 40);
    for (int i = 0; text[i]; i++) {
        SHORT vkI = VkKeyScan(text[i]);
        BYTE vk = LOBYTE(vkI), sh = HIBYTE(vkI);
        if (sh & 1) keybd_event(VK_SHIFT, 0, 0, 0);
        keybd_event(vk, 0, 0, 0); keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
        if (sh & 1) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        Sleep(delay);
    }
}

// The core helper: Handles the dialog and returns the filename part
char* SimpleFile(HWND hwnd, LPCSTR filter, char* path) {
    OPENFILENAMEA ofn = {sizeof(ofn), hwnd};
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = "."; 
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn)) {
        char* n = strrchr(path, '\\');
        return n ? n + 1 : path;
    }
    return NULL;
}

void DrawCustomBtn(LPDRAWITEMSTRUCT pDI) {
    HBRUSH hBr = CreateSolidBrush(pDI->itemState & ODS_SELECTED ? GetSysColor(COLOR_HIGHLIGHT) : btnColor);
    FillRect(pDI->hDC, &pDI->rcItem, hBr);
    
    // Draw 3D Edges (Sunken when pressed, Raised otherwise)
    DrawEdge(pDI->hDC, &pDI->rcItem, (pDI->itemState & ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT);

    SetTextColor(pDI->hDC, txtColor);
    SetBkMode(pDI->hDC, TRANSPARENT);
    
    char txt[128]; GetWindowTextA(pDI->hwndItem, txt, 128);
    HFONT hFont = CreateFontA(fontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName);
    HFONT hOld = (HFONT)SelectObject(pDI->hDC, hFont);

    // Offset text by 1 pixel if pressed for 3D effect
    RECT textRc = pDI->rcItem;
    if (pDI->itemState & ODS_SELECTED) OffsetRect(&textRc, 1, 1);
    
    DrawTextA(pDI->hDC, txt, -1, &textRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

    SelectObject(pDI->hDC, hOld);
    DeleteObject(hFont);
    DeleteObject(hBr);
}