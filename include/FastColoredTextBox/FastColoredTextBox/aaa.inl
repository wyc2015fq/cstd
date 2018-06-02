
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define readonly
#define internal

#include "cstd.h"
#include "draw/draw2d.inl"

#if 0
#include <gdiplus.h>
#pragma comment(lib,"GdiPlus.lib")
using namespace Gdiplus;
#endif

template <typename T>
void myrealloc(T** p, int n) {
  *p = (T*)realloc(*p, n);
}

struct string {
  char* p;
  string() {p=NULL;}
  ~string() {
    free(p);}
  string(const string& o) {
    p = NULL;
    insert(0, o, -1);
  }
  string(const char* s) {
    p = NULL;
    insert(0, s, -1);
  }
  string(const char* s1, int n1, const char* s2, int n2) {
    p = NULL;
    cat2(s1, n1, s2, n2);
  }
  int len() {
    return p ? strlen(p) : 0;
  }
  int resize(int n) {
    int n1 = len();
    if (n>n1) {
      myrealloc(&p, n);
      n1 = n;
    }
    return n1;
  }
  operator const char*() const {
    return p;
  }
  const char* c_str() const {return p;}
  int insert(int i, const char* s, int ns = -1) {
    int n = len();
    ns = ns<0 ? strlen(s) : ns;
    resize(n+ns + 1);
    i = BOUND(i, 0, n);
    memmove(p + i, p+ns+i, n-i);
    memcpy(p+i, s, ns);
    n += ns;
    p[n] = 0;
    return n;
  }
  int cat2(const char* s1, int n1=-1, const char* s2 = NULL, int n2 = -1) {
    int n = len();
    n1 = (s1) ? (n1<0 ? strlen(s1) : n1) : 0;
    n2 = (s2) ? (n2<0 ? strlen(s2) : n2) : 0;
    resize(n+n1 + n2 + 1);
    if (s1) {
      memcpy(p+n, s1, n1);
    }
    if (s2) {
      memcpy(p+n+n1, s2, n2);
    }
    n += n1 + n2;
    p[n] = 0;
    return n;
  }
};

string operator + (const char* str1, const string& s2) {
  return string(str1, -1, s2, -1);
}

string operator + (const string& s1, const char* str2) {
  return string(s1, -1, str2, -1);
}
string operator + (const int& i, const string& s2) {
  char buf[32];
  itoa(i, buf, 10);
  return string(buf, -1, s2, -1);
}
string operator + (const string& s1, const int& i) {
  char buf[32];
  itoa(i, buf, 10);
  return string(s1, -1, buf, -1);
}

#define null NULL
#define GETDEF(_T, _V)  _T& get##_V () const { return _V;}
#define SETDEF(_T, _V)  void set##_V (const _T& x) {_V = x;}
#define override

typedef COLOR Color;
Color FromArgb(int alpha, Color color) {
  return _ARGB(alpha, GetRV(color), GetGV(color), GetBV(color));
}
unsigned int GetHashCode(int x) {
  return x;
}

struct FastColoredTextBox;
struct RTFStyleDescriptor;

struct Math_Class {
  template <typename T> T Min(T a, T& b) {
    return a<b?a:b;
  }
  template <typename T> T Max(T a, T& b) {
    return a>b?a:b;
  }
};
struct StringBuilder {
  string s;
  int Append(char c) {
    return s.cat2(&c, 1);
  }
};

static Math_Class Math;
struct Rectangle {
  float X, Y, Width, Height;
  float GetLeft() const {return X;}
  float GetRight() const {return X + Width;}
  float GetBottom() const {return Y + Height;}
};

typedef Path GraphicsPath;

void FillRectangle(Graphics* graphics, Brush* br, Rectangle rc) {
  return ;
}
void DrawRectangle(Graphics* graphics, Pen* pen, Rectangle rc) {
  return ;
}
void DrawLine(Graphics* graphics, Pen* pen, float x1, float y1, float x2, float y2) {
  return ;
}

