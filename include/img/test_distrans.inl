
#include <stdio.h>

#define cvCreateImage2(_r, _c, _t, _x)  pmalloc((_r)*(_c)*(_x)*CC_ELEM_SIZE(_t))

//char wndname[] = "Distance transform";
char tbarname[] = "Threshold";
int mask_size = CC_DIST_MASK_3;
int build_voronoi = 0;
int edge_thresh = 2;

// The output and temporary images
float* dist = 0;
uchar* dist8u1 = 0;
uchar* dist8u2 = 0;
uchar* dist8u = 0;
int* dist32s = 0;

img_t* gray = 0;
uchar* edge = 0;
int* labels = 0;
int edge_step = 0;
int dist_step = 0;
int labels_step = 0;
int dist8u_step = 0;
void
cvDistTransform(const uchar* src, float* dst,
    int distType, int maskSize,
    const float* mask,
    int* labels, CvSize size, int src_step, int dst_step);

#define WINDNAMEGRAY "gray"

CvScalar hsv2rgb1(float hue)
{
  int rgb[3], p, sector;
  static const int sector_data[][3] =
  {{0, 2, 1}, {1, 2, 0}, {1, 0, 2}, {2, 0, 1}, {2, 1, 0}, {0, 1, 2}};
  hue *= 0.033333333333333333333333333333333f;
  sector = cvFloor(hue);
  p = cvRound(255 * (hue - sector));
  p ^= sector & 1 ? 255 : 0;

  rgb[sector_data[sector][0]] = 255;
  rgb[sector_data[sector][1]] = 0;
  rgb[sector_data[sector][2]] = p;

  return cvScalar(rgb[2], rgb[1], rgb[0], 0);
}

void get_color(double fa0, int* pr, int* pg, int* pb)
{
  int r, g, b, a;
  double fa;
  static const uchar colors[][3] = {
    {0, 0, 0},
    {255, 0, 0},
    //{255,128,0},
    {255, 255, 0},
    {0, 255, 0},
    //{0,128,255},
    {0, 255, 255},
    {0, 0, 255},
    //{128,0,255},
    {255, 0, 255},
    {255, 255, 255}
  };
  ASSERT(fa0 <= 1.);
  fa = (sizeof(colors) / sizeof(colors[0])) * fa0;
  a = (int)fa;
  r = (int)(colors[a + 1][0] * (fa - a) + colors[a][0] * (1. - fa + a));
  g = (int)(colors[a + 1][1] * (fa - a) + colors[a][1] * (1. - fa + a));
  b = (int)(colors[a + 1][2] * (fa - a) + colors[a][2] * (1. - fa + a));

  if (0) {
    CvScalar cc = hsv2rgb1((float)(fa * 6));
    r = (int)cc.val[0];
    g = (int)cc.val[1];
    b = (int)cc.val[2];
  }

  *pr = r;
  *pg = g;
  *pb = b;
  //return CC_RGB(r, g, b);
}

/*
               1 1 1 1 1 0 0 0 0 0 1 1 1 1 1
forward pass:  1 2 3 4 5 0 0 0 0 0 1 2 3 4 5
backward pass: 5 4 3 2 1 0 0 0 0 0 1 2 3 4 5
forward pass:  a a1 a2 a3 a4 0 0 0 0 0 1 2 3 4 5
backward pass: 5 4 3 2 1 0 0 0 0 0 1 2 3 4 5
*/
#define worktype float
static CvStatus CC_STDCALL
icvDistanceTransform_3x3_C1R11(const uchar* src, int srcstep, worktype* dist, int dststep, CvSize size)
{
  int i, j;
  worktype t, t0;
#define ICC_DIST_SHIFT  16
#define ICC_INIT_DIST0  (INT_MAX >> 2)

  for (i = 0; i < size.height; i++) {
    worktype* d = (dist + i * dststep);

    for (j = 0; j < size.width; j++) {
      d[j] = (worktype)ICC_INIT_DIST0;
    }
  }

#define SSS(_I)   (d[_I] + ABSSUB(s[_I],s[j]) )
#define DDD(_I)   t = SSS(_I); if (t<0) {printf("%3d,%3d,%3.3f\n", i, j, t);} if( t0 > t ) t0 = t;
#define BBB(_a, _b, _c, _d)   t0 = d[j]; DDD(_a);DDD(_b);DDD(_c);DDD(_d); d[j] = t0;

  // forward pass
  for (i = 1; i < size.height; i++) {
    const uchar* s = src + i * srcstep;
    worktype* d = (dist + i * dststep);
    j = 0;
    BBB(j - srcstep + 1, j - srcstep, j - srcstep, j - srcstep);

    for (++j; j < size.width - 1; j++) {
      if (!s[j]) {
        d[j] = 0;
        continue;
      }

      BBB(j - srcstep - 1, j - srcstep, j - srcstep + 1, j - 1);
    }

    BBB(j - srcstep - 1, j - srcstep, j - srcstep, j - 1);
    j = size.width - 1;
    BBB(j - srcstep - 1, j - srcstep, j - srcstep, j - srcstep);

    for (--j; j > 0; j--) {
      BBB(j - srcstep - 1, j - srcstep, j - srcstep + 1, j + 1);
    }

    BBB(j - srcstep, j - srcstep, j - srcstep + 1, j + 1);
  }

  // backward pass
  for (i = size.height - 2; i >= 0; i--) {
    const uchar* s = src + i * srcstep;
    worktype* d = (dist + i * dststep);
    j = size.width - 1;
    BBB(j + srcstep - 1, j + srcstep, j + srcstep, j + srcstep);

    for (--j; j > 0; j--) {
      BBB(j + srcstep + 1, j + srcstep, j + srcstep - 1, j + 1);
    }

    BBB(j + srcstep + 1, j + srcstep, j + srcstep, j + 1);
    j = 0;
    BBB(j + srcstep + 1, j + srcstep, j + srcstep, j + srcstep);

    for (++j; j < size.width - 1; j++) {
      BBB(j + srcstep + 1, j + srcstep, j + srcstep - 1, j - 1);
    }

    BBB(j + srcstep, j + srcstep, j + srcstep - 1, j - 1);
  }

  return CC_OK;
}

