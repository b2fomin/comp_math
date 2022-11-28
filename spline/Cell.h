#pragma once
#include<Windows.h>
#include<tchar.h>
#include<commctrl.h>
#include<string>
#include<sstream>

#define CC_KEYDOWN 15000
#define CC_LBUTTONDOWN 15001

class Cell
{
private:
	HWND hWnd;
	HINSTANCE hInst;
public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	Cell(HWND, HINSTANCE, int, int, int, int);
	Cell() = default;
	~Cell();

	Cell(const Cell&);
	Cell& operator=(const Cell&);
	HWND GetHWND() noexcept;

	template<typename T>
	T GetData() const
	{
		RECT rt;
		GetClientRect(hWnd, &rt);
		auto cell_width = rt.right - rt.left;
		std::string s; s.resize(cell_width);
		GetWindowTextA(hWnd, &*s.begin(), cell_width);
		std::stringstream ss; ss << s; T value; ss >> value;
		return value;
	}

	template<typename T>
	void SetData(T value)
	{
		auto s = std::to_string(value);
		SetWindowTextA(hWnd, s.data());
	}

	bool empty() const noexcept;
};