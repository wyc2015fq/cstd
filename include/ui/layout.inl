#include "xml.h"
#define MAXROW 20
#define MAXCOL 20
#define LAY_HEADER_ADAPT 'A'
#define LAY_HEADER_EXPAND 'X'
#define LAY_EXPAND_HORIZONTAL '-'
#define LAY_EXPAND_VERTICAL '+'
#define LAY_NULL ' '
//#define LAY_INDEX '#'
#define MAXCTRL 1000000
#include "evalayout.inl"
typedef struct _Layout {
  int Hmargin; // left and right margin
  int Vmargin; // top and bottom margin
  int* id; // 控件id对应wid
  int rows, cols;
  SIZE adapt;
  SIZE gap; // distance between rows and columns
} Layout;
int CreateLayout(Layout* lay, str_t s)
{
  int i, j, k;
  str_t s0 = s;
  str_t s1 = STRsplit(s, '\n', " \t\r\n", &s);
  char* se = s.s + s.l;
  lay->rows = lay->cols = 0;
  sscanf(s1.s, "EvaLayout,%d,%d,%d,%d", &lay->Hmargin, &lay->Vmargin, &lay->gap.w, &lay->gap.h);
  s0 = s;
  for (j = 0; s.s < se; ++j) {
    str_t s1 = STRsplit(s, '\n', "\r\n", &s);
    s1 = STRtrim(s1, " \t\r\n");
    if (0 < s1.l) {
      lay->rows++;
    }
    for (i = 0; 0 < s1.l; ++i) {
      STRsplit(s1, ',', " \t", &s1);
    }
    if (2 == lay->rows && i > lay->cols) {
      lay->cols = i;
    }
  }
  lay->id = (int*)malloc(lay->rows * lay->cols * sizeof(int));
  s = s0;
  for (j = 0; 0 < s.l;) {
    str_t s2, s1 = STRsplit(s, '\n', " \t\r\n", &s);
    if (0 < s1.l) {
      j++;
    }
    for (i = 0; 0 < s1.l && i < lay->cols; ++i) {
      s2 = STRsplit(s1, ',', " \t", &s1);
      k = -1;
      if (1 == j || 0 == i) {
        if (0 < s2.l) {
          if (LAY_HEADER_ADAPT == s2.s[0]) {
            k = -LAY_HEADER_ADAPT;
          }
          else if (LAY_HEADER_EXPAND == s2.s[0]) {
            k = -LAY_HEADER_EXPAND;
          }
          else {
            k = atoi(s2.s);
          }
        }
        else {
          k = -LAY_HEADER_ADAPT;
        }
      }
      else {
        if (0 < s2.l) {
          if (LAY_EXPAND_HORIZONTAL == s2.s[0]) {
            k = -LAY_EXPAND_HORIZONTAL;
          }
          else if (LAY_EXPAND_VERTICAL == s2.s[0]) {
            k = -LAY_EXPAND_VERTICAL;
          }
          else {
            k = atoi(s2.s);
            ASSERT(k >= 0);
          }
        }
        else {
          k = -1;
        }
      }
      lay->id[(j - 1)*lay->cols + i] = k;
    }
  }
  lay->adapt.h = 20;
  lay->adapt.w = 100;
  if (0) {
    for (j = 0; j < lay->rows; ++j) {
      for (i = 0; i < lay->cols; ++i) {
        printf("%3d, ", lay->id[j * lay->cols + i]);
      }
      printf("\n");
    }
  }
  return 1;
}
int doLayout(const Layout* lay, RECT rc, RECT* ld, int ldlen)
{
  int Hpos[MAXCOL] = {0}; // calculated initial horizontal position
  int Vpos[MAXROW] = {0}; // calculated row width
  int Hdim[MAXCOL] = {0}; // calculated column width
  int Vdim[MAXROW] = {0}; // calculated initial vertical position
  int HextraPos[MAXCOL] = {0}; // calculated on every paint
  int VextraPos[MAXROW] = {0}; // calculated on every paint
  int colsReparto[MAXCOL] = {0}; // indexes of columns where the size need to be distributed
  int rowsReparto[MAXROW] = {0}; // indexes of rows where the size need to be distributed
  int rowsRepartoLen = 0;
  int colsRepartoLen = 0;
  int fijoH; // intern precalculated not variable width
  int fijoV; // intern precalculated not variable height
  int i, ii, cc, rr, res, repartH, repartV;
  RECT indxPos[100] = {0};
  int toX = rc.left, toY = rc.top, totWidth = RCW(&rc), totHeight = RCH(&rc);
  if (totWidth <= 0 || totHeight <= 0) {
    return 0;
  }
  ASSERT(lay->rows < MAXROW && lay->cols < MAXCOL);
  for (i = 0; i < lay->cols; ++i) {
    Hdim[i] = Hpos[i] = HextraPos[i] = colsReparto[i] = 0;
  }
  for (i = 0; i < lay->rows; ++i) {
    Vdim[i] = Vpos[i] = VextraPos[i] = rowsReparto[i] = 0;
  }
  // compute Vdim
  fijoV = lay->Vmargin;
  for (rr = 0; rr < lay->rows; rr ++) {
    int heaRow = lay->id[rr * lay->cols];
    int gap = (rr == 0) ? 0 : lay->gap.h;
    Vdim[rr] = (0);
    Vpos[rr] = (0);
    VextraPos[rr] = (0);
    if (heaRow == -LAY_HEADER_EXPAND) {
      rowsReparto[rowsRepartoLen++] = rr; // compute later
    }
    else if (heaRow == -LAY_HEADER_ADAPT) {
      Vdim[rr] = lay->adapt.h; // maximum-minimum of the row
    }
    else {
      Vdim[rr] = heaRow; // indicated size
    }
    Vpos[rr] = fijoV + gap;
    fijoV += Vdim[rr];
    fijoV += gap;
  }
  fijoV += lay->Vmargin;
  // compute Hdim
  fijoH = lay->Hmargin;
  for (cc = 0; cc < lay->cols; cc ++) {
    int heaCol = lay->id[cc];
    int gap = (cc == 0) ? 0 : lay->gap.w;
    Hdim[cc] = (0);
    Hpos[cc] = (0);
    HextraPos[cc] = (0);
    if (heaCol == -LAY_HEADER_EXPAND) {
      colsReparto[colsRepartoLen++] = (cc); // compute later
    }
    else if (heaCol == -LAY_HEADER_ADAPT) {
      Hdim[cc] = lay->adapt.w; // maximum-minimum of the column
    }
    else {
      Hdim[cc] = heaCol; // indicated size
    }
    Hpos[cc] = fijoH + gap;
    fijoH += Hdim[cc];
    fijoH += gap;
  }
  fijoH += lay->Hmargin;
  // finding all components in the layout array
  for (cc = 1; cc < lay->cols; cc ++) {
    for (rr = 1; rr < lay->rows; rr ++) {
      int ava, indx = lay->id[rr * lay->cols + cc];
      if (indx < 0) {
        continue;
      }
      // set position x,y
      indxPos[indx].left = cc;
      indxPos[indx].top = rr;
      // set position x2,y2
      ava = cc;
      while (ava + 1 < lay->cols && lay->id[rr * lay->cols + ava + 1] == -LAY_EXPAND_HORIZONTAL) {
        ava ++;
      }
      indxPos[indx].right = ava;
      ava = rr;
      while (ava + 1 < lay->rows && lay->id[(ava + 1)*lay->cols + cc] == -LAY_EXPAND_VERTICAL) {
        ava ++;
      }
      indxPos[indx].bottom = ava;
    }
  }
  // repartir H
  for (ii = 0; ii < lay->cols; ii ++) {
    HextraPos[ii] = 0;
  }
  // 对自适应X 的处理
  repartH = (totWidth - fijoH) / ((colsRepartoLen == 0) ? 1 : colsRepartoLen);
  for (ii = 0; ii < colsRepartoLen; ii ++) {
    int indx = colsReparto[ii];
    Hdim[indx] = repartH;
    for (res = indx + 1; res < lay->cols; res ++) {
      HextraPos[res] += repartH;
    }
  }
  // repartir V
  for (ii = 0; ii < lay->rows; ii ++) {
    VextraPos[ii] = 0;
  }
  repartV = (totHeight - fijoV) / ((rowsRepartoLen == 0) ? 1 : rowsRepartoLen);
  for (ii = 0; ii < rowsRepartoLen; ii ++) {
    int indx = rowsReparto[ii];
    Vdim[indx] = repartV;
    for (res = indx + 1; res < lay->rows; res ++) {
      VextraPos[res] += repartV;
    }
  }
  for (ii = 0; ii < ldlen; ii ++) {
    int x, y, dx, dy, mm;
    RECT li_indxPos = indxPos[ii];
    x = Hpos[li_indxPos.left] + HextraPos[li_indxPos.left];
    y = Vpos[li_indxPos.top] + VextraPos[li_indxPos.top];
    dx = 0, dy = 0;
    for (mm = li_indxPos.left; mm <= li_indxPos.right; mm ++) {
      if (mm != li_indxPos.left) {
        dx += lay->gap.w;
      }
      dx += Hdim[mm];
    }
    for (mm = li_indxPos.top; mm <= li_indxPos.bottom; mm ++) {
      if (mm != li_indxPos.top) {
        dy += lay->gap.h;
      }
      dy += Vdim[mm];
    }
    assert(dy <= totHeight);
    assert(dx <= totWidth);
    if (x < 0 || y < 0 || dx < 0 || dy < 0) {
      continue;
    }
    ld[ii] = iRECT(toX + x, toY + y, toX + x + dx, toY + y + dy);
  }
  for (cc = 1; cc < lay->cols; cc ++) {
    for (rr = 1; rr < lay->rows; rr ++) {
      int indx = lay->id[rr * lay->cols + cc];
      if (indx < 0) {
        continue;
      }
    }
  }
  return 0;
}
typedef struct elem_info {
  int isrow;
  double x[2];
  double cx[2];
  double gap[2];
  str_t s0;
  int type; // 节点类型: 0 空元素 1 叶子 2 复合
  int t;
  int ttype; // 尺寸类型: 0 平均分配 1 固定像素 2 比例分配
  int id;
} elem_info;
// border 边框厚度
// , 逗号表示分割列，距 gapx
// : 冒号后接宽或高
// [] 中括号
// 例子 [0:20,]
int layout_it_recursive(const elem_info* e0, RECT* rc)
{
  int t;//深度
  int i, n, nn = 0;
  char* s, *s0e;
  elem_info ei[20] = {0};
  str_t s0 = e0->s0;
  const double* x = e0->x;
  const double* cx = e0->w;
  const double* gap = e0->gap;
  int isrow = !!e0->isrow;
  s0e = s0.s + s0.l;
  s = s0.s;
  // 获取元素个数
  i = 0;
  for (t = 0; s != s0e && t >= 0;) {
    if ('[' == *s) {
      if (0 == t) {
        ei[i].type = 2;
        ei[i].s0.s = s + 1;
      }
      ++t;
      ++s;
      continue;
    }
    if (']' == *s) {
      --t;
      if (0 == t) {
        ASSERT(ei[i].type == 2);
        ei[i].s0.l = s - ei[i].s0.s;
      }
      ++s;
      continue;
    }
    if (']' == *s) {
      --t;
      if (0 == t) {
        ASSERT(ei[i].type == 2);
        ei[i].s0.l = s - ei[i].s0.s;
      }
      ++s;
      continue;
    }
    if (0 == t) {
      if (',' == *s) {
        ++i;
        ++s;
        continue;
      }
      if (':' == *s) {
        ei[i].t = (int)strtod(s + 1, &s);
        ei[i].ttype = 1;
        if ('/' == *s) {
          ei[i].ttype = 2;
          ++s;
        }
        continue;
      }
      if ('0' <= *s && '9' >= *s) {
        ei[i].type = 1;
        ei[i].id = (int)strtod(s, &s);
        continue;
      }
      if (' ' == *s) {
        ++s;
        continue;
      }
      ASSERT(0);
    }
    else {
      ++s;
    }
  }
  n = i + 1;
  {
    double xx = x[isrow], dy = cx[isrow] - (n - 1) * gap[isrow], rr = n;
    for (i = 0; i < n; ++i) {
      if (ei[i].t > 0) { // 只有复合类型能指定高度
        dy -= ei[i].t;
        --rr;
      }
    }
    rr = MAX(rr, 1);
    for (i = 0; i < n; ++i) {
      ei[i].w[isrow] = (ei[i].t > 0) ? ei[i].t : dy / rr;
      ei[i].x[isrow] = xx;
      xx += ei[i].w[isrow] + gap[isrow];
      ei[i].x[!isrow] = x[!isrow];
      ei[i].w[!isrow] = cx[!isrow];
    }
    for (i = 0; i < n; ++i) {
      if (1 == ei[i].type) {
        RECT* rc0 = rc + ei[i].id;
        rc0->left = (int)ei[i].x[0];
        rc0->top = (int)ei[i].x[1];
        rc0->right = (int)(ei[i].x[0] + ei[i].w[0]);
        rc0->bottom = (int)(ei[i].x[1] + ei[i].w[1]);
      }
    }
    nn = n;
    for (i = 0; i < n; ++i) {
      if (2 == ei[i].type) {
        ei[i].isrow = !isrow;
        ei[i].gap[0] = gap[0];
        ei[i].gap[1] = gap[1];
        nn += layout_it_recursive(ei + i, rc);
      }
    }
  }
  return nn;
}
#define layout_itR(s, rc, gapx, gapy, outrc) layout_it(s, (rc)->left, (rc)->top, RCW(rc), RCH(rc), gapx, gapy, outrc)
int layout_it(const char* s, double x, double y, double cx, double cy, double gapx, double gapy, RECT* rc)
{
  elem_info e0[1] = {0, x, y, cx, cy, gapx, gapy};
  e0->s0 = STR1(s);
  return layout_it_recursive(e0, rc);
}
int test_layout_it()
{
  RECT rc[10] = {0};
  layout_it("0:20,1:20,2:20", 10, 10, 100, 100, 2, 2, rc);
  layout_it("[1,2,3]:20,0", 10, 10, 100, 100, 2, 2, rc);
  return 0;
}
int imRectAlign(img_t* im, int iFrame, int cx, int cy, UINT uFmt, img_t* im2)
{
  RECT rc2, rc = {0, 0, im->w, im->h};
  RectScaling(rc, cx, cy, &rc2);
  *im2 = *im, im2->f = 1;
  im2->data = im->data + iFrame * im->s * im->h + rc2.top * im->s + rc2.left * im->c;
  im2->h = RCH(&rc2);
  im2->w = RCW(&rc2);
  return 0;
}

