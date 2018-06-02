/* Demo of modified Lucas-Kanade optical flow algorithm.
  See the printf below */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "macro.h"
#include "window.h"
#include "xtypes.h"
#include "imcolor.h"
#include "cvimgproc.h"
#include "cap.h"
#include "cstd.h"

void
cvGoodFeaturesToTrack(CvSize size,
    const unsigned char* img, int img_step,
    float* eig, int eig_step,
    float* tmp, int tmp_step,
    CvPoint2D32f* corners, int* corner_count,
    double quality_level, double min_distance,
    const unsigned char* mask, int mask_step,
    int block_size, int use_harris, double harris_k);

void
cvFindCornerSubPix(CvSize size, const unsigned char* src, int src_step, CvPoint2D32f* corners,
    int count, CvSize win, CvSize zeroZone, CvTermCriteria criteria);

void
cvCalcOpticalFlowPyrLK(CvSize imgSize,
    const unsigned char* imgA, int imgA_step,
    const unsigned char* imgB, int imgB_step,
    int pyrA_height, unsigned char* pyrA, int pyrA_step,
    int pyrB_height, unsigned char* pyrB, int pyrB_step,
    const CvPoint2D32f* featuresA,
    CvPoint2D32f* featuresB,
    int count, CvSize winSize, int level,
    char* status, float* error,
    CvTermCriteria criteria, int flags);

unsigned char* image = 0, *grey = 0, *prev_grey = 0, *pyramid = 0, *prev_pyramid = 0, *swap_temp;

int win_size = 10;
const int MAX_COUNT = 500;
CvPoint2D32f* points[ 2 ] = {0, 0}, *swap_points;
char* status = 0;
int count = 0;
int need_to_init = 0;
int night_mode = 0;
int flags = 0;
int add_remove_pt = 0;
IPOINT pt;


void on_mouse(int event, int x, int y, int flags, void* param)
{
  if (!image) {
    return ;
  }

  //if( image->origin )
  //    y = image->h - y;

  if (event == CC_EVENT_LBUTTONDOWN) {
    pt = iPOINT(x, y);
    add_remove_pt = 1;
  }
}


