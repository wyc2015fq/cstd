
#include "cstd.h"
#include "str.h"
#include "draw/imdraw.inl"
#include "ui/draw.h"
#include "ui/window.inl"


#include "resource.h"
#include "Matrix.inl"
#include "Moves.inl"

#include "KSolver.inl"

#include "WndMsg.h"
#include "RubixCube.inl"

#if 0
#include "DiagExtr.h"
#include "DiagCmd.inl"

#include "DiagInp.h"
#include "DiagOut.h"

#include "Extract.inl"

#include "FileDialogSa.h"
#include "FileDialogSa.inl"

#include "DiagCtrl.inl"
#include "DiagExtr.inl"
#include "DiagInp.inl"
#include "DiagOut.inl"
#include "FileDialogSa.inl"
#include "MainFrm.inl"
#include "StaticEx.inl"
#endif

#include "ui/test_ui.inl"
#include "test_RubixCubeui.inl"
//#include "vector"
//#include "test/test_cpp.inl"

int test_RubixCube()
{
  //return test_cpp();
  _chdir("E:\\pub\\bin\\RubixCube");

  if (0) {
    const char* msg = "L:OGGRRWOWO F:YRROBBGGR R:YRRWOGGOW B:BYYOGYGBW U:BBWYWGRBB D:YWWRYYBOO";
    const char* sov = "R U B2 R' D2 F U2 R D' F L2 U2 B2 U' F2 R2 U2 L2 U' L2 U R2 F2 L2";
    KSolve(msg, 0);
  }

  if (1) {
    test_RubixCubeui_t test_RubixCubeui[1] = {0};

    ctrl_set(test_RubixCubeui_proc, test_RubixCubeui, test_RubixCubeui->c, 0, WT_POPUP, "test_RubixCubeui");
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

  return 0;
}
