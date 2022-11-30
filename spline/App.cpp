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

LRESULT CALLBACK App::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sx, sy;

	switch (msg)
	{
	case WM_SIZE:
	{
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);

		MoveWindow(tab.GetHWND(), 0, 0, sx, sy, TRUE);
		UpdateWindow(tab.GetHWND());
	}
	return 0;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_CLEAR:
		{
			tab.clear();
		}
		return 0;
		case IDM_GRAPH:
			if (!IsWindow(graph.GetHWND()))
				graph = Graph(_T("График"), WS_SYSMENU | WS_POPUP | WS_VISIBLE | WS_THICKFRAME | WS_CAPTION,
					hWnd, hInst, 100, 100, 640, 480, 5, 5);
			else graph.clear();
			for (int i = 1; i < tab.size().first; ++i)
			{
				auto coeff = tab.GetRow<double>(i);
				if (coeff.empty()) continue;
				graph.AddFuncGraph(
					[&coeff, &i](double x)
					{
						return coeff[2] + coeff[3] * (x - coeff[0]) + coeff[4] * pow((x - coeff[0]), 2) + coeff[5] * pow(x - coeff[0], 3);
					},
					tab.GetRow<double>(i - 1)[0], coeff[0], 100);
			}
			graph.ShowGraph();
			return 0;
		case IDM_CALC:
		{
			if (tab.size().second == 2)
				for (int i = 0; i < 4; ++i)	tab.AddColumn(true);
			auto x = tab.GetColumn<double>(0);
			auto y = tab.GetColumn<double>(1);
			std::vector<std::vector<double>> data(x.size());
			for (int i = 0; i < data.size(); ++i)
				data[i] = std::vector<double>{ x[i], y[i] };
			auto coeff = spline(data);
			for (int i = 0; i < coeff.size(); ++i)
				tab.SetRow(2, i + 1, coeff[i]);
		}
		return 0;
		default:return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}