
//#include "cstd.h"
//#include "cfile.h"
//#include "ui/window.inl"

#include "std/gui_c.h"
#include "std/fileio_c.h"
#include "std/mem_c.h"
#include "std/img_c.h"
#include "std/iconv_c.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"
//#include "font/msdfgen.inl"

int gb2uni_code1(int code) {
  char str[8] = { 0 };
  wchar_t uni[2] = { 0 };
  if ((code) & 0x80) {
    str[0] = code & 0xff;
    str[1] = (code >> 8) & 0xff;
  }
  else {
    str[0] = code & 0xff;
  }
  iconv_c(ICONV_GB2312, ICONV_UCS2LE, str, 2, (char*)uni, 4);
  return uni[0];
}
int meminv(int n, void* ptr) {
  char* pc = (char*)ptr;
  char* pe = pc + n;
  for (; pc < --pe; ++pc) {
    char c = *pc;
    char e = *pc;
    *pc = e;
    *pe = c;
  }
  return 0;
}
int drawText(stbtt_fontinfo* font, float font_size, int x, int y, wchar_t* wstr, int len, uchar* screen, int h, int w, int step, int cn, COLOR color) {
  int ascent, baseline, k = 0;
  float scale, xpos = x, ypos = y;
  uchar* bit = NULL;
  len = len < 0 ? wcslen(wstr) : len;
  scale = stbtt_ScaleForPixelHeight(font, font_size);
  stbtt_GetFontVMetrics(font, &ascent, 0, 0);
  baseline = (int)(ascent*scale);
  ypos += baseline;
  for (k = 0; k < len; ++k) {
    int advance, lsb, x0, y0, x1, y1;
    float x_shift = xpos - (float)floor(xpos);
    wchar_t ch = wstr[k];
    stbtt_GetCodepointHMetrics(font, ch, &advance, &lsb);
    stbtt_GetCodepointBitmapBoxSubpixel(font, ch, scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
    x0 += xpos;
    x1 += xpos;
    y0 += ypos;
    y1 += ypos;
    if (x1<0 || y1<0 || x0 > w || y0>h) break;
    x0 = BOUND(x0, 0, w);
    x1 = BOUND(x1, 0, w);
    y0 = BOUND(y0, 0, h);
    y1 = BOUND(y1, 0, h);
    if (x0 < x1 && y0 < y1) {
      int hh = y1 - y0, ww = x1 - x0;
      bit = (uchar*)realloc(bit, hh*ww);
      memset(bit, 0, hh*ww);
      stbtt_MakeCodepointBitmapSubpixel(font, bit, ww, hh, ww, scale, scale, x_shift, 0, ch);
      uchar* p = screen + (y0)*step + (x0)*cn;
      draw_font_bitmap(hh, ww, p, step, cn, bit, ww, color);
    }
    xpos += (advance * scale);
    if ((k + 1) < len) {
      wchar_t ch1 = wstr[k + 1];
      xpos += scale*stbtt_GetCodepointKernAdvance(font, ch, ch1);
    }
  }
  if (bit) { free(bit); }
  return k;
}

COLOR rand_rgb() {
  return _RGB((rand() % 256), (rand() % 256), (rand() % 256));
}

COLOR rand_gray() {
  uchar g = rand() % 256;
  return _RGB(g, g, g);
}

int test_stb_truetype()
{
  const char* fontfile = 0;
  unsigned char* ttf_buffer = NULL;
  int len = 0;
  float ss1;
  wchar_t *wstr;
  stbtt_fontinfo font[1] = { 0 };
  fontfile = "c:/windows/fonts/arialbd.ttf";
  fontfile = "c:/windows/fonts/simfang.ttf";
  fontfile = "D:/code/git/cstd/include/ocr/train/chinese_fonts/huawenxihei.ttf";
  ttf_buffer = (uchar*)loaddata(fontfile, &len);
  stbtt_InitFont(font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
  wstr = L"13843213251X";
  wstr = L"gf我取1得着色器中的顶123fdf点的索引";
  if (0) {
    unsigned char *bitmap;
    int k;
    for (k = 0; wstr[k]; ++k) {
      int w, h, i, j, c = 'A', s = 20;
      int xoff = 0, yoff = 0;
      //wchar_t* tt = L"我";
      //c = gb2uni_code(*(wchar_t*)"我");
      c = wstr[k];
      ss1 = 4 * stbtt_ScaleForPixelHeight(font, (float)s);
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
  if (0) {
    int ascent, baseline, k = 0;
    float scale, xpos = 2;// leave a little padding in case the character extends left
    //memcpy(text, tt, 2

    scale = stbtt_ScaleForPixelHeight(font, 32);
    stbtt_GetFontVMetrics(font, &ascent, 0, 0);
    baseline = (int)(ascent*scale);
    unsigned char screen[32][280] = { 0 };

    for (k = 0; wstr[k]; ) {
      int advance, lsb, x0, y0, x1, y1;
      float x_shift = xpos - (float)floor(xpos);
      wchar_t ch = wstr[k++];
      stbtt_GetCodepointHMetrics(font, ch, &advance, &lsb);
      stbtt_GetCodepointBitmapBoxSubpixel(font, ch, scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
      if ((xpos + advance * scale) > 280) break;
      stbtt_MakeCodepointBitmapSubpixel(font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, 280, scale, scale, x_shift, 0, ch);
      xpos += (advance * scale);
      if (wstr[k + 1]) {
        wchar_t ch1 = wstr[k];
        xpos += scale*stbtt_GetCodepointKernAdvance(font, ch, ch1);
      }
    }
    ShowImagePal("asdf", 32, 280, &screen[0][0], 280, 1, PF_8bppGray, NULL);
    waitkey(-1);
  }
  if (0) {
    img_t im[1] = { 0 };
    imsetsize(im, 60, 400, 1, 1);
    drawText(font, 64, 0, 0, wstr, -1, im->data, im->h, im->w, im->s, im->c, _RGBA(255, 0, 0, 80));
    imshow_(im); waitkey(-1);
  }
  const char* file = "E:/data/ew_id/t1_0911.txt";
  if (1) {
    FILE* pf = fopen(file, "rb");
    char buf[1024];
    char gbbuf[1024];
    wchar_t wbuf[1024];
    int j = 0;
    idcard id[1] = { 0 };
    int sslen = 0;
    img_t im[1] = { 0 };
    int h = 64;
    imsetsize(im, h, 400, 1, 1);
    for (j = 0; fgets(buf, 1024, pf) > 0 && j < 100000; ++j) {
      int len = strlen(buf);
      fillImage(im->h, im->w, im->data, im->s, im->c, rand_rgb());
      len = iconv_c(ICONV_UTF8, ICONV_GB2312, buf, len, gbbuf, 1024);
      gbbuf[len] = 0;
      get_idcard(gbbuf, id);
      len = iconv_c(ICONV_GB2312, ICONV_UCS2LE, id->address, -1, (char*)wbuf, 2 * 1024) / 2;
      wbuf[len] = 0;
      int k = drawText(font, h, 0, 0, wstr, -1, im->data, im->h, im->w, im->s, im->c, rand_rgb());
      imshow_(im); waitkey(-1);
    }
    imfree(im);
    fclose(pf);
  }
  return 0;
}
