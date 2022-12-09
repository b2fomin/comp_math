#pragma once
#include<Windows.h>
#include<tchar.h>
#include<commctrl.h>
#include<vector>
#include<utility>
#include<sstream>
#include"Cell.h"

class Table
{
private:
	std::vector<std::vector<Cell>> cells;
	HWND hWnd;
	HINSTANCE hInst;
	static LPCTSTR _ClassName;
	static int count;
	int cell_width, cell_height, iVscrollPos;
	std::pair<std::size_t, std::size_t> focused_cell;
public:
	Table
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
		int rows_number = 10, int cols_number = 2
	);
	Table(const Table&);
	Table& operator=(const Table&);
	Table() { InitWndClass(); ++count; };
	~Table();
	static LRESULT CALLBACK InitWndProc(HWND, UINT, WPARAM, LPARAM);
private:
	bool InitWndClass();
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	void AddColumn(bool = false);
	void AddRow(bool = false);
	template<typename T>
	std::vector<T> GetColumn(int) const;
	template<typename T>
	std::vector<T> GetRow(int) const;
	template<typename T>
	void SetRow(int col_index, int row_index, std::vector<T> value);
	std::pair<std::size_t, std::size_t> size() const noexcept;
	HWND GetHWND() const noexcept;
	void clear();
};

template<typename T>
std::vector<T> Table::GetColumn(int index) const
{
	std::vector<T> data;
	for (int i = 0; i < cells.size(); ++i)
	{
		auto& cell = cells[i][index];
		if (!cell.empty())
			data.push_back(cell.GetData<T>());
	}
	return data;
}

template<typename T>
std::vector<T> Table::GetRow(int index) const
{
	std::vector<T> data;
	const auto& row = cells[index];
	for (auto& cell : row)
	{
		if (!cell.empty())
			data.push_back(cell.GetData<T>());
	}
	return data;
}

template<typename T>
void Table::SetRow(int col_index, int row_index, std::vector<T> value)
{
	auto& row = cells[row_index];
	for (int i = col_index; i < row.size(); ++i)
	{
		std::string data;
		if constexpr (std::is_same_v<std::remove_const_t<T>, std::string>) data = value[i - col_index];
		else if constexpr (std::is_arithmetic_v<T>) data = std::to_string(value[i - col_index]).data();
		else continue;
		row[i].SetData(data.data());
	}
}