/*

DUPLA:
SAMUEL ARANTES GONZALES 830662
MATHEUS SIMÕES PEIXEIRO 835733

*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include<unistd.h>

#define Assert(c) do { if (!(c)) __debugbreak(); } while (0)

#pragma comment (lib, "user32.lib")
#pragma comment (lib, "gdi32.lib")

typedef uint32_t u32;

int ClientWidth;
int ClientHeight;
void *Memory;

typedef struct {
    int x;
    int y;
} Point;

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
        Point vertices[] = {
            {100, 100},   // Canto superior esquerdo
            {200, 100},   // Canto superior direito
            {200, 200},   // Canto inferior direito
            {100, 200},    // Canto inferior esquerdo

        };


        FillPolygon(vertices, sizeof(vertices) / sizeof(vertices[0]), 0x23AF21);//0xff00ff


        //Imagem
        StretchDIBits(DeviceContext, 0, 0, ClientWidth, ClientHeight, 0, 0, ClientWidth, ClientHeight, Memory, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    }
    getc();
    return 0;
}

void FillPolygon(Point* vertices, int numVertices, u32 fillColor) {
    // Verifique se há pelo menos três vértices para formar um polígono.
    if (numVertices < 3) {
        return;
    }

    // Determine a caixa delimitadora do polígono.
    int minX = vertices[0].x;
    int minY = vertices[0].y;
    int maxX = vertices[0].x;
    int maxY = vertices[0].y;

    for (int i = 1; i < numVertices; ++i) {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
    }
    int frames = 0;
    // Preenche o polígono varrendo cada linha horizontal.
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            int intersections = 0;
            for (int i = 0; i < numVertices; ++i) {
                int x1 = vertices[i].x;
                int y1 = vertices[i].y;
                int x2 = vertices[(i + 1) % numVertices].x;
                int y2 = vertices[(i + 1) % numVertices].y;

                // Verifique se a linha horizontal cruza a aresta.
                if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) {
                    float slope = (float)(x2 - x1) / (float)(y2 - y1);

                    // Calcule a interseção da linha horizontal com a aresta.
                    float xIntersection = x1 + (y - y1) * slope;

                    // Se a interseção estiver à direita do ponto de verificação,
                    // conte como uma interseção.
                    if (xIntersection > x) {
                        intersections++;
                    }
                }
            }

            // Se houver um número ímpar de interseções, o ponto está dentro do polígono.
            if (intersections % 2 != 0) {
                DrawPixel(x, y, fillColor);
                //Sleep(0.90);
            }
        }
    }
}

