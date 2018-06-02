#include "cstd.h"
//#include "ui/window.inl"
#include "ui/uiwin.inl"
#include "ui/calc_ctrl.inl"
#include "VisualManager.inl"
//#include "windowsx.h"
//#include "draw/dc.inl"
#include "multipane_ctrl.inl"
typedef struct axis_info_t {
  IRECT rcAxisLabel;
  IRECT rcLabel;
  IRECT rcTickLabel;
  double vMin, vMax;
  const char* sAxisLabel;
  const char* sLabel;
  int row, col, step;
  const double* data;
  int origin;
  double vTickStep;
  int szTick;
  int nTick;
  int tmin;
  int tklen;
  int t, b;
} axis_info_t;
typedef struct data_info_t {
  int h, w, xl, yl;//, xi, yi;
  const double* x;
  const double* y;
} data_info_t;
typedef struct graph_info_t {
  axis_info_t x, y;
  IRECT rcContent;
  IRECT rcClip;
} graph_info_t;
#include "draw/imdrawchart.inl"
#if 0
//#include "ribbon.inl"
#include "test_circles.inl"
#include "test_sudoku.inl"
#include "test_uidemo.inl"
//#include "test_tweetable_mathematical_art.inl"
//#include "cap/capvfw.inl"
#include "TabCtrl.inl"
#include "toolbar.inl"
#include "mdi.inl"
#include "test_isee.inl"
#include "test_edit.inl"
#include "test_ChzzMain.inl"
#include "test_gradients.inl"
#include "test_sdk.inl"
#include "test_multipane.inl"
#include "test_spirograph.inl"
#include "test_clip.inl"
#include "test_imgsee.inl"
#include "test_tetris.inl"
#include "test_vena.inl"
#include "test_ctrl.inl"
#endif
#include "test_aa_test.inl"
//#include "ui/window.inl"
//#include "BCG/test_bcg.inl"
//#include "test_face_recog.inl"
//#include "magic_cube.inl"
#ifdef TEST
int test_ui()
{
  uidraw_t drawer[1] = {0};
#if 0
  test_circles_t test_circles[1] = {0};
  test_uidemo_t test_uidemo[1] = {0};
  test_edit_t test_edit[1] = {0};
  test_isee_t test_isee[1] = {0};
  test_sudoku_t test_sudoku[1] = {0};
  test_ChzzMain_t test_ChzzMain[1] = {0};
  test_gradients_t test_gradients[1] = {0};
  test_multipane_t test_multipane[1] = {0};
  test_spirograph_t test_spirograph[1] = {0};
  test_clip_t test_clip[1] = {0};
  test_imgsee_t test_imgsee[1] = {0};
  test_tetris_t test_tetris[1] = {0};
  test_vena_recog_t test_vena_recog[1] = {0};
  test_ctrl_t test_ctrl[1] = {0};
#endif
  test_aa_test_t test_aa_test[1] = {0};
  if (1) {
    char* aa = "22 22 136 2";
    int    height,         // Number of lines in image
           ncolors;       // Number of colors in image
    sscanf(aa, "%*d%d%d", &height, &ncolors);
  }
  //VisualManagerXP_init(drawer, sys);
  //VisualManager2003_init(drawer, sys);
  //return test_bcg();
  //test_face_recog_t test_face_recog[1] = {0};
  //return test_sdk();
  //return test_imload_gif();
#if 0
  //ctrl_set(test_face_recog_proc, test_face_recog, test_face_recog->c, 0, "test_face_recog", WT_WINDOWS, 0, 0, 0);
  ctrl_set(test_ChzzMain_proc, test_ChzzMain, test_ChzzMain->c, 0, "test_ChzzMain", WT_POPUP, 100, 100, 0, 0, 0);
  ctrl_set(test_isee_proc, test_isee, test_isee->c, 0, "test_isee", WT_WINDOWS, 0, 0, 0, 0, 0);
  ctrl_set(test_clip_proc, test_clip, test_clip->c, 0, "test_clip", WT_WINDOWS, 0, 0, 0, 0, 0);
  ctrl_set(test_gradients_proc, test_gradients, test_gradients->c, 0, "test_gradients", WT_WINDOWS, 100, 100, 0, 0, 0);
  ctrl_set(test_spirograph_proc, test_spirograph, test_spirograph->c, 0, "test_spirograph", WT_WINDOWS, 100, 100, 0, 0, 0);
  ctrl_set(test_sudoku_proc, test_sudoku, test_sudoku->c, 0, "test_sudoku", WT_WINDOWS, 0, 0, 0, 0, 0);
  ctrl_set(test_imgsee_proc, test_imgsee, test_imgsee->c, 0, "test_imgsee", WT_WINDOWS, 0, 0, 0, 0, 0);
  ctrl_set(test_tetris_proc, test_tetris, test_tetris->c, 0, "test_tetris", WT_WINDOWS, 0, 0, 0, 0, 0);
  ctrl_set(calc_proc, test_clip, test_clip->c, 0, "calc", WT_WINDOWS, 0, 0, 0, 0, 0);
  ctrl_set(test_ctrl_proc, test_ctrl, test_uidemo->c, 0, "test_ctrl", WT_WINDOWS, 0, 0, 0, 0, 0);
  ctrl_set(test_vena_recog_proc, test_vena_recog, test_vena_recog->c, 0, WT_POPUP, "Ö¸¾²ÂöÑÝÊ¾demo");
  ctrl_set(test_edit_proc, test_edit, test_edit->c, 0, WT_POPUP, "test_edit");
  ctrl_set(test_circles_proc, test_circles, test_circles->c, 0, WT_POPUP, "test_circles");
  ctrl_set(test_uidemo_proc, test_uidemo, test_uidemo->c, 0, WT_POPUP, "test_uidemo");
#endif
  ctrl_set(test_aa_test_proc, test_aa_test, test_aa_test->c, 0, WT_WINDOWS, "test_aa_test");
  //ctrl_set(button_proc, test_clip, test_clip->c, 0, "button", WT_WINDOWS, 0, 0, 0, 0, 0);
  //win_set(win, "test_tweetable_mathematical_art", test_tweetable_mathematical_art_proc);
  for (;;) {
    int ch = waitkey(-1);
    if (-2 == ch) {
      break;
    }
    //printf("--%08x\n", &ch);
  }
  sys_exit();
  return 0;
}
#endif

