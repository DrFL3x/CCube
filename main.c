#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

WINDOW_HEIGHT = 500;
WINDOW_WIDTH = 500;

typedef struct {
	int x;
	int y;
} xyStart;

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

void DrawRectangle(xyStart sCoord, int w, int h, uint32_t color)
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

int main()
{
	printf("Hello there");
	
	int rect_w = 100;
	int rect_h = 100;
	xyStart sCoord = { 100 ,100 };

	// Window buffer
	pixel_buffer = malloc(WINDOW_HEIGHT * WINDOW_WIDTH * sizeof(uint32_t));
	uint32_t color = 0x00FF0000; // Red
	DrawRectangle(sCoord, rect_w, rect_h, color);

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
		L"Cube window",
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