// threshold trackbar callback
void on_trackbar(int dummy)
{
  static const uchar colors[][3] = {
    {0, 0, 0},
    {255, 0, 0},
    {255, 128, 0},
    {255, 255, 0},
    {0, 255, 0},
    {0, 128, 255},
    {0, 255, 255},
    {0, 0, 255},
    {255, 0, 255}
  };

  int i, j;
  int msize = mask_size;
  int h = gray->h, w = gray->w, gray_step = gray->s;
  //build_voronoi=1;

  cvThreshold(h, w, gray->data, gray_step, edge, edge_step, (float)edge_thresh, (float)edge_thresh, CC_THRESH_BINARY);

  if (build_voronoi) {
    msize = CC_DIST_MASK_5;
  }

  for (i = 0; i < h; i++) {
    uchar* g = (gray->data + i * gray_step);
    uchar* e = (edge + i * edge_step);

    for (j = 0; j < w; j++) {
      e[j] = e[j] ? g[j] : e[j];
    }
  }

  //cvDistTransform( edge, dist, CC_DIST_L2, msize, NULL, build_voronoi ? labels : NULL,
  //  cvSize(w, h), edge_step, dist_step );

  icvDistanceTransform_3x3_C1R11(edge, edge_step, dist, dist_step, cvSize(w, h));

  if (!build_voronoi) {
#if 0
    // begin "painting" the distance transform result
    cvConvertScale(dist, dist, 5000.0, 0);
    cvPow(dist, dist, 0.5);

    cvConvertScale(dist, dist32s, 1.0, 0.5);
    cvAndS(dist32s, cvScalarAll(255), dist32s, 0);
    cvConvertScale(dist32s, dist8u1, 1, 0);
    cvConvertScale(dist32s, dist32s, -1, 0);
    cvAddS(dist32s, cvScalarAll(255), dist32s, 0);
    cvConvertScale(dist32s, dist8u2, 1, 0);
    cvMerge(dist8u1, dist8u2, dist8u2, 0, dist8u);
    // end "painting" the distance transform result
#endif
    double aaa = 0;

    for (i = 0; i < h; i++) {
      float* dd = (float*)(dist + i * dist_step);

      for (j = 0; j < w; j++) {
        //dd[j] = pow(dd[j], 0.3);
        if (dd[j] > aaa) {
          aaa = dd[j];
        }
      }
    }

    for (i = 0; i < h; i++) {
      int* ll = (int*)(labels + i * labels_step);
      float* dd = (float*)(dist + i * dist_step);
      uchar* d = (uchar*)(dist8u + i * dist8u_step);

      for (j = 0; j < w; j++) {
        int idx = ((int)(dd[j] / 10)) % 9;
        int b = cvRound(colors[idx][0]);
        int g = cvRound(colors[idx][1]);
        int r = cvRound(colors[idx][2]);

        if (dd[j] < 0) {
          printf("%3d, %3d, %3.3f\n", i, j, dd[j]);
          dd[j] = 0;
        }

        get_color(dd[j] / aaa, &r, &g, &b);

        if (dd[j] > aaa) {
          aaa = dd[j];
        }

        d[j * 3] = (uchar)b;
        d[j * 3 + 1] = (uchar)g;
        d[j * 3 + 2] = (uchar)r;
      }
    }

    //aaa = dist[0];
  }
  else {
    int i, j;

    for (i = 0; i < h; i++) {
      int* ll = (int*)(labels + i * labels_step);
      float* dd = (float*)(dist + i * dist_step);
      uchar* d = (uchar*)(dist8u + i * dist8u_step);

      for (j = 0; j < w; j++) {
        int idx = ll[j] == 0 || dd[j] == 0 ? 0 : (ll[j] - 1) % 8 + 1;
        int b = cvRound(colors[idx][0]);
        int g = cvRound(colors[idx][1]);
        int r = cvRound(colors[idx][2]);
        d[j * 3] = (uchar)b;
        d[j * 3 + 1] = (uchar)g;
        d[j * 3 + 2] = (uchar)r;
      }
    }
  }

  cvShowImage(wndname, h, w, dist8u, dist8u_step, 3);
  cvShowImage(WINDNAMEGRAY, h, w, edge, edge_step, 1);
}

