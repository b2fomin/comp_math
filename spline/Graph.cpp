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
