#include <windows.h>
#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")
#include <math.h>
//#include <time.h>
#define M_PI 3.14159265358979323846264338327950288
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE Reserved;
    };
}_RGBQUAD, * PRGBQUAD;

typedef struct {
    FLOAT h, s, l;
} HSL;

namespace Colors {
    HSL rgb2hsl(RGBQUAD rgb) {
        HSL hsl;
        BYTE r = rgb.rgbRed;
        BYTE g = rgb.rgbGreen;
        BYTE b = rgb.rgbBlue;
        FLOAT _r = (FLOAT)r / 255.f;
        FLOAT _g = (FLOAT)g / 255.f;
        FLOAT _b = (FLOAT)b / 255.f;
        FLOAT rgbMin = min(min(_r, _g), _b);
        FLOAT rgbMax = max(max(_r, _g), _b);
        FLOAT fDelta = rgbMax - rgbMin;
        FLOAT deltaR;
        FLOAT deltaG;
        FLOAT deltaB;
        FLOAT h = 0.f;
        FLOAT s = 0.f;
        FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);
        if (fDelta != 0.f) {
            s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
            deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
            deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
            deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);
            if (_r == rgbMax)      h = deltaB - deltaG;
            else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
            else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
            if (h < 0.f)           h += 1.f;
            if (h > 1.f)           h -= 1.f;
        }
        hsl.h = h; hsl.s = s; hsl.l = l;
        return hsl;
    }

    RGBQUAD hsl2rgb(HSL hsl) {
        RGBQUAD rgb;
        FLOAT r = hsl.l;
        FLOAT g = hsl.l;
        FLOAT b = hsl.l;
        FLOAT h = hsl.h;
        FLOAT sl = hsl.s;
        FLOAT l = hsl.l;
        FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);
        FLOAT m;
        FLOAT sv;
        FLOAT fract;
        FLOAT vsf;
        FLOAT mid1;
        FLOAT mid2;
        INT sextant;
        if (v > 0.f) {
            m = l + l - v;
            sv = (v - m) / v;
            h *= 6.f;
            sextant = (INT)h;
            fract = h - sextant;
            vsf = v * sv * fract;
            mid1 = m + vsf;
            mid2 = v - vsf;
            switch (sextant) {
            case 0:
                r = v; g = mid1; b = m;
                break;
            case 1:
                r = mid2; g = v; b = m;
                break;
            case 2:
                r = m; g = v; b = mid1;
                break;
            case 3:
                r = m; g = mid2; b = v;
                break;
            case 4:
                r = mid1; g = m; b = v;
                break;
            case 5:
                r = v; g = m; b = mid2;
                break;
            }
        }
        rgb.rgbRed = (BYTE)(r * 255.f);
        rgb.rgbGreen = (BYTE)(g * 255.f);
        rgb.rgbBlue = (BYTE)(b * 255.f);
        return rgb;
    }
}

int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) {
    if (red != length) {
        red < length; red++;
        if (ifblue == true) {
            return RGB(red, 0, length);
        }
        else {
            return RGB(red, 0, 0);
        }
    }
    else {
        if (green != length) {
            green < length; green++;
            return RGB(length, green, 0);
        }
        else {
            if (blue != length) {
                blue < length; blue++;
                return RGB(0, length, blue);
            }
            else {
                red = 0; green = 0; blue = 0;
                ifblue = true;
            }
        }
    }
}

COLORREF RndRGB() {
    int clr = rand() % 5;
    if (clr == 0) return RGB(255, 0, 0);
    if (clr == 1) return RGB(0, 255, 0);
    if (clr == 2) return RGB(0, 0, 255);
    if (clr == 3) return RGB(255, 0, 255);
    if (clr == 4) return RGB(255, 255, 0);
}

LPCWSTR string() {
    srand(GetTickCount());
    WCHAR string[32];
    for (int i = 0; i < 32; i++) {
        string[i] = (WCHAR)(rand() % 256) + 1;
    }
    return string;
}

