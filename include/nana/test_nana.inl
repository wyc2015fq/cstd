
#include "nana/c++defines.hpp"
#include "nana/native_window_interface.inl"
#include "nana/bedrock_windows.inl"
//#include "nana/widgets/label.inl"

//#include "test/HelloWord.inl"

struct label : public widget {
  label(window* win) : widget(win) {}
  virtual int paint(native_window_type wnd, native_graphics_type hdc, rectangle* rect) {
    RECT rc = {0, 0, 100, 100};
    DrawTextA((HDC)hdc, "asdf", 4, &rc, 0);
    return 0;
  }
};

int test_nana() {
  window win(NULL, false, rectangle(100, 100, 300, 300));
  label label1(&win);
  wait_event(-1);
  return 0;
}
