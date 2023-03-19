#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include<GL/gl.h>
#include<GL/glu.h>

struct Point {
	float x, y, z;
};

struct Vector {
	float x, y, z;
};

struct Triangle {
	Point w1, w2, w3;
};

struct Rectangle {
	Point w1, w2, w3, w4;
};

struct Color {
	float c1, c2, c3, c4 = 1;
};


void renderRectangle(Point observer);
void renderTriangle(Point observer);

void glVertex3f(Point point) {
	glVertex3f(point.x, point.y, point.z);
}

void loadTriangle(Color color, Triangle tr) {
	glBegin(GL_TRIANGLES);
	glColor4f(color.c1, color.c2, color.c3, color.c4);
	glVertex3f(tr.w1);
	glVertex3f(tr.w2);
	glVertex3f(tr.w3);
	glEnd();
}

void loadRectangle(Color color, struct Rectangle r) {
	glBegin(GL_QUADS);
	glColor4f(color.c1, color.c2, color.c3, color.c4);
	glVertex3f(r.w1);
	glVertex3f(r.w2);
	glVertex3f(r.w3);
	glVertex3f(r.w4);
	glEnd();
}

Vector calculateNormalVector(Triangle tr) {

	// W1 = w2 - w1
	// W2 = w3 - w1

	Vector W1 = {
		tr.w2.x - tr.w1.x,
		tr.w2.y - tr.w1.y,
		tr.w2.z - tr.w1.z
	};

	Vector W2 = {
		tr.w3.x - tr.w1.x,
		tr.w3.y - tr.w1.y,
		tr.w3.z - tr.w1.z
	};

	Vector Normal = {
		W1.y * W2.z - W2.y * W1.z,
		W2.x * W1.z - W1.x * W2.z,
		W1.x * W2.y - W2.x * W1.y
	};

	//glBegin(GL_LINES);
	//glColor3f(0, 0, 1);
	//glVertex3f(tr.w1.x, tr.w1.y, tr.w1.z);
	//glVertex3f(tr.w1.x + Normal.x, tr.w1.y + Normal.y, tr.w1.z + Normal.z);
	//glEnd();


	return Normal;
}

Vector calculateNormalVector(struct Rectangle tr) {
	return calculateNormalVector(Triangle{
			tr.w1,
			tr.w2,
			tr.w3
		});
}

Vector calculateObservatorVector(Point observer, Point w1) {
	Vector v = {
		observer.x - w1.x,
		observer.y - w1.y,
		observer.z - w1.z
	};

	return v;
}

bool isVisible(Vector observer, Vector normal) {
	bool result = (observer.x * normal.x + observer.y * normal.y + observer.z * normal.z) > 0;
	return result;
}

bool isTriangleVisible(Point observer, struct Triangle rectangle) {
	Vector observerVector = calculateObservatorVector(observer, rectangle.w1);
	Vector normalVector = calculateNormalVector(rectangle);

	return isVisible(observerVector, normalVector);
}

bool isRectangleVisible(Point observer, struct Rectangle rectangle) {
	return isTriangleVisible(observer, Triangle{
			rectangle.w1,
			rectangle.w2,
			rectangle.w3
		});
}


void DrawScene(GLfloat xRot, GLfloat yRot)
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);



	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(10, 0, 0);
	glVertex3f(-10, 0, 0);
	glVertex3f(0, 10, 0);
	glVertex3f(0, -10, 0);
	glVertex3f(0, 0, 10);
	glVertex3f(0, 0, -10);
	glEnd();

	

	Point observer = {
		-10, -5, 6
	};

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(observer.x - 1, observer.y, observer.z);
	glVertex3f(observer.x + 1, observer.y, observer.z);

	glVertex3f(observer.x, observer.y - 1, observer.z);
	glVertex3f(observer.x, observer.y + 1, observer.z);

	glVertex3f(observer.x, observer.y, observer.z - 1);
	glVertex3f(observer.x, observer.y, observer.z + 1);

	glEnd();

	renderTriangle(observer);
	renderRectangle(observer);

	glFlush();

	glEnd();
	glFinish();
}

