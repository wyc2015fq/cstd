

int skip_hist(int h, int w, uchar* A, int al, uchar* B, int bl) {
  int histr[256];
  int histg[256];
  int histb[256];
  int i,j;
  int sumr=0, sumg=0, sumb=0;
  float scale = 255.f/(w*h);
  for (i=0; i<256; ++i) histr[i]=histg[i]=histb[i]=0;
  for (i=0; i<h; ++i) {
    uchar* Ai = A + i*al;
    for (j=0; j<w; ++j) {
      histr[Ai[j*3+0]]++;
      histg[Ai[j*3+1]]++;
      histb[Ai[j*3+2]]++;
    }
  }
  for( i = 0; i < 256; ++i ) {
    sumr += cvRound(histr[i]);
    sumg += cvRound(histg[i]);
    sumb += cvRound(histb[i]);
    histr[i] = (uchar)cvRound(sumr*scale);
    histg[i] = (uchar)cvRound(sumg*scale);
    histb[i] = (uchar)cvRound(sumb*scale);
  }

  for (i=0; i<h; ++i) {
    uchar* Ai = A + i*al;
    uchar* Bi = B + i*bl;
    for (j=0; j<w; ++j) {
      Bi[j*3+0]=histr[Ai[j*3+0]];
      Bi[j*3+1]=histg[Ai[j*3+1]];
      Bi[j*3+2]=histb[Ai[j*3+2]];
    }
  }
  return 0;
}
