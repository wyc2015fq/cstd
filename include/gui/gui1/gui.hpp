
//#include <iostream>
//#include <iterator>
//#include <vector>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Platform.h"
#include "BlueRenderEngine.hpp"
using namespace Platform;

struct Label : public Widget {
  void mouseEvent(const MouseEvent & e) {
    printf("Label mouseEvent\n");
  }
};
Label* label()
{
  return new Label;
}

struct Button : public Widget {
  int m_cxWidth;
  SizeF m_szPadding;
  uint32_t m_uTextStyle;
  uint32_t m_uButtonState;

  virtual void OnPaint(Surface* s, RectF& rcPaint) {
    //printf("Button OnPaint\n");
    s->FillRectangle(rcPaint, Color(Color::Red));
    //s->Ellipse(rcPaint, Color(Color::Red), Color(Color::Green));
  }
  virtual void mouseEvent(const MouseEvent& e) {
    switch(e.button) {
    case Mouse::LeftButton:
      printf("Button LeftButton\n");
      break;
    case Mouse::RightButton:
      printf("Button RightButton\n");
      break;
    }
  }
};
Button* button()
{
  return new Button;
}

#include "PlatWin.inl"

