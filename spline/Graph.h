#pragma once
#include<Windows.h>
#include<tchar.h>
#include<vector>

class Graph
{
private:
	HWND hWnd;
	HINSTANCE hInst;
	std::vector<POINT> points;
	struct DOUBLE_POINT { double x; double y; };
	std::vector<DOUBLE_POINT> data;
	static LPCTSTR _ClassName;
	int sx, sy;
	static int count;
	double min_x, max_x, min_y, max_y;
	int GraphWidth, GraphSize, ticksX, ticksY, indent;
	HPEN hline;

public:
	Graph() = default;
	Graph(LPCTSTR, DWORD, HWND, HINSTANCE, int, int, int, int, int, int);
	~Graph();
	Graph(const Graph&);
	Graph& operator=(const Graph&);
	static LRESULT CALLBACK InitWndProc(HWND, UINT, WPARAM, LPARAM);
private:
	bool InitWndClass();
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	template<typename T>
	void AddFuncGraph(T, double, double, int);
	void ShowGraph();
	void clear() noexcept;
	HWND GetHWND() noexcept;
};

template<typename T>
void Graph::AddFuncGraph(T func, double min_x, double max_x, int count)
{
	for (double x = min_x; x < max_x; x += (max_x - min_x) / count)
	{
		data.push_back({ x, func(x) });
	}
}
