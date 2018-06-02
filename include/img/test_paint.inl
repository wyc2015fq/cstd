
//#include "cv.h"
//#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <DIRECT.h>
#include "cstd.h"
#include "findfile.h"
#include "cstd.h"

#pragma comment (lib,"fmtimglib.lib")
#pragma comment (lib,"highguilib.lib")

// void cvInpaint2( const CvArr* _input_img, const CvArr* _inpaint_mask, CvArr* _output_img, double inpaintRange, int flags );

#define CC_INPAINT_NS      0
#define CC_INPAINT_TELEA   1

unsigned char* inpaint_mask = 0, *img0 = 0, *img = 0, *inpainted = 0;
int imgheight = 0, imgwidth = 0, imgstep = 0, imgcn = 0;

IPOINT prev_pt = { -1, -1};

CvScalar coltab[ 2 ] = {
  {0, 255, 0},
  {255, 0, 0},
};

int colidx = 0;
#define GETCOLOR()  coltab[!!colidx]

void on_mouse(int event, int x, int y, int flags, void* ptr)
{
  if (!img) {
    return ;
  }

  if (event == CC_EVENT_LBUTTONUP || !(flags & CC_EVENT_FLAG_LBUTTON)) {
    prev_pt.x = -1, prev_pt.y = -1;
  }
  else if (event == CC_EVENT_LBUTTONDOWN) {
    prev_pt.x = x, prev_pt.y = y;
  }
  else if (event == CC_EVENT_MOUSEMOVE && (flags & CC_EVENT_FLAG_LBUTTON)) {
    IPOINT pt = {x, y};

    if (prev_pt.x < 0) {
      prev_pt = pt;
    }

    DrawLine(imgheight, imgwidth, inpaint_mask, imgstep, imgcn,
        prev_pt, pt, GETCOLOR(), 5, 8, 0);
    DrawLine(imgheight, imgwidth, img, imgstep, imgcn,
        prev_pt, pt, GETCOLOR(), 5, 8, 0);
    prev_pt = pt;
    cvShowImage("image", imgheight, imgwidth, img, imgstep, imgcn);
  }
}

int main(int argc, char** argv)
{
  //char* filename = argc >= 2 ? argv[1] : (char*)"F:\\public\\7bed000f.jpg";
  struct fmtimgreader* reader;

  cvNamedWindow("image", 1);
  _mkdir(".\\mark\\");

  begin_find(info, "*.*");
  reader = new_reader(filename, &imgheight, &imgwidth, 0);

  if (!reader) {
    continue;
  }

  imgcn = 3; //reader->m_iscolor ? 3 : 1;
  imgstep = imgwidth * imgcn;
  img0 = (unsigned char*) pmalloc(imgheight * imgwidth * (4 * imgcn));
  memset(img0, 0, imgheight * imgwidth * (4 * imgcn));
  img = img0 + (imgheight * imgstep);
  inpainted = img + (imgheight * imgstep);
  inpaint_mask = inpainted + (imgheight * imgstep);

  imread(reader, img0, imgstep, imgcn);
  memcpy(img, img0, (imgheight * imgstep));

  printf("快捷键: \n"
      "\tESC - 退出程序\n"
      "\tc - 清除\n"
      "\ts - 保存\n"
      "\t\n");


  cvShowImage("image", imgheight, imgwidth, img, imgstep, imgcn);
  //cvShowImage( "watershed transform", imgheight, imgwidth, inpainted, imgstep, imgcn );
  cvSetMouseCallback("image", on_mouse, 0);

  for (;;) {
    int c = cvWaitKey(0);

    if (c == 27 || c == 'q' || c == 'Q') {
      break;
    }

    if (c == 'r') {
      memset(inpaint_mask, 0, (imgheight * imgwidth));
      memcpy(img, img0, (imgheight * imgstep));
      cvShowImage("image", imgheight, imgwidth, img, imgstep, imgcn);
    }

    if (c == '1') {
      colidx = 0;
      printf("选用绿色画笔\n");
    }

    if (c == '2') {
      colidx = 1;
      printf("选用蓝色画笔\n");
    }

    if (c == 's' || c == 'S') {
      char buf[ 256 ];
      _snprintf(buf, 256, ".\\mark\\%s", filename);
      imwrite(buf, imgheight, imgwidth, inpaint_mask, imgstep, imgcn);
      printf("保存: %s\n", buf);
    }

    if (c == 'c' || c == 'C') {
      memset(inpaint_mask, 0, imgheight * imgstep);
      memcpy(img, img0, (imgheight * imgstep));
      cvShowImage("image", imgheight, imgwidth, img, imgstep, imgcn);
      printf("清除\n");
    }
  }

  if (img0) {
    pfree(img0);
  }

  end_find(info);

  return 1;
}
