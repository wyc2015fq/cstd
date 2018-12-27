
//#include "cstd.h"
//#include "cfile.h"
//#include "ui/window.inl"

#include "std/fileio_c.h"
#include "std/mem_c.h"
#include "std/img_c.h"
#include "std/iconv_c.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"
//#include "font/msdfgen.inl"

unsigned char screen[200][1400];

int gb2uni_code1(int code) {
  char str[8] = {0};
  wchar_t uni[2] = {0};
  if ((code)&0x80) {
    str[0] = code&0xff;
    str[1] = (code>>8)&0xff;
  } else {
    str[0] = code&0xff;
  }
  iconv_c(ICONV_GB2312, ICONV_UCS2LE,str,2,(char*)uni,4);
  return uni[0];
}
int meminv(int n, void* ptr) {
  char* pc = (char*)ptr;
  char* pe = pc + n;
  for (; pc < --pe;++pc) {
    char c = *pc;
    char e = *pc;
    *pc = e;
    *pe = c;
  }
  return 0;
}


int test_stb_truetype()
{
  const char* fontfile = 0;
  unsigned char* ttf_buffer = NULL;
  int len = 0;
  float ss1;
  stbtt_fontinfo font[1] = {0};
  fontfile = "c:/windows/fonts/arialbd.ttf";
  fontfile = "c:/windows/fonts/simfang.ttf";
  fontfile = "D:/code/git/cstd/include/ocr/train/chinese_fonts/huawenxihei.ttf";
  ttf_buffer = (uchar*)loaddata(fontfile, &len);
  stbtt_InitFont(font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer,0));
  if (1) {
    unsigned char *bitmap;
    wchar_t *wstr;
    int k;
    wstr = L"取得着色器中的顶点的索引";
    wstr = L"13843213251X";
    for (k = 0; wstr[k]; ++k) {
      int w, h, i, j, c = 'A', s = 20;
      int xoff=0, yoff = 0;
      //wchar_t* tt = L"我";
      //c = gb2uni_code(*(wchar_t*)"我");
      c = wstr[k];
      ss1 = 4*stbtt_ScaleForPixelHeight(font, (float)s);
      bitmap = stbtt_GetCodepointBitmap(font, 0.f, ss1, c, &w, &h, 0, 0);
      if (1) {
        //char invc[] = "@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
        //meminv(countof(invc), invc);
        for (j = 0; j < h; ++j) {
          for (i = 0; i < w; ++i) {
            int t = bitmap[j*w + i] >> 5;
            putchar(" .:ioVM@"[t]);
            putchar(" .:ioVM@"[t]);
          }
          putchar('\n');
        }
        printf("=%d %d\n", w, h);
      }
      free(bitmap);
    }
    printf("");
  }
#if 0
  if (0) {
    int i,j,ascent,baseline,k=0;
    float scale, xpos=2;// leave a little padding in case the character extends left
    char text[100] = "Heljo World!";// intentionally misspelled to show 'lj' brokenness
    //memcpy(text, tt, 2);
    
    stbtt_InitFont(font, ttf_buffer, 0);
    
    scale = stbtt_ScaleForPixelHeight(font, 15);
    stbtt_GetFontVMetrics(font, &ascent,0,0);
    baseline = (int) (ascent*scale);
    
    while (text[k]) {
      int advance,lsb,x0,y0,x1,y1;
      float x_shift = xpos - (float) floor(xpos);
      wchar_t ch = nextCharA(text, k, &k);
      stbtt_GetCodepointHMetrics(font, ch, &advance, &lsb);
      stbtt_GetCodepointBitmapBoxSubpixel(font, ch, scale,scale,x_shift,0, &x0,&y0,&x1,&y1);
      stbtt_MakeCodepointBitmapSubpixel(font, &screen[baseline + y0][(int) xpos + x0], x1-x0,y1-y0, 1400, scale,scale,x_shift,0, ch);
      // note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
      // because this API is really for baking character bitmaps into textures. if you want to render
      // a sequence of characters, you really need to render each bitmap to a temp ttf_buffer, then
      // "alpha blend" that into the working ttf_buffer
      xpos += (advance * scale);
      if (text[k+1]) {
        wchar_t ch1 = nextCharA(text, k, 0);
        xpos += scale*stbtt_GetCodepointKernAdvance(font, ch, ch1);
      }
    }
    
    for (j=0;j < 20;++j) {
      for (i=0;i < 78;++i)
        putchar(" .:ioVM@"[screen[j][i]>>5]);
      putchar('\n');
    }
  }
#endif
  return 0;
}
