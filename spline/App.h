#pragma once
#include<Windows.h>
#include<tchar.h>
#include"Table.h"
#include"Graph.h"
#include"resource.h"
#include<vector>
#include"spline_calc.h"

class App
{
private:
	HWND hWnd;
	HINSTANCE hInst;
	Table tab;
	Graph graph;
	std::vector<std::vector<double>> coeff;
	static LPCTSTR _ClassName;
	static int count;
public:
	App(HINSTANCE);
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	~App();
private:
	static LRESULT CALLBACK InitWndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	bool InitWndClass();
public:
	void Run();
};
