#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <stdio.h>
#include <wchar.h>


double get_time()
{
  struct timeval timeval;
  gettimeofday(&timeval, NULL);
  return (double)timeval.tv_sec + (((double)timeval.tv_usec) / 1000000);
}

int test_xlib5()
{
  double start_time = get_time();
  xcb_connection_t* connection = xcb_connect(NULL, NULL);
  const xcb_setup_t* setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t* screen = iter.data;

  xcb_window_t window = xcb_generate_id(connection);
  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 100, 100, 400, 300, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, NULL);
  xcb_map_window(connection, window);
  xcb_flush(connection);
  printf("time %fs¡£\n", get_time() - start_time);
  fflush(stdout);

  //pause();
  xcb_disconnect(connection);
  return 0;
}