VOID WINAPI ci(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}
DWORD WINAPI swirl(LPVOID lpParam) {

    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, 0, 0, w, h, hdc, -20, 0, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, w - 20, 0, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, -0, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 0, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, -20, 0, 0x812345);
        BitBlt(hdc, 0, 0, w, h, hdc, w - 20, 0, 0x812345);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, -0, 0x812345);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 0, 0x812345);
        ReleaseDC(0, hdc);
        //Sleep(10);
    }
}
DWORD WINAPI shader(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    HWND wnd = GetDesktopWindow();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmp = { 40, w, h, 1, 24 };
    PRGBTRIPLE rgbtriple;
    while (true) {
        hdc = GetDC(0);
        HDC mdc = CreateCompatibleDC(hdc);
        HBITMAP hbit = CreateDIBSection(hdc, &bmp, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(mdc, hbit);
        BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            rgbtriple[i].rgbtRed += 10;
            rgbtriple[i].rgbtGreen += 10;
        }
        BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
        ReleaseDC(wnd, hdc);
        DeleteDC(hdc); DeleteDC(mdc); DeleteObject(hbit); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&w); DeleteObject(&h); DeleteObject(&bmp);
    }
}
DWORD WINAPI RGB2(LPVOID lpParam)
{

    HWND v3;
    HBITMAP h;
    HDC hdcSrc;
    HDC hdc;
    void* lpvBits;
    int nHeight;
    int nWidth;
    DWORD v12;
    int j;
    int v14;
    int i;
    v12 = GetTickCount();
    nWidth = GetSystemMetrics(0);
    nHeight = GetSystemMetrics(1);
    lpvBits = VirtualAlloc(0, 4 * nWidth * (nHeight + 1), 0x3000u, 4u);
    for (i = 0; ; i = (i + 1) % 2)
    {
        hdc = GetDC(0);
        hdcSrc = CreateCompatibleDC(hdc);
        h = CreateBitmap(nWidth, nHeight, 1u, 0x20u, lpvBits);
        SelectObject(hdcSrc, h);
        BitBlt(hdcSrc, 0, 0, nWidth, nHeight, hdc, 0, 0, 0xCC0020u);
        GetBitmapBits(h, 4 * nHeight * nWidth, lpvBits);
        v14 = 0;
        if (GetTickCount() - v12 > 0xA)
            rand();
        for (j = 0; nHeight * nWidth > j; ++j)
        {
            if (!(j % nHeight) && !(rand() % 110))
                v14 = rand() % 24;
            *((BYTE*)lpvBits + 4 * j + v14) -= 5;
        }
        SetBitmapBits(h, 4 * nHeight * nWidth, lpvBits);
        BitBlt(hdc, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, 0xCC0020u);
        DeleteObject(h);
        DeleteObject(hdcSrc);
        DeleteObject(hdc);
    }
}
DWORD WINAPI text(LPVOID lpParam)
{
    HDC hdc;
    int sx = 0, sy = 0;
    LPCWSTR lpText = L"Heif.exe";
    while (1)
    {
        hdc = GetWindowDC(GetDesktopWindow());
        sx = GetSystemMetrics(0);
        sy = GetSystemMetrics(1);
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        TextOutW(hdc, rand() % sx, rand() % sy, lpText, wcslen(lpText));
    }
}

DWORD WINAPI train(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, 0, 0, w, h, hdc, -20, 0, SRCERASE);
        BitBlt(hdc, 0, 0, w, h, hdc, w - 20, 0, SRCERASE);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, -1, SRCERASE);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 1, SRCERASE);
        ReleaseDC(0, hdc);
        //Sleep(10);
    }
}
DWORD WINAPI bitblt(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(0);
        int x = SM_CXSCREEN;
        int y = SM_CYSCREEN;
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCAND);
        Sleep(10);
    }
}
DWORD WINAPI waves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    double angle = 0.0;
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);

        for (int i = 0; i < x + y; i++) {
            BitBlt(hdc, 0, i, x, 1, mdc, sin(angle) * 40, i, SRCCOPY);

            angle += rand() % 5 / 4;
        }

        Sleep(10);
    }
}
DWORD WINAPI elipse(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        Pie(hdc, rand() % x, rand() % y, rand() % x, rand() % y, rand() % x, rand() % y, rand() % x, rand() % y);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
}

