#if 0
int x1, y1, x2, y2;
double e, f, x, y, theta = dp * i;
// 求直角坐标中对应的点
x = x0 + j * cos(theta);
y = y0 + j * sin(theta);

if (x < 1)
{
  x = 1;
}
else if (x > pic_xsize - 2)
{
  x = pic_xsize - 2;
}

if (y < 1)
{
  y = 1;
}
else if (y > pic_ysize - 2)
{
  y = pic_ysize - 2;
}

// 在直角坐标中的周围四点
x1 = (int)floor(x);
y1 = (int)floor(y);
x2 = x1 + 1;
y2 = y1 + 1;
pos = pic->tt.data + pic->s * (x1 - 1) + (y1 - 1);
// 处理越界情况
value_yue_jie(x1, x2, y1, y2, pic_xsize, pic_ysize);
//在X和Y方向分别进行线性插值
e = pos[0] * (x - x1) + pos[pic->s] * (x2 - x);
f = pos[1] * (x - x1) + pos[pic->s + 1] * (x2 - x);
polarv->tt.data[polarv->s * (num_i - 1) + j - 1] = round(fabs(e * (y - y1) + f * (y2 - y)));
#else
int x1, y1, x2, y2;
int e, f, x, y, theta = ((int)i) % 360;
// 求直角坐标中对应的点
x = (x0 << 16) + j * cos360[(theta)];
y = (y0 << 16) + j * sin360[(theta)];

if (x < 1 << 16)
{
  x = 1 << 16;
}
else if (x > ((pic_xsize - 2) << 16))
{
  x = (pic_xsize - 2) << 16;
}

if (y < 1 << 16)
{
  y = 1 << 16;
}
else if (y > (pic_ysize - 2) << 16)
{
  y = (pic_ysize - 2) << 16;
}

// 在直角坐标中的周围四点
x1 = (x) >> 16;
y1 = (y) >> 16;
x2 = x1 + 1;
y2 = y1 + 1;
pos = pic->tt.data + pic->s * (x1 - 1) + (y1 - 1);
// 处理越界情况
value_yue_jie(x1, x2, y1, y2, pic_xsize, pic_ysize);
//在X和Y方向分别进行线性插值
#if 0
e = (pos[0] * (x & 0xffff) + pos[pic->s] * ((1 << 16) - (x & 0xffff))) >> 16;
f = (pos[1] * (x & 0xffff) + pos[pic->s + 1] * (0xffff - (x & 0xffff))) >> 16;
polarv->tt.data[polarv->s * (num_i - 1) + j - 1] = (int)(e * (y & 0xffff) + f * (0xffff - (y & 0xffff))) >> 16;
#else
e = (pos[0] * (x & 0xffff) + pos[1] * (0xffff - (x & 0xffff))) >> 16;
f = (pos[pic->s] * (x & 0xffff) + pos[pic->s + 1] * (0xffff - (x & 0xffff))) >> 16;
polarv->tt.data[polarv->s * (num_i - 1) + j - 1] = (e * (y & 0xffff) + f * (0xffff - (y & 0xffff))) >> 16;
#endif
#endif

