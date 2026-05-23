#include "shared.h"

ULONG_PTR gdiplusToken;
GpImage *gpBitmap = NULL;

void InitImageSystem() {
    // In C, we must initialize the version member
    struct GdiplusStartupInput gsi;
    memset(&gsi, 0, sizeof(gsi));
    gsi.GdiplusVersion = 1; 
    
    GdiplusStartup(&gdiplusToken, &gsi, NULL);
}

void LoadBackgroundImage(const char* path) {
    if (gpBitmap) {
        GdipDisposeImage(gpBitmap);
        gpBitmap = NULL;
    }
    if (path && strlen(path) > 0) {
        WCHAR wPath[MAX_PATH];
        MultiByteToWideChar(CP_ACP, 0, path, -1, wPath, MAX_PATH);
        GdipLoadImageFromFile(wPath, &gpBitmap);
    }
}

void DrawBackgroundImage(HWND hwnd, HDC hdc) {
    if (gpBitmap) {
        RECT r;
        GetClientRect(hwnd, &r);
        GpGraphics *graphics;
        GdipCreateFromHDC(hdc, &graphics);
        // Using GdipDrawImageRectI for the flat C API
        GdipDrawImageRectI(graphics, gpBitmap, 0, 0, r.right, r.bottom);
        GdipDeleteGraphics(graphics);
    }
}

void ShutdownImageSystem() {
    if (gpBitmap) GdipDisposeImage(gpBitmap);
    GdiplusShutdown(gdiplusToken);
}
