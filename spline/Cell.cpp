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

HWND Cell::GetHWND() const noexcept { return hWnd; };

bool Cell::empty() const noexcept
{
	return !SendMessage(hWnd, EM_LINELENGTH, 0, 0);
}

LRESULT CALLBACK Cell::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto init_wndproc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	HWND hwnd;
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_TAB:
		case VK_RETURN:
			SendMessage(GetParent(hWnd), CC_KEYDOWN, wParam, lParam);
			return 0;
		case VK_LEFT:
		{
			int i, j = 0;
			SendMessage(hWnd, EM_GETSEL, reinterpret_cast<WPARAM>(&i), reinterpret_cast<LPARAM>(&j));
			if (!i) SendMessage(GetParent(hWnd), CC_KEYDOWN, wParam, lParam);
			else SendMessage(hWnd, EM_SETSEL, i - 1, j - 1);
		}
		return 0;
		case VK_RIGHT:
		{
			int i, j = 0;
			SendMessage(hWnd, EM_GETSEL, reinterpret_cast<WPARAM>(&i), reinterpret_cast<LPARAM>(&j));
			int length = SendMessage(hWnd, EM_LINELENGTH, 0, 0);
			if (i == length) SendMessage(GetParent(hWnd), CC_KEYDOWN, wParam, lParam);
			else SendMessage(hWnd, EM_SETSEL, i + 1, j + 1);
		}
		return 0;
		case VK_UP:
		case VK_DOWN:
			SendMessage(GetParent(hWnd), CC_KEYDOWN, wParam, lParam);
			return 0;
		default: return init_wndproc(hWnd, msg, wParam, lParam);
		}
	}
	case WM_LBUTTONDOWN:
		SendMessage(GetParent(hWnd), CC_LBUTTONDOWN, 0, reinterpret_cast<LPARAM>(hWnd));
		return 0;
	default: return init_wndproc(hWnd, msg, wParam, lParam);
	}
}