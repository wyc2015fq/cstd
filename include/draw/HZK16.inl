#ifndef _EASYDRAW_INL_
#define _EASYDRAW_INL_
#include "HZK16.inl"
//英文使用的是8*16的矩阵，中文用24*24的 矩阵显示“中”字。
static const unsigned char fontdata_8x16[] = {
#include "fontdata_8x16.txt"
};
static const unsigned char HZK16[] = {
#include "HZK16.txt"
};
int HZK16_getbit(const struct font_t* fo, int ch, font_bit_t* bit)
{
  uchar hi = (ch >> 8) & 0xff;
  uchar lo = (ch) & 0xff;
  int bx = 0, step = 0;
  uchar* data = NULL;
  UNUSED(fo);
  if (ch < 128) {
    data = (unsigned char*)(fontdata_8x16 + ch * 16);
    bx = 8;
    step = 1;
  }
  else {
    unsigned int area = hi - 0xA1;
    unsigned int where = lo - 0xA1;
    data = (unsigned char*)HZK16 + (area * 94 + where) * 32;
    bx = 16;
    step = 2;
  }
  if (bit) {
    bit->x = bit->y = 0;
    bit->by = 16;
    bit->bx = bx;
    bit->step = step;
    bit->bit = data;
  }
  return bx;
}
int HZK16_font_init(font_t* fo)
{
  strncpy(fo->name, "HZK16", countof(fo->name));
  fo->h = 16;
  fo->w = 16;
  fo->bl = 0;
  fo->bpp = 1;
  fo->getbit = HZK16_getbit;
  return 0;
}
/* lcd pixel display, color = 0x00RRGGBB */
void imput_pixel(img_t* im, int x, int y, unsigned int color)
{
  int line_width = im->s, pixel_width = im->c, bits_per_pixel = im->c << 3;
  unsigned char* fbmem = im->tt.data;
  unsigned char* pen_8 = fbmem + y * line_width + x * pixel_width; /* 计算像素点的位置 */
  unsigned short* pen_16;
  unsigned int* pen_32 ;
  unsigned int red, green, blue;
  if ((unsigned)x > (unsigned)im->w || (unsigned)y > (unsigned)im->h) {
    return ;
  }
  pen_16 = (unsigned short*)pen_8;
  pen_32 = (unsigned int*)pen_8;
  switch (bits_per_pixel) {
  case 8: {
    *pen_8 = color;
    break;
  }
  case 16: {
    /* 提取R:G:B = 5 6 5 */
    red = (color >> 16) & 0xff;
    green = (color >> 8) & 0xff;
    blue = (color >> 0) & 0xff;
    /* 重新组合颜色 */
    color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
    *pen_16 = color;/* 写入象数点 */
    break;
  }
  case 24: {
    /* 提取R:G:B = 5 6 5 */
    red = (color >> 16) & 0xff;
    green = (color >> 8) & 0xff;
    blue = (color >> 0) & 0xff;
    /* 重新组合颜色 */
    pen_8[0] = blue;
    pen_8[1] = green;
    pen_8[2] = red;
    *pen_16 = color;/* 写入象数点 */
    break;
  }
  case 32: {
    *pen_32 = color;
    break;
  }
  default : {
    printf("can't support %d bpp\n", bits_per_pixel);
    break;
  }
  }
}
/* A central LCD display */
void imput_ascii(img_t* im, int x, int y, unsigned char c, COLOR color)
{
  unsigned char* dots = (unsigned char*) &fontdata_8x16[c * 16];
  int i, b;
  unsigned char byte;
  for (i = 0; i < 16; i++) {
    byte = dots[i];
    for (b = 7; b >= 0; b--) {
      if (byte & (1 << b)) {
        /* show */
        imput_pixel(im, x + 7 - b, y + i, color); /* white */
      }
      else {
        /* hide */
        imput_pixel(im, x + 7 - b, y + i, 0); /* black */
      }
    }
  }
}
/* Chinese characters displayed on the LCD */
void imput_chinese(img_t* im, int x, int y, void* str0, const unsigned char* hzkmem, COLOR color)
{
  unsigned char* str = (unsigned char*)str0;
  unsigned int area = str[0] - 0xA1;
  unsigned int where = str[1] - 0xA1;
  const unsigned char* dots = hzkmem + (area * 94 + where) * 32;
  unsigned char byte;
  int i, j, b;
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 2; j++) {
      byte = dots[i * 2 + j];
      for (b = 7; b >= 0; b--) {
        if (byte & (1 << b)) {
          /* show */
          imput_pixel(im, x + j * 8 + 7 - b, y + i, color); /* white */
        }
        else {
          /* hide */
          imput_pixel(im, x + j * 8 + 7 - b, y + i, 0); /* black */
        }
      }
    }
  }
}
void imput_str(img_t* im, int x, int y, const void* str0, int len, COLOR color)
{
  int i, x0=x;
  unsigned char* str = (unsigned char*)str0;
  len = len < 0 ? strlen((char*)str) : len;
  for (i = 0; i < len;) {
    if ('\n'==str[i]) {
        x=x0;
        y += 16;
        i += 1;
        continue;
    }
    if (0x80 & str[i]) {
      imput_chinese(im, x, y, str + i, HZK16, color);
      i += 2;
      x += 16;
    }
    else {
      imput_ascii(im, x, y, str[i], color);
      x += 8;
      ++i;
    }
  }
  return;
}
void imput_rect_solid(img_t* im, int x, int y, int w, int h, COLOR color_fill)
{
  int i, j;
  if (color_fill) {
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        imput_pixel(im, x + j, y + i, color_fill);
      }
    }
  }
}
void imput_rect(img_t* im, int x, int y, int w, int h, COLOR color_fill, COLOR color_line, int line_w)
{
  if (color_fill) {
    imput_rect_solid(im, x, y, w, h, color_fill);
  }
  if (color_line) {
    imput_rect_solid(im, x, y, w, line_w, color_line);
    imput_rect_solid(im, x, y + h - line_w, w, line_w, color_line);
    imput_rect_solid(im, x, y, line_w, h, color_line);
    imput_rect_solid(im, x + w - line_w, y, line_w, h, color_line);
  }
  return ;
}
void imput_circle(img_t* im, int cx, int cy, int radius, COLOR color)
{
  int mx = cx, my = cy;
  int x = 0, y = radius;
  int d = 1 - radius; //起点(0,R),下一点中点(1,R-0.5),d=1*1+(R-0.5)*(R-0.5)-R*R=1.25-R,d只参与整数运算，所以小数部分可省略
  while (y > x) { // y>x即第一象限的第1区八分圆
    imput_pixel(im, x + mx, y + my, color);
    imput_pixel(im, y + mx, x + my, color);
    imput_pixel(im, -x + mx, y + my, color);
    imput_pixel(im, -y + mx, x + my, color);
    imput_pixel(im, -x + mx, -y + my, color);
    imput_pixel(im, -y + mx, -x + my, color);
    imput_pixel(im, x + mx, -y + my, color);
    imput_pixel(im, y + mx, -x + my, color);
    if (d < 0) {
      d = d + 2 * x + 3;
    }
    else {
      d = d + 2 * (x - y) + 5;
      y--;
    }
    x++;
  }
  return;
}
#endif // _EASYDRAW_INL_

