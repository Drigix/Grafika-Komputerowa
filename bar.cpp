#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include<GL/gl.h>
#include<GL/glu.h>

// Aleksander Grzybowski

    typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

    typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

    static hsv      rgb2hsv(rgb in);
    static rgb      hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
            // s = 0, v is undefined
        out.s = 0.0;
        out.h = 0;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}


void DrawScene(GLfloat xRot, GLfloat yRot) 
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();


    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glBegin(GL_LINES);
		glColor3f(1,1,1);
		glVertex3f(10,0,0);
		glVertex3f(-10,0,0);
		glVertex3f(0,10,0);
		glVertex3f(0,-10,0);
		glVertex3f(0,0,10);
		glVertex3f(0,0,-10);
	glEnd();

	glBegin(GL_QUAD_STRIP); //ref bar
        


		glColor3f(0,0,0);
		glVertex3f(-5,-7,6);
		glVertex3f(-5,-4,6);
		
		for(i=1;i<=10;i++)
		{
			glColor3f(0,0.1*(GLfloat)i,0);
			glVertex3f(-5+(GLfloat)i,-7,6);
			glVertex3f(-5+(GLfloat)i,-4,6);
		}
	glEnd();

	glBegin(GL_QUAD_STRIP); //mod bar
		glColor3f(0,0,0);
		glVertex3f(-5,0,6);
		glVertex3f(-5,-3,6);
		
        double gamma = 1.9;
		for(i=1;i<=10;i++)
		{
			glColor3f(0,pow(0.1*(GLfloat)i, 1.0/gamma),0);
			glVertex3f(-5+(GLfloat)i,0,6);
			glVertex3f(-5+(GLfloat)i,-3,6);
		}
	glEnd();

    glBegin(GL_QUAD_STRIP); //ref bar
        
        int h = 0;
        int s = 1;
        int v = 1;



		glColor3f(0,0,0);
		glVertex3f(-5,7,6);
		glVertex3f(-5,4,6);
		
		for(i=0;i<=10;i++)
		{
            
            hsv hs = {h, s, v};
            rgb rg = hsv2rgb(hs);
			glColor3f(rg.r, rg.g, rg.b);
			glVertex3f(-5+(GLfloat)i,7,6);
			glVertex3f(-5+(GLfloat)i,4,6);
            h += 36;
		}
	glEnd();





	glBegin(GL_QUADS);
		glColor3f(0,0,0);
		glVertex3f(-3,-3,-3);
		glColor3f(0,1,0);
		glVertex3f(-3,3,-3);
		glColor3f(1,1,0);
		glVertex3f(3,3,-3);
		glColor3f(1,0,0);
		glVertex3f(3,-3,-3);

		glColor3f(0,0,1);
		glVertex3f(-3,-3,3);
        glColor3f(0,1,1);
		glVertex3f(-3,3,3);
        glColor3f(1,1,1);
		glVertex3f(3,3,3);
        glColor3f(1,0,1);
		glVertex3f(3,-3,3);

        glColor3f(0,0,0);
		glVertex3f(-3,-3,-3);
		glColor3f(0,1,0);
		glVertex3f(-3,3,-3);
        glColor3f(0,1,1);
		glVertex3f(-3,3,3);
        glColor3f(0,0,1);
		glVertex3f(-3,-3,3);

        glColor3f(1,1,1);
		glVertex3f(3,3,3);
        glColor3f(1,0,1);
		glVertex3f(3,-3,3);
        glColor3f(1,0,0);
		glVertex3f(3,-3,-3);
        glColor3f(1,1,0);
		glVertex3f(3,3,-3);
		
        glColor3f(0,1,1);
		glVertex3f(-3,3,3);
        glColor3f(0,1,0);
		glVertex3f(-3,3,-3);
		glColor3f(1,1,0);
		glVertex3f(3,3,-3);
        glColor3f(1,1,1);
		glVertex3f(3,3,3);

        glColor3f(0,0,0);
		glVertex3f(-3,-3,-3);
        glColor3f(1,0,0);
		glVertex3f(3,-3,-3);
        glColor3f(1,0,1);
		glVertex3f(3,-3,3);
        glColor3f(0,0,1);
		glVertex3f(-3,-3,3);


	glEnd();



	glEnd();
	glFinish();
}

void SetMyPixelFormat(HDC hdc) 
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);
}
void ResizeWindow(int width, int height)
{
	if (height*width==0) return;
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// set the blend mode
  	glEnable(GL_BLEND); //obligatory for blending and transparencies
	glEnable(GL_LINE_SMOOTH);

	//glEnable(GL_CULL_FACE);
	
	glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HGLRC hrc;
	static GLfloat xRot = 0.0f;
	static GLfloat yRot = 0.0f;

	switch (message) 
	{
	case WM_SIZE: 
		ResizeWindow(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_CREATE:
		hdc = GetDC(hWnd);
		SetMyPixelFormat(hdc);
		hrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hrc);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_KEYDOWN:
		if(wParam==VK_UP) xRot-= 5.0f;
		if(wParam==VK_DOWN) xRot += 5.0f;
		if(wParam==VK_LEFT) yRot -= 5.0f;
		if(wParam==VK_RIGHT) yRot += 5.0f;

		if(xRot > 356.0f) xRot = 0.0f;
		if(xRot < -1.0f) xRot = 355.0f;
		if(yRot > 356.0f) yRot = 0.0f;
		if(yRot < -1.0f) yRot = 355.0f;
		
		InvalidateRect(hWnd,NULL,FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawScene(xRot, yRot);
		SwapBuffers(hdc);
		EndPaint(hWnd, &ps);		
		break;
	case WM_ERASEBKGND:
		return 1;
		break;
	case WM_DESTROY:
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "Primitives";
	wcex.hIconSm		= NULL;
	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hWnd = CreateWindow("Primitives", "OGL color lab", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd) return FALSE;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return FALSE;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}
