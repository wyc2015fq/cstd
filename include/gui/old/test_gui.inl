#define image_geticon(x) (0)
#define WD_MANAGER_INTERNAL_LOCK()
#define WD_MANAGER_INTERNAL_UNLOCK()
#include "cstd.h"
#include "cfile.h"
#include "window.inl"
#include "graphics.inl"
#include "vsstyle.h"
#include "widget.inl"
#include "res.inl"
#include "uitools.inl"
#include "scroll.inl"
#include "static.inl"
#include "button.inl"
#include "dialog.inl"
#include "listbox.inl"
#ifdef __cplusplus
#include "afx.h"
#include "afxwin.h"
#include "wincore.inl"
#include "afx.inl"
//#include "afxcmn.inl"
#endif
#if 0
#include "test/testall.inl"
#else
#include "test1/testall.inl"
#endif
//#include "form.inl"
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
#if 0
HWID new_form(IRECT* prc, int appearance)
{
  IRECT r = prc ? *prc : make_center(300, 200);
  f->onwer = CreateWidget(NULL, false, r, appearance);
  return f;
}
int form_show(widget_t* f, bool show, bool active)
{
  if (f->onwer && f->onwer->hwnd) {
    return show_window(f->onwer->hwnd, show, active);
  }
  return 0;
}
#endif
int RegisterAllWidgetClass()
{
  WIDCLASS wc[1] = {0};
  static int inited = 0;
  if (!inited) {
    inited = 1;
    RegisterWidgetClass(WC_BUTTONA, CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW | CS_PARENTDC, ButtonWndProc_common, 0, NB_EXTRA_BYTES, 0);
  }
  return 0;
}
int _RegisterAllWidget_inited = RegisterAllWidgetClass();
int test_gui()
{
  char buf[256] = {0};
  //CButton btn;
  if (1) {
    MSG msg;
    HWID hwnd = CreateWidget(0, "BUTTON", "asdfas", 0, 0, 0, 100, 100, 0, 0);
    ShowWidget(hwnd, 1);
    UpdateWidget(hwnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  //pretty_file_size("F:\\VM\\U16\\U16.vmdk", buf, 256);
  CreateWidget(0, WC_BUTTONA, "asdf", WS_VISIBLE, 0, 0, 200, 100, 0, 0);
#if 0
  widget_t* lb = new_label(fm, iRect(fm.size()));
  lb.caption("Hello, World");
  fm.show();
  exec();
  form_show(fm, true, true);
#endif
  pump_event(0, 0);
  return 0;
}