DWORD WINAPI patblt(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN),
            h = GetSystemMetrics(SM_CYSCREEN);

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
    }
}
DWORD WINAPI cubes(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        SetStretchBltMode(hdc, HALFTONE);
        StretchBlt(hdc, -10, -10, x + 20, y + 20, hdc, 0, 0, x, y, SRCCOPY);
        StretchBlt(hdc, 10, 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI textout(LPVOID lpParam)
{
    HDC hdc;
    int sx = 0, sy = 0;
    LPCWSTR lpText = L"Heif.exe";
    while (1)
    {
        hdc = GetWindowDC(GetDesktopWindow());
        sx = GetSystemMetrics(0);
        sy = GetSystemMetrics(1);
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        TextOutW(hdc, rand() % sx, rand() % sy, lpText, wcslen(lpText));
    }
}
DWORD WINAPI icons(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = rand() % GetSystemMetrics(SM_CXSCREEN);
        int y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hdc, x, y, LoadIcon(0, IDI_ERROR));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hdc, x, y, LoadIcon(0, IDI_WARNING));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hdc, x, y, LoadIcon(0, IDI_APPLICATION));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hdc, x, y, LoadIcon(0, IDI_QUESTION));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hdc, x, y, LoadIcon(0, IDI_INFORMATION));
        ReleaseDC(0, hdc);
    }
}
VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t & t % 255) - (t * 3 & t >> 13 & t >> 6);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 9 ^ (t >> 9) - 1 ^ 1) % 13 * t;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(64 * (t >> 3 | t >> 4 | t >> 9) + (t >> 11 & t << 2) ^ 2 * (t >> 16 | t | t >> 7) + 32 * (t >> t & 32) | t >> 7);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t | t % 255 | t % 257);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 5 | t << 4 | t & 1023 ^ 1981 | t - 67 >> 4);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + (1 & t >> 14)) >> (3 & t >> (t & 2048 ? 2 : 10)) | t >> (t & 16384 ? t & 4096 ? 10 : 3 : 2) + t & t >> 8 + t ^ t >> 2 + t & t >> 23 + t | t >> 26);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int main() {

    if (MessageBoxW(NULL, L"Do you want to run GDI malware?", L"Heif.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)

        ExitProcess(0);

    else
    {
        if (MessageBoxW(NULL, L"Are you sure you want it?", L"Heif.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        {
            ExitProcess(0);
        }

        else {
            sound1();
            Sleep(5000);
            HANDLE thread2 = CreateThread(0, 0, shader, 0, 0, 0);
            HANDLE thread2dot = CreateThread(0, 0, swirl, 0, 0, 0);
            Sleep(25000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            TerminateThread(thread2dot, 0);
            CloseHandle(thread2dot);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread3 = CreateThread(0, 0, RGB2, 0, 0, 0);
            HANDLE thread3dot = CreateThread(0, 0, text, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            TerminateThread(thread3dot, 0);
            CloseHandle(thread3dot);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread4 = CreateThread(0, 0, train, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread5 = CreateThread(0, 0, bitblt, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread6 = CreateThread(0, 0, elipse, 0, 0, 0);
            HANDLE thread6dot = CreateThread(0, 0, waves, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            TerminateThread(thread6dot, 0);
            CloseHandle(thread6dot);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE thread7 = CreateThread(0, 0, patblt, 0, 0, 0);
            HANDLE thread7dot = CreateThread(0, 0, cubes, 0, 0, 0);
            HANDLE thread8 = CreateThread(0, 0, textout, 0, 0, 0);
            HANDLE thread8dot = CreateThread(0, 0, icons, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            TerminateThread(thread7dot, 0);
            CloseHandle(thread7dot);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            TerminateThread(thread8dot, 0);
            CloseHandle(thread8dot);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            Sleep(30000);
            Sleep(-1);


        }
    }
}

