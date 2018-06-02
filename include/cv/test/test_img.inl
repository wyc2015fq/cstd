
int test_img() {
  int i, m=100, n = 100;

  Img a(n, n, CC_64F), b(n, n, CC_64F), c(n, n, CC_64F);
  CRNG rng = 10;
  cvRandArr(&rng, &a, CC_RAND_NORMAL, cRealScalar(0), cRealScalar(1));
  cvRandArr(&rng, &b, CC_RAND_NORMAL, cRealScalar(0), cRealScalar(1));
  cvRandArr(&rng, &c, CC_RAND_NORMAL, cRealScalar(0), cRealScalar(1));
  TIC;
  for (i=0; i<m; ++i) {
    b = a+a+a+a+a+a;
    //b = a*a+a*a*7;
  }
  TOC;
#
  TIC;
  for (i=0; i<m; ++i) {
    //cvMatMul(&a, &b, &c);
    if (1) {
      cvAdd(&a, &a, &c);
      cvAdd(&a, &c, &c);
      cvAdd(&a, &c, &c);
      cvAdd(&a, &c, &c);
      cvAdd(&a, &c, &c);
    }
    if (0) {
      Img t0, t1;
      cvMatMul(&a, &a, &t0);
      cvMatMul(&a, &a, &t1);
      cvMulS(&t1, 7, &t1);
      cvAdd(&t0, &t1, &c);
    }
  }
  TOC;
  double dis = cvNorm(&b, &c);
  printf("%lf\n", dis);
  tictoc;
  return 0;
}