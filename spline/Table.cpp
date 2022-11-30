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

Table::Table(const Table& other)
{
	RECT rt;
	TCHAR name[256];
	GetWindowText(other.hWnd, name, 256);
	GetWindowRect(other.hWnd, &rt);
	MapWindowPoints(HWND_DESKTOP, GetParent(other.hWnd), reinterpret_cast<LPPOINT>(&rt), 2);
	hWnd = CreateWindow(_ClassName, name, GetWindowLong(other.hWnd, GWL_STYLE), rt.left, rt.top,
		rt.right - rt.left, rt.bottom - rt.top, GetParent(other.hWnd), GetMenu(other.hWnd), other.hInst, this);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	hInst = other.hInst;
	cells = other.cells;
	for (int i = 0; i < cells.size(); ++i)
		for (int j = 0; j < cells[i].size(); ++j)
		{
			GetWindowRect(other.cells[i][j].GetHWND(), &rt);
			MapWindowPoints(HWND_DESKTOP, other.hWnd, reinterpret_cast<LPPOINT>(&rt), 2);
			SetParent(cells[i][j].GetHWND(), hWnd);
			MoveWindow(cells[i][j].GetHWND(), rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);
		}
	cell_width = other.cell_width;
	cell_height = other.cell_height;
	focused_cell = other.focused_cell;
	SetFocus(cells[focused_cell.first][focused_cell.second].GetHWND());
}

Table& Table::operator=(const Table& other)
{
	RECT rt;
	TCHAR name[256];
	GetWindowText(other.hWnd, name, 256);
	GetWindowRect(other.hWnd, &rt);
	MapWindowPoints(HWND_DESKTOP, GetParent(other.hWnd), reinterpret_cast<LPPOINT>(&rt), 2);
	hWnd = CreateWindow(_ClassName, name, GetWindowLong(other.hWnd, GWL_STYLE), rt.left, rt.top,
		rt.right - rt.left, rt.bottom - rt.top, GetParent(other.hWnd), GetMenu(other.hWnd), other.hInst, this);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	hInst = other.hInst;
	cells = other.cells;
	for (int i = 0; i < cells.size(); ++i)
		for (int j = 0; j < cells[i].size(); ++j)
		{
			GetWindowRect(other.cells[i][j].GetHWND(), &rt);
			MapWindowPoints(HWND_DESKTOP, other.hWnd, reinterpret_cast<LPPOINT>(&rt), 2);
			SetParent(cells[i][j].GetHWND(), hWnd);
			MoveWindow(cells[i][j].GetHWND(), rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);
		}
	cell_width = other.cell_width;
	cell_height = other.cell_height;
	focused_cell = other.focused_cell;
	SetFocus(cells[focused_cell.first][focused_cell.second].GetHWND());
	return *this;
}

LRESULT CALLBACK Table::InitWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Table* tab;
	if (msg == WM_NCCREATE)
	{
		tab = static_cast<Table*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(tab));
		return tab->WndProc(hWnd, msg, wParam, lParam);
	}
	else tab = reinterpret_cast<Table*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!tab) return DefWindowProc(hWnd, msg, wParam, lParam);
	else return tab->WndProc(hWnd, msg, wParam, lParam);
}

void Table::AddColumn(bool is_readonly)
{
	for (int i = 0; i < cells.size(); ++i)
	{
		cells[i].push_back(Cell(hWnd, hInst, cells[i].size() * cell_width,
			i * cell_height, cell_width, cell_height));
		if (is_readonly) SendMessage(cells[i][cells[i].size() - 1].GetHWND(), EM_SETREADONLY, ES_READONLY, 0);
	}
}

void Table::AddRow(bool is_readonly)
{
	cells.push_back(std::vector<Cell>(0));
	auto& row = *(cells.end() - 1);
	for (int i = 0; i < cells[0].size(); ++i)
	{
		row.push_back(Cell(hWnd, hInst, i * cell_width,
			(cells.size() - 1) * cell_height, cell_width, cell_height));
		if (is_readonly) SendMessage(row[i].GetHWND(), EM_SETREADONLY, ES_READONLY, 0);
	}
}

