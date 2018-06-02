#define image_geticon(x) (0)
#define WD_MANAGER_INTERNAL_LOCK()
#define WD_MANAGER_INTERNAL_UNLOCK()
#include "cstd.h"
#include "cfile.h"
#include "basic_types.inl"
#include "window.inl"
//#include "charset.inl"
//#include "datetime.inl"
//#include "deploy.inl"
//#include "filesystem.inl"
//#include "layout_utility.inl"
//#include "bedrock_windows.inl"
//#include "graphics.inl"
//#include "pixel_buffer.inl"
//#include "platform_spec_windows.inl"
//#include "native_paint_interface.inl"
//#include "graphics.inl"
//#include "image.inl"
//#include "native_window_interface.inl"
form_t* new_form(IRECT* prc, int appearance)
{
  form_t* f = NULL;
  IRECT r = prc ? *prc : make_center(300, 200);
  MYREALLOC(f, 1);
  f->onwer = create_window(NULL, false, r, appearance);
  f->onwer->form = f;
  return f;
}
int form_show(form_t* f, bool show, bool active)
{
  if (f->onwer && f->onwer->hwnd) {
    return show_window(f->onwer->hwnd, show, active);
  }
  return 0;
}
int test_HelloWord()
{
  char buf[256] = {0};
  //pretty_file_size("F:\\VM\\U16\\U16.vmdk", buf, 256);
  form_t* fm = new_form(0, appearance_default);
#if 0
  label* lb = new_label(fm, iRect(fm.size()));
  lb.caption("Hello, World");
  fm.show();
  exec();
#endif
  form_show(fm, true, true);
  pump_event(0, 0);
  return 0;
}
int test_nana()
{
  return 0;
}

