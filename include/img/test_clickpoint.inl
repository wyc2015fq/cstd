
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cstd.h"
#include "vkeydef.h"
#include "cstd.h"
#include "macro.h"
#include "imresize.h"
#include "findfile.h"
#include "../fmtimg/fmtimg.inl"
#include "../imdraw/window.inl"
#include "../imdraw/imdraw.inl"
#include "subsystem.h"
#include "imopt.inl"
//#include "winuser.h"

int draw_id = 1;
int radius = 5; // 点半径
#define SAFE_FREE(ptr)  if (ptr) { pfree(ptr); ptr = NULL; }

//#define PTNUM 85
#define PTNUM 109

typedef struct STDFACE {
  char* stdpicname;
  int point_number;
  IPOINT strpt[ 200 ];
}
STDFACE;

STDFACE stdface = {
#if 85==PTNUM
#include "std109.txt"
#endif
#if 109==PTNUM
#include "std109.txt"
#endif
};

int bihuan[][ 2 ] = {
#if 85==PTNUM
#include "标点闭环85.txt"
#endif
#if 109==PTNUM
#include ".\标点闭环109.txt"
#endif
};

//#undef ENDL
//#define ENDL "\r\n"

int selectedptid = -1;
int savept(const char* txtfname, const STDFACE* p, int cnt, float sc)
{
  int i, j;
  FILE* pf = fopen(txtfname, "wb");

  if (!pf) {
    return 0;
  }

  for (i = 0; i < cnt; ++i) {
    fprintf(pf, "%s %3d", p[ i ].stdpicname, PTNUM);

    for (j = 0; j < PTNUM; ++j) {
      fprintf(pf, " %3d %3d", (int)(p[ i ].strpt[ j ].x / sc), (int)(p[ i ].strpt[ j ].y / sc));
    }

    fprintf(pf, ENDL);
  }

  return 0;
}

int loadpt(const char* txtfname, STDFACE** pfacept)
{
  int i, j, n = 0, it;
  enum {BUFSIZE = 10240};
  char buf[ BUFSIZE ];

  if (pfacept && file_exist(txtfname)) {
    STDFACE* facept = NULL;
    FILE* pf = fopen(txtfname, "r");

    if (!pf) {
      return 0;
    }

    for (n = 0; fgets(buf, BUFSIZE, pf); ++n) {
      int aaa = 0;
    }

    fseek(pf, 0, SEEK_SET);
    facept = (STDFACE*) pmalloc(sizeof(STDFACE) * n);

    for (i = 0; i < n; ++i) {
      facept[ i ].stdpicname = (char*) pmalloc(256);
      fscanf(pf, "%s\t%d", facept[ i ].stdpicname, &it);
      facept[ i ].point_number = PTNUM;

      for (j = 0; j < PTNUM; ++j) {
        fscanf(pf, "%d %d", &facept[ i ].strpt[ j ].x, &facept[ i ].strpt[ j ].y);
      }
    }

    *pfacept = facept;
    return n;
  }

  return 0;
}

int loadlist(const char* txtfname, STDFACE** pfacept)
{
  int i, n = 0, it = 0;
  char buf[ 1024 ];

  if (pfacept && file_exist(txtfname)) {
    STDFACE* facept = NULL;
    FILE* pf = fopen(txtfname, "r");

    if (!pf) {
      return 0;
    }

    for (n = 0; fgets(buf, 1024, pf);) {
      ++n;
    }

    fseek(pf, 0, SEEK_SET);
    facept = (STDFACE*) pmalloc(sizeof(STDFACE) * n);

    for (i = 0; i < n; ++i) {
      facept[ i ] = stdface;
      fscanf(pf, "%s", buf);
      facept[ i ].stdpicname = (char*) pmalloc(strlen(buf) + 2);
      strcpy(facept[ i ].stdpicname, buf);
      facept[ i ].point_number = PTNUM;
    }

    *pfacept = facept;
    return n;
  }

  return 0;
}


void free_stdface(STDFACE* facept, int cnt)
{
  int i;

  for (i = 0; i < cnt; ++i) {
    pfree(facept[ i ].stdpicname);
  }

  pfree(facept);
}

