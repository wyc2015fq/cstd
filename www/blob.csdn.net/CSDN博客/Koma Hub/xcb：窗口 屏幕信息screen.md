# xcb：窗口/屏幕信息screen - Koma Hub - CSDN博客
2019年01月03日 22:01:08[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：37
```cpp
#include <xcb/xcb.h>
#include  <stdio.h>
int
main()
{
	xcb_connection_t * conn;
	xcb_screen_t *screen;
	int screen_nbr;
	xcb_screen_iterator_t iter;
	
	conn = xcb_connect(NULL, &screen_nbr);
	
	iter = xcb_setup_roots_iterator(xcb_get_setup(conn));
	for(; iter.rem; --screen_nbr, xcb_screen_next(&iter))
	{
		if (screen_nbr == 0)
		{
			screen = iter.data;
			break;
		}
	}
	printf("\nInformations of screen %ld:\n", screen->root);
	printf("width:%d\nheight:%d\n", screen->width_in_pixels, screen->height_in_pixels);
	printf("white pixel:%ld\nblack pixel:%ld\n", screen->white_pixel, screen->black_pixel);
	xcb_disconnect(conn);
	return 0;
}
```
