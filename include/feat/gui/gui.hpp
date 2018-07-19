
#define NANA_WINDOWS

//#include <iostream>
//#include <iterator>
//#include <vector>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//#include <initializer_list>

struct Widget {
	Widget** sub;
	int len;
	Widget* add(Widget* w) {
		len++;
		sub = (Widget**)realloc(sub, len*sizeof(void*));
		sub[len-1] = (w);
		return this;
	}
};

Widget* widget() {
	Widget* w = (Widget*)malloc(sizeof(Widget));
	assert(w);
	memset(w, 0, sizeof(Widget));
	return w;
}
void del_widget(Widget* w) {
	if (w) {
		for (int i = 0; i < w->len; ++i) {
			del_widget(w->sub[i]);
		}
		free(w);
	}
	return;
}

#if defined(NANA_WINDOWS)
#include <windows.h>
struct Window {
	typedef Window self;
	enum style {
		default = WS_SYSMENU | WS_CLIPCHILDREN,
		minimize = WS_MINIMIZEBOX,
		maximize = WS_MAXIMIZEBOX,
		sizable = WS_THICKFRAME,
		decoration = WS_OVERLAPPED | WS_CAPTION,
	};
	HWND hWnd;
	Window() { hWnd = NULL; }

	static void loop() {
		MSG msg;
		//当检取到WM_QUIT消息时，退出消息循环。
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	static LRESULT WINAPI guiWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* host = (Window*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		int ret = 0;
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	static WNDCLASSEXA* getWndClass()
	{
#define IMGUIWIN32_CLASSNAME "guiwindow"
		static WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, guiWndProc, 0L, 0L, NULL, NULL, NULL, NULL, NULL, IMGUIWIN32_CLASSNAME, NULL };
		if (!wc.hInstance) {
			//LOGFONT lf;
			wc.hInstance = GetModuleHandle(NULL);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			if (!RegisterClassExA(&wc)) {
				return 0;
			}
		}
		return &wc;
	}
	//platform-dependent
	self& open()
	{
		assert(NULL== hWnd);
		DWORD style = WS_OVERLAPPEDWINDOW;
		DWORD style_ex = WS_EX_NOPARENTNOTIFY;
		int nested = false;
		WNDCLASSEXA* pwc;
		pwc = getWndClass();
		//style |= (nested ? WS_CHILD : WS_POPUP);
		//style_ex |= (app.taskbar ? WS_EX_APPWINDOW : WS_EX_TOOLWINDOW);

		//if (floating)	style_ex |= WS_EX_TOPMOST;
		hWnd = ::CreateWindowExA(style_ex, IMGUIWIN32_CLASSNAME, "Window", style, 0, 0, 300, 300, NULL, 0, ::GetModuleHandle(0), 0);
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)this);
		return *this;
	}

	self& style(int style) {
		assert(0);
		return *this;
	}
	self& show(bool b) {
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		return *this;
	}
	self& move(int x, int y, int width, int height) {
		//A Window may have a border, this should be adjusted the client area fit for the specified size.
		::RECT client, wd_area;
		::GetClientRect(hWnd, &client);	//The right and bottom of client by GetClientRect indicate the width and height of the area
		::GetWindowRect(hWnd, &wd_area);

		//if (x>0)
		{
			wd_area.left = x;
			wd_area.top = y;
		}
		//a dimension with borders and caption title
		wd_area.right -= wd_area.left;	//wd_area.right = width
		wd_area.bottom -= wd_area.top;	//wd_area.bottom = height

		int delta_w = static_cast<int>(width) - client.right;
		int delta_h = static_cast<int>(height) - client.bottom;

		::MoveWindow(hWnd, wd_area.left, wd_area.top, wd_area.right + delta_w, wd_area.bottom + delta_h, true);

		::GetClientRect(hWnd, &client);
		::GetWindowRect(hWnd, &wd_area);

		wd_area.right -= wd_area.left;
		wd_area.bottom -= wd_area.top;
		return *this;
	}

};



#endif
