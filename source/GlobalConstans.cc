//
// Created by kajbr on 19.02.2020.
//
#include "GlobalConstans.hh"

#include <windows.h>

GLfloat latitude, longitude, latinc, longinc;
GLdouble radius;

/* Windows globals, defines, and prototypes */
HWND ghWnd;
HDC ghDC;
HGLRC ghRC;

/* main window procedure */
LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LONG lRet = 1;
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg) {

        case WM_CREATE:
            ghDC = GetDC(hWnd);
            if (!bSetupPixelFormat(ghDC))
                PostQuitMessage(0);

            ghRC = wglCreateContext(ghDC);
            wglMakeCurrent(ghDC, ghRC);
            GetClientRect(hWnd, &rect);
            initializeGL(rect.right, rect.bottom);
            break;

        case WM_PAINT:
            BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_SIZE:
            GetClientRect(hWnd, &rect);
            resize(rect.right, rect.bottom);
            break;

        case WM_CLOSE:
            if (ghRC)
                wglDeleteContext(ghRC);
            if (ghDC)
                ReleaseDC(hWnd, ghDC);
            ghRC = nullptr;
            ghDC = nullptr;

            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            if (ghRC)
                wglDeleteContext(ghRC);
            if (ghDC)
                ReleaseDC(hWnd, ghDC);

            PostQuitMessage(0);
            break;

        case WM_KEYDOWN:
            switch (wParam) {
                case VK_LEFT:
                    longinc += 0.5F;
                    break;
                case VK_RIGHT:
                    longinc -= 0.5F;
                    break;
                case VK_UP:
                    latinc += 0.5F;
                    break;
                case VK_DOWN:
                    latinc -= 0.5F;
                    break;
                default:
                    break;
            }

        default:
            lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
    }

    return lRet;
}

BOOL bSetupPixelFormat(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                    PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
    ppfd->cColorBits = 8;
    ppfd->cDepthBits = 16;
    ppfd->cAccumBits = 0;
    ppfd->cStencilBits = 0;

    if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0) {
        MessageBox(nullptr, "ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) {
        MessageBox(nullptr, "SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

/* OpenGL code */

GLvoid resize(GLsizei width, GLsizei height) {
    GLfloat aspect;

    glViewport(0, 0, width, height);

    aspect = (GLfloat) width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 3.0, 7.0);
    glMatrixMode(GL_MODELVIEW);
}

GLvoid createObjects() {
    GLUquadricObj *quadObj;

    glNewList(GLOBE, GL_COMPILE);
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_LINE);
    gluSphere(quadObj, 1.5, 16, 16);
    glEndList();

    glNewList(CONE, GL_COMPILE);
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    gluCylinder(quadObj, 0.3, 0.0, 0.6, 15, 10);
    glEndList();

    glNewList(CYLINDER, GL_COMPILE);
    glPushMatrix();
    glRotatef((GLfloat) 90.0, (GLfloat) 1.0, (GLfloat) 0.0, (GLfloat) 0.0);
    glTranslatef((GLfloat) 0.0, (GLfloat) 0.0, (GLfloat) - 1.0);
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    gluCylinder(quadObj, 0.3, 0.3, 0.6, 12, 2);
    glPopMatrix();
    glEndList();
}

GLvoid initializeGL(GLsizei width, GLsizei height) {
    GLfloat maxObjectSize, aspect;
    GLdouble near_plane, far_plane;

    glClearIndex((GLfloat) BLACK_INDEX);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    aspect = (GLfloat) width / height;
    gluPerspective(45.0, aspect, 3.0, 7.0);
    glMatrixMode(GL_MODELVIEW);

    near_plane = 3.0;
    far_plane = 7.0;
    maxObjectSize = 3.0F;
    radius = near_plane + maxObjectSize / 2.0;

    latitude = 0.0F;
    longitude = 0.0F;
    latinc = 6.0F;
    longinc = 2.5F;

    createObjects();
}

void polarView(GLdouble radius, GLdouble twist, GLdouble latitude, GLdouble longitude) {
    glTranslated(0.0, 0.0, -radius);
    glRotated(-twist, 0.0, 0.0, 1.0);
    glRotated(-latitude, 1.0, 0.0, 0.0);
    glRotated(longitude, 0.0, 0.0, 1.0);


}

GLvoid drawScene(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    latitude += latinc;
    longitude += longinc;

    polarView(radius, 0, latitude, longitude);

    glIndexi(RED_INDEX);
    glCallList(CONE);

    glIndexi(BLUE_INDEX);
    glCallList(GLOBE);

    glIndexi(GREEN_INDEX);
    glPushMatrix();
    glTranslatef(0.8F, -0.65F, 0.0F);
    glRotatef(30.0F, 1.0F, 0.5F, 1.0F);
    glCallList(CYLINDER);
    glPopMatrix();

    glPopMatrix();

    SwapBuffers(ghDC);
}