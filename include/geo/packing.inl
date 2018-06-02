#ifndef _PACKING_INL_
#define _PACKING_INL_

typedef struct {
  int x, y; //左下角顶点坐标
  int w, h; //矩形长和宽
  int i;
} idrect_t;
int packing_BL_Singal(int x1, int x2, int x3, int x4)
{
  //异号阻挡 ，同号通过 1为异号，0 为同号
  if (x1 - x4 < 0 && x2 - x3 > 0) {
    return 1; // 覆盖
  }
  if (x1 - x4 >= 0 && x2 - x3 > 0) {
    return 0;
  }
  if (x1 - x4 < 0 && x2 - x3 <= 0) {
    return 0;
  }
  return 0;
}
int packing_sortw(int n, idrect_t* rec)
{
#define RECLESS(a, b) ((a).w>(b).w)
  QSORT(rec, rec + n, RECLESS, idrect_t);
#undef RECLESS
  return 0;
}
int packing_sorth(int n, idrect_t* rec)
{
#define RECLESS(a, b) ((a).h>(b).h)
  QSORT(rec, rec + n, RECLESS, idrect_t);
#undef RECLESS
  return 0;
}
double packing_rate(int n, const idrect_t* rec)
{
  double ret = 0;
  if (n > 0) {
    IPOINT ptmin = iPOINT(rec[0].x, rec[0].y), ptmax = iPOINT(rec[0].x + rec[0].w, rec[0].y + rec[0].h);
    int i, sumarea = rec[0].w * rec[0].h;
    for (i = 1; i < n; ++i) {
      //ASSERT(rec[i].x>=0 && rec[i].y>=0);
      ptmin.x = MIN(ptmin.x, rec[i].x);
      ptmin.y = MIN(ptmin.y, rec[i].y);
      ptmax.x = MAX(ptmax.x, rec[i].x + rec[i].w);
      ptmax.y = MAX(ptmax.y, rec[i].y + rec[i].h);
      sumarea += rec[i].w * rec[i].h;
    }
    //printf("%d %d %d\n", sumarea, (ptmax.x-ptmin.x), (ptmax.y-ptmin.y));
    ret = sumarea * 1. / ((ptmax.x - ptmin.x) * (ptmax.y - ptmin.y));
  }
  return ret;
}
int packing_BL(int width, int n, idrect_t* rec)
{
  int i, j, max_y = rec[0].h;
  rec[0].x = 0;
  rec[0].y = 0;
  for (i = 1; i < n; ++i) {
    int Flag1 = 0, Flag2 = 0, tempx = 0, tempy = 0;
    rec[i].x = width - rec[i].w;
    rec[i].y = 60000;
    while (Flag1 == 0 || Flag2 == 0) {
      for (j = 0; j <= i - 1; j++) { //向下
        if (packing_BL_Singal(rec[i].x, rec[i].x + rec[i].w, rec[j].x, rec[j].x + rec[j].w)) {
          if (tempy < rec[j].y + rec[j].h) {
            tempy = rec[j].y + rec[j].h;
          }
          else {
            Flag1 = 1;
          }
        }
        else {
          //rec[i].y=0;
          Flag1 = 1;
        }
      }
      if (tempy < rec[i].y) {
        rec[i].y = tempy;
        Flag1 = 0;
      }
      else {
        Flag1 = 1;
      }
      tempy = 0;
      for (j = 0; j <= i - 1; j++) { //向左
        if (packing_BL_Singal(rec[i].y, rec[i].y + rec[i].h, rec[j].y, rec[j].y + rec[j].h)) {
          if (tempx < rec[j].x + rec[j].w) {
            tempx = rec[j].x + rec[j].w;
          }
          else {
            Flag2 = 1;
          }
        }
        else {
          Flag2 = 1;
        }
      }
      if (tempx < rec[i].x) {
        rec[i].x = tempx;
        Flag2 = 0;
      }
      else {
        Flag2 = 1;
      }
      tempx = 0;
    }
    max_y = MAX(max_y, rec[i].y + rec[i].h);
  }
  return max_y;
}
// 满足一刀切要求的水平线拍样算法
int packing_FFD(int width, int n, idrect_t* rec)
{
  int i, j, y = 0, x = 0, w = width, max_y = 0;
  for (i = 0; i < n; ++i) {
    rec[i].x = -1;
    rec[i].y = 0;
  }
  //packing_sort(n, rec);
  for (i = 0; i < n;) {
    idrect_t* r = rec + i;
    if (r->x >= 0) {
      ++i;
      continue;
    }
    if (r->w < w) {
      max_y = MAX(max_y, y + r->h);
      r->x = x, r->y = y;
      w -= r->w, x += r->w;
      ++i;
    }
    else {
      for (j = 1; j < n; ++j) {
        idrect_t* r = rec + j;
        if (r->x >= 0) {
          continue;
        }
        if (r->w < w) {
          max_y = MAX(max_y, y + r->h);
          r->x = x, r->y = y;
          w -= r->w, x += r->w;
          break;
        }
      }
      if (j == n) {
        x = 0, y = max_y, w = width;
      }
    }
  }
  return max_y;
}
// 区间(a1, a2), 包含a
#define COVER1(a, b1, b2) ((a)>(b1) && (a)<(b2))
// 区间[a1, a2), 相交[b1, b2)
#define COVER2(a1, a2, b1, b2) ((a1)<(b2) && (b1)<(a2))
#define RECSET(R, X, Y, W, H) ((R).x=X,(R).y=Y,(R).w=W,(R).h=H)
#define RECAREA(R) ((R).w * (R).h)
#define RECX2(R) ((R).x+(R).w)
#define RECY2(R) ((R).y+(R).h)
#define PTINREC(R, X, Y) ((X)>=(R).x && (X)<RECX2(R) && (Y)>=(R).y && (Y)<RECY2(R))
// 改进的FFD算法
int packing_FFD2(int width, int n, idrect_t* rec)
{
  int i, j, y = 0, x = 0, w = width, max_y = 0, isinv = 1;
  int pre_y = 0;
  for (i = 0; i < n; ++i) {
    rec[i].x = -1;
    rec[i].y = 0;
  }
  //packing_sort(n, rec);
  for (i = 0; i < n;) {
    idrect_t* r = rec + i;
    if (r->x >= 0) {
      ++i;
      continue;
    }
    if (r->w < w) {
      max_y = MAX(max_y, y + r->h);
      r->x = isinv ? width - x - r->w : x, r->y = y;
      w -= r->w, x += r->w;
      ++i;
    }
    else {
      for (j = 1; j < n; ++j) {
        idrect_t* r = rec + j;
        if (r->x >= 0) {
          continue;
        }
        if (r->w < w) {
          max_y = MAX(max_y, y + r->h);
          r->x = isinv ? width - x - r->w : x, r->y = y;
          w -= r->w, x += r->w;
          break;
        }
      }
      if (j == n) {
        isinv = !isinv;
        if (isinv) {
          int k, max_y2 = y;
          for (k = 0; k < n; ++k) {
            idrect_t* rk = rec + k;
            if (rk->x > 0 && rk->y == y) {
              for (j = 0; j < n; ++j) {
                idrect_t* rj = rec + j;
                if (rj->x > 0 && rj->y == pre_y && COVER2(rk->x, RECX2(*rk), rj->x, RECX2(*rj))) {
                  max_y2 = MAX(max_y2, RECY2(*rj) + rk->h);
                  if (max_y2 > 1000) {
                    int asdf = 0;
                  }
                }
              }
            }
          }
          if (max_y2 < max_y) {
            max_y = max_y2;
            for (k = 0; k < n; ++k) {
              idrect_t* rk = rec + k;
              if (rk->y == y) {
                rk->y = max_y2 - rk->h;
              }
            }
          }
        }
        pre_y = y;
        x = 0, y = max_y, w = width;
      }
    }
  }
  return max_y;
}



#endif // _PACKING_INL_
