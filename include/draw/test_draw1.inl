
//#include "draw.inl"
//#include "resize.inl"
//#include "test_drawline.inl"
// °´Å¥
IRECT btnrc = {
  10, 10, 100, 40
};
UINT btnitemState = 0;
int g_x = 0, g_y = 0;
void test_draw_event(int event, int x, int y, int flags, void* param)
{
  POINT pt;
  pt.x = x, pt.y = y;
  g_x = x, g_y = y;
  btnitemState = 0;

  if (PtInRect(&btnrc, pt)) {
    btnitemState |= ODS_OVER;

    if (CC_EVENT_LBUTTONDOWN == flags) {
      btnitemState |= ODS_SELECTED;
    }
  }
}
DWORD colortable[] = {
#define COLDEF(R, G, B, NAME) _RGB(R, G, B),
#include "colortable.txt"
#undef COLDEF
};
int test_draw1()
{
  char buf[256];
  int i, ch, h = 800, w = 1024;
  int off[2] = {
    1, - 1
  };
  Image im[1] = {0};
  TDrawDC hDC[1];
  const char* winname = "test_draw";
  int x = w / 2, y = h / 2;
  srand(12);
  newImage(w, h, 4, im);
  HDDCSET_IMAGE(hDC, im);
  cvNamedWindow(winname, 1);
  cvMoveWindow(winname, 0, 0);
  cvSetMouseCallback(winname, test_draw_event, 0);

  //char buf[256];
  for (i = 0;; ++i) {
    DWORD clr, clr2;
    int x = 0, y = 0;
    IRECT rc = iRECT(x, y, hDC->w, y + hDC->h);
    clr = _RGB(255, 0, 0);
    clr2 = _RGB(0, 255, 0);
    x = ((rand() % 4) * 12 + x + w) % w;
    y = ((rand() % 4) * 12 + y + w) % h;
    memset(hDC->data, 255, h * w * 4);

    //test_DrawShade(hDC, ch);
    if (0) {
      COLORREF clrs[1000] = {0};
      // PAINTMENUITEMDATA _pmid;
      glyph_t* gg = &g_glyph_btn_expand_left_2003;
      IRECT rc2 = {0, 0, 100, 20};
      //memset(clrs, 255, sizeof(clrs));
      //PaintGlyph(hDC, 0,0, gg, clrs);
      //DrawColorCtrl(hDC);
      //InitPAINTMENUITEMDATA(&_pmid, 0, &rc2, 0, _T(""), _T(""), NULL, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 0);
      //PaintMenuItem(hDC, &_pmid);
      //PaintMenuExpandButtonXP(hDC, &rc2, 0);
      //Draw3DFrame(hDC, &rc2, _RGB(255, 0, 0), _RGB(0,0,255));
      //DrawGripper(hDC, rc2, 1);
      //DrawCheck(hDC, rc2);
      DrawToolBar(hDC, rc2);
    }

    //test_DrawBarChart(hDC, ch);
    //DrawBackgroundArt(hDC, 0);
    //DrawDiamond(hDC, &rc, _RGB(0, 0, 255));
    if (0) {
      IRECT rc2 = rc;
      RCOFFSET(&rc2, 100, 100, - 100, - 100);
      DrawClock(hDC, &rc2);
      DrawDropArrows(hDC, 100, 0, _RGB(255, 0, 0));
    }

    if (0) {
      char* fn = "power.ico";
      //Image* im2 = loadicofile(fn, 32, 0);
      //TDrawImage(hDC, 0, 0, im2->w, im2->h, im2, 0, 0);
      //savebmpfile("asdf.bmp", im2, 32, 32, 0);
      //free(im2);
    }

    if (0) {
      utime_start(_start_time);
      test_DrawLion(hDC, ch);
      printf("%f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);
      test_DrawHeader(hDC);
      printf("%f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);
      test_DrawNSChart(hDC, ch);
      printf("%f\t", utime_elapsed(_start_time));
    }

    if (1) {
      utime_start(_start_time);
      //test_truetypefont(hDC, ch);
      test_drawaa(hDC, ch);
      printf("%f\t", utime_elapsed(_start_time));
    }

    //test_drawdigi(hDC, ch);
    //test_curve(hDC, ch);
    //test_FillPolyCell(hDC, ch);
    //test_drawctrls(hDC, ch);
    //test_DrawGradient(hDC);
    //test_drawfractal(hDC);
    //test_3DStatic(hDC);
    //test_drawUtils(hDC);
    //test_ClipPolygon(hDC, ch);
    //test_ScanFill(hDC);
    //test_FillTiXin(hDC);
    //test_affineTixin(hDC);
    //test_DrawSphere(hDC);
    //test_drawbase(hDC, ch);
    if (0) {
      utime_start(_start_time);
      test_BRender(hDC, ch);
      printf("%f\t", utime_elapsed(_start_time));
    }

    //TDrawButton(&ddc, &btnrc, btnitemState, "OK", 2, );
    //DrawLeaf2(hDC, x, y, clr);
    //DrawRose(hDC, x, y, clr);
    //Ddaline( hDC, 0, 0, g_x, g_y, clr);
    //Bresenhamline( hDC, 0, 0, g_x, g_y, clr);
    //Midpointline( hDC, 512, 512, g_x, g_y, clr);
    //Midpointcircle( hDC, x, y, x/2, clr, 1);
    //Bresenhamcircle( hDC, x, y, x/2, clr, 1);
    //Midpointellispe( hDC, x, y, 200, 100, clr, 1);
    //ScanFill( hDC, clr);
    if (1) {
      int hh = 40, ww = 40;
      int x = BOUND(g_x - ww / 2, 0, hDC->w - ww), y = BOUND(g_y - hh / 2, 0, hDC->h - hh);
      _snprintf(buf, 256, "%d,%d", g_x, g_y);
      //TTextOut2(hDC, g_x, g_y, buf, _RGB(255,0,0));
      cvNamedWindow("asdf", 0);
      cvResizeWindow("asdf", 800, 800);
      //cvShowImage("asdf", hh, ww, (uchar*)(im+1)+((im->h-hh)/2)*im->bw+((im->w-ww)/2)*4, im->bw, 4);
      cvShowImage("asdf", hh, ww, im->data + (y) *im->step + (x) * 4, im->step, 4);
    }

    cvShowImage(winname, im->height, im->width, im->data, im->step, 4);
    ch = 100;
    //ch=-1;
    ch = cvWaitKey(ch);

    //printf("\n");
    if ('q' == ch) {
      break;
    }

    printf("\n");
  }

  imfree(im);
  return 0;
}

int test_ScanFill(HDDC hDC)
{
  enum {
    MAX_points = 4
  };
  int len = 4;
  COLORREF clr = _RGB(255, 0, 0);
  DPOINT PointSet2[MAX_points] = {
    100, 10, 10, 200, 100, 50, 300, 300
  };
  DPOINT PointSet3[MAX_points] = {
    100, 210, 10, 100, 100, 150, 300, 10
  };
  DPOINT PointSet4[MAX_points] = {
    100, 20, 300, 300, 300, 10, 20, 100
  };
  RandPoints(PointSet3, MAX_points, hDC->w, hDC->h);
  len = MAX_points;
  RandPoints(PointSet4, MAX_points, hDC->w, hDC->h);
  len = MAX_points;
  //len = Graham_scan(PointSet3, PointSet4, len);
  //{utime_start(_start_time);
  //PointFill(hDC, PointSet4, len, clr);
  //printf("%f ", utime_elapsed(_start_time));}
  //cvShowImage(winname, h, w, (uchar*)test_draw_img, w * 4, 4);cvWaitKey(-1);
  //memset(test_draw_img, 0, h * w * 4);
  //ScanFill(hDC, PointSet4, 4, clr);
  {
    utime_start(_start_time);
    //polygonFill(hDC, PointSet4, len, clr);
    //ScanFill(hDC, PointSet4, len, _RGB(0, 255, 0));
    printf("%f\n", utime_elapsed(_start_time));
  }
  //PointFill(hDC, PointSet, MAX_points, clr);
  cvShowImage("fff", hDC->h, hDC->w, (uchar*)(hDC->data), hDC->bw, 4);
  cvWaitKey(- 1);
  return 0;
}
