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

LRESULT CALLBACK Graph::InitWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Graph* graph;
	if (msg == WM_NCCREATE)
	{
		graph = static_cast<Graph*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(graph));
	}
	else graph = reinterpret_cast<Graph*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!graph) return DefWindowProc(hWnd, msg, wParam, lParam);
	else return graph->WndProc(hWnd, msg, wParam, lParam);
}

template<typename T>
void Graph::AddFuncGraph(T func, double min_x, double max_x, int count)
{
	for (double x = min_x; x < max_x; x += (max_x - min_x) / count)
	{
		data.push_back({ x, func(x) });
	}
}

void Graph::ShowGraph()
{
	max_x = max_y = -INFINITY;
	min_x = min_y = INFINITY;
	for (auto& elem : data)
	{
		auto x = elem.x; auto y = elem.y;
		if (x < min_x) min_x = x;
		else if (x > max_x) max_x = x;

		if (y < min_y) min_y = y;
		else if (y > max_y) max_y = y;
	}

	for (auto& elem : data)
	{
		POINT pt;
		auto x = elem.x; auto y = elem.y;
		pt.y = static_cast<int>((y - min_y) * GraphWidth / (max_y - min_y) + 0.5);
		pt.x = static_cast<int>((x - min_x) * GraphWidth / (max_x - min_x) + 0.5);
		points.push_back(pt);
	}
	InvalidateRect(hWnd, nullptr, TRUE);
}

LRESULT CALLBACK Graph::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
	}
	return 0;
	case WM_PAINT:
	{
		if (points.empty()) return DefWindowProc(hWnd, msg, wParam, lParam);
		PAINTSTRUCT ps;
		RECT rt;
		GetWindowRect(hWnd, &rt);
		auto hdc = BeginPaint(hWnd, &ps);
		double hx = (max_x - min_x) / ticksX;
		double hy = (max_y - min_y) / ticksY;
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, GraphSize, -GraphSize, nullptr);
		SetViewportExtEx(hdc, min(sx, sy), min(sx, sy), nullptr);
		SetViewportOrgEx(hdc, 3 * indent, min(sx, sy) - indent, nullptr);
		SetTextAlign(hdc, TA_RIGHT | TA_TOP);
		TCHAR s[20];
		double z; int i;
		for (z = min_x, i = 0; i <= ticksX; z += hx, ++i)
		{
			int x = ((z - min_x) * GraphWidth / (max_x - min_x) + 0.5);
			_stprintf_s(s, _T("%.1lE"), z);
			TextOut(hdc, x, 0, s, _tcslen(s));
			MoveToEx(hdc, x, -10, nullptr);
			LineTo(hdc, x, 10);
		}
		MoveToEx(hdc, 0, 0, nullptr);
		LineTo(hdc, GraphWidth, 0);
		SetTextAlign(hdc, TA_RIGHT | TA_BOTTOM);
		for (z = min_y, i = 0; i <= ticksY; z += hy, ++i)
		{
			int y = int((z - min_y) * GraphWidth / (max_y - min_y) + 0.5);
			_stprintf_s(s, _T("%.1lE"), z);
			TextOut(hdc, 0, y, s, _tcslen(s));
			MoveToEx(hdc, -10, y, nullptr);
			LineTo(hdc, 10, y);
		}
		MoveToEx(hdc, 0, 0, nullptr);
		LineTo(hdc, 0, GraphWidth);
		SelectObject(hdc, hline);
		Polyline(hdc, points.data(), points.size());
		EndPaint(hWnd, &ps);
		UpdateWindow(hWnd);
	}
	return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