void DrawLine(Graphics* graphics, Pen* pen, Point start, Point end) {
  DrawLine(graphics, pen, start.X, start.Y, end.X, end.Y);
}
void DrawLines(Graphics* graphics, Pen* pen, const Point *points, INT count) {
  return ;
}

  //typedef Gdiplus::Rect Rectangle;


struct Style;
struct Range;
struct IDisposable {};
struct UserControl {};
struct ISupportInitialize {};
struct Cursor {};
struct EventArgs {};
template <typename T> struct EventHandler {};
template <typename T> struct IEnumerable {};
template <typename T> struct IEquatable {};
template <typename T> struct List {};
template <typename T> struct IList {};
template <typename TKey, typename TValue> struct Dictionary {};


struct Cursors_Class {
  Cursor Hand;
};

Cursors_Class Cursors;


#if Styles32
/// <summary>
/// Style index mask (32 styles)
/// </summary>
enum StyleIndex
{
  None = 0,
    Style0 = 0x1,
    Style1 = 0x2,
    Style2 = 0x4,
    Style3 = 0x8,
    Style4 = 0x10,
    Style5 = 0x20,
    Style6 = 0x40,
    Style7 = 0x80,
    Style8 = 0x100,
    Style9 = 0x200,
    Style10 = 0x400,
    Style11 = 0x800,
    Style12 = 0x1000,
    Style13 = 0x2000,
    Style14 = 0x4000,
    Style15 = 0x8000,
    
    Style16 = 0x10000,
    Style17 = 0x20000,
    Style18 = 0x40000,
    Style19 = 0x80000,
    Style20 = 0x100000,
    Style21 = 0x200000,
    Style22 = 0x400000,
    Style23 = 0x800000,
    Style24 = 0x1000000,
    Style25 = 0x2000000,
    Style26 = 0x4000000,
    Style27 = 0x8000000,
    Style28 = 0x10000000,
    Style29 = 0x20000000,
    Style30 = 0x40000000,
    Style31 = 0x80000000,
    
    All = 0xffffffff
};
#else
/// <summary>
/// Style index mask (16 styles)
/// </summary>
typedef unsigned short StyleIndex;
enum StyleIndex_
{
  StyleIndex_None = 0,
    StyleIndex_Style0 = 0x1,
    StyleIndex_Style1 = 0x2,
    StyleIndex_Style2 = 0x4,
    StyleIndex_Style3 = 0x8,
    StyleIndex_Style4 = 0x10,
    StyleIndex_Style5 = 0x20,
    StyleIndex_Style6 = 0x40,
    StyleIndex_Style7 = 0x80,
    StyleIndex_Style8 = 0x100,
    StyleIndex_Style9 = 0x200,
    StyleIndex_Style10 = 0x400,
    StyleIndex_Style11 = 0x800,
    StyleIndex_Style12 = 0x1000,
    StyleIndex_Style13 = 0x2000,
    StyleIndex_Style14 = 0x4000,
    StyleIndex_Style15 = 0x8000,
    StyleIndex_All = 0xffff
};
#endif

enum MouseButtons {
  //     未曾按下鼠标按钮。
  MouseButtons_None = 0,
    //     鼠标左按钮曾按下。
    MouseButtons_Left = 1048576,
    //     鼠标右按钮曾按下。
    MouseButtons_Right = 2097152,
    //     鼠标中按钮曾按下。
    MouseButtons_Middle = 4194304,
    //     第 1 个 XButton 曾按下。
    MouseButtons_XButton1 = 8388608,
    //     第 2 个 XButton 曾按下。
    MouseButtons_XButton2 = 16777216,
};

struct MouseEventArgs : EventArgs
{
  MouseEventArgs(MouseButtons button, int clicks, int x, int y, int delta) {
    Button = button;
    Clicks = clicks;
    X = x;
    Y = y;
    Delta = delta;
  }
  
  MouseButtons Button;
  int Clicks;
  int Delta;
  Point Location;
  int X;
  int Y;
};
