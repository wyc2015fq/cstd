
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "macro.h"
#include "fractal.h"


typedef unsigned int uint;

typedef struct fractal_Branch {
  BOOL m_bDrawPixel;  // 以点代线
  uint m_branch_num;         // 树枝数
  uint m_last_branch_length; // 最小长
  uint m_branch_length;    // 主杆长
  uint m_right_branch_angle; // 左张度
  uint m_left_branch_angle; // 右张度
  double m_cut_rate;   // 切分点
  color_t m_color;   // 颜色
}
fractal_Branch;

void DrawBranch(IDC* pDC, fractal_Branch* fb, int x, int y, double Length, int StartAngle)
{

  double x1, y1, nx, ny, count;
  double nLength;

  x1 = x + Length * cos(StartAngle * PI / 180);
  y1 = y - Length * sin(StartAngle * PI / 180);
  MoveTo(pDC, (int) x, (int) y);

  if (fb->m_bDrawPixel) {
    SetPixelV(pDC, (int) x1, (int) y1, fb->m_color);
  }
  else {
    LineTo(pDC, (int) x1, (int) y1);
  }

  if (Length < fb->m_last_branch_length) {
    return ;
  }

  nLength = Length;
  nx = x;
  ny = y;

  for (count = 0; count < fb->m_branch_num; count++) {
    nx += nLength * (1 - fb->m_cut_rate) * cos(StartAngle * PI / 180);
    ny -= nLength * (1 - fb->m_cut_rate) * sin(StartAngle * PI / 180);
    DrawBranch(pDC, fb, (int) nx, (int) ny, nLength * (1 - fb->m_cut_rate), StartAngle + (int) fb->m_left_branch_angle);
    DrawBranch(pDC, fb, (int) nx, (int) ny, nLength * (1 - fb->m_cut_rate), StartAngle - (int) fb->m_right_branch_angle);
    nLength *= fb->m_cut_rate;
  }

}

uint DrawBranch2(IDC* pDC)
{
  time_t now;
  fractal_Branch ff;
  fractal_Branch* fb = &ff;

  if (time(&now) & 1) {
    fb->m_bDrawPixel = FALSE;
    fb->m_branch_num = 40;
    fb->m_last_branch_length = 10;
    fb->m_branch_length = 300;
    fb->m_right_branch_angle = 20;
    fb->m_left_branch_angle = 80;
    fb->m_cut_rate = 0.618;
    fb->m_color = _RGB(128, 77, 3);
  }
  else {
    fb->m_bDrawPixel = TRUE;
    fb->m_branch_num = 3;
    fb->m_last_branch_length = 3;
    fb->m_branch_length = 100;
    fb->m_right_branch_angle = 9;
    fb->m_left_branch_angle = 9;
    fb->m_cut_rate = 0.3;
    fb->m_color = _RGB(230, 60, 20);
  }

  DrawBranch(pDC, fb, pDC->w / 2, pDC->h - 10, fb->m_branch_length, 90);


  return 0;
}
