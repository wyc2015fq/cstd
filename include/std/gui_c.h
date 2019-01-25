
#ifndef _GUI_C_H_
#define _GUI_C_H_

#include "types_c.h"
#include "algo.h"
#include "img_c.h"
#include "geo_c.h"

/* Errors */
#define HG_OK          0 /* Don't bet on it! */
#define HG_BADNAME    -1 /* Bad window or file name */
#define HG_INITFAILED -2 /* Can't initialize HigHGUI */
#define HG_WCFAILED   -3 /* Can't create a window */
#define HG_NULLPTR    -4 /* The null pointer where it should not appear */
#define HG_BADPARAM   -5
#define __BEGIN__ __CC_BEGIN__
#define __END__  __CC_END__
#define EXIT __CC_EXIT__
#define CC_WINDOW_MAGIC_VAL     0x00420042
#define CC_TRACKBAR_MAGIC_VAL   0x00420043
#define CC_CAP_GSTREAMER_1394   0
#define CC_CAP_GSTREAMER_V4L    1
#define CC_CAP_GSTREAMER_V4L2   2
#define CC_CAP_GSTREAMER_FILE   3

///////////////////////////////////////////////////////////////////////////////////////////
enum { CC_WINDOW_AUTOSIZE = 1 };
// YV
enum {
  CC_WND_PROP_FULLSCREEN = 0,
  CC_WND_PROP_AUTOSIZE = 1,
  CC_WINDOW_NORMAL = 0,
  CC_WINDOW_FULLSCREEN = 1
};

#define WINBMPCN 4


typedef enum {
  //KEY_ENTER = '\n', // FOR TEXT EDIT
  //KEY_TAB, // FOR TABBING THROUGH FIELDS
  KEY_LEFT, // FOR TEXT EDIT
  KEY_RIGHT,// FOR TEXT EDIT
  KEY_UP, // FOR TEXT EDIT
  KEY_DOWN, // FOR TEXT EDIT
  KEY_PAGEUP,
  KEY_PAGEDOWN,
  KEY_HOME, // FOR TEXT EDIT
  KEY_END, // FOR TEXT EDIT
  KEY_DELETE, // FOR TEXT EDIT
  KEY_BACK, // FOR TEXT EDIT
  KEY_ESCAPE, // FOR TEXT EDIT
  //KEY_CTRL,
  //KEY_SHIFT,
  //KEY_ALT,
  //KEY_SUPER,
  KEY_COUNT
} KeyCode;
typedef enum {
  MOUSE_LBUTTON,
  MOUSE_MBUTTON,
  MOUSE_RBUTTON,
} MouseCode;
enum {
  MSG_FLAG_LBUTTON = 1,
  MSG_FLAG_RBUTTON = 2,
  MSG_FLAG_MBUTTON = 4,
  MSG_FLAG_CTRLKEY = 8,
  MSG_FLAG_SHIFTKEY = 16,
  MSG_FLAG_ALTKEY = 32
};
typedef enum WindowMsg_ {
  MSG_NOP,
  MSG_TIMER,
#if 1
  MSG_LBUTTONDOWN,
  MSG_LBUTTONUP,
  MSG_RBUTTONDOWN,
  MSG_RBUTTONUP,
  MSG_MBUTTONDOWN,
  MSG_MBUTTONUP,
  MSG_LBUTTONDBLCLK,
  MSG_RBUTTONDBLCLK,
  MSG_MBUTTONDBLCLK,
#else
  MSG_MOUSEDOWN,
  MSG_MOUSEUP,
#endif
  MSG_MOUSEWHEEL,
  MSG_MOUSEWHEELUP,
  MSG_MOUSEWHEELDOWN,
  MSG_MOUSEMOVE,
  MSG_KEYDOWN,
  MSG_SIZE,
  MSG_KEYUP,
  MSG_CHAR,
  MSG_CLOSE,
} WindowMsg;

#define mouse_proc_default NULL
///////////////////////////////////////////////////////////////////////////////////////////
#define waitkey(delay)  WaitMsg(delay, false)
#define imshowpal(_NAME, _IM, pal) ShowImagePal(_NAME, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, pal)
#define imshow_(_IM) ShowImagePal(#_IM, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, PixFmtMax, 0)
#define imshow_f(_IM, f) ShowImagePal(#_IM, (_IM)->h, (_IM)->w, (_IM)->tt.data+f*(_IM)->h*(_IM)->s, (_IM)->s, (_IM)->c, PixFmtMax, 0)
#define imshowfmt(_IM, fmt) ShowImagePal(#_IM, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, fmt, 0)
#define imshowrc(_IM, rc) ShowImagePal(#_IM, RCH(&rc), RCW(&rc), (_IM)->tt.data + (_IM)->s*(rc).t + (_IM)->c*(rc).l, (_IM)->s, (_IM)->c, 0)
#define imshow2(_NAME, _IM) ShowImagePal(_NAME, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, PixFmtMax, 0)
#define imshow_cn(_NAME, _IM, icn) ShowImagePal(_NAME, (_IM)->h, (_IM)->w, (_IM)->tt.data+icn, (_IM)->s, (_IM)->c, PF_8bppGrayUchar, NULL)
//
#define imshowmat_u1(mm) cvShowMat( #mm , "%I8u", mm->h, mm->w*mm->c, mm->tt.data, mm->s, 1, -1)
#define imshowmat_u4(mm) cvShowMat( #mm , "%I32u", mm->h, mm->w*mm->c/4, mm->tt.data, mm->s, 4, -1)
#define imshowmat_x4(mm) cvShowMat( #mm , "%08x", mm->h, mm->w*mm->c/4, mm->tt.data, mm->s, 4, -1)
#define imshowmat_u1_cn(mm, icn) cvShowMat( #mm , "%I8u", mm->h, mm->w, mm->tt.data+icn, mm->s, mm->c, -1)
#define imshowmat_f4(mm) cvShowMat( #mm , "%I32f", mm->h, mm->w, mm->tt.f8, mm->s, mm->c, -1)
#define imshowmat_f8(mm) cvShowMat( #mm , "%I64f", mm->h, mm->w, mm->tt.f8, mm->s, mm->c, -1)
#define imshowmat_f8_cn(mm, icn) cvShowMat( #mm , "%I64f", mm->h, mm->w, mm->tt.f8+icn, mm->s, mm->c, -1)
#define imshowmat_f8_f(mm, f) cvShowMat( #mm , "%I64f", mm->h, mm->w, mm->tt.data+(f)*mm->w*mm->s, mm->s, mm->c, -1)
#define cvShowMat_f4(mm, h, w) cvShowMat( #mm , "%I32f", h, w, mm, w*sizeof(float), sizeof(float), -1)

typedef int(*TrackbarCB)(int pos);
typedef int(*MouseCB)(int event, int x, int y, int flags, void* param);
typedef int(*TrackbarCB2)(int pos, void* userdata);

//static int CreateTrackbar(const char* trackbar_name, const char* window_name, int* val, int count, TrackbarCB on_notify);

#ifdef _WIN32
#include "std/gui_win32.h"
#endif


#endif // _GUI_C_H_
