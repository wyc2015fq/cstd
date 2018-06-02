img_t* imFilter2D(img_t* a, CvSize ksize, kernel_type* kernel, IPOINT anchor)
{
  img_t* b = imnew(a->h, a->w, a->c);
  cvFilter2D(a->h, a->w, a->data, a->s, CC_MAKETYPE(CC_8U, 1),
      b->data, b->s, CC_MAKETYPE(CC_8U, 1), ksize, kernel, anchor);
  return b;
}

void cvGabor(int height, int width, unsigned char* src, int src_step, unsigned char* dst, int dst_step,
    int iMu, int iNu, double dSigma, double dF, int Type);

img_t* imGabor(img_t* a, int iMu, int iNu, double dSigma, double dF, int Type)
{
  img_t* b = imnew(a->h, a->w, a->c);
  cvGabor(a->h, a->w, a->data, a->s, b->data, b->s,
      iMu, iNu, dSigma, dF, Type);
  return b;
}

int test_cvFilter2D()
{
  int i;
  strlist_t slist = {0};
  get_file_list_txt("F:/pub/bin/imgproc/list.txt", &slist);

  for (i = 0; i < slist.used; ++i) {
    img_t* a = imread2(slist.strs[i], 1), *b;

    if (a) {
      kernel_type sobel[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
      kernel_type sobel2[9] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
      {
        utime_start(_start_time);
        //b = imFilter2D(a, cvSize(3,3), sobel2, iPOINT(1,1));
        b = imGabor(a, 1, 1, 1, 1, 3);
        printf("%f\n", utime_elapsed(_start_time));
      }
      imshow("a", a);
      imshow("b", b);
      cvWaitKey(-1);
      imfree(a);
      imfree(b);
    }
  }

  free_strlist(&slist);
  return 0;
}
