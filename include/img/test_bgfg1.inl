
int test_bgfg1()
{
  enum { h2 = 100, w2 = 100, al2 = w2 * 3, ai = 3, buflen = 50000 };
  CvBGStatModel* pBGStatModel = NULL;
  unsigned char img2[h2 * al2];
  unsigned char buf[buflen];
  int n = h2 * w2;
  pBGStatModel = cvCreateAvgBGModel_process(h2, w2, img2, al2, ai, 0, buf, buflen);
  //pBGStatModel = cvCreateGaussianBGModel( h2, w2, img2, al2, ai, 0 );
  //pBGStatModel = cvCreateGaussianBG2Model( h2, w2, img2, al2, ai, 0 );
  //pBGStatModel = cvCreateFGDStatModel( h2, w2, img, al2, ai, 0 );

  {
    //utime_start( _start_time );
    cvUpdateBGStatModel(img2, al2, pBGStatModel);
    //m = get_no_zero_pix_number(n, pBGStatModel->foreground);
    //printf( "%1.4f %1.6f ", m*1./n, utime_elapsed( _start_time ) );
  }
  return 0;
}
