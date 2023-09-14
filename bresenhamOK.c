/*

DUPLA:
SAMUEL ARANTES GONZALES 830662
MATHEUS SIMÕES PEIXEIRO 835733

*/


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>

#define Assert(c) do { if (!(c)) __debugbreak(); } while (0)

#pragma comment (lib, "user32.lib")
#pragma comment (lib, "gdi32.lib")

typedef uint32_t u32;

int ClientWidth;
int ClientHeight;
void *Memory;

void DrawPixel(int X, int Y, u32 Color) {
    u32 *Pixel = (u32 *)Memory;
    Pixel += Y * ClientWidth + X;
    *Pixel = Color;
}

void ClearScreen(u32 Color) {
    u32 *Pixel = (u32 *)Memory;
    for(int Index = 0; Index < ClientWidth * ClientHeight; ++Index) {
        *Pixel++ = Color;
    }
}

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch(Message) {
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        case WM_KEYDOWN: {
            switch(WParam) {
                case 'O': {
                    DestroyWindow(Window);
                } break;
            }
        } break;
        default: {
            return DefWindowProcW(Window, Message, WParam, LParam);
        }
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR Cmd, int CmdShow) {

    // Create a window.

    WNDCLASSW WindowClass = {
        .lpszClassName = L"MyWindowClass",
        .lpfnWndProc = WindowProc,
        .hInstance = Instance,
        .hCursor = LoadCursor(NULL, IDC_CROSS),
    };
    ATOM Atom = RegisterClassW(&WindowClass);
    Assert(Atom && "Falha ao registrar a janela grafica");

    HWND Window = CreateWindowW(WindowClass.lpszClassName, L"Desenhando pixels", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, Instance, NULL);
    Assert(Window && "Falha ao registrar a janela grafica");

    ShowWindow(Window, CmdShow);

    // Allocate memory.

    RECT Rect;
    GetClientRect(Window, &Rect);
    ClientWidth = Rect.right - Rect.left;
    ClientHeight = Rect.bottom - Rect.top;

    Memory = VirtualAlloc(0, ClientWidth * ClientHeight * 4, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    // Cria BITMAPINFO struct para usar em StretchDIBits().

    BITMAPINFO BitmapInfo;
    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = ClientWidth;
    // biHeight negativo colocado como sistema de coordenadas na origem. Caso contrario embaixo a esquerda.
    BitmapInfo.bmiHeader.biHeight = -ClientHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    HDC DeviceContext = GetDC(Window);

    for(;;) {
        MSG Message;
        if(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
            if(Message.message == WM_QUIT) break;
            TranslateMessage(&Message);
            DispatchMessage(&Message);
            continue;
        }

        // Limpa a tela

        ClearScreen(0x333333);

        // Desenha um pixel na posição 100,100 na cor branca

        //DrawPixel(100, 100, 0xffffff);

        int x1, y1, x2, y2;

        // Defina as coordenadas iniciais e finais da linha
        x1 = 100;
        y1 = 30;
        x2 = 30;
        y2 = 100;
        //inc_line(x1, y1, x2, y2,  0xffffff);
        drawLine(x1, y1, x2, y2,  0xffffff);

        // Mostra a imagem

        StretchDIBits(DeviceContext, 0, 0, ClientWidth, ClientHeight, 0, 0, ClientWidth, ClientHeight, Memory, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    }
    getc();
    return 0;
}

void drawLine(int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int err2;
    //DrawPixel(x, y, color);
    while (1) {
        // Desenhe o ponto atual (x1, y1) aqui

        if (x1 == x2 && y1 == y2) {
            break;
        }

        err2 = 2 * err;

        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
        DrawPixel(x1, y1, color);
    }
}



