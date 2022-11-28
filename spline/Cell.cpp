#include"Cell.h"

Cell::Cell(HWND hWndParent, HINSTANCE hInst, int x, int y, int width, int height)
	: hWnd{ CreateWindow(WC_EDIT, nullptr, WS_CHILD | WS_VISIBLE | WS_DLGFRAME | ES_NOHIDESEL, x,
		y, width, height, hWndParent, nullptr, hInst, nullptr) }, hInst{ hInst }
{
	SetWindowLongPtr(hWnd, GWLP_USERDATA, GetWindowLongPtr(hWnd, GWLP_WNDPROC));
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
}

Cell::~Cell()
{
	DestroyWindow(hWnd);
}