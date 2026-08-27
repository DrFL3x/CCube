#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include <math.h>

WINDOW_HEIGHT = 500;
WINDOW_WIDTH = 500;

typedef struct {
	int x;
	int y;
} xyStart;

typedef struct {
	int x;
	int y;
}Point2D;

typedef struct {
	float x;
	float y;
	float z;
}Point3D;

const char windowClassName[] = "CubeWindow";
uint32_t* pixel_buffer;
BITMAPINFO bmpi;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		StretchDIBits(
			hdc,
			0,
			0,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			0,
			0,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			pixel_buffer,
			&bmpi,
			DIB_RGB_COLORS,
			SRCCOPY
		);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void swap_points2d(Point2D* point1, Point2D*  point2)
{
	Point2D temp = *point1;
	*point1 = *point2;
	*point2 = temp;
}

void DrawTriangle(Point2D point1, Point2D point2, Point2D point3)
{
	Point2D vertice_top;
	Point2D vertice_middle;
	Point2D vertice_bottom;

	vertice_top= point1;
	vertice_middle = point2;
	vertice_bottom = point3;

	if (vertice_top.y < vertice_middle.y)
	{
		swap_points2d(&vertice_top,&vertice_middle);
	}
	if (vertice_top.y < vertice_bottom.y )
	{
		swap_points2d(&vertice_top, &vertice_bottom);
	}
	if (vertice_middle.y < vertice_bottom.y)
	{
		swap_points2d(&vertice_middle, &vertice_bottom);
	}

	//int delta_x1 = 
}

void DrawPrimitiveRectangle(xyStart sCoord, int w, int h, uint32_t color)
{
	for (int py = sCoord.y; py < sCoord.y + h; py++)
	{
		for (int px = sCoord.x; px < sCoord.x + w; px++)
		{
			int i = py * WINDOW_WIDTH + px;
			pixel_buffer[i] = color;
		}
	}
}


void DrawCircle(xyStart sCoord, int radius, uint32_t color)
{
	int x_distance = 0;
	int y_distance = 0;
	double point_distance=0.0;
	for (int py = 0; py < WINDOW_HEIGHT; py++)
	{
		for (int px = 0; px < WINDOW_WIDTH; px++)
		{
			x_distance = abs(sCoord.x - px);
			y_distance = abs(sCoord.y - py);
			point_distance = (double)sqrt(x_distance*x_distance + y_distance*y_distance);
			if (point_distance <= radius)
			{
				int i = py * WINDOW_WIDTH + px;
				pixel_buffer[i] = color;
			}
		}
	}
}


int main()
{
	printf("Hello there");
	
	int rect_w = 50;
	int rect_h = 50;
	xyStart sCoord = { 100 ,100 };
	xyStart sCoordCircle = { 200 ,200 };

	// Window buffer
	pixel_buffer = malloc(WINDOW_HEIGHT * WINDOW_WIDTH * sizeof(uint32_t));
	uint32_t color = 0x00FF0000; // Red
	
	// Draw
	//DrawPrimitiveRectangle(sCoord, rect_w, rect_h, color);
	//DrawCircle(sCoordCircle, 50, color);


	Point2D xy1 = {1,1};
	Point2D xy2 = {2,2};
	Point2D xy3 = {3,3};
	DrawTriangle(xy1, xy2, xy3);
	
	// Bitmap
	bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpi.bmiHeader.biWidth = WINDOW_WIDTH;
	bmpi.bmiHeader.biHeight = -WINDOW_HEIGHT; // Negative height = top-down
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	// Register Window Class
	WNDCLASSEX wc;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	//Create the Window
	HWND hwnd;
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
		windowClassName,
		L"CCube",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return 0;
}