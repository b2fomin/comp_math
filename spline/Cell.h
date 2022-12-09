#pragma once
#include<Windows.h>
#include<tchar.h>
#include<commctrl.h>
#include<string>
#include<algorithm>
#include<sstream>

#define CC_KEYDOWN 15000
#define CC_LBUTTONDOWN 15001
#define CC_PASTE 15002

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
	HWND GetHWND() const noexcept;

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

	void SetData(const char*);

	bool empty() const noexcept;
};
