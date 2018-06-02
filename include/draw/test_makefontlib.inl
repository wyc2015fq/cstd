
int makefontlib(int n, unsigned char* in, unsigned char* out, int thd)
{
  int i, j, k;

  for (i = 0, k = 0; i < n;) {
    unsigned char bt = 0;

    for (j = 0; j < 8 && i < n; ++j, ++i) {
      bt |= (in[i] > thd) << (7 - j);
    }

    out[k++] = bt;
  }

  return 0;
}
int test_makefontlib(int fh, int fw, char* FontName, char* str)
{
  int y, x;
  int cn = 1;
  int h = fh;
  int w = h * strlen(str) / 2;
  int al = w * cn;
  char buf[256];
  unsigned char* img = MALLOC(unsigned char, h * w * cn);
  int outlen = (h * w * cn + 7) / 8;
  unsigned char* out = MALLOC(unsigned char, outlen);
  assert(0 == (h % 8) && 0 == (w % 8));
  y = 0;
  x = 0;
  memset(img, 0, h * w * cn);
  xDrawText(h, w, img, al, cn, y, x, FontName, fh, fw, 0, 0, str, CC_RGB(255, 255, 255));
  makefontlib(h * w, img, out, 128);
  _snprintf(buf, 256, "%s.bmp", str);
  //imwrite( buf, h, w, img, al, cn );
  {
    FILE* pf;
    _snprintf(buf, 256, "%s.txt", str);
    pf = fopen(buf, "wb");

    if (pf) {
      int i;

      for (i = 0; i < outlen; ++i) {
        fprintf(pf, "0x%02x\r\n", out[i]);
      }

      fclose(pf);
    }
  }
  {
    FILE* pf;
    unsigned char color[3] =  {
      255, 255, 255
    };
    _snprintf(buf, 256, "%s.txt", str);
    pf = fopen(buf, "rb");
    //memset(out, 0, outlen);
    memset(img, 0, h * w * cn);

    if (pf) {
      int i;

      for (i = 0; i < outlen; ++i) {
        //fscanf(pf, "0x%x", out+i);
      }

      fclose(pf);
    }

    PutOneChar(h, w, img, al, cn, 0, 0, h, w, out, w / 8, color);
    _snprintf(buf, 256, "test_%s.bmp", str);
    //imwrite( buf, h, w, img, al, cn );
  }
  SAFEFREE(img);
  SAFEFREE(out);
  return 0;
}
int tool_makefontlib(int argc, char* argv[])
{
  if (argc > 3) {
    int fh, fw;
    char* FontName;
    char* str;
    fh = atoi(argv[1]);
    fw = atoi(argv[2]);
    FontName = argv[3];
    str = argv[4];
    test_makefontlib(fh, fw, FontName, str);
  }

  //test_Digi7Segment();
  return 0;
}
