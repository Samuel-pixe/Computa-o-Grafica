#define _WIN32_WINNT 0x0600
#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void CirclePoints(int x, int y, int color);
void MidPointCircle(int r, int color);

int main() {
    // Criar uma janela de console
    HWND meu_console = GetConsoleWindow();
    HDC meu_dc = GetDC(meu_console);
    COLORREF COR = RGB(255, 255, 0);   /* cor amarela */

    MidPointCircle(100, COR);

    getch();
    return 0;
}

void CirclePoints(int x, int y, int color) {
    HDC hdc = GetDC(GetConsoleWindow());

    SetPixel(hdc, x + 200, y + 200, color);
    SetPixel(hdc, x + 200, -y + 200, color);
    SetPixel(hdc, -x + 200, y + 200, color);
    SetPixel(hdc, -x + 200, -y + 200, color);
    SetPixel(hdc, y + 200, x + 200, color);
    SetPixel(hdc, y + 200, -x + 200, color);
    SetPixel(hdc, -y + 200, x + 200, color);
    SetPixel(hdc, -y + 200, -x + 200, color);

    ReleaseDC(GetConsoleWindow(), hdc);
}

void MidPointCircle(int r, int color) {
    int x, y;
    float d;

    x = 0;
    y = r;
    d = 5 / 4 - r;
    CirclePoints(x, y, color);

    while (x <= y) {
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        CirclePoints(x, y, color);
    }
}


