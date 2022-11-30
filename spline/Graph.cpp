#include"Graph.h"

LPCTSTR Graph::_ClassName = _T("Graph");
int Graph::count = 0;

bool Graph::InitWndClass()
{
	if (!count)
	{
		WNDCLASSEX wc{ 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = InitWndProc;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wc.lpszClassName = _ClassName;
		wc.hInstance = hInst;
		if (!RegisterClassEx(&wc))
			return false;
	}
	else return true;
}

Graph::Graph(LPCTSTR lpWindowName, DWORD dwStyle, HWND hWndParent, HINSTANCE hInst, int x, int y, int width, int height, int ticksX, int ticksY)
	: hInst{ hInst }, GraphSize{ 1200 }, GraphWidth{ 1000 }, ticksX{ ticksX }, ticksY{ ticksY }, indent{ 25 },
	min_x{ INFINITY }, max_x{ -INFINITY }, min_y{ INFINITY }, max_y{ -INFINITY }, hline{ CreatePen(PS_SOLID, 6, RGB(0, 0, 255)) }
{
	InitWndClass();
	++count;
	hWnd = CreateWindow(_ClassName, lpWindowName, dwStyle,
		x, y, width, height, hWndParent, nullptr, hInst, this);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

Graph::~Graph()
{
	DeleteObject(hline);
	DestroyWindow(hWnd);
	if (!--count) UnregisterClass(_ClassName, hInst);
};
