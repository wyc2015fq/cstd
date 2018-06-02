#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <xcb/xcb.h>


#define    WIDTH 600
#define    HEIGHT 400
#define TEST_COOKIE(fn,errMessage) \
  cookie=fn; \
  error=xcb_request_check(connection,cookie); \
  if(error){ \
    fprintf(stderr, "Error: %s : %"PRIu8"\n", errMessage, error->error_code); \
  }
   
static void drawText(xcb_connection_t* connection, xcb_screen_t* screen, xcb_window_t window,
    int16_t x, int16_t y, const char* font_name, const char* string)
{
  /*cookie and error, for TEST_COOKIE */
  xcb_void_cookie_t cookie;
  xcb_generic_error_t* error;

  xcb_font_t font = xcb_generate_id(connection);
  TEST_COOKIE(xcb_open_font_checked(connection, font, strlen(font_name), font_name), "Can't open font");
  xcb_gcontext_t gc = xcb_generate_id(connection);
  uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
  uint32_t values[3] = {screen->black_pixel, screen->white_pixel, font};
  TEST_COOKIE(xcb_create_gc_checked(connection, gc, window, mask, values), "Can't create GC")
  /*draw the text*/
  TEST_COOKIE(xcb_image_text_8_checked(connection, strlen(string), window, gc, x, y, string), "Can't draw text");
  /*close the font*/
  xcb_close_font(connection, font);
  xcb_free_gc(connection, gc);
}

int test_xlib10()
{
  xcb_connection_t* connection = xcb_connect(NULL, NULL);
  xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

  uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t values[2];
  values[0] = screen->white_pixel;
  values[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_BUTTON_PRESS |
      XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION;

  /*Create the window*/
  xcb_window_t window = xcb_generate_id(connection);
  xcb_window_t window2 = xcb_generate_id(connection);
  xcb_create_window(connection, screen->root_depth, window, screen->root, 20, 200,
      WIDTH, HEIGHT, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, values);
  xcb_create_window(connection, screen->root_depth, window2, screen->root, 20, 200,
      WIDTH, HEIGHT, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, values);
  xcb_map_window(connection, window);
  xcb_map_window(connection, window2);
  xcb_flush(connection);

  /*event loop*/
  xcb_generic_event_t* event;

  while ((event = xcb_wait_for_event(connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_EXPOSE: {
      drawText(connection, screen, window, 10, 20, "fixed", "Press ESC key to exit.11");
      drawText(connection, screen, window2, 100, 20, "fixed", "asdfasdfadsf.");
      break;
    }

    case XCB_KEY_RELEASE: {
      xcb_key_release_event_t* kr = (xcb_key_release_event_t*)event;

      switch (kr->detail) {
      case 9:  /* ESC */
        free(event);
        xcb_disconnect(connection);
        return 0;
      }
    }
    }

    free(event);
  }

  return 0;
}
