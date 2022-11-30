#include"Table.h"


LPCTSTR Table::_ClassName = _T("Table");
int Table::count = 0;

bool Table::InitWndClass()
{
	if (!count)
	{
		WNDCLASSEX wc{ 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = InitWndProc;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wc.lpszClassName = _ClassName;
		if (!RegisterClassEx(&wc))
			return false;
	}
	return true;
}

Table::Table
(
	LPCWSTR lpWindowName,
	DWORD dwStyle,
	int X,
	int Y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	int rows_number, int cols_number
) :
	hInst{ hInstance }
{
	InitWndClass();
	++count;
	hWnd = CreateWindow(_ClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight,
		hWndParent, hMenu, hInstance, this);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	cells.push_back(std::vector<Cell>(0));
	for (int i = 0; i < cols_number; ++i) AddColumn();
	for (int i = 0; i < rows_number; ++i) AddRow();
	focused_cell = std::make_pair(0, 0);
	SetFocus(cells[0][0].GetHWND());
}

Table::~Table()
{
	cells.clear();
	DestroyWindow(hWnd);
	if (!--count) UnregisterClass(_ClassName, hInst);
}
