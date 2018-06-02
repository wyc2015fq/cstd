
//#include "cv.h"
//#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "window.h"
#include "cstd.h"
#include "cstd.h"

// void cvInpaint2( const CvArr* _input_img, const CvArr* _inpaint_mask, CvArr* _output_img, double inpaintRange, int flags );

#define CC_INPAINT_NS      0
#define CC_INPAINT_TELEA   1

void
cvInpaint2(int input_img_rows, int input_img_cols,
    const unsigned char* input_img, int input_img_step,
    const unsigned char* inpaint_mask, int inpaint_mask_step,
    unsigned char* output_img, int output_img_step,
    int imgcn, double inpaintRange, int flags);

unsigned char* inpaint_mask = 0, *img0 = 0, *img = 0, *inpainted = 0;
int imgheight = 0, imgwidth = 0, imgstep = 0, imgcn = 0;

IPOINT prev_pt = { -1, -1};

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

    DrawLine(imgheight, imgwidth, inpaint_mask, imgwidth, 1,
        prev_pt, pt, CC_RGB(255, 255, 255), 5, 8, 0);
    DrawLine(imgheight, imgwidth, img, imgstep, imgcn,
        prev_pt, pt, CC_RGB(255, 255, 255), 5, 8, 0);
    prev_pt = pt;
    cvShowImage("image", imgheight, imgwidth, img, imgstep, imgcn);
  }
}


int main(int argc, char** argv)
{
  char* filename = argc >= 2 ? argv[ 1 ] : (char*) "F:\\public\\7bed000f.jpg";

  struct fmtimgreader* reader = new_reader(filename);

  if (!reader) {
    return 0;
  }

  imgheight = reader->m_height, imgwidth = reader->m_width, imgcn = reader->m_iscolor ? 3 : 1;
  imgstep = imgwidth * imgcn;
  img0 = (unsigned char*) pmalloc(imgheight * imgwidth * (3 * imgcn + 1));
  memset(img0, 0, imgheight * imgwidth * (3 * imgcn + 1));
  img = img0 + (imgheight * imgstep);
  inpainted = img + (imgheight * imgstep);
  inpaint_mask = inpainted + (imgheight * imgstep);

  imread(reader, img0, imgstep, imgcn);
  memcpy(img, img0, (imgheight * imgstep));

  printf("Hot keys: \n"
      "\tESC - quit the program\n"
      "\tr - restore the original image\n"
      "\ti or ENTER - run inpainting algorithm\n"
      "\t\t(before running it, paint something on the image)\n");

  cvNamedWindow("image", 1);

  cvShowImage("image", imgheight, imgwidth, img, imgstep, imgcn);
  cvShowImage("watershed transform", imgheight, imgwidth, inpainted, imgstep, imgcn);
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

    if (c == 'i' || c == '\r') {
      cvNamedWindow("inpainted image", 1);
      //cvInpaint2( img, inpaint_mask, inpainted, 3, CC_INPAINT_TELEA );
      {
        cvInpaint2(imgheight, imgwidth, img, imgstep, inpaint_mask, imgwidth,
            inpainted, imgstep, imgcn, 3, CC_INPAINT_TELEA);
      }
      cvShowImage("inpainted image", imgheight, imgwidth, inpainted, imgstep, imgcn);
    }
  }

  return 1;
}
