//
// Created by kajbr on 19.02.2020.
//

// Basic logic bases on:
// https://docs.microsoft.com/en-us/windows/win32/opengl/the-program-ported-to-win32

#ifndef FAKE3DRENDERER_GLOBALCONSTANS_HH
#define FAKE3DRENDERER_GLOBALCONSTANS_HH

#include <GL/gl.h>
#include <GL/glu.h>

/* Windows globals, defines, and prototypes */
extern HWND ghWnd;
extern HDC ghDeviceConext;
extern HGLRC ghRenderingContext;

#define SWAPBUFFERS SwapBuffers(ghDC)
constexpr int BLACK_INDEX = 0;
constexpr int RED_INDEX = 13;
constexpr int GREEN_INDEX = 14;
constexpr int BLUE_INDEX = 16;
constexpr int WIDTH = 300;
constexpr int HEIGHT = 200;

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);

BOOL bSetupPixelFormat(HDC);

/* OpenGL globals, defines, and prototypes */
extern GLfloat latitude, longitude, latinc, longinc;
extern GLdouble radius;

constexpr int GLOBE = 1;
constexpr int CYLINDER = 2;
constexpr int CONE = 3;


GLvoid resize(GLsizei, GLsizei);

GLvoid initializeGL(GLsizei, GLsizei);

GLvoid drawScene(GLvoid);

void polarView(GLdouble, GLdouble, GLdouble, GLdouble);

#endif //FAKE3DRENDERER_GLOBALCONSTANS_HH
