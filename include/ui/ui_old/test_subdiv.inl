
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // for time
#include "cstd.h"

//#include "geometry.inl"
#include "geo/subdiv.h"
#include "ui/window.inl"


#define PT_NUM (10000)
//   convex_hull2
// PT_NUM    time(s)
//   1000  0.000374908
//  10000  0.005043378
// 100000  0.130528880

CvPoint2D32f pt[ PT_NUM ];
CvPoint2D32f pt2[ PT_NUM ];
myCvSubdiv2D* subdiv;
IRECT rect = {0, 0, 640, 480};

#define PT_IN_RECT(PT, RECT)  ( ((PT).x>(RECT).x) && ((PT).x<((RECT).x+(RECT).w)) &&\
    ((PT).y>(RECT).y) && ((PT).y<((RECT).y+(RECT).h)) )

int ptlen = 0;
int ptlen2 = 0;

// 卷包裹法求点集凸壳, 参数说明同 graham 算法
//int convex_closure(int setnum, const CvPoint2D32f* pointset, CvPoint2D32f* ch);
//int graham_scan(int setnum, const CvPoint2D32f* pointset, CvPoint2D32f* ch);
//int convex_hull2(int total, const CvPoint2D32f* array, CvPoint2D32f* hull_storage, int orientation);

#define CONVEX_HULL2(total, array, hull_storage, ptlen2)                      \
  do {                                                                          \
    utime_start(_start_count);                                                  \
    ptlen2 = convex_hull2(total, array, hull_storage, 1);                       \
    printf(" %13.9f\n", utime_elapsed(_start_count));                           \
  } while(0)

void on_mouse(int event, int x, int y, int flags, void* param)
{
  if (event == CV_EVENT_LBUTTONDOWN && ptlen < 100) {
    pt[ ptlen ].x = (real) x;
    pt[ ptlen ].y = (real) y;
    cvSubdivDelaunay2DInsert(subdiv, pt[ ptlen ]);
    ++ptlen;
    printf("%3d ", subdiv->edge_num);
    printf("(%3d, %3d)", x, y);
    //CONVEX_HULL2(ptlen, pt, pt2, ptlen2);
  }
}
int g_iclr = 0;
void draw_subdiv_facet(int h, int w, unsigned char* ptr, int step, int pix_size, CvSubdiv2DEdge edge)
{
  CvSubdiv2DEdge t = edge;
  int i, count = 0;
  IPOINT* buf = 0;

  // count number of edges in facet
  do {
    count++;
    t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_DST);
  }
  while (t != edge);

  buf = (IPOINT*) malloc(count * sizeof(buf[ 0 ]));

  // gather points
  t = edge;

  for (i = 0; i < count; i++) {
    CvPoint2D32f* pt = CV_SUBDIV2D_EDGE_ORG(t);

    if (!pt) {
      break;
    }

    if (!iPtInRect(&rect, pt->x, pt->y)) {
      break;
    }

    buf[ i ].x = (int)(pt->x);
    buf[ i ].y = (int)(pt->y);
    t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);
  }

#define DEFULT_COLOR g_ColTable[g_iclr=(g_iclr++)%countof(g_ColTable)].colVal

  if (i == count) {
    CvPoint2D32f* pt = CV_SUBDIV2D_EDGE_DST(CV_SUBDIV2D_ROTATE_EDGE(edge, 1));
    FillConvexPoly(h, w, ptr, step, pix_size, buf, count, DEFULT_COLOR, CV_AA, 0);
    //PolyLine( img, &buf, &count, 1, 1, CV_RGB(0,0,0), 1, CV_AA, 0);
    //draw_subdiv_point( img, pt->pt, CV_RGB(0,0,0));
  }

  free(buf);
}

int draw_face_pt(int h, int w, unsigned char* ptr, int step, int pix_size)
{
  int i;
  int radius = 2;
#define DEFULT_COLOR_SET(x) g_iclr=x;
  DEFULT_COLOR_SET(0);

  //cvCalcSubdivVoronoi2D( subdiv );
  for (i = 3; i < subdiv->edge_num; ++i) {
    CvPoint2D32f* org_pt;
    CvPoint2D32f* dst_pt;
    CvQuadEdge2D* edge = (CvQuadEdge2D*)(subdiv->edges + i);
    org_pt = CV_SUBDIV2D_EDGE_ORG(edge);
    dst_pt = CV_SUBDIV2D_EDGE_DST(edge);

    if (org_pt && dst_pt) {
      IPOINT pta = {XFLOOR(org_pt[ 0 ].x), XFLOOR(org_pt[ 0 ].y) };
      IPOINT ptb = {XFLOOR(dst_pt[ 0 ].x), XFLOOR(dst_pt[ 0 ].y) };

      if (PT_IN_RECT(pta, rect) && PT_IN_RECT(ptb, rect)) {
        draw_subdiv_facet(h, w, ptr, step, pix_size, (CvSubdiv2DEdge) edge);
        DrawLine(h, w, ptr, step, pix_size, pta, ptb, CV_RGB(255, 123, 123), 1, 0, 0);
      }
    }
  }

  for (i = 0; i < ptlen2; ++i) {
    int j = (i + 1) % ptlen2;
    IPOINT pta = {XFLOOR(pt2[ i ].x), XFLOOR(pt2[ i ].y) };
    IPOINT ptb = {XFLOOR(pt2[ j ].x), XFLOOR(pt2[ j ].y) };
    DrawLine(h, w, ptr, step, pix_size, pta, ptb, CV_RGB(255, 255, 0), 1, 0, 0);
  }

  for (i = 0; i < ptlen; ++i) {
    DrawCircle(h, w, ptr, step, pix_size, pt[ i ].x, pt[ i ].y, radius, CV_RGB(255, 0, 0), -1, 0, 0);
  }

  return 0;
}

int test_subdiv()
{
  int h = rect.h, w = rect.w, ai = 3, al = w * ai, i, autotest = 0, j;
  unsigned char* img = MALLOC(unsigned char, h * al);
  char ch = 0;
  subdiv = CreateSubdivDelaunay2D(1000, rect);
  srand(time(0));
  cvNamedWindow("cam", 1);
  cvSetMouseCallback("cam", on_mouse, pt);

  for (i = 0; (ch) != 'q' && (ch) != 'Q'; ++i) {
    //IPOINT ipp[3] = {{0,0},{100,0},{0,100}};
    memset(img, 0, h * al);
    //FillConvexPoly( h, w, img, al, ai, ipp, 3, CV_RGB(55, 123, 123), CV_AA, 0 );
    //cvShowImage("cam", h, w, img, al, ai);
    //cvWaitKey(0);
    draw_face_pt(h, w, img, al, ai);
    cvShowImage("cam", h, w, img, al, ai);
    ch = cvWaitKey(20);

    if ('c' == ch) {
      memset(pt, 0, sizeof(CvPoint2D32f));
      ptlen = 0;
    }

    if ('a' == ch) {
      autotest = !autotest;
      ptlen = autotest ? PT_NUM : 0;
    }

    if (autotest) {
      //ptlen = UNIFORM(PT_NUM);
      for (j = 0; j < ptlen; ++j) {
        pt[ j ].x = UNIFORM01() * w;
        pt[ j ].y = UNIFORM01() * h;
      }

      printf("%5d", ptlen);
      CONVEX_HULL2(ptlen, pt, pt2, ptlen2);
    }
  }

  cvClearSubdivVoronoi2D(subdiv);
  return 0;
}