int buttondown = 0;
void on_mouse(int event, int x, int y, int flags, void* param)
{
  int h = gray->h, w = gray->w, gray_step = gray->s;

  switch (event) {
  case CC_EVENT_MOUSEMOVE:
    if (buttondown) {
      int ww = 5;
      DrawCircle(h, w, gray->data, gray_step, 1, iPOINT(x, y), ww, CC_RGB(0, 0, 0), -1, 0, 0);
      DrawCircle(h, w, edge, edge_step, 1, iPOINT(x, y), ww, CC_RGB(0, 0, 0), -1, 0, 0);
      cvShowImage(WINDNAMEGRAY, h, w, edge, edge_step, 1);
    }

    break;

  case CC_EVENT_LBUTTONDOWN:
    buttondown = 1;

    if (buttondown) {
      int ww = 5;
      DrawCircle(h, w, gray->data, gray_step, 1, iPOINT(x, y), ww, CC_RGB(0, 0, 0), -1, 0, 0);
      DrawCircle(h, w, edge, edge_step, 1, iPOINT(x, y), ww, CC_RGB(0, 0, 0), -1, 0, 0);
      cvShowImage(WINDNAMEGRAY, h, w, edge, edge_step, 1);
    }

    break;

  case CC_EVENT_LBUTTONUP:
    buttondown = 0;
    on_trackbar(0);
    break;
  }
}

int test_distrans(int argc, char** argv)
{
  char* filename = argc == 2 ? argv[1] : (char*)"lena.jpg";

  int h, w;

  if ((gray = imread2(filename, 1)) == 0) {
    return -1;
  }

  printf("Hot keys: \n"
      "\tESC - quit the program\n"
      "\t3 - use 3x3 mask\n"
      "\t5 - use 5x5 mask\n"
      "\t0 - use precise distance transform\n"
      "\tv - switch Voronoi diagram mode on/off\n"
      "\tENTER - loop through all the modes\n");

  h = gray->h, w = gray->w;
  edge_step = dist_step = labels_step = w;
  dist8u_step = 3 * w;
  dist = cvCreateImage2(h, w, IMG_DEPTH_32F, 1);
  dist8u1 = cvCreateImage2(h, w, IMG_DEPTH_8U, 1);
  dist8u2 = cvCreateImage2(h, w, IMG_DEPTH_8U, 1);
  dist8u = cvCreateImage2(h, w, IMG_DEPTH_8U, 3);
  dist32s = cvCreateImage2(h, w, IMG_DEPTH_32S, 1);
  edge = cvCreateImage2(h, w, IMG_DEPTH_8U, 1);
  labels = cvCreateImage2(h, w, IMG_DEPTH_32S, 1);

  cvNamedWindow(wndname, 1);
  cvNamedWindow(WINDNAMEGRAY, 1);

  cvSetMouseCallback(wndname, on_mouse, 0);
  cvSetMouseCallback(WINDNAMEGRAY, on_mouse, 0);
  cvCreateTrackbar(tbarname, wndname, &edge_thresh, 255, on_trackbar);

  for (;;) {
    int c;

    // Call to update the view
    on_trackbar(0);

    c = cvWaitKey(0);

    if (c == 27) {
      break;
    }

    if (c == 'c') {
      imfree(gray);
      gray = imread2(filename, 1);
      memcpy(edge, gray->data, h * w);
      cvShowImage(WINDNAMEGRAY, h, w, edge, edge_step, 1);
    }

    if (c == '3') {
      mask_size = CC_DIST_MASK_3;
    }
    else if (c == '5') {
      mask_size = CC_DIST_MASK_5;
    }
    else if (c == '0') {
      mask_size = CC_DIST_MASK_PRECISE;
    }
    else if (c == 'v') {
      build_voronoi ^= 1;
    }
    else if (c == '\r') {
      if (build_voronoi) {
        build_voronoi = 0;
        mask_size = CC_DIST_MASK_3;
      }
      else if (mask_size == CC_DIST_MASK_3) {
        mask_size = CC_DIST_MASK_5;
      }
      else if (mask_size == CC_DIST_MASK_5) {
        mask_size = CC_DIST_MASK_PRECISE;
      }
      else if (mask_size == CC_DIST_MASK_PRECISE) {
        build_voronoi = 1;
      }
    }
  }

  cvReleaseImage(&gray);
  cvReleaseImage(&edge);
  cvReleaseImage(&dist);
  cvReleaseImage(&dist8u);
  cvReleaseImage(&dist8u1);
  cvReleaseImage(&dist8u2);
  cvReleaseImage(&dist32s);
  cvReleaseImage(&labels);

  imfree(gray);
  cvDestroyWindow(wndname);

  return 0;
}
