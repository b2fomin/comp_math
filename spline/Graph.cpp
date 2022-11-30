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

Graph::Graph(const Graph& other)
{
	hInst = other.hInst;
	RECT rt;
	TCHAR name[256];
	GetWindowText(other.hWnd, name, 256);
	GetWindowRect(other.hWnd, &rt);
	MapWindowPoints(HWND_DESKTOP, GetParent(other.hWnd), reinterpret_cast<LPPOINT>(&rt), 2);
	hWnd = CreateWindow(_ClassName, name, GetWindowLong(other.hWnd, GWL_STYLE), rt.left, rt.top,
		rt.right - rt.left, rt.bottom - rt.top, GetParent(other.hWnd), GetMenu(other.hWnd), other.hInst, nullptr);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	data = other.data;
	GraphSize = other.GraphSize;
	GraphWidth = other.GraphWidth;
	indent = other.indent;
	max_x = other.max_x;
	max_y = other.max_y;
	min_x = other.min_x;
	min_y = other.min_y;
	ticksX = other.ticksX;
	ticksY = other.ticksY;
	sx = other.sx;
	sy = other.sy;
	hline = CreatePen(PS_SOLID, 6, RGB(0, 0, 255));
}

Graph& Graph::operator=(const Graph& other)
{

	hInst = other.hInst;
	RECT rt;
	TCHAR name[256];
	GetWindowText(other.hWnd, name, 256);
	GetWindowRect(other.hWnd, &rt);
	MapWindowPoints(HWND_DESKTOP, GetParent(other.hWnd), reinterpret_cast<LPPOINT>(&rt), 2);
	hWnd = CreateWindow(_ClassName, name, GetWindowLong(other.hWnd, GWL_STYLE), rt.left, rt.top,
		rt.right - rt.left, rt.bottom - rt.top, GetParent(other.hWnd), GetMenu(other.hWnd), other.hInst, nullptr);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	data = other.data;
	GraphSize = other.GraphSize;
	GraphWidth = other.GraphWidth;
	indent = other.indent;
	max_x = other.max_x;
	max_y = other.max_y;
	min_x = other.min_x;
	min_y = other.min_y;
	ticksX = other.ticksX;
	ticksY = other.ticksY;
	sx = other.sx;
	sy = other.sy;
	hline = CreatePen(PS_SOLID, 6, RGB(0, 0, 255));
	return *this;
}
