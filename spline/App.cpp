#include"App.h"

LPCTSTR App::_ClassName = _T("MainWnd");
int App::count = 0;

bool App::InitWndClass()
{
	if (!count)
	{
		WNDCLASSEX wc{ 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = InitWndProc;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wc.lpszClassName = _ClassName;
		wc.hInstance = hInst;
		wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
		if (!RegisterClassEx(&wc))
			return false;
	}
	return true;
}

App::App(HINSTANCE hInst) : hInst{ hInst }
{
	InitWndClass();
	hWnd = CreateWindow(_ClassName, _T("Приложение"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, nullptr, hInst, this);
	tab = Table(nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, nullptr, hInst);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
}

App::~App()
{
	DestroyWindow(hWnd);
	if (!--count) UnregisterClass(_ClassName, hInst);
}
