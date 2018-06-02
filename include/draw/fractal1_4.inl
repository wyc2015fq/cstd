
typedef struct TViewRect {
  double x0, y0, r;
} TViewRect;

//---------------------------------------------------------------------------------
CC_INLINE long mandelbrot1(const double x0, const double y0, const long max_iter)
{
  double x = x0, y = y0, tmp;
  long i = 0;

  for (; i < max_iter; ++i) {
    if (x * x + y * y >= 4) {
      break;
    }

    tmp = x * x - y * y + x0;
    y = x * y * 2 + y0;
    x = tmp;
  }

  return i;
}

//color=i%256
//    /  /  /  /  /
//   /  /  /  /  /
//  /  /  /  /  /
CC_INLINE COLOR coloring1(uchar* clr, long iter, long max_iter)
{
  if (iter == max_iter) {
    return SETRGB(clr, 255, 0, 0);
  }
  else {
    return SETRGB(clr, (iter * 20) % 256, (iter * 15 + 85) % 256, (iter * 30 + 171) % 256);
  }
}

CC_INLINE int imdraw_mandelbrot1(img_t* im, TViewRect rect, const long max_iter)
{
  int x, y;

  for (y = 0; y < im->h; ++y) {
    uchar* ptr_x = im->tt.data + y * im->s;

    for (x = 0; x < im->w; ++x, ptr_x += im->c) {
      double x0 = (2 * rect.r) * x / im->w + rect.x0 - rect.r;
      double yr = rect.r * im->h / im->w; //y轴半径
      double y0 = (2 * yr) * y / im->h + rect.y0 - yr;
      long iter = mandelbrot1(x0, y0, max_iter);
      coloring1(ptr_x, iter, max_iter);
    }
  }

  return 0;
}

//---------------------------------------------------------------------------------

//color=(i%510)-255
//    /\    /\    /
//   /  \  /  \  /
//  /    \/    \/
CC_INLINE long modColor2(long iter)
{
  return abs((iter + 255) % 510 - 255);
}

CC_INLINE COLOR coloring2(uchar* clr, const long iter, const long max_iter)
{
  if (iter == max_iter) {
    return SETRGB(clr, 255, 0, 0);
  }
  else {
    return SETRGB(clr, modColor2(iter * 20), modColor2(iter * 15 + 85), modColor2(iter * 30 + 171));
  }
}

int imdraw_mandelbrot2(img_t* im, const TViewRect* rect, const long max_iter)
{
  int x, y;

  for (y = 0; y < im->h; ++y) {
    uchar* ptr_x = im->tt.data + y * im->s;

    for (x = 0; x < im->w; ++x, ptr_x += im->c) {
      double x0 = (2 * rect->r) * x / im->w + rect->x0 - rect->r;
      double yr = rect->r * im->h / im->w;
      double y0 = (2 * yr) * y / im->h + rect->y0 - yr;
      long iter = mandelbrot1(x0, y0, max_iter);
      coloring2(ptr_x, iter, max_iter);
    }
  }

  return 0;
}
CC_INLINE COLOR coloring3_s(uchar* clr, const long iter, const long max_iter)
{
  if (iter == max_iter) {
    return SETRGB(clr, 255, 0, 0);
  }
  else {
    return SETRGB(clr, modColor2(iter), modColor2(iter + 85), modColor2(iter + 171));
  }
}

void imdraw_mandelbrot3_s(img_t* im, const TViewRect* rect, const long max_iter)
{
  int x, y;

  for (y = 0; y < im->h; ++y) {
    uchar* ptr_x = im->tt.data + y * im->s;

    for (x = 0; x < im->w; ++x, ptr_x += im->c) {
      double x0 = (2 * rect->r) * x / im->w + rect->x0 - rect->r;
      double yr = rect->r * im->h / im->w;
      double y0 = (2 * yr) * y / im->h + rect->y0 - yr;
      long iter = mandelbrot1(x0, y0, max_iter);
      coloring3_s(ptr_x, iter, max_iter);
    }
  }
}

//---------------------------------------------------------------------------------

#define  _divLog2  (1.0 / log(2.0));
CC_INLINE double log2(double x)
{
  return log(x) * _divLog2;
}