int draw_face_pt(int h, int w, unsigned char* ptr, int step, int pix_size,
    IPOINT* strpt, int point_number, int radius)
{
  int i;
  int bihuanlen = sizeof(bihuan) / (2 * sizeof(int));

  for (i = 0; i < bihuanlen; ++i) {
    int a = bihuan[ i ][ 0 ], b = bihuan[ i ][ 1 ];
    DrawLine(h, w, ptr, step, pix_size, strpt[ a ], strpt[ b ], CC_RGB(255, 255, 0), 1, 0, 0);

    for (; a < b; ++a) {
      DrawLine(h, w, ptr, step, pix_size, strpt[ a ], strpt[ a + 1 ], CC_RGB(255, 255, 0), 1, 0, 0);
    }
  }

  if (0) {
    // 横线
    int a = 73, b = 84;

    for (; a < b; ++a) {
      IPOINT p0 = {0, strpt[ a ].y}, p1 = {w, strpt[ a ].y};
      DrawLine(h, w, ptr, step, pix_size, p0, p1, CC_RGB(0, 255, 0), 1, 0, 0);
    }
  }

  if (0) {
    // 竖线
    //int sx[] = {7, 11, 22, 26, 64};
    int sx[] = {80, 83, 86};

    for (i = 0; i < sizeof(sx) / sizeof(int); ++i) {
      int a = sx[ i ];
      IPOINT p0 = {strpt[ a ].x, 0}, p1 = {strpt[ a ].x, h};
      DrawLine(h, w, ptr, step, pix_size, p0, p1, CC_RGB(0, 255, 0), 1, 0, 0);
    }
  }

#define PT_COLOR CC_RGB(255, 0, 0)        // 点的颜色
#define SELECT_PT_COLOR CC_RGB(0, 255, 0) // 被选择点的颜色

  for (i = 0; i < point_number; ++i) {
    DrawCircle(h, w, ptr, step, pix_size, strpt[ i ], radius, PT_COLOR, -1, 0, 0);

    if (draw_id) {
      char buf[ 25 ];
      _snprintf(buf, 25, "%d", i);
      xDrawText(h, w, ptr, step, pix_size, strpt[ i ].y, strpt[ i ].x,
          "新宋体", MIN(MAX(radius, 5) + 5, 20), 0, 0, 1, buf, SELECT_PT_COLOR);
    }
  }

  if (selectedptid >= 0 && selectedptid < PTNUM) {
    char buf[ 25 ];
    IPOINT org = {10, 10};
    DrawCircle(h, w, ptr, step, pix_size, strpt[ selectedptid ], radius, SELECT_PT_COLOR, -1, 0, 0);
    _snprintf(buf, 25, "选择点=%3d", selectedptid);
    xDrawText(h, w, ptr, step, pix_size, 0, 0, "新宋体", 40, 0, 0, 1, buf, SELECT_PT_COLOR);
  }

  return 0;
}

int on_mouse(int event, int x, int y, int flags, void* param)
{
  STDFACE* facept = (STDFACE*) param;
  IPOINT* pt = facept->strpt;
  int i;
  int mindist = 5000000;
  int ddd = MAX(radius, 3);   //吸附半径
  ddd = ddd * ddd;

  if (event == CC_EVENT_LBUTTONDOWN) {
    if (selectedptid >= 0 && selectedptid < PTNUM) {
      pt[ selectedptid ].x = x;
      pt[ selectedptid ].y = y;
    }
  }

  if (event == CC_EVENT_MOUSEMOVE) {   //修改控制点坐标
    for (i = 0; i < PTNUM; ++i) {
      int dx = pt[ i ].x - x, dy = pt[ i ].y - y;
      int dd = dx * dx + dy * dy;
      //if (dd < mindist)
      {
        //mindist = dd;
        if (dd < ddd) {
          selectedptid = i;
        }

        if (dd > 1500 * ddd) {
          //selectedptid = -1;
        }
      }
    }
  }

  return 0;
}

