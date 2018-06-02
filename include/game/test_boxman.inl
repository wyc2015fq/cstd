#include "img/imgio.inl"
//#include "BoxManClass.inl"
enum {
  WALLB,
  NULLWALLB,
  NULLB,
  BOXB,
  BOXATPOINTB,
  POINTB,
  MANB,
  MANATPOINTB,
  MAXMAPSIZEX = 20,
  MAXMAPSIZEY = 20,
};
typedef struct {
  texture_t tex[1];
  IRECT rctex[8];
  char smap[128];
  int w, h;
  char* sstr;
  char m_map[MAXMAPSIZEY][MAXMAPSIZEX];
  char way[2048];
} boxman_t;
bool loadMap(boxman_t* bm, const char* p, int pn, int index)
{
  int i = 0, x, y, value = 0;
  int m_NumberOfBox;
  index = MAX(index, 0);
  for (i = 0; i < pn; ++i) {
    if (p[i] == 'M') {
      if (value == index) {
        //find the map
        int i0 = i, cx = 0, cy = 0;
        bm->w = 0;
        for (; i < pn && '\n' != p[i]; ++i) {}
#if 0
        for (y = 0, x = 0; i < pn && 'M' != p[i]; ++i) {
          for (x = 0; i < pn && x < MAXMAPSIZEX && '\n' != p[i] && '\r' != p[i]; ++cx) {}
          for (; i < pn && '\n' != p[i]; ++i) {}
          if ('\n' == p[i]) {
            cx = MAX(bm->w, cx);
            ++cy;
          }
        }
#endif
        ++i;
        m_NumberOfBox = 0;
        for (y = 0, x = 0; i < pn && 'M' != p[i]; ++i) {
          for (x = 0; i < pn && x < MAXMAPSIZEX && '\n' != p[i] && '\r' != p[i];) {
            bm->m_map[y][x++] = p[i++];
          }
          for (; i < pn && '\n' != p[i]; ++i) {}
          if ('\n' == p[i]) {
            bm->w = MAX(bm->w, x);
            ++y;
            x = 0;
          }
        }
        bm->h = y;
        if (bm->w > MAXMAPSIZEX || bm->h > MAXMAPSIZEY) {
          bm->w = bm->h = 0;
          return false;
        }
        return true;
      }
      ++value;
    }
  }
  return false;
}
#define canmove(x) (' '==(x) || '.'==(x))
#define boxman_boxadd(x)
char* boxman_findman(boxman_t* bm, int* px, int* py)
{
  int x, y;
  *px = -1, *py = -1;
  for (y = 0; y < bm->h; ++y) {
    for (x = 0; x < bm->w; ++x) {
      int t = bm->m_map[y][x];
      if ('^' == t || '*' == t) {
        *px = x, *py = y;
        return &bm->m_map[y][x];
      }
    }
  }
  return NULL;
}
char* boxman_canmove(boxman_t* bm, int direction, int* px, int* py)
{
  int x = *px, y = *py;
  switch (direction) {
  case 'L':
    --x;
    break;
  case 'T':
    --y;
    break;
  case 'R':
    ++x;
    break;
  case 'B':
    ++y;
    break;
  }
  if ((x >= 0 && x < bm->w) && (y >= 0 && y < bm->h)) {
    *px = x, *py = y;
    return &bm->m_map[y][x];
  }
  return NULL;
}
int boxman_changebox(int x, int is_box)
{
#define MANCHANGE(A, B) case A: return B; case B: return A
  if (is_box) {
    switch (x) {
      MANCHANGE(' ', '#');
      MANCHANGE('.', '$');
    }
  }
  else {
    switch (x) {
      MANCHANGE(' ', '^');
      MANCHANGE('.', '*');
    }
  }
#undef MANCHANGE
  return x;
}
int boxman_move(boxman_t* bm, int direction)
{
  int x = 0, y = 0, ret = 0;
  char* por = boxman_findman(bm, &x, &y);
  if (por) {
    char* pod = boxman_canmove(bm, direction, &x, &y);
    if (pod) {
      if (' ' == *pod || '.' == *pod) {
        *pod = boxman_changebox(*pod, 0);
        *por = boxman_changebox(*por, 0);
        ret = 1;
      }
      else if ('#' == *pod || '$' == *pod) {
        int x1 = x, y1 = y;
        char* pob = boxman_canmove(bm, direction, &x1, &y1);
        if (pob) {
          if (' ' == *pob || '.' == *pob) {
            *pob = boxman_changebox(*pob, 1);
            *pod = boxman_changebox(*pod, 1);
            *pod = boxman_changebox(*pod, 0);
            *por = boxman_changebox(*por, 0);
            ret = 1;
          }
        }
      }
    }
  }
  return ret;
}
int boxman(boxman_t* bm)
{
  GETGUIIO2();
  void* c = bm;
  int i, j, hot;
  IRECT rc = {0};
  if (1) {
    if (!calc_rect(100, 100, 0, &rc)) {
      return 0;
    }
  }
  if (ISHOVEREDWIN2()) {
    int x = io->MousePos.x, y = io->MousePos.y;
    hot = iPtInRect(&rc, x, y);
    switch (io->lastMsg) {
    case MSG_LBUTTONUP:
      if (ISHIT(c)) {
        if (hot) {
        }
      }
      break;
    case MSG_LBUTTONDOWN:
      if (hot) {
        SETHIT(c);
        SETFOCUS(c);
      }
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        if (ISHOT(c) != hot) {
          SETHOT2(c, hot);
        }
      }
      break;
    case MSG_KEYDOWN:
      switch (io->lastChar) {
      case KEY_LEFT:
        boxman_move(bm, 'L');
        break;
      case KEY_UP:
        boxman_move(bm, 'T');
        break;
      case KEY_RIGHT:
        boxman_move(bm, 'R');
        break;
      case KEY_DOWN:
        boxman_move(bm, 'B');
        break;
      }
      break;
    }
  }
  if (1) {
    int cx = RCW(bm->rctex);
    int cy = RCH(bm->rctex);
    cx = cy = 50;
    for (i = 0; bm->sstr[i]; ++i) {
      bm->smap[bm->sstr[i]] = i;
    }
    for (i = 0; i < bm->h; ++i) {
      for (j = 0; j < bm->w; ++j) {
        int t = bm->smap[bm->m_map[i][j]];
        IRECT rc1 = iRECT2(rc.l + j * cx, rc.t + i * cy, cx, cy);
        gcRectImageR(ctx, rc1, bm->tex, bm->rctex + t);
      }
    }
  }
  return 0;
}
//#include "map.inl"
// + 墙
// 墙内地板
// - 墙外地板
int test_boxman()
{
  GETGUIIO2();
  char aa1[1] = {0};
  char* aa = aa1;
  static int inited = 0;
  static boxman_t bm[1] = {0};
  static char strmap[20 * 1024];
  static strpair_t res[] =  {
#include "E:/pub/bin/codec/base64/res.txt"
  };

  //test_InitDate();
  if (0 == inited) {
    //char* picpath = "";
    char* mappath = "";
    if (1) {
      mappath = "E:/code/cstd/include/game";
      sys_chdir(mappath);
    }
    //picpath = "E:/code/c/Game/推箱子/hxw_box_man_1017/res/Map.bmp";
    mappath = "E:/code/c/Game/推箱子/hxw_box_man_1017/Map.dat";
    mappath = "map.txt";
    res_loadTexture(res, countof(res), "boxman", bm->tex);
    iRectMatrix(iRECT2(0, 0, bm->tex->w, bm->tex->h), 1, 8, 0, 0, 0, countof(bm->rctex), bm->rctex);
    inited = 1;
    loadfile(mappath, strmap, countof(strmap), 0);
    loadMap(bm, strmap, countof(strmap), 1);
    bm->sstr = "+ -#$.^*";
  }
  set_layout_flags(LF_alighParentLeft | LF_alighParentTop);
  if (0) {
    if (button_ctrl(aa++, "prev map")) {}
    if (button_ctrl(aa++, "next map")) {}
    if (button_ctrl(aa++, "prev step")) {}
    if (button_ctrl(aa++, "next step")) {}
  }
  boxman(bm);
  return 0;
}

