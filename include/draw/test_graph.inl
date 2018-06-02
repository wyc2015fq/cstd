
#include "Graph.h"
#include "draw3d.inl"

typedef struct test_graph_t {
  uictrl base;
  UI_engine ui[1];
  CGraph testGraph[1];
  float m[16];
  int hit;
  POINT pt;
} test_graph_t;

int test_graph_paint_proc(test_graph_t* s, const event* e)
{
  UIHDC(e);
  draw_clear(hDC, _RGB(0, 0, 0));
  draw_clear(hDC, _RGB(255, 255, 255));


  test_freeman(hDC);
  //DrawCube(hDC, s->base.rc, s->m);
  //s->testGraph->DrawGraph(hDC, s->base.rc);
  return 0;
}

int test_graph_frame_proc(test_graph_t* s, const event* e)
{
  UIEVT(e);

  switch (e->msg) {
  case WM_TIMER:
    force_redraw(e);
    break;

  case EVENT_EXIT:
    if (e->sender == s) {
      printf("EVENT_EXIT %x\n", e->sender);
      PostQuitMessage(0);
    }

    break;

  case EVENT_INIT:
    if (e->sender == s) {
      Win_MoveCenter(s->ui->hwnd, 800, 600);
      SetTimer(s->ui->hwnd, 0, 30, 0);
    }

    break;

  case WM_LBUTTONUP:
    if (s) {
      s->hit = 0;
    }

    break;

  case WM_LBUTTONDOWN:
    if (s) {
      s->hit = 1;
      s->pt = e->pt;
    }

    break;

  case WM_MOUSEMOVE:
    if (s->hit) {
      int dx = e->pt.x - s->pt.x, dy = e->pt.y - s->pt.y;
      float v1[4];
      float v2[4];
      float v3[4] = {0};
      float v4[4];
      float v5[4];
      float m1[16];
      float r;
      V4SET(v1, (float)dx, (float) - dy, 0.f, 1.f);
      V4SET(v2, 0, 0, 1, 1.f);
      V3CROSS(v3, v1, v2);
      m4_inv(s->m, m1);
      M4MULV4(m1, v3, v4);
      M4MULV4(s->m, v4, v5);
      r = (float)V3MAG(v1) / 1.f;
      m4_rotatef(s->m, r, V0(v4), V1(v4), V2(v4));
      s->pt = e->pt;
      force_redraw(e);
    }

    break;

  case EVENT_LCLICK:
    if (e->sender == s) {
    }

    break;

  case WM_SIZE:
    if (1) {
      s->testGraph->hitrect(s->base.rc);
    }

    break;

  }

  return 0;
}

//set up some series
char* sLabels[] = {"day 1", "day 2", "day 3"};
//set the colors of my bars
//set up legend
CGraphLegendSet ls[] = {
#define CGRAPHLEGENDSETDEF(label, clr)  {label, clr, 0,0,0,0,0,0,0,0}
  CGRAPHLEGENDSETDEF("game 1"  , FOREST_GREEN),
  CGRAPHLEGENDSETDEF("game 2"  , SKY_BLUE),
  CGRAPHLEGENDSETDEF("game 3"  , DUSK),
  CGRAPHLEGENDSETDEF("game 4"  , HOT_PINK),
  CGRAPHLEGENDSETDEF("game 5"  , LAVENDER),
  CGRAPHLEGENDSETDEF("game 6"  , ROYAL_BLUE),
  //CGRAPHLEGENDSETDEF("game 7"  , BROWN),
  //CGRAPHLEGENDSETDEF("game 8"  , MAROON),
  //CGRAPHLEGENDSETDEF("game 9"  , GREY),
  //CGRAPHLEGENDSETDEF("game 10" , TAN),
};

int data[countof(sLabels)*countof(ls)] = {0};

BOOL test_graph_init(CGraph* testGraph)
{
  int i = 0;

  for (i = 0; i < countof(sLabels)*countof(ls); ++i) {
    data[i] = 100 + rand() % 200;
  }

  testGraph->set(BAR_GRAPH_3D);
  testGraph->graphTitle = ("Bar Chart");

  testGraph->axisXLabel = ("Games");
  testGraph->axisYLabel = ("Scores");

  testGraph->legend = ls;
  testGraph->nlegend = countof(ls);
  testGraph->nseries = countof(sLabels);
  testGraph->seriesLabel = sLabels;
  testGraph->datay = data;
  testGraph->graphHasLegend = TRUE;
  return TRUE;
}

int test_graph()
{
  test_graph_t s[1] = {0};

  M4ID(s->m);
  test_graph_init(s->testGraph);

  UISETCALL(s, test_graph_paint_proc, test_graph_frame_proc);
  UI_set(s->ui, "test_graph", &s->base, 0);

  waitkey(s->ui, -1);
  res_free();
  return 0;
}
