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

void App::Run()
{
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return;
}

LRESULT CALLBACK App::InitWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	App* app;
	if (msg == WM_NCCREATE)
	{
		app = static_cast<App*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
	}
	else app = reinterpret_cast<App*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!app) return DefWindowProc(hWnd, msg, wParam, lParam);
	return app->WndProc(hWnd, msg, wParam, lParam);
}
