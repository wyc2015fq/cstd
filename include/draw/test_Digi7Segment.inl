
static char _buffer[100];
#undef STRDATE
#undef STRTIME
#undef STRRAND
#define STRDATE()  _strdate(_buffer)
#define STRTIME()  _strtime(_buffer)
#define STRRAND()  dtoa(rand()/100., _buffer, 9)
char* dtoa(double d, char* str, int len)
{
  _snprintf(str, len, "%21.20f", d);
  return str;
}
int test_Digi7Segment()
{
  int line_type = CC_AA; // change it to 8 to see non-antialiased graphics
  CvRNG rng = 0;
  int width = 1000, height = 700, cn = 3;
  int width3 = width * 3, height3 = height * 3;
  int step = width * 3;
  int ymin = 0;
  // Load the source image
  unsigned char* image = MALLOC(unsigned char, height * step);
  unsigned char* image2 = MALLOC(unsigned char, height * step);
  int cy = 40, cx = 40;
  int bgmix = 1;
  DWORD col = CC_RGB(0, 255, 0); //random_color(&rng);
#define CPoint(x, y)  {x, y}
  // Create a window
  cvNamedWindow(wndname, 1);
  cvCreateTrackbar("×ÖÌå¿í", wndname, &cx, width, 0);
  cvCreateTrackbar("×ÖÌå¸ß", wndname, &cy, width, 0);
  cvCreateTrackbar("±³¾°", wndname, &bgmix, 100, 0);
  memset(image, 0, height * step);
  cvShowImage(wndname, height, width, image, step, cn);
  cvWaitKey(DELAY);

  for (; 1;) {
    int ch = cvWaitKey(10);
    char buf[10] =  {
      0
    };
    int kk = (cy + 10);
    int y = 10;
    int fill = 1; //cvRandInt(rng)&1;
    utime_start(_start_time);

    if ('q' == ch || 'Q' == ch) {
      break;
    }

    //cy = cx;
    memset(image, 0, height * step);
    //xDrawDigi(height, width, image, step, cn, ".:*+-()/\\'", col, cy, cx, y, 10, fill, bgmix / 100.);
    y += kk;
    //xDrawDigi(height, width, image, step, cn, "1234567890", col, cy, cx, y, 10, fill, bgmix / 100.);
    y += kk;
    //xDrawDigi(height, width, image, step, cn, "ABCDEFGHIJKLMN", col, cy, cx, y, 10, fill, bgmix / 100.);
    y += kk;
    //xDrawDigi(height, width, image, step, cn, "OPQRSTUVWXYZ", col, cy, cx, y, 10, fill, bgmix / 100.);
    y += kk;
    //xDrawDigi(height, width, image, step, cn, STRTIME(), col, cy, cx, y, 10, fill, bgmix / 100.);
    y += kk;
    //utime_restart(_start_time);
    //xDrawDigi(height, width, image, step, cn, STRRAND(), col, cy, cx, y, 10, fill, bgmix / 100.);
    printf("%f\n", utime_elapsed(_start_time));
    cvShowImage(wndname, height, width, image, step, cn);
  }

  SAFEFREE(image);
  SAFEFREE(image2);
  cvDestroyWindow(wndname);
  return 0;
}
