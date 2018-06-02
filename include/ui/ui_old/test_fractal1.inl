
void CSceneCaDoDraw(void* ssf, int dst_height, int dst_width, unsigned char* dst, int dst_step, int cn);
void DrawNewton2(IDC* pDC);
#if 0
int test_fractal()
{
  int line_type = CV_AA; // change it to 8 to see non-antialiased graphics
  CvRNG rng = 0;
  int width = 800, height = 600, cn = 3;
  int width3 = width * 3, height3 = height * 3;
  int step = width * 3;
  int ymin = 0;
  // Load the source image
  unsigned char* image = MALLOC(unsigned char, height * step);
  unsigned char* imagebk = MALLOC(unsigned char, height * step);
  int cy = 40, cx = 40;
  int bgmix = 1;
  void* sj;
  void* si;
  srand(time(0));
  sj = fractal_new("Water");
  si = fractal_new("King");

  //sr = InitSurfaces(height, width);
  if (1) {
    int height1, width1;
    const TCHAR* picname = _T("C:\\Documents and Settings\\All Users\\Documents\\My Pictures\\Ê¾ÀýÍ¼Æ¬\\Water lilies.jpg");
    struct fmtimgreader* reader = new_reader(picname, &height1, &width1, 0);

    if (reader) {
      int step1;
      unsigned char* data;
      //cn = 1;
      step1 = width1 * cn;
      data = (unsigned char*)malloc(height1 * step1);
      memset(data, 128, height1 * step1);
      imread(reader, data, step1, cn > 1);
      free_reader(reader);
      IMRESIZE(height1, width1, data, step1, cn, height, width, imagebk, step, cn, INTER_BILINEARITY);
      free(data);
    }
  }

  // Create a window
  //cvNamedWindow(wndname, 1);
#define UTIME_BEGIN() { utime_start(_start_time);
#define UTIME_END()   printf("%f\n", utime_elapsed(_start_time)); }

  /*
  UTIME_BEGIN()
  UTIME_END()
   */
  for (; 1;) {
    int ch = 0;

    if ('q' == ch || 'Q' == ch) {
      break;
    }

    if (0) {
      memset(image, 0, height * step);
    }
    else {
      memcpy(image, imagebk, height * step);
    }

    fractal_update(sj, 50);
    fractal_update(si, 50);
    UTIME_BEGIN();
    {
      IDC idc;
      InitDC(&idc, height, width, image, step, cn, _RGB(255, 255, 0), 1);
      {
        int ax = 10, ay = 10, bx = 400, by = 10;
        //Canto(&idc, ax, ay, bx, by);
        //Fractal(&idc, ax, ay, bx, by);
        //Koch(&idc, 30);
        //Hillbert(&idc, 3);
        //Julia1(&idc, ax, ay);
        //SierpinskiTriangele3(&idc);
        //L_systemDraw2(&idc);
        //IFS_system(&idc);
        //DrawRipple(&idc, sr, idc.data, idc.step);
        //MandlbrotJuliaDraw2(height, width, image, step, cn,2);
        //DrawCloud(&idc);
        DrawNewton2(&idc);
        //DrawBranch2(&idc);
      }
      //fractal_draw(sj, height, width, image, step, cn);
      //fractal_draw(si, height, width, image, step, cn);
    }
    //CSceneCaDoDraw(sj, height, width, image, step, cn);
    UTIME_END();
    //break;
    //imwrite("b.bmp", height, width, image, step, cn);
    cvShowImage(wndname, height, width, image, step, cn);
    ch = cvWaitKey(10);
  }

  fractal_clear(sj);
  fractal_clear(si);
  SAFEFREE(image);
  SAFEFREE(imagebk);
  cvDestroyWindow(wndname);
  return 0;
}
#endif
