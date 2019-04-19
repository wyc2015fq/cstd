# xcb：GC - Koma Hub - CSDN博客
2019年01月03日 21:54:13[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：30
个人分类：[xcb](https://blog.csdn.net/Rong_Toa/article/category/8512020)
```cpp
#include <stdio.h>
#include <xcb/xcb.h>
#include <unistd.h>
int 
main()
{
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_drawable_t win;
	xcb_gcontext_t black;
	uint32_t mask;
	uint32_t value[1];
	conn = xcb_connect(NULL, NULL);
	screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	win = screen->root;
	mask = XCB_GC_FOREGROUND;
	black = xcb_generate_id(conn);
	value[0] = screen->black_pixel;
	xcb_create_gc(conn,
			black,
			win,
			mask,
			value);
	xcb_flush(conn);
	pause();
	return 0;
}
```
