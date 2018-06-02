//#include "img/imgio.inl"
int test_hatch(ctrl_t* c)
{
  SYSIO;
  IRECT rc = c->rc, rcLeft;
  int i, left_w = 200;
  static int hatchstyle = HatchStyleHorizontalBrick;
  double t1 = 0;
  static int inited = 0;
  bool flip_y = 0;
  if (!inited) {
#if 0
    if (0) {
      img_t im[1] = {0};
      uchar* data = (uchar*)HatchBrushes[HatchStyleZigZag];
      inited = 1;
      imread("D:/temp/ZigZag.png", 1, 1, im);
      {
        int x, y;
        int imin = 255, imax = 0, thd = 128, offy = 18, offx = 32, ttt = 0;
        for (y = 0; y < 8; ++y) {
          uchar* pix = im->tt.data + (y + offy) * im->s + offx;
          for (x = 0; x < 8; ++x) {
            if (pix[x] > imax) {
              imax = pix[x];
            }
            else if (pix[x] < imin) {
              imin = pix[x];
            }
          }
        }
        //imshow(im);cvWaitKey(-1);
        thd = (imin + imax) >> 1;
        for (y = 0; y < 8; ++y) {
          uchar* pix = im->tt.data + (y + offy) * im->s + offx;
          uchar datay = 0;
          for (x = 0; x < 8; ++x) {
            int t = !(pix[x] > thd);
            datay |= (t) << (x);
            printf(t ? "O" : "_");
          }
          data[y] = datay;
          printf("\n");
        }
        for (y = 0; y < 8; ++y) {
          printf("0x%02x, ", data[y]);
        }
        printf("\n");
      }
      //imshow(im);cvWaitKey(-1);
      imfree(im);
    }
#endif
  }
  iRectCutT(&rc, 20, 0, &rcLeft);
  {
    static ctrl_t cc[10] = {0};
    IRECT rcs[HatchStyleMax];
    static const char* s_hatch[] = {
#define HATCHSTYLEDEF(data0, data1, data2, data3, data4, data5, data6, data7, name, str) #name ,
      HATCHSTYLEDEF_DEF(HATCHSTYLEDEF)
#undef HATCHSTYLEDEF
    };
    flexboxlay(rcLeft, countof(cc), cc, 2, flex_column);
    iRectMatrix(rc, ((HatchStyleMax + 7) / 8), 8, 4, 2, 2, HatchStyleMax, rcs);
    t1 = 0;
    for (i = 0; i < HatchStyleMax; ++i) {
      IRECT rc1 = rcs[i], rc2;
      iRectCutB(&rc1, -20, 0, &rc2);
      gcSolidText(g, rc2, s_hatch[i], NULL, io->font, 0, TF_CENTER | TF_VCENTER, ColorBlack);
      gcEdgeRect1(g, rc1, 0, ColorBlack, 1);
      {
        spanv_t sp[1] = {0};
        utime_start(_start_time);
        spanv_set_rect(sp, im, NULL, rc1.l, rc1.t, RCW(&rc1), RCH(&rc1));
        spanv_hatch(sp, im, flip_y, 0, 0, _rgba(1, 0, 0, 1), _rgba(0, 1, 0, 1), g_hatch_datas[i]);
        spanv_free(sp);
        t1 += utime_elapsed(_start_time);
      }
      //solid_fillspan(im, sp, _rgba(0, 0, 1, 0.5));
    }
    fmtlabel_ctrl(cc, "time1 %5.2fms", t1 * 1000);
    //rbox_ctrl(cc + i++, "hatchstyle", s_hatch, &hatchstyle);
  }
  return 0;
}

