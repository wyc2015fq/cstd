typedef struct test_buttonst_t {
  uictrl base;
  UI_engine ui[1];
  tab_ctrl m_tab[1];
  matrix_layout_ctrl m_lay[5];
  button_ctrl m_btn[1];
#define ASDFSADF(x) buttonst m_btn##x[1]
  ASDFSADF(Standard);
  ASDFSADF(Halloween);
  ASDFSADF(KeyManager);
  ASDFSADF(Disabled);
  ASDFSADF(Lamp);
  ASDFSADF(Zip);
  ASDFSADF(Jpeg);
  ASDFSADF(CDRom);
  ASDFSADF(Tooltip);
  ASDFSADF(Search);
  ASDFSADF(Back);
  ASDFSADF(Next);
  ASDFSADF(Derived);
  ASDFSADF(HyperLink);
  ASDFSADF(Cursor);
  ASDFSADF(FocusRect);
  ASDFSADF(m_chkCheckbox);
  ASDFSADF(Bitmap);
  ASDFSADF(LogOff);
  ASDFSADF(Workgroup);
  ASDFSADF(Open);
  ASDFSADF(Search2);
  ASDFSADF(Explorer);
  ASDFSADF(Help);
  ASDFSADF(About);
  ASDFSADF(Cannibal);
  ASDFSADF(m_chkCheckbox2);
  ASDFSADF(Metal);
  ASDFSADF(Noise);
  ASDFSADF(V_Shade);
  ASDFSADF(HardBump);
  ASDFSADF(V_Bump);
  ASDFSADF(H_Shade);
  ASDFSADF(SoftBump);
  ASDFSADF(H_Bump);
  ASDFSADF(Diagonal);
#undef ASDFSADF
} test_buttonst_t;
int test_buttonst_paint_proc(test_buttonst_t* g, const event* e)
{
  UIHDC(e);
  //RECT rcClient;
  //UIGetWorkArea(g->ui, &g->m_tab->base.rect);
  draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));
  drawaa_tab_ctrl(g->m_tab, e, prc);
  return 0;
}
int test_buttonst_frame_proc(test_buttonst_t* g, const event* e)
{
  UI_engine* ui = e->ui;

  switch (e->msg) {
  case EVENT_LCLICK:
    if (e->sender) {
      printf("EVENT_LCLICK %d\n", e->wp);
    }

    break;
  }

  tab_ctrl_event(g->m_tab, e, prc);
  return 0;
}
#define RESDEF(x) "D:/code/c/button/CButtonST_demo/res/"#x
#define IDI_CANCEL1 RESDEF(Cancel1_32x32x16.png)
#define IDI_CANCEL4 RESDEF(Cancel4_32x32x2.png)
#define IDI_EOAPP RESDEF(EOapp.png)
#define IDI_HALLOWEEN1 RESDEF(Halloween1.png)
#define IDI_HALLOWEEN2 RESDEF(Halloween2.png)
#define IDI_KEYMANAGER RESDEF(Key manager.png)
#define IDI_SOUND RESDEF(Sound.png)
#define IDI_LAMP1 RESDEF(Lamp1.png)
#define IDI_ZIP1 RESDEF(Winzip1.png)
#define IDI_JPEG RESDEF(JPEG Image.png)
#define IDI_BALOON RESDEF(Baloon.png)
#define IDI_CDROM RESDEF(CDRom.png)
#define IDI_SEARCH1 RESDEF(Search1.png)
#define IDI_WORKGROUP RESDEF(Workgroup.png)
#define IDI_LOGOFF RESDEF(LogOff.png)
#define IDI_OPEN RESDEF(Open.png)
#define IDI_HELP RESDEF(Help.png)
#define IDI_EXPLORER RESDEF(Explorer.png)
#define IDI_ABOUT RESDEF(About.png)
#define IDI_LEDON RESDEF(LedOn.png)
#define IDI_LEDOFF RESDEF(LedOff.png)
#define IDI_WEB2 RESDEF(Web2.png)
#define IDI_TOOLS4 RESDEF(Tools4.png)
#define IDI_RUN RESDEF(Run.png)
#define IDI_OK3 RESDEF(Ok3_32x32x256.png)
#define IDI_CANCEL3 RESDEF(Cancel3_32x32x256.png)
#define IDI_IEDOCUMENT RESDEF(IEDocument_48x48x256.png)
#define IDI_HELP2 RESDEF(Help2_32x32x256.png)
#define IDI_RAZOR RESDEF(Razor_32x32x256.png)
#define IDI_CLASSES1 RESDEF(Classes1_32x32x16.png)
#define IDI_NO3 RESDEF(No3_32x32x256.png)
#define IDI_RIGHT6 RESDEF(Right6_32x32x256.png)
#define IDI_LEFT6 RESDEF(Left6_32x32x256.png)
#define IDI_BUTTERFLY RESDEF(Butterfly.png)
#define IDB_BMPBACK RESDEF(BmpBack.bmp)
#define IDB_SKY RESDEF(Sky.bmp)
#define IDB_CANNIBAL RESDEF(Cannibal.bmp)
#define IDB_STLOGO RESDEF(STLogo_Small.bmp)
#define IDB_FACE RESDEF(Face.bmp)
#define IDR_TOOLBAR RESDEF(toolbar.bmp)
#define IDB_BUTTON RESDEF(Button.bmp)
int test_buttonst()
{
  test_buttonst_t g[1] = {0};
  matrix_layout_item li[100] = {0};
  tab_item ti[] = {
    -1, _T("Basic features"), &g->m_lay[0].base,
    -1, _T("Advanced features"), &g->m_lay[1].base,
    -1, _T("Transparent effect"), &g->m_lay[2].base,
    -1, _T("CShadeButtonST"), &g->m_lay[3].base,
    -1, _T("About"), &g->m_lay[4].base,
  };
  tab_ctrl_set(g->m_tab, countof(ti), ti);
  g->m_tab->cur = 3;
  {
#define ASDFSADF(x) li[n].subctrl=&g->m_btn##x->base, buttonst_set(g->m_btn##x, #x), ++n
    int b = 0, n = 0, k = 0;
    COLORREF clrtrans = 0xffc8c8c8;
    ASDFSADF(Standard);
    ASDFSADF(Halloween);
    ASDFSADF(KeyManager);
    ASDFSADF(Disabled);
    ASDFSADF(Lamp);
    ASDFSADF(Zip);
    ASDFSADF(Jpeg);
    ASDFSADF(CDRom);
    ASDFSADF(Tooltip);
    ASDFSADF(Search);
    ASDFSADF(Back);
    ASDFSADF(Next);
    {
      short shBtnColor = 30;
      char* ss[10];
      // Standard button
      GetImage(g->m_btnStandard->m_csBitmaps, IDI_EOAPP, clrtrans);
      g->m_btnStandard->m_bIsFlat = (FALSE);
      // Halloween button
      ARRINIT2(ss, IDI_HALLOWEEN2, IDI_HALLOWEEN1);
      GetImages(g->m_btnHalloween->m_csBitmaps, 2, ss, clrtrans);
      // 48x48 icon button
      GetImage(g->m_btnKeyManager->m_csBitmaps, IDI_KEYMANAGER, clrtrans);
      buttonst_offsetcolor(g->m_btnKeyManager, BTNST_COLOR_IN, shBtnColor);
      // Disabled button
      GetImage(g->m_btnDisabled->m_csBitmaps, IDI_SOUND, clrtrans);
      buttonst_offsetcolor(g->m_btnDisabled, BTNST_COLOR_IN, shBtnColor);
      // Lamp button
      GetImage(g->m_btnLamp->m_csBitmaps, IDI_LAMP1, clrtrans);
      g->m_btnLamp->m_nBitmapOut = BTNST_AUTO_GRAY;
      g->m_btnLamp->m_bDrawBorder = (FALSE);
      // Zip button
      GetImage(g->m_btnZip->m_csBitmaps, IDI_ZIP1, clrtrans);
      buttonst_offsetcolor(g->m_btnZip, BTNST_COLOR_IN, shBtnColor);
      // Jpeg button
      GetImage(g->m_btnJpeg->m_csBitmaps, IDI_JPEG, clrtrans);
      buttonst_offsetcolor(g->m_btnJpeg, BTNST_COLOR_IN, shBtnColor);
      // CDRom button
      GetImage(g->m_btnCDRom->m_csBitmaps, IDI_CDROM, clrtrans);
      g->m_btnCDRom->m_bDrawBorder = (FALSE);
      // Tooltip button
      GetImage(g->m_btnTooltip->m_csBitmaps, IDI_BALOON, clrtrans);
      buttonst_offsetcolor(g->m_btnTooltip, BTNST_COLOR_IN, shBtnColor);
      // Search button
      GetImage(g->m_btnSearch->m_csBitmaps, IDI_SEARCH1, clrtrans);
      buttonst_offsetcolor(g->m_btnSearch, BTNST_COLOR_IN, shBtnColor);
      g->m_btnSearch->m_byAlign = (ST_ALIGN_VERT);
      // Back button
      GetImage(g->m_btnBack->m_csBitmaps, IDI_LEFT6, clrtrans);
      buttonst_offsetcolor(g->m_btnBack, BTNST_COLOR_IN, shBtnColor);
      // Next button
      GetImage(g->m_btnNext->m_csBitmaps, IDI_RIGHT6, clrtrans);
      buttonst_offsetcolor(g->m_btnNext, BTNST_COLOR_IN, shBtnColor);
      g->m_btnNext->m_byAlign = (ST_ALIGN_HORIZ_RIGHT);
    }
    matrix_layout_ctrl_set(g->m_lay + k, 0, 4, 4, n - b, li + b), b = n, ++k;
    ASDFSADF(Derived);
    ASDFSADF(HyperLink);
    ASDFSADF(Cursor);
    ASDFSADF(FocusRect);
    ASDFSADF(m_chkCheckbox);
    ASDFSADF(Bitmap);
    matrix_layout_ctrl_set(g->m_lay + k, 0, 4, 4, n - b, li + b), b = n, ++k;
    ASDFSADF(LogOff);
    ASDFSADF(Workgroup);
    ASDFSADF(Open);
    ASDFSADF(Search2);
    ASDFSADF(Explorer);
    ASDFSADF(Help);
    ASDFSADF(About);
    ASDFSADF(Cannibal);
    ASDFSADF(m_chkCheckbox2);
    matrix_layout_ctrl_set(g->m_lay + k, 0, 4, 4, n - b, li + b), b = n, ++k;
    ASDFSADF(Metal);
    ASDFSADF(Noise);
    ASDFSADF(V_Shade);
    ASDFSADF(HardBump);
    ASDFSADF(V_Bump);
    ASDFSADF(H_Shade);
    ASDFSADF(SoftBump);
    ASDFSADF(H_Bump);
    ASDFSADF(Diagonal);
    {
      GetImage(g->m_btnMetal->m_csBitmaps, IDI_RAZOR, clrtrans);
      g->m_btnMetal->shade.id = (SHS_METAL);
      //
      GetImage(g->m_btnHardBump->m_csBitmaps, IDI_HELP, clrtrans);
      g->m_btnHardBump->m_byAlign = (ST_ALIGN_VERT);
      g->m_btnHardBump->shade.id = (SHS_HARDBUMP);
      //
      GetImage(g->m_btnSoftBump->m_csBitmaps, IDI_SEARCH1, clrtrans);
      g->m_btnSoftBump->m_nBitmapOut = BTNST_AUTO_GRAY;
      g->m_btnSoftBump->shade.id = (SHS_SOFTBUMP);
      //
      GetImage(g->m_btnNoise->m_csBitmaps, IDI_OK3, clrtrans);
      g->m_btnNoise->m_byAlign = (ST_ALIGN_VERT);
      shade_style_set(&g->m_btnNoise->shade, SHS_NOISE, 33, 10, 5, _RGB(55, 55, 255));
      //
      GetImage(g->m_btnV_Bump->m_csBitmaps, IDI_HELP2, clrtrans);
      shade_style_set(&g->m_btnV_Bump->shade, SHS_VBUMP, 8, 20, 5, _RGB(55, 55, 255));
      //
      GetImage(g->m_btnH_Bump->m_csBitmaps, IDI_WEB2, clrtrans);
      shade_style_set(&g->m_btnH_Bump->shade, SHS_HBUMP, 8, 20, 5, _RGB(55, 55, 255));
      //
      GetImage(g->m_btnV_Shade->m_csBitmaps, IDI_CANCEL3, clrtrans);
      g->m_btnV_Shade->m_byAlign = (ST_ALIGN_HORIZ_RIGHT);
      shade_style_set(&g->m_btnV_Shade->shade, SHS_VSHADE, 8, 20, 5, _RGB(55, 55, 255));
      //
      GetImage(g->m_btnH_Shade->m_csBitmaps, IDI_RUN, clrtrans);
      shade_style_set(&g->m_btnH_Shade->shade, SHS_HSHADE, 8, 20, 5, _RGB(55, 55, 255));
      //
      GetImage(g->m_btnDiagonal->m_csBitmaps, IDI_IEDOCUMENT, clrtrans);
      shade_style_set(&g->m_btnDiagonal->shade, SHS_DIAGSHADE, 8, 10, 5, _RGB(55, 255, 55));
    }
    matrix_layout_ctrl_set(g->m_lay + k, 0, 3, 4, n - b, li + b), b = n, ++k;
#undef ASDFSADF
  }
  UISETCALL(g, test_buttonst_paint_proc, test_buttonst_frame_proc);
  UI_set(g->ui, "test_buttonst", &g->base, 0);
  waitkey(g->ui, -1);
  FreeMemDC();
  return 0;
}