void usage()
{
  static const char* help =
      "usage: \n"
      "  q - 退出\n"
      "  ] - 增加点的半径\n"
      "  [ - 减少点的半径\n"
      "  s - 保存（退出时也会自动保存）\n"
      "  l - 载入标准点\n"
      "  h - 显示本帮助\n"
      "  h - 显示本帮助\n"
      "  r或↑ - 向上微调\n"
      "  f或↓ - 向下微调\n"
      "  d或← - 向左微调\n"
      "  g或→ - 向右微调\n"
      "\n"
      ;
  printf(help);
}

int test_clickpoint1()
{
  char ch;
  int high = 0;
  size_t strlistlen = 0, strlistidx = 0;
  int h, w, al, ai = 1;
  int h2, w2, al2, i, j;
  FILE* phis;
  float sc = 2.f;
  unsigned char* img = NULL, *img2 = NULL;
  STDFACE* facept;
  img_t im[1] = {0};
  usage();
  strlistlen = loadpt("out.txt", &facept);

  if (file_exist("his.txt")) {
    phis = fopen("his.txt", "r");

    if (phis) {
      fscanf(phis, "%d", &strlistidx);
      fclose(phis);
    }
  }

  if (!strlistlen) {
    strlistlen = loadlist("list.txt", &facept);
  }

  if (!strlistlen) {
    return 0;
  }

  for (j = 0; j < (int) strlistlen; ++j) {
    for (i = 0; i < PTNUM; ++i) {
      facept[ j ].strpt[ i ].x = (int)(facept[ j ].strpt[ i ].x * sc);
      facept[ j ].strpt[ i ].y = (int)(facept[ j ].strpt[ i ].y * sc);
    }
  }

  cvNamedWindow("pic", 1);

  //cvCreateTrackbar( "high", "pic", &high, 300, 0 );
  for (strlistidx -= 1, ch = 'n'; 'q' != ch && 'Q' != ch;) {
    if ('n' == ch || 'p' == ch) {
      strlistidx += 'n' == ch ? 1 : -1;
      strlistidx %= strlistlen;
      imread(facept[ strlistidx ].stdpicname, 3, 1, im);
      h = im->h, w = im->w, ai = 3, img = im->data;
      //open_img( , &h, &w, &ai, &img );
      printf(facept[ strlistidx ].stdpicname);
      printf("%d/%d\n", strlistidx, strlistlen);
      cvSetMouseCallback("pic", on_mouse, facept + strlistidx);

      if (img) {
        al = w * ai;
        h2 = (int)(h * sc);
        w2 = (int)(w * sc);
        al2 = (int)(al * sc);
        SAFE_FREE(img2);
        img2 = pmalloc(h2 * al2);
      }
    }

    if ('[' == ch || ']' == ch) {
      radius += ']' == ch ? 1 : -1;
      radius = MIN(MAX(radius, 0), 50);
    }

    if ('i' == ch || 'I' == ch) {
      draw_id = !draw_id;
    }

    if ('s' == ch || 'S' == ch) {
      savept("out.txt", facept, strlistlen, sc);
      printf("save to out.txt\n");
    }

    if ('h' == ch || 'H' == ch) {
      usage();
    }

    if ('l' == ch || 'L' == ch) {
      memcpy(facept[ strlistidx ].strpt, stdface.strpt, sizeof(facept[ strlistidx ].strpt));

      for (i = 0; i < PTNUM; ++i) {
        facept[ strlistidx ].strpt[ i ].x = (int)(facept[ strlistidx ].strpt[ i ].x * sc);
        facept[ strlistidx ].strpt[ i ].y = (int)(facept[ strlistidx ].strpt[ i ].y * sc);
      }
    }

    if (0 <= selectedptid && selectedptid < PTNUM) {
      if (VK_UP == ch || 'r' == ch) {
        facept[ strlistidx ].strpt[ selectedptid ].y -= 1;
      }

      if (VK_DOWN == ch || 'f' == ch) {
        facept[ strlistidx ].strpt[ selectedptid ].y += 1;
      }

      if (VK_LEFT == ch || 'd' == ch) {
        facept[ strlistidx ].strpt[ selectedptid ].x -= 1;
      }

      if (VK_RIGHT == ch || 'g' == ch) {
        facept[ strlistidx ].strpt[ selectedptid ].x += 1;
      }
    }

    //memcpy(img2, img, h*al);
    IMRESIZE(h, w, img, al, ai, h2, w2, img2, al2, ai, INTER_BILINEARITY);
    draw_face_pt(h2, w2, img2, al2, ai, facept[ strlistidx ].strpt, facept[ strlistidx ].point_number, radius);
    cvShowImage("pic", h2, w2, img2, al2, ai);
    //imwrite("asdf.bmp", int height, int width, img2, int step, int cn);
    ch = cvWaitKey(30);
  }

  imfree(im);
  savept("out.txt", facept, strlistlen, sc);
  SAFE_FREE(img);
  SAFE_FREE(img2);

  if (facept) {
    free_stdface(facept, strlistlen);
  }

  {
    phis = fopen("his.txt", "w");

    if (phis) {
      fprintf(phis, "%d", strlistidx);
      fclose(phis);
    }
  }

  return 0;
}