int main(int argc, char** argv)
{
  capcam* capture = 0;
  unsigned char* gry = NULL;
  unsigned char* rgb = NULL;

  if (argc == 1 || (argc == 2 && strlen(argv[ 1 ]) == 1 && isdigit(argv[ 1 ][ 0 ]))) {
    capture = cam_start(argc == 2 ? argv[ 1 ][ 0 ] - '0' : 0);
  }
  else if (argc == 2) {
    capture = cam_startavi(argv[ 1 ]);
  }

  if (!capture) {
    fprintf(stderr, "Could not initialize capturing...\n");
    return -1;
  }

  /* print a welcome message, and the OpenCV version */
  //printf ("Welcome to lkdemo, using OpenCV version %s (%d.%d.%d)\n",
  //  CC_VERSION,
  //  CC_MAJOR_VERSION, CC_MINOR_VERSION, CC_SUBMINOR_VERSION);

  printf("Hot keys: \n"
      "\tESC - quit the program\n"
      "\tr - auto-initialize tracking\n"
      "\tc - delete all the points\n"
      "\tn - switch the \"night\" mode on/off\n"
      "To add/remove a feature point click it\n");

  cvNamedWindow("LkDemo", 1);
  cvSetMouseCallback("LkDemo", on_mouse, 0);

  for (;;) {
    unsigned char* frame = 0;
    int i, k, c;
    int frame_step, cn, grey_step, pyramid_step;
    CvSize size;

    cam_QueryFrame(capture, size.height, size.width, frame, frame_step, cn);

    //frame = cvQueryFrame( capture );
    if (!frame) {
      break;
    }

    if (!image) {
      /* allocate all the buffers */
      pyramid_step = grey_step = size.width;
      image = MALLOC(unsigned char, size.height * frame_step);
      //image->origin = frame->origin;
      grey = MALLOC(unsigned char, size.height * size.width);
      prev_grey = MALLOC(unsigned char, size.height * size.width);
      pyramid = MALLOC(unsigned char, size.height * size.width);
      prev_pyramid = MALLOC(unsigned char, size.height * size.width);
      points[ 0 ] = (CvPoint2D32f*) cvAlloc(MAX_COUNT * sizeof(points[ 0 ][ 0 ]));
      points[ 1 ] = (CvPoint2D32f*) cvAlloc(MAX_COUNT * sizeof(points[ 0 ][ 0 ]));
      status = (char*) cvAlloc(MAX_COUNT);
      flags = 0;
    }

    // cvCopy( frame, image, 0 );
    memcpy(image, frame, size.height * frame_step);

    //FLIP2D(size.height, size.width*cn, image, frame_step, 1, temp_uchar);
    //cvCvtColor( image, grey, CC_BGR2GRAY );
    if (2 == cn) {
      if (!rgb) {
        rgb = MALLOC(unsigned char, size.height * size.width * 3);
      }

      icvYUV4222BGR_8u_C2C3R(size.height, size.width, image, frame_step, rgb, size.width * 3, 3);
      frame_step = size.width * 3;
      cn = 3;
    }
    else {
      rgb = image;
    }

    IMTRANS(size.height, size.width, rgb, frame_step, cn, grey, size.width, 1, BGR2GRAY);

    if (night_mode) {
      memset(rgb, 0, size.height * frame_step);
    }

    if (need_to_init) {
      /* automatic initialization */
      float* eig = MALLOC(float, size.height * size.width);
      float* temp = MALLOC(float, size.height * size.width);
      double quality = 0.01;
      double min_distance = 10;
      int eig_step = size.width;
      int temp_step = size.width;

      count = MAX_COUNT;
      cvGoodFeaturesToTrack(size, grey, grey_step, eig, eig_step, temp, temp_step,
          points[ 1 ], &count, quality, min_distance, 0, 0, 3, 0, 0.04);
      cvFindCornerSubPix(size, grey, grey_step, points[ 1 ], count,
          cvSize(win_size, win_size), cvSize(-1, -1),
          cvTermCriteria(CC_TERMCRIT_ITER | CC_TERMCRIT_EPS, 20, 0.03));
      SAFEFREE(eig);
      SAFEFREE(temp);

      add_remove_pt = 0;
    }
    else if (count > 0) {
      cvCalcOpticalFlowPyrLK(size, prev_grey, grey_step, grey, grey_step,
          size.height, prev_pyramid, pyramid_step,
          size.height, pyramid, pyramid_step,
          points[ 0 ], points[ 1 ], count, cvSize(win_size, win_size), 3, status, 0,
          cvTermCriteria(CC_TERMCRIT_ITER | CC_TERMCRIT_EPS, 20, 0.03), flags);
      flags |= CC_LKFLOW_PYR_A_READY;

      for (i = k = 0; i < count; i++) {
        if (add_remove_pt) {
          double dx = pt.x - points[ 1 ][ i ].x;
          double dy = pt.y - points[ 1 ][ i ].y;

          if (dx * dx + dy * dy <= 25) {
            add_remove_pt = 0;
            continue;
          }
        }

        if (!status[ i ]) {
          continue;
        }

        points[ 1 ][ k++ ] = points[ 1 ][ i ];
        {
          // cvCircle( image, cvPointFrom32f(points[1][i]), 3, CC_RGB(0,255,0), -1, 8,0);
          IPOINT centor = {(int) points[ 1 ][ i ].x, (int) points[ 1 ][ i ].y};
          DrawCircle(size.height, size.width, rgb, frame_step, cn,
              centor, 3, CC_RGB(0, 255, 0), -1, 8, 0);
        }
      }

      count = k;
    }

    if (add_remove_pt && count < MAX_COUNT) {
      points[ 1 ][ count ].x = (float) pt.x;
      points[ 1 ][ count ].y = (float) pt.y;
      count++;
      cvFindCornerSubPix(size, grey, grey_step, points[ 1 ] + count - 1, 1,
          cvSize(win_size, win_size), cvSize(-1, -1),
          cvTermCriteria(CC_TERMCRIT_ITER | CC_TERMCRIT_EPS, 20, 0.03));
      add_remove_pt = 0;
    }

    CC_SWAP(prev_grey, grey, swap_temp);
    CC_SWAP(prev_pyramid, pyramid, swap_temp);
    CC_SWAP(points[ 0 ], points[ 1 ], swap_points);
    need_to_init = 0;
    cvShowImage("LkDemo", size.height, size.width, rgb, frame_step, cn);

    c = cvWaitKey(10);

    if ((char) c == 27 || (char) c == 'q' || (char) c == 'Q') {
      break;
    }

    switch ((char) c) {
    case 'r':
      need_to_init = 1;
      break;

    case 'c':
      count = 0;
      break;

    case 'n':
      night_mode ^= 1;
      break;

    default:
      ;
    }
  }

  cam_stop(capture);
  cvDestroyWindow("LkDemo");

  return 0;
}
