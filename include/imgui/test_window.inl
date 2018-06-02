int test_window()
{
  int i;
  GETGUIIO2();
  if (0) {
    float blendPositions[] = {0, 1};
    ImColor presetColors[] = {_RGB(0, 1, 121), _RGB(244, 100, 121)};
    // Rendering
    //ImDraw_lineWidth(ctx, 1);
    //gcFillColor(ctx, ImRGBA(244, 1, 121, 99));
    //gcStrokeColor(ctx, _RGB(0, 1, 121));
    gcFillGradient(ctx, 0, 0, 170, 150, countof(blendPositions), blendPositions, presetColors);
    gcFillGradient(ctx, 320, 30, 320, 50, countof(blendPositions), blendPositions, presetColors);
    gcText(ctx, 1, "adsf　你好", 0, 0, 320, 30, 100, 100, 0);
    //gcLineTo(io->ctx, 0, 0, 100, 100, _RGB(244, 1, 121), 10);
    //gcRect(io->ctx, 20, 30, 100, 100);
    //gcFillRect(ctx, io->MousePos.x - 50, io->MousePos.y - 50, 100, 100);
    //gcStrokeRect(ctx, io->MousePos.x - 50, io->MousePos.y - 50, 100, 100);
    //gcFillColor(ctx, _RGBA(255, 0, 0, 128));
    //gcFillRect(ctx, 320, 30, 100, 100);
    //gcSolidRect(ctx, 320, 30, 500, 500, _RGBA(255, 0, 0, 128));
    //gcSolidCircle(ctx, 50, 50, 20, 0, Color_Black, 2);
    //ImDraw_menuimg(ctx, iRECT2(20, 30, 100, 100), TF_VCENTER|TF_CENTER, IdArowLeftLarge, Color_Black);
    //gcSolidText(ctx, iRECT2(320, 30, 100, 100), "adsf你好", NULL, 0, 0, Color_Black);
    //imuiRenderFrame(ctx, iRECT2(20, 30, 100, 100), _RGB(244, 1, 121), TRUE, 10, 8+4);
    //gcBeginPath(ctx);
    //gcFillRoundRect(ctx, 20, 30, 100, 100, 10, 10, 10, 10);
    //gcFillRoundRect(ctx, 320, 30, 100, 100, 10, 0, 0, 0);
    //gcStrokeRoundRect(ctx, 20, 30, 100, 100, 10, 10, 10, 10);
    //gcClosePath(ctx);
  }
  if (0) {
    //IRECT rc = iRECT2(20, 30, 20, 20);
    static IRECT rc1;
    //ImGuiDrag(scrollbar1, rc, 10, 200, 10, 200, Drag_x|Drag_y, &rc);
    // scrollbar_ctrl(iRECT2(20, 30, 100, 100), &rc1);
  }
  if (0) {
    static float v1[] = {1, 1, 1};
    char* aa = (char*)(v1 + 1);
    if (autolayout_begin(NULL)) {
      set_layout_flags(LF_alighParentLeft);
      button_ctrl(aa++, "asdfadsfasdfasdf 1");
      button_ctrl(aa++, "asdfadsfasdfasdf 2");
      label_ctrl("adsfasd");
      if (autolayout_begin(NULL)) {
        set_layout_flags(LF_alighParentTop);
        button_ctrl(aa++, "asdfadsfasdfasdf 11");
        button_ctrl(aa++, "asdfadsfasdfasdf 21");
        label_ctrl("adsfasd 1");
        linearlayout_end();
      }
      linearlayout_end();
    }
  }
  if (0) {
    static float v1[] = {1, 1, 1};
    char* aa = (char*)(v1 + 1);
    static char buf[256] = {0};
    if (linearlayout_begin(NULL, true, 1, countof(v1), v1)) {
      set_layout_flags(LF_alighParentTop);
      if (1) {
        for (i = 0; i < 3; ++i) {
          if (autolayout_begin("group %d", i)) {
            set_layout_flags(LF_alighParentBottom);
            label_ctrl("adsfasd %d", i);
            set_layout_flags(LF_alighParentLeft | LF_alighParentBottom);
            button_ctrl(aa++, "asdf 1");
            button_ctrl(aa++, "asdf 2");
            set_layout_flags(LF_alighParentRight | LF_alighParentTop);
            button_ctrl(aa++, "asdf 3");
            button_ctrl(aa++, "asdf 4");
            set_layout_flags(LF_alighParentTop);
            button_ctrl(aa++, "asdfadsfasdfasdf");
            edit_ctrl("edit 0", buf + (countof(buf) / 3)*i, countof(buf) / 3);
            //button_ctrl(aa++, "asdf", LF_alighParentTop);
            //button_ctrl(aa++, "asdf", LF_alighParentBottom);
            linearlayout_end();
          }
        }
      }
      if (1) {
        if (autolayout_begin(NULL)) {
          set_layout_flags(LF_alighParentLeft);
          button_ctrl(aa++, "asdfadsfasdfasdf 1");
          button_ctrl(aa++, "asdfadsfasdfasdf 2");
          label_ctrl("adsfasd");
          if (autolayout_begin(NULL)) {
            set_layout_flags(LF_alighParentTop);
            button_ctrl(aa++, "asdfadsfasdfasdf 11");
            button_ctrl(aa++, "asdfadsfasdfasdf 21");
            label_ctrl("adsfasd 1");
            linearlayout_end();
          }
          linearlayout_end();
        }
      }
      linearlayout_end();
    }
  }
  if (0) {
    static char aa[1000] = {0};
    IRECT rcbtn[100];
    static BOOL v = 0;
    iRectMatrix(host->rootWindow->rect, 10, 10, 1, 1, TF_CENTER, 1000, rcbtn);
    for (i = 0; i < 1; ++i) {
      char buf[32];
      pushrect(rcbtn[i]);
      _snprintf(buf, 32, "Button %d", i);
      //if (ImButton(btn + i, rcbtn[i], buf, 0))
      if (button_ctrl(aa + i, buf)) {
        printf("Button %d pressed\n", i);
        v = !v;
      }
      poprect();
      if (popup_begin(&v)) {
        set_layout_flags(LF_alighParentTop);
        button_ctrl(aa + 100, "asdfadf 0");
        button_ctrl(aa + 101, "asdfadf 1");
        button_ctrl(aa + 102, "asdfadf 2");
        label_ctrl("asdfadf 2 %d", v);
        popup_end();
      }
    }
  }
  if (0) {
    static float v1[] = {1, 2.};
    static float v2[] = {.1f, .1f, .1f, .1f, .1f};
    static char btn2[100] = {0};
    //IRECT ClientRect = host->rootWindow->rect;
    groupbox_ctrl("GroupBox 1");
    if (linearlayout_begin(NULL, 0, true, countof(v1), v1)) {
      if (linearlayout_begin(NULL, TRUE, true, countof(v2), v2)) {
        for (i = 0; i < 5; ++i) {
          char buf[32];
          _snprintf(buf, 32, "Buttons %d", i);
          if (button_ctrl(btn2 + i, buf)) {
            printf("Button %d pressed\n", i);
          }
        }
        linearlayout_end();
      }
      linearlayout_end();
    }
  }
  if (0) {
    static window_t win1[1] = {0};
    static char btn2[1000] = {0};
    static IRECT rcWin = {50, 150, 50 + 200, 150 + 200};
    static float v1[] = {20, 40};
    static float v2[] = {1, 1, 1, 1};
    static float v3[] = {1, 1, 1};
    if (window_begin(win1, "win 1", &rcWin, NULL, 0)) {
      if (linearlayout_begin(NULL, 0, true, countof(v1), v1)) {
        if (linearlayout_begin(NULL, TRUE, false, countof(v2), v2)) {
          for (i = 0; i < countof(v2); ++i) {
            char buf[32];
            _snprintf(buf, 32, "win 1 btn %d 按钮", i);
            if (button_ctrl(btn2 + i, buf)) {
              printf("win 1 Button %d pressed\n", i);
            }
            if (ISHOT(btn2 + i)) {
              int asdf = 0;
              //tooltip_ctrl(buf);
            }
          }
          linearlayout_end();
        }
        if (linearlayout_begin(NULL, TRUE, false, countof(v3), v3)) {
          static int id = 0;
          const char* items[] = {"aaaaaa 1", "bbbbbb 2", "cccccc 3", "dddddd 4"};
          if (rbox_ctrl("ImRBoxGroup", 4, items, &id, 0)) {
            printf("select id %d %s\n", id, items[id]);
          }
          if (ISHOT(&id)) {
            int asdf = 0;
            tooltip_ctrl("aaaaaa 1\nbbbbbb 2\ncccccc 3\ndddddd 4");
          }
          linearlayout_end();
        }
        linearlayout_end();
      }
      window_end();
    }
  }
  if (0) {
    static window_t win1[1] = {0};
    static char btn2[1000] = {0};
    static IRECT rcWin = {150, 150, 150 + 200, 150 + 200};
    if (window_begin(win1, "win 2", &rcWin, NULL, 0)) {
      static float v1[] = {1, 2.};
      static float v2[] = {.1f, .1f, .1f, .1f, .1f};
      if (linearlayout_begin(NULL, 0, true, countof(v1), v1)) {
        if (linearlayout_begin(NULL, TRUE, false, countof(v2), v2)) {
          for (i = 0; i < 5; ++i) {
            char buf[32];
            _snprintf(buf, 32, "win 2 btn %d", i);
            if (button_ctrl(btn2 + i, buf)) {
              printf("win 2 Button %d pressed\n", i);
            }
          }
          linearlayout_end();
        }
        linearlayout_end();
      }
      window_end();
    }
  }
  if (0) {
    static char buf1[256] = "我爱我爱我爱我爱我爱";
    static char buf2[256] = "41324dfqv 32 adfasdf ";
    static int ch = 0;
    edit_ctrl(NULL, buf1, countof(buf1));
    //editctrl(iRECT2(60, 80, 200, 20), "", buf2, countof(buf2));
    cbox_ctrl("asdfasdf", &ch);
  }
  if (0) {
    static float v1 = 0;
    static int v2 = 1;
    if (slider_float_ctrl(iRECT2(60, 110, 200, 20), "float %.4f", &v1, 0, 10)) {
      printf("v1 %.3f\n", v1);
    }
    slider_int_ctrl(iRECT2(60, 140, 200, 20), "int %d", &v2, 0, 100);
  }
  return 0;
}

