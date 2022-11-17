#pragma once
#include<Windows.h>
#include<tchar.h>
#include<commctrl.h>
#include<vector>

class Table
{
private:
	std::vector<std::vector<HWND>> cells;
	std::vector<std::vector<double>> _table;
	HWND hWnd;
	HINSTANCE hInst;
	static LPCSTR _ClassName;
public:
	Table
	(
		_In_opt_ LPCWSTR lpWindowName,
		_In_ DWORD dwStyle,
		_In_ int X,
		_In_ int Y,
		_In_ int nWidth,
		_In_ int nHeight,
		_In_opt_ HWND hWndParent,
		_In_opt_ HMENU hMenu,
		_In_opt_ HINSTANCE hInstance
	);
	Table(const Table&) = delete;
	Table operator=(const Table&) = delete;
	~Table();
	static LPCTSTR ClassName() noexcept;
	static LRESULT CALLBACK InitWndProc(HWND, UINT, WPARAM, LPARAM);
private:
	bool InitWndClass();
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

auto Table::_ClassName = _T("Table");

bool Table::InitWndClass()
{
	static bool is_initialized=false;
	if (!is_initialized)
	{
		WNDCLASSEX wc{ 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = InitWndProc;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wc.lpszClassName = _ClassName;
		if (RegisterClassEx(&wc))
			return (is_initialized = true);
		else return false;
	}
}

LPCTSTR Table::ClassName() noexcept
{
	return _ClassName;
}

Table::Table
(
	_In_opt_ LPCWSTR lpWindowName,
	_In_ DWORD dwStyle,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_opt_ HWND hWndParent,
	_In_opt_ HMENU hMenu,
	_In_opt_ HINSTANCE hInstance
)
{
	InitWndClass();
	hInst = hInstance;
	hWnd = CreateWindow(_ClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight,
		hWndParent, hMenu, hInstance, reinterpret_cast<LPVOID>(this));
}

Table::~Table()
{
	for (auto& row : cells)
		for (auto& cell : row) DestroyWindow(cell);
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Table::InitWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		auto tab = static_cast<Table*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		return tab->WndProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}