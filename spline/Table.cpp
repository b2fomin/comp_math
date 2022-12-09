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
	hInst{ hInstance }, iVscrollPos{ 0 }
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
	int min_pos, max_pos;
	GetScrollRange(other.hWnd, SB_VERT, &min_pos, &max_pos);
	SetScrollRange(hWnd, SB_VERT, min_pos, max_pos, FALSE);
	iVscrollPos = GetScrollPos(other.hWnd, SB_VERT);
	SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
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
	int min_pos, max_pos;
	GetScrollRange(other.hWnd, SB_VERT, &min_pos, &max_pos);
	SetScrollRange(hWnd, SB_VERT, min_pos, max_pos, FALSE);
	iVscrollPos = GetScrollPos(other.hWnd, SB_VERT);
	SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
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
	int min_pos, max_pos;
	GetScrollRange(hWnd, SB_VERT, &min_pos, &max_pos);
	if (cells.size() > max_pos)
		SetScrollRange(hWnd, SB_VERT, min_pos, ++max_pos, TRUE);
}

LRESULT CALLBACK Table::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		TEXTMETRIC tm;
		HDC hdc = GetDC(hWnd);
		GetTextMetrics(hdc, &tm);
		ReleaseDC(hWnd, hdc);
		cell_width = 10 * tm.tmAveCharWidth;
		cell_height = tm.tmHeight + tm.tmExternalLeading + GetSystemMetrics(SM_CXSIZEFRAME);
	}
	return 0;
	case WM_SIZE:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		SetScrollRange(hWnd, SB_VERT, 0, max(cells.size(), pt.y / cell_height), FALSE);
		SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
	}
	return 0;
	case WM_VSCROLL:
	{
		RECT rt;
		GetWindowRect(hWnd, &rt);
		MapWindowPoints(HWND_DESKTOP, hWnd, reinterpret_cast<LPPOINT>(&rt), 2);
		switch (LOWORD(wParam))
		{
		case SB_LINEUP: --iVscrollPos; break;
		case SB_LINEDOWN: ++iVscrollPos; break;
		case SB_PAGEUP: iVscrollPos -= (rt.bottom - rt.top) / cell_height; break;
		case SB_PAGEDOWN: iVscrollPos += (rt.bottom - rt.top) / cell_height; break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION: iVscrollPos = HIWORD(wParam); break;
		}
		int min_pos, max_pos;
		GetScrollRange(hWnd, SB_VERT, &min_pos, &max_pos);
		iVscrollPos = max(0, min(iVscrollPos, max_pos));
		if (iVscrollPos != GetScrollPos(hWnd, SB_VERT))
		{
			SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
			RECT rt;
			GetWindowRect(cells[0][0].GetHWND(), &rt);
			MapWindowPoints(HWND_DESKTOP, hWnd, reinterpret_cast<LPPOINT>(&rt), 2);
			for (int i = min_pos + iVscrollPos; i < min(cells.size(), max_pos + iVscrollPos); ++i)
			{
				for (int j = 0; j < cells[i].size(); ++j)
				{
					MoveWindow(cells[i][j].GetHWND(), j * cell_width + rt.left,
						(i - (min_pos + iVscrollPos)) * cell_height, cell_width, cell_height, TRUE);
					UpdateWindow(cells[i][j].GetHWND());
				}
			}
		}
	}
	return 0;
	case CC_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_TAB:
		{
			if (focused_cell.first == cells.size() - 1 && focused_cell.second == cells[0].size() - 1)
			{
				AddRow();
			}
			focused_cell.second = (focused_cell.second + 1) % cells[0].size();
			if (!focused_cell.second) ++focused_cell.first;
			SetFocus(cells[focused_cell.first][focused_cell.second].GetHWND());
		}
		return 0;
		case VK_RETURN:
		{
			if (focused_cell.first == cells.size() - 1)	AddRow();
			++focused_cell.first;
			SetFocus(cells[focused_cell.first][focused_cell.second].GetHWND());
		}
		return 0;
		case VK_LEFT:
		{
			if (focused_cell.second) --focused_cell.second;
			SetFocus(cells[focused_cell.first][focused_cell.second].GetHWND());
		}
		return 0;
		case VK_RIGHT:
		{
			if (focused_cell.second < cells[0].size() - 1) ++focused_cell.second;
			SetFocus(cells[focused_cell.first][focused_cell.second].GetHWND());
		}
		return 0;
		case VK_UP:
		{
			if (focused_cell.first)	--focused_cell.first;
			auto cell_hWnd = cells[focused_cell.first][focused_cell.second].GetHWND();
			int min_pos, max_pos; GetScrollRange(hWnd, SB_VERT, &min_pos, &max_pos);
			if (focused_cell.first < iVscrollPos + min_pos) SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
			SetFocus(cell_hWnd);
		}
		return 0;
		case VK_DOWN:
		{
			if (focused_cell.first < cells.size() - 1)	++focused_cell.first;
			auto cell_hWnd = cells[focused_cell.first][focused_cell.second].GetHWND();
			int min_pos, max_pos; GetScrollRange(hWnd, SB_VERT, &min_pos, &max_pos);
			RECT rt;
			GetWindowRect(hWnd, &rt);
			MapWindowPoints(HWND_DESKTOP, hWnd, reinterpret_cast<LPPOINT>(&rt), 2);
			if (focused_cell.first > iVscrollPos + min_pos+(rt.bottom-rt.top)/cell_height)
				SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
			SetFocus(cell_hWnd);
		}
		return 0;
		default: return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return 0;
	case CC_PASTE:
	{
		auto cell_hWnd = reinterpret_cast<HWND>(wParam);
		std::stringstream ss; ss << reinterpret_cast<char*>(lParam);

		int i = 0, j = 0;
		for (int i = 0; i < cells.size(); ++i)
		{
			for (int j = 0; j < cells[j].size(); ++j)
			{
				if (cell_hWnd == cells[i][j].GetHWND())
					break;
			}

			if (cell_hWnd == cells[i][j].GetHWND())
				break;
		}
		std::string data;
		while (!ss.eof())
		{
			if (ss.peek() == '\n')
			{
				ss.get();
				cells[i][j].SetData(data.data());
				UpdateWindow(cells[i][j].GetHWND());
				data.clear();
				if (i == cells.size() - 1) AddRow();
				++i; j = 0;
			}
			else if (ss.peek() == '\t')
			{
				ss.get();
				cells[i][j].SetData(data.data());
				UpdateWindow(cells[i][j].GetHWND());
				data.clear();
				if (j == cells[0].size() - 1) AddColumn();
				++j;
			}
			else data += ss.get();
		}
		if (!data.empty())
		{
			cells[i][j].SetData(data.data());
			UpdateWindow(cells[i][j].GetHWND());
		}
		focused_cell = std::make_pair(i, j);
		SetFocus(cells[i][j].GetHWND());
	}
	return 0;
	case CC_LBUTTONDOWN:
	{
		auto hwnd = reinterpret_cast<HWND>(lParam);
		for (int i = 0; i < cells.size(); ++i)
		{
			for (auto j = 0; j < cells[i].size(); ++j)
			{
				if (cells[i][j].GetHWND() == hwnd)
				{
					focused_cell = std::make_pair(i, j);
					SetFocus(hwnd);
					return 0;
				}
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

std::pair<std::size_t, std::size_t> Table::size() const noexcept
{
	return std::make_pair(cells.size(), cells[0].size());
}

HWND Table::GetHWND() const noexcept { return hWnd; };

void Table::clear()
{
	for (auto& row : cells)
		for (auto& cell : row)
			SetWindowText(cell.GetHWND(), nullptr);
}