void renderTriangle(Point observer) {

	float a = 8;
	float half_a = a / 2;

	float traingleHeight = sqrt(3) / 2 * a;
	float height = sqrt(2) * 2 / 3 * traingleHeight;

	Triangle tr1 = {
		Point{0, traingleHeight / 3 * 2, -height / 3},
		Point {half_a, -traingleHeight / 3, -height / 3},
		Point {-half_a, -traingleHeight / 3, -height / 3}
	};

	Triangle tr2 = {
		Point {0, traingleHeight / 3 * 2, -height / 3},
		Point {-half_a, -traingleHeight / 3, -height / 3},
		Point {0, 0, height / 3 * 2}
	};

	Triangle tr3 = {
		Point {-half_a, -traingleHeight / 3, -height / 3},
		Point {half_a, -traingleHeight / 3, -height / 3},
		Point {0, 0, height / 3 * 2}
	};

	Triangle tr4 = {
		Point {0, traingleHeight / 3 * 2, -height / 3},
		Point {0, 0, height / 3 * 2},
		Point {half_a, -traingleHeight / 3, -height / 3}
	};

	if (isTriangleVisible(observer, tr1)) {
		loadTriangle(Color{ 0, 1.0, 0, 0.6 }, tr1);
	}

	if (isTriangleVisible(observer, tr2)) {
		loadTriangle(Color{ 1, 0, 0 }, tr2);
	}

	if (isTriangleVisible(observer, tr3)) {
		loadTriangle(Color{ 0.26f, 0.53f, 0.96f }, tr3);
	}

	if (isTriangleVisible(observer, tr4)) {
		loadTriangle(Color{ 0.09f, 0.89f, 0.608f }, tr4);
	}
}

void renderRectangle(Point observer) {

	float a = 4, b = 6, c = 8;
	float half_a = (float)a / 2, half_b = (float)b / 2, half_c = (float)c / 2;

	struct Rectangle r1 {
		Point{ -half_a, half_b, half_c },
			Point{ -half_a, -half_b, half_c },
			Point{ half_a,-half_b, half_c },
			Point{ half_a, half_b, half_c }
	};

	struct Rectangle r2 {
		Point{ -half_a, -half_b, -half_c },
			Point{ -half_a, half_b, -half_c },
			Point{ half_a, half_b, -half_c },
			Point{ half_a, -half_b, -half_c }
	};

	struct Rectangle r3 {
		Point{ -half_a, -half_b, half_c },
			Point{ -half_a, -half_b, -half_c },
			Point{ half_a, -half_b, -half_c },
			Point{ half_a, -half_b, half_c }
	};

	struct Rectangle r4 {
		Point{ -half_a, half_b, -half_c },
			Point{ -half_a, half_b, half_c },
			Point{ half_a, half_b, half_c },
			Point{ half_a, half_b, -half_c }
	};

	struct Rectangle r5 {
		Point{ -half_a, half_b, -half_c },
			Point{ -half_a, -half_b, -half_c },
			Point{ -half_a, -half_b, half_c },
			Point{ -half_a, half_b, half_c }
	};

	struct Rectangle r6 {
		Point{ half_a, half_b, half_c },
			Point{ half_a, -half_b, half_c },
			Point{ half_a, -half_b, -half_c },
			Point{ half_a, half_b, -half_c }
	};




	if (isRectangleVisible(observer, r1)) {
		loadRectangle(Color{ 0, 1.0, 0, 0.6 }, r1);
	}

	if (isRectangleVisible(observer, r2)) {
		loadRectangle(Color{ 1, 0, 0 }, r2);
	}


	if (isRectangleVisible(observer, r3)) {
		loadRectangle(Color{ 0.26f, 0.53f, 0.96f }, r3);
	}


	if (isRectangleVisible(observer, r4)) {
		loadRectangle(Color{ 0.09f, 0.89f, 0.608f }, r4);
	}


	if (isRectangleVisible(observer, r5)) {
		loadRectangle(Color{ 0.596f, 0.09f, 0.89f }, r5);
	}


	if (isRectangleVisible(observer, r6)) {
		loadRectangle(Color{ 0.93f, 0.545f, 0.074f }, r6);
	}
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
	if (height * width == 0) return;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// set the blend mode
	//glEnable(GL_BLEND); //obligatory for blending and transparencies
	glEnable(GL_LINE_SMOOTH);

	//glEnable(GL_CULL_FACE);

	glShadeModel(GL_FLAT);
	//glShadeModel(GL_SMOOTH);
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
		ResizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_CREATE:
		hdc = GetDC(hWnd);
		SetMyPixelFormat(hdc);
		hrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hrc);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP) xRot -= 5.0f;
		if (wParam == VK_DOWN) xRot += 5.0f;
		if (wParam == VK_LEFT) yRot -= 5.0f;
		if (wParam == VK_RIGHT) yRot += 5.0f;

		if (xRot > 356.0f) xRot = 0.0f;
		if (xRot < -1.0f) xRot = 355.0f;
		if (yRot > 356.0f) yRot = 0.0f;
		if (yRot < -1.0f) yRot = 355.0f;

		InvalidateRect(hWnd, NULL, FALSE);
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
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "Primitives";
	wcex.hIconSm = NULL;
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
	return (int)msg.wParam;
}