# xcb：一个简单的窗口 - Koma Hub - CSDN博客
2019年01月03日 21:56:07[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：60
```cpp
#include <unistd.h>
#include <xcb/xcb.h>
int 
main()
{
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_window_t win;
	conn = xcb_connect(NULL,NULL);
	screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	win = xcb_generate_id(conn);
	xcb_create_window(conn,
			XCB_COPY_FROM_PARENT,
			win,
			screen->root,
			0,0,
			150,150,
			10,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			screen->root_visual,
			0,NULL);
	xcb_map_window(conn, win);
	xcb_flush(conn);
	pause();
	return 0;
}
```
