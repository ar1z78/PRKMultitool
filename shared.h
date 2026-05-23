#ifndef SHARED_H
#define SHARED_H

#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <stdio.h>
#include <string.h>

// Include GDI+ here so all .c files know what GpImage and ULONG_PTR are
#include <gdiplus/gdiplus.h>

// --- IDs ---
#define ID_BTN_GO1 101
#define ID_BTN_GO2 102
#define ID_BTN_BOTH 103
#define ID_CB_MAIN 201
#define ID_CB_ALT 202
#define ID_BASE_TOOL 300
#define ID_BTN_ARROW_MAIN 203
#define ID_BTN_ARROW_ALT  204

#define ID_MGR_CB 401
#define ID_MGR_PASS 402
#define ID_MGR_SAVE 403
#define ID_MGR_DEL 404
#define ID_MGR_STATUS 405

// IDs for picker dialogs
#define ID_SET_CB_SLOT 600
#define ID_SET_BTN_SAVE 601
#define ID_SET_BTN_BROWSE 602
#define ID_SET_BTN_CLEAR 603
#define ID_SET_BTN_FCLR 604
#define ID_SET_BTN_BCLR 605
#define ID_SET_BTN_TCLR 606
#define ID_SET_BTN_BGIMG 607
#define ID_SET_BTN_FONT 608

// --- Globals ---
extern HWND hCbMain, hCbAlt, hMainWnd;
extern HWND hMain, hGo1, hGo2, hBoth, hParams, hPress, hSlotCb, hTName, hTCmd;
extern HWND hWaitTitle, hWaitTab, hTypeDelay;
extern char INI[MAX_PATH], GAME_EXE[MAX_PATH], GAME_DIR[MAX_PATH], BG_PATH[MAX_PATH];
extern HFONT hCustomFont;
extern HBRUSH hBtnBrush, hNullBrush;
extern char keyGo1, keyGo2, keyBoth, keyTools[8];

// GDI+ Globals
extern ULONG_PTR gdiplusToken;
extern GpImage *gpBitmap;

// Settings variables
extern char go1Lbl[64], go2Lbl[64], bothLbl[64], aoParams[128], fontName[32];
extern int pressEnter, waitTitle, waitTab, typeDelay, fontSize;
extern COLORREF formColor, btnColor, txtColor;
// Move or add these handle definitions alongside your other global definitions
extern HWND hMgrCb, hMgrPass, hStatus;


// Add this alongside your other global function declarations
void LoadToolList();
// Add this declaration inside shared.h
void DirectSaveTool(int slot);

// --- Functions ---
void InitImageSystem();
void LoadBackgroundImage(const char* path);
void DrawBackgroundImage(HWND hwnd, HDC hdc);
void ShutdownImageSystem();

void InitDefaults();
void LoadAllSettings();
void LoadAccounts(HWND hTarget);
void UpdateAccountList(const char* user, int remove);

int GetInt(const char* k, int d);
void SetInt(const char* k, int v);
void Xcrypt(const char* in, char* out, int dec, const char* user);

HWND FindWindowByPID(DWORD pid);
BOOL IsProcessRunning(DWORD pid);
void SendText(const char* text);
void LaunchGame(HWND hCombo, const char* label, const char* pidKey);
void ShowAccountMenu(HWND hwnd, HWND hTarget, const char* settingsKey);
void FormatAccountDisplay(HWND hTarget, const char* name);
void SetStr(const char* k, HWND h, const char* sec);
void LoadToolData();
void LoadUIValues();
void SaveAllToINI();
char* SimpleFile(HWND hwnd, LPCSTR filter, char* path);
void DrawCustomBtn(LPDRAWITEMSTRUCT pDI);
// --- GUI Layer Functions ---
void CreateMainGUI(HWND hwnd);
void CreateSettingsGUI(HWND hwnd);
void CreateAccountsGUI(HWND hwnd);

#endif
