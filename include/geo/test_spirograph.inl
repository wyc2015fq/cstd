
#include "spirograph.inl"


#define TEST_SPIROGRAPHDEF_DEF(TEST_SPIROGRAPHDEF) \
  TEST_SPIROGRAPHDEF(Epicycloid) \
  TEST_SPIROGRAPHDEF(Epitrochoid) \
  TEST_SPIROGRAPHDEF(Hypocycloid) \
  TEST_SPIROGRAPHDEF(Hypotrochoid) \
  TEST_SPIROGRAPHDEF(Lissajous) \
  TEST_SPIROGRAPHDEF(Rose) \
  TEST_SPIROGRAPHDEF(FarrisWheel) \


int gcDrawCyclicCurve(gc_t* g, IRECT rc, BOOL fitrc, CyclicCurve* curve, CyclicCurveGetPoint_f fun, float wline, font_t* font, int div) {
  FPOINT* pt = NULL;
  int j, len = 0;
  fun(curve, 0, NULL, NULL);
  for (j=0; j<curve->npara; ++j) {
    curve->arg[j] = BOUND(curve->arg[j], curve->para[j].minv, curve->para[j].maxv);
  }
  if (div) {
    len = vcgen_curvebase_div(NULL, 0, curve, fun, 5);
    if (len>0) {
      MYREALLOC(pt, len);
      len = vcgen_curvebase_div(pt, 0, curve, fun, 5);
    }
  } else {
    int maxlen = 1000;
    len = vcgen_curvebase_inc(NULL, 0, curve, fun, 64, maxlen);
    MYREALLOC(pt, len);
    len = vcgen_curvebase_inc(pt, 0, curve, fun, 64, maxlen);
  }
  if (fitrc) {
    pt_center(len, pt, rc.l, rc.t, RCW(&rc), RCH(&rc), 1, 1);
  } else {
    pt_add(len, pt, (rc.l+rc.r)/2, (rc.t+rc.b)/2);
  }
  gcPenColor(g, ColorRed);
  wline = BOUND(wline, 0.5, 3);
  gcLineWidth(g, wline);
  gcSolidBrush(g, _rgba(0, 0, 1, 0.2));
  gcPolygon(g, GcOptEOFillStroke, pt, len, 1, GC_FLOAT, sizeof(*pt));
  gcSolidFormatText(g, rc, font, 0, 0, ColorBlack, "%d", len);
  FREE(pt);
  return len;
}

int test_spirograph(ctrl_t* c) {
  SYSIO;
  IRECT rc = c->rc, rcRight;
  int i, j, left_w = 300;
  static int testid = 120;
  static int inited = 0;
  static double t1 = 0;
  static double times[20];
  static int ntime=0;
  static int itime=0;
  static int showgrid = 1;
  static int fitrc = 0;
  static int div = 0;
  double t2 = 0;
  IRECT rcs[256];
  static CyclicCurve curve[1] = {0};
  CyclicCurveGetPoint_f fun;
  const CyclicCurveGetPoint_f graph_fun[] = {
#define TEST_SPIROGRAPHDEF(FUN)   FUN,
    TEST_SPIROGRAPHDEF_DEF(TEST_SPIROGRAPHDEF)
#undef TEST_SPIROGRAPHDEF
  };
  iRectCutR(&rc, -left_w, 0, &rcRight);
  testid = BOUND(testid, 0, countof(graph_fun)-1);
  fun = graph_fun[testid];
  {
    utime_start(time1);
    //graph_fun[testid](cc);
    if (showgrid) {
      int row = 1+(int)BOUND(curve->arg[0], 0, 15);
      int col = 1+(int)BOUND(curve->arg[1], 0, 15);
      iRectMatrix(rc, row, col, 4, 4, 0, countof(rcs), rcs);
      for (i=0; i<row; ++i) {
        for (j=0; j<col; ++j) {
          curve->arg[0] = i + 1;
          curve->arg[1] = j + 1;
          gcDrawCyclicCurve(g, rcs[i*col + j], true, curve, fun, 1, io->font, div);
        }
      }
      curve->arg[0] = row-1;
      curve->arg[1] = col-1;
    } else {
      gcDrawCyclicCurve(g, rc, fitrc, curve, fun, 2, io->font, div);
    }
    t1 = utime_elapsed(time1);
    QUEUE_PUSH(times, ntime, itime, t1);
    QUEUE_SUM(times, ntime, t2);
    t2/=ntime;
  }
  //gcResetPath(g);
  gcPenAlignment(g, PenAlignmentInset);
  {
    static ctrl_t cc[20] = {0};
    const char* s_test = "|"
#define TEST_SPIROGRAPHDEF(FUN)   #FUN "|"
      TEST_SPIROGRAPHDEF_DEF(TEST_SPIROGRAPHDEF)
#undef TEST_SPIROGRAPHDEF
      ;
    
    i = 0;
    flexboxlay(rcRight, countof(cc), cc, 2, flex_column);
    fmtlabel_ctrl(cc + i++, "time1 %5.2fms q=%d", t2 * 1000, curve->q);
    if (!inited || rbox_ctrl(cc + i++, "test_spirograph", s_test, &testid)) {
      inited = 1;
      testid = BOUND(testid, 0, countof(graph_fun)-1);
      fun = graph_fun[testid];
      fun(curve, 0, NULL, NULL);
      for (j=0; j<curve->npara; ++j) {
        curve->arg[j] = (curve->para[j].minv + curve->para[j].maxv)/2;
        if (curve->para[j].isint) {
          curve->arg[j] = (int)(curve->arg[j]);
        }
      }
    }
    cbox_ctrl(cc + i++, "fitrc", &fitrc);
    cbox_ctrl(cc + i++, "div", &div);
    cbox_ctrl(cc + i++, "showgrid", &showgrid);
    if (curve->formula) {
      fmtlabel_ctrl(cc + i++, "%s", curve->formula);
    }
    if (curve->para) {
      for (j=0; j<curve->npara; ++j) {
        char fmt[30] = {0};
        strcat(fmt, curve->para[j].name);
        if (curve->para[j].isint) {
          strcat(fmt, " %d");
        } else {
          strcat(fmt, " %.2f");
        }
        slider_float_impl(cc+i++, fmt, curve->arg+j, curve->para[j].minv, curve->para[j].maxv, curve->para[j].isint);
      }
    }
  }
  return 0;
}