CC_INLINE double mandelbrot3(const double x0, const double y0, const long max_iter)
{
  long i = 0;
  double tmp, x, y;
  x = x0, y = y0;

  for (; i < max_iter; ++i) {
    if (x * x + y * y >= 256) {
      break;
    }

    tmp = x * x - y * y + x0;
    y = x * y * 2 + y0;
    x = tmp;
  }

  if (i != max_iter) {
    return i + 1 - log2(log2(x * x + y * y));
  }
  else {
    return i;
  }
}

//color=(i%510)-255
CC_INLINE long modColor3(double iter)
{
  return (long)(fabs(fmod(iter + 255, 510) - 255));
}
CC_INLINE COLOR coloring3(uchar* clr, const double iter, const long max_iter)
{
  if (iter == max_iter) {
    return SETRGB(clr, 255, 0, 0);
  }
  else {
    return SETRGB(clr, modColor3(iter * 20), modColor3(iter * 15 + 85), modColor3(iter * 30 + 171));
  }
}

void imdraw_mandelbrot3(img_t* im, const TViewRect* rect, const long max_iter)
{
  int x, y;

  for (y = 0; y < im->h; ++y) {
    uchar* ptr_x = im->tt.data + y * im->s;

    for (x = 0; x < im->w; ++x, ptr_x += im->c) {
      double x0 = (2 * rect->r) * x / im->w + rect->x0 - rect->r;
      double yr = rect->r * im->h / im->w;
      double y0 = (2 * yr) * y / im->h + rect->y0 - yr;
      double iter = mandelbrot3(x0, y0, max_iter);
      coloring3(ptr_x, iter, max_iter);
    }
  }
}

//---------------------------------------------------------------------------------

//color=sin(i)
CC_INLINE long sinColor(double iter)
{
  return (long)((sin(iter * 2 * 3.1415926 / 510 - 3.1415926 * 0.5) + 1) * 0.5 * 255);
}
CC_INLINE COLOR coloring4(uchar* clr, const double iter, const long max_iter)
{
  if (iter == max_iter) {
    return SETRGB(clr, 255, 0, 0);
  }
  else {
    return SETRGB(clr, sinColor(iter * 20), sinColor(iter * 15 + 85), sinColor(iter * 30 + 171));
  }
}

int imdraw_mandelbrot4(img_t* im, const TViewRect* rect, const long max_iter)
{
  int x, y;

  for (y = 0; y < im->h; ++y) {
    uchar* ptr_x = im->tt.data + y * im->s;

    for (x = 0; x < im->w; ++x) {
      double x0 = (2 * rect->r) * x / im->w + rect->x0 - rect->r;
      double yr = rect->r * im->h / im->w;
      double y0 = (2 * yr) * y / im->h + rect->y0 - yr;
      double iter = mandelbrot3(x0, y0, max_iter);
      coloring4(ptr_x, iter, max_iter);
    }
  }

  return 0;
}

#if 0
//---------------------------------------------------------------------------------

typedef void (*TDrawMandelbrotProc)(img_t* im, const TViewRect* rect, const long max_iter);

void test(const char* proc_name, const TDrawMandelbrotProc fproc)
{
  const long max_iter = 1000;
  TViewRect rect;
  rect.x0 = -0.5;
  rect.y0 = 0;
  rect.r = 2;

  TPixels32 dstPic;
  dstPic.resizeFast(640, 480);

  std::cout << proc_name << ": ";
  fproc(dstPic.getRef(), rect, max_iter);
  std::cout << " ok" << std::endl;

  {
    //save pic
    std::string dstFileName(proc_name);
    dstFileName += ".bmp";
#if defined(__APPLE__) && defined(__MACH__)
    TFileOutputStream bmpOutStream("/Users/hss/Desktop/GraphicDemo/ColorToGray/" + dstFileName);
#else
    TFileOutputStream bmpOutStream(dstFileName.c_str());
#endif
    TBmpFile::save(dstPic.getRef(), &bmpOutStream); //保存结果图片
  }
}

//等待一个回车输入
static void waitInputChar()
{
  while (true) {
    int c = getchar();

    if (c == '\n') {
      break;
    }
  }
}
//---------------------------------------------------------------------------------

int main()
{

  test("draw_mandelbrot1"   , draw_mandelbrot1);
  test("draw_mandelbrot2"   , draw_mandelbrot2);
  test("draw_mandelbrot2_s" , draw_mandelbrot3_s);
  test("draw_mandelbrot3"   , draw_mandelbrot3);
  test("draw_mandelbrot4"   , draw_mandelbrot4);

  std::cout << std::endl << " 运行完成. ";
  //waitInputChar();
  return 0;
}

#endif
