/*
 * Example of a Windows OpenGL program.
 * The OpenGL code is the same as that used in
 * the X Window System sample
 */

#include <string>
#include <windows.h>

#include "GlobalConstans.hh"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wndclass;

    /* Register the frame class */
    const std::string appClassName = "Fake3DRenderer";
    const std::string appWindowName = "Fake3DRenderer - introduction";
    wndclass.style = 0;
    wndclass.lpfnWndProc = static_cast<WNDPROC>(MainWndProc);
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, appClassName.c_str());
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wndclass.lpszMenuName = appClassName.c_str();
    wndclass.lpszClassName = appClassName.c_str();

    if (!RegisterClass(&wndclass))
        return FALSE;

    /* Create the frame */
    ghWnd = CreateWindow (appClassName.c_str(),
                          appWindowName.c_str(),
                          WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          WIDTH,
                          HEIGHT,
                          nullptr,
                          nullptr,
                          hInstance,
                          nullptr);

    /* make sure window was created */
    if (!ghWnd)
        return FALSE;

    /* show and update main window */
    ShowWindow(ghWnd, nCmdShow);

    UpdateWindow(ghWnd);

    /* animation loop */
    while (true) {
        /*
         *  Process all pending messages
         */
        while (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE) == TRUE) {
            if (GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                return TRUE;
            }
        }
        drawScene();
    }
}
