
#if 0
#include "cstd.h"
#include "draw/HZK16.inl"
#include "imgui/imgui.inl"
#include "imgui/ctrl.inl"
#include "cfile.h"

// ijg12.lib ijg16.lib opengl32.lib

int seedcm_ctrl(ctrl_t* c)
{
  static char buf[256];
  static ctrl_t cc[10] = {0};
  int i = 0;
  IRECT rc = c->rc;
  ctrl_t* btn_openfolder = cc + i++;
  ctrl_t* btn_openfile = cc + i++;
  static int inited = 0;
  static dirlist_t dl[1] = {0};

  flexboxlay(rc, countof(cc), cc, 4, flex_column);

  if (button_ctrl(btn_openfolder, "打开dcm目录")) {
    sys_browse_folder("打开dcm目录", buf, countof(buf));
    printf("打开dcm目录 %s\n", buf);
  }

  if (button_ctrl(btn_openfile, "打开单个dcm文件")) {
    sys_open_filename("*.dcm", buf, countof(buf));
    printf("打开单个dcm文件 %s\n", buf);
  }

  if (!inited) {
    inited = 1;
    const char* dcmpath = "D:/CTdata/CENOVIXupcompress";
    dirlist(dl, dcmpath, "*.dcm", 1);

    if (dl->n > 0) {
      DICOMINFO di[1] = {0};
      GetDicomInfo(di, dl->v[0].name);
    }
  }

  return 0;
}

int test_seedcm()
{
  ctrl_t* c = NULL;

  for (; c = host_wait(300);) {
    //test_facerecog(c);
    seedcm_ctrl(c);
  }

  return 0;
}
#else

#include "dicom/mydcmtk.inl"
int test_seedcm()
{
  int test_mydcmtk();
  test_mydcmtk();
  return 0;
}
#endif
