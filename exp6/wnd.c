#include <windows.h>
#include <stdio.h>
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
HWND hWnd;
LPCTSTR ClsName = "WndMsg";
LPCTSTR WindowCaption = "Windows and Controls Messages";
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, 
          LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    MSG         Msg;
    WNDCLASSEX  WndClsEx;

    WndClsEx.cbSize        = sizeof(WNDCLASSEX);
    WndClsEx.style         = CS_HREDRAW | CS_VREDRAW;
    WndClsEx.lpfnWndProc   = WndProc;
    WndClsEx.cbClsExtra    = 0;
    WndClsEx.cbWndExtra    = 0;
    WndClsEx.hInstance     = hInstance;
    WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    WndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClsEx.lpszMenuName  = NULL;
    WndClsEx.lpszClassName = ClsName;
    WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&WndClsEx);

    hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                          ClsName, WindowCaption, WS_OVERLAPPEDWINDOW,
                          100, 120, 640, 480,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while(GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
} 
//---------------------------回调函数由操作系统调用----------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, 
   LPARAM lParam)
{
    char buff[20];
    sprintf(buff,"your presssed '%c'\n",wParam);
    switch(Msg)
    {
    // 请在这里填入代码
    case WM_CHAR:
        MessageBox(0,buff,"MSg",0);
        break;//这里 
    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;
    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    return 0;
}