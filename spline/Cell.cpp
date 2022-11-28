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

Cell::Cell(const Cell& other)
{
	hInst = other.hInst;
	RECT rt;
	TCHAR name[256];
	GetWindowText(other.hWnd, name, 256);
	GetWindowRect(other.hWnd, &rt);
	MapWindowPoints(HWND_DESKTOP, GetParent(other.hWnd), reinterpret_cast<LPPOINT>(&rt), 2);
	hWnd = CreateWindow(WC_EDIT, name, GetWindowLong(other.hWnd, GWL_STYLE), rt.left, rt.top,
		rt.right - rt.left, rt.bottom - rt.top, GetParent(other.hWnd), GetMenu(other.hWnd), other.hInst, nullptr);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, GetWindowLongPtr(hWnd, GWLP_WNDPROC));
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
}

Cell& Cell::operator=(const Cell& other)
{
	hInst = other.hInst;
	RECT rt;
	TCHAR name[256];
	GetWindowText(other.hWnd, name, 256);
	GetWindowRect(other.hWnd, &rt);
	MapWindowPoints(HWND_DESKTOP, GetParent(other.hWnd), reinterpret_cast<LPPOINT>(&rt), 2);
	hWnd = CreateWindow(WC_EDIT, name, GetWindowLong(other.hWnd, GWL_STYLE), rt.left, rt.top,
		rt.right - rt.left, rt.bottom - rt.top, GetParent(other.hWnd), GetMenu(other.hWnd), other.hInst, nullptr);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, GetWindowLongPtr(hWnd, GWLP_WNDPROC));
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
	return *this;
}