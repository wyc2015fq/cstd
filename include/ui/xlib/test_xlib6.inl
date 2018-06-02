#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <stdio.h>

double get_time()
{
  struct timeval timeval;
  gettimeofday(&timeval, NULL);
  return (double)timeval.tv_sec + (((double)timeval.tv_usec) / 1000000);
}

int test_xlib6()
{
  double start_time = get_time();
  int screen_number;

  xcb_connection_t* connection = xcb_connect(NULL, &screen_number);
  const xcb_setup_t* setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  printf("当前的Screen Number为：%d\n", screen_number);
  printf("iter.rem = %d，下面开始遍历:\n", iter.rem);

  xcb_screen_t* screen;

  for (; iter.rem != 0; xcb_screen_next(&iter)) {
    screen = iter.data;
    printf("*****看到多少行这个提示，就说明有多少个Screen。*****\n");
    printf("     Screen->root:%d\n", screen->root);
    printf("     Screen->root_depth:%d\n", screen->root_depth);
    printf("     Screen->white_pixel:%d\n", screen->white_pixel);
    printf("     Screen->black_pixel:%d\n", screen->black_pixel);
    printf("     Screen->width_in_pixels:%d\n", screen->width_in_pixels);
    printf("     Screen->height_in_pixels:%d\n", screen->height_in_pixels);
    printf("     Screen->width_in_millimeters:%d\n", screen->width_in_millimeters);
    printf("     Screen->height_in_millimeters:%d\n", screen->height_in_millimeters);
  }

  xcb_window_t window = xcb_generate_id(connection);
  xcb_create_window(
      connection,
      XCB_COPY_FROM_PARENT,
      window,
      screen->root,
      100, 100,
      400, 300,
      10,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      screen->root_visual,
      0, NULL);
  xcb_map_window(connection, window);
  xcb_flush(connection);

  printf("花费时间：%f秒。", get_time() - start_time);
  fflush(stdout);

  //pause();
  xcb_disconnect(connection);
  return 0;
}