typedef struct param_strlist_t {
  strlist_t slist;
  int i;
  int ptnum;
  IPOINT pt[1000];
  int changed;
} param_strlist_t;

double g_sc = 3.;

int del_param_strlist(param_strlist_t* p)
{
  if (p->ptnum > 0) {
    --(p->ptnum);
    p->changed = 1;
  }

  return 0;
}

#define PT_DIST(pt1, pt2)  ((pt1.x-pt2.x)*(pt1.x-pt2.x) + (pt1.y-pt2.y)*(pt1.y-pt2.y))

int pt_find_near(int n, IPOINT* pts, IPOINT pt, double* pmindist)
{
  int i, id = -1;
  double dist, mindist = 10000000;

  for (i = 0; i < n; ++i) {
    dist = PT_DIST(pts[i], pt);

    if (dist < mindist) {
      mindist = dist;
      id = i;
    }
  }

  *pmindist = mindist;
  return id;
}



int pt_get(param_strlist_t* p, char* pathbuf)
{
  int i, tmp;
  char* buf;
  sscanf(p->slist.strs[ p->i ], "%s %d", pathbuf, &tmp);
  buf = p->slist.strs[ p->i ] + strlen(pathbuf);

  for (i = 0; *buf; ++i) {
    p->pt[i].x = strtol(buf, &buf, 10);
    p->pt[i].y = strtol(buf, &buf, 10);
  }

  p->ptnum = i;
  return 0;
}
int pt_put(param_strlist_t* p, const char* pathbuf)
{
  int i;
  char buf[4096];
  char buf2[256];
  _snprintf(buf, 4096, "%s ", pathbuf);

  for (i = 0; i < p->ptnum; ++i) {
    _snprintf(buf2, 4096, "%d %d ", p->pt[i].x, p->pt[i].y);
    strcat(buf, buf2);
  }

  SAFE_FREE(p->slist.strs[ p->i ]);
  p->slist.strs[ p->i ] = STRDUP(buf);
  return 0;
}
int id_select = -1;
int id_click = -1;
int on_mouse2(int event, int x, int y, int flags, void* param)
{
  param_strlist_t* p = ((param_strlist_t*) param);
  ASSERT(p->i < p->slist.used);
  x = (int)(x / g_sc);
  y = (int)(y / g_sc);

  if (event == CC_EVENT_LBUTTONDOWN) {
    if (-1 == id_select) {
      p->pt[p->ptnum].x = x;
      p->pt[p->ptnum].y = y;
      ++(p->ptnum);
      p->changed = 1;
    }
    else {
      id_click = id_select;
    }
  }

  if (event == CC_EVENT_MOUSEMOVE) {
    IPOINT pt = {x, y};
    double dist;

    if (id_click == -1) {
      int id = pt_find_near(p->ptnum, p->pt, pt, &dist);
      id_select = -1;

      if (dist < 10) {
        id_select = id;
      }
    }

    if (id_click != -1) {
      p->pt[id_click].x = x;
      p->pt[id_click].y = y;
    }
  }

  if (event == CC_EVENT_LBUTTONUP) {
    p->changed = 1;
    id_click = -1;
  }

  return 0;
}
int is_show_num = 1;
int test_clickpoint()
{
  char pathbuf[256];
  int i;
  img_t a[1] = {0};
  int frames = 0;
  int radius = 3;
  param_strlist_t p[1] = {0};
#define TXTPATH1 "F:/pub/bin/点点的工具/虹膜/list.txt"
#define TXTPATH "./list.txt"
  get_file_list_txt(TXTPATH, &p->slist);
  cvNamedWindow("test_clickpoint", 1);
  cvSetMouseCallback("test_clickpoint", on_mouse2, p);

  //get_file_list("./*.*", &slist);
  for (p->i = 0; p->slist.used; ++frames) {
    char ch;
    ch = cvWaitKey(50);
    ch = tolower(ch);

    if (0 == a->h) {
      int ib = p->i;
      p->i = (p->i + p->slist.used) % p->slist.used;

      for (; ;) {
        pt_get(p, pathbuf);
        cvSetWindowText("test_clickpoint", pathbuf);
        imread(pathbuf, 3, 1, a);

        if (ib == p->i) {
          break;
        }

        if (NULL != a) {
          break;
        }

        p->i = (p->i + 1 + p->slist.used) % p->slist.used;
      }
    }

    if ('n' == ch || 'p' == ch || ' ' == ch) {
      imfree(a);
      p->i += 'n' == ch ? 1 : -1;
      p->i = (p->i + p->slist.used) % p->slist.used;
      ch = 's';
    }

    if (0 == frames % 100) {
      ch = 's';
    }

    if ('z' == ch) {
      del_param_strlist(p);
    }

    if ('+' == ch) {
      g_sc = MIN(3, g_sc * 1.1);
    }

    if ('-' == ch) {
      g_sc = MAX(0.1, g_sc / 1.1);
    }

    if ('q' == ch || 's' == ch || 'z' == ch || -2 == ch || 1 == p->changed) {
      if (1 == p->changed) {
        FILE* pf = fopen(TXTPATH, "wb");
        p->changed = 0;
        pt_put(p, pathbuf);

        if (pf) {
          for (i = 0; i < p->slist.used; ++i) {
            fprintf(pf, "%s\r\n", p->slist.strs[ i ]);
          }

          fclose(pf);
        }
      }

      if ('q' == ch) {
        imfree(a);
        break;
      }
    }

    if (-2 == ch) {
      break;
    }

    if ('t' == ch) {
      is_show_num = !is_show_num;
    }

    if ('[' == ch) {
      radius = MAX(1, radius - 1);
    }

    if (']' == ch) {
      radius = MIN(6, radius + 1);
    }

    if (a->h) {
      //printf( "%s\n", p->slist.strs[ i ] );
      //_snprintf( pathbuf, 256, "%s_out.bmp", slist.strs[ i ] );
      //imwrite( pathbuf, a->h, a->w, a->data, a->s, a->c );
      img_t b[1] = {0};
      imresize2(a, g_sc, b);

      for (i = 0; i < p->ptnum; ++i) {
        IPOINT pt = {p->pt[i].x * g_sc, p->pt[i].y * g_sc};
        DrawCircle(b->h, b->w, b->data, b->s, b->c, pt, radius, i == id_select ? _RGB(0, 255, 0) : PT_COLOR, -1, 0, 0);

        if (is_show_num) {
          char buf[ 25 ];
          _snprintf(buf, 25, "%d", (i + 1));
          xDrawText(b->h, b->w, b->data, b->s, b->c, pt.y, pt.x,
              "新宋体", MIN(MAX(radius, 5) + 5, 20), 0, 0, 1, buf, SELECT_PT_COLOR);
        }
      }

      imshow2("test_clickpoint", b);
      imfree(b);
    }
  }

  imfree(a);
  free_strlist(&p->slist);
  return 0;
}

