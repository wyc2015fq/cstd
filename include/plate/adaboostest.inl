
XRECT B[1000];
CASCADE* ca = 0;
//PCASCADE cascade_scanf( FILE* pf, int maxstagelen, int maxweaklen );
//int detect( PCASCADE ca, int height, int width, const unsigned char* data, int step,
//       real ssmin, real ssmax, real ss, int stepxy, XRECT* B, int B_len );
int rectdetect(int height, int width, const unsigned char* data, int imgwidth)
{
  //int rlen;
  int num;
  int step = 1;
  float ssmin = (float)1.1;
  float ssmax = (float)6.5;
  float ss = (float)1.1;
  int stepxy = 1;
  int B_len = 1000;

  static int inited = 0;

  if (0 == inited) {
    FILE* pf = 0;
    pf = fopen("cas.txt", "rb");
    ca = cascade_scanf(pf, 0, 0);
    fclose(pf);
    inited = 1;
  }

  num = detect(ca,  height, width,  data, imgwidth, ssmin, ssmax, ss, stepxy, B, B_len);
  //int runlen = rect_partition( B, B+num, 0.5f, 0, 0, B+num, &rlen );
  return num;
}
//runlen = rect_partition( run, run+runlen, 0.5f, 0, 0, run+runlen, &rlen );
void enddetect(void)
{
  del_cascade(ca);
}
