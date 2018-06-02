//
// 亮度/对比度调整
//
// Photoshop图像亮度/对比度调整的原理：
//
//     一、对比度算法公式。
//
//     Photoshop对于对比度增量，是按给定值的正负分别处理的。
//
//     如果用newRGB表示图像像素新的R、G、B分量，RGB表示图像像素R、G、B分量，Threshold为给定的阀值，Contrast为对比度增量，当Contrast大于0时：
//
//          1) newRGB = RGB + (RGB - Threshold) * (1 / (1 - Contrast / 255) - 1)
//
//     其中，当Contrast等于255时(RGB - Threshold) * (1 / (1 - Contrast / 255) - 1)为无限(±)，由于RGB最大最小值分别为255和0，因此，只能按Threshold来确定newRGB，即newRGB = RGB >= Threshold? 255 : 0，这实际就是设置图像阀值，图像由最多八种颜色组成，即红、黄、绿、青、蓝、紫及黑与白，在灰度图上也只有最多8条线。
//
//     当Contrast小于0时：
//
//         2) newRGB = RGB + (RGB - Threshold) * Contrast / 255
//
//     其中，当Contrast等于-255时，图像RGB各分量都等于阀值，图像呈全灰色，灰度图上只有1条线，即阀值灰度。
//
//     二、图像亮度调整。本文采用的是最常用的非线性亮度调整（Phoposhop CS3以下版本也是这种亮度调整方式，CS3及以上版本也保留了该亮度调整方式的选项）。
//
//     三、图像亮度/对比度综合调整算法。这个很简单，当亮度、对比度同时调整时，如果对比度大于0，现调整亮度，再调整对比度；当对比度小于0时，则相反，先调整对比度，再调整亮度。
//
//     下面是用BCB2007和GDI+位图数据写的Photoshop图像亮度/对比度调整代码，包括例子代码：
//
// 在亮度/对比度调整函数BrightAndContrast中，首先按前面介绍的原理制造了一个256个元素大小的查找表，然后对图像数据逐像素按R、G、B分量值在查找表中取得调整后的数据，因此处理速度相当快。
// 下面是例子程序运行界面截图（对比度255时）：


//---------------------------------------------------------------------------
INT CheckValue(INT value)
{
  return (value & ~0xff) == 0 ? value : value > 255 ? 255 : 0;
}
//---------------------------------------------------------------------------

// 亮度/对比度调整
int BrightAndContrast(const img_t* im, INT bright, INT contrast, BYTE threshold, img_t* im2)
{
  BYTE values[256] = {0};
  FLOAT cv;
  INT i, x, y;

  if (bright == 0 && contrast == 0) {
    return 0;
  }

  cv = contrast <= -255 ? -1.0f : contrast / 255.0f;

  if (contrast > 0 && contrast < 255) {
    cv = 1.0f / (1.0f - cv) - 1.0f;
  }


  for (i = 0; i < 256; i ++) {
    INT v = contrast > 0 ? CheckValue(i + bright) : i;

    if (contrast >= 255) {
      v = v >= threshold ? 255 : 0;
    }
    else {
      v = CheckValue(v + (INT)((v - threshold) * cv + 0.5f));
    }

    values[i] = contrast <= 0 ? CheckValue(v + bright) : v;
  }

  imsetsamesize(im2, im);

  switch (im->c) {
  case 1:
    for (y = 0; y < im->h; ++y) {
      const uchar* p = im->tt.data + y * im->s;
      uchar* p2 = im2->tt.data + y * im2->s;

      for (x = 0; x < im->w; ++x, p += im->c, p2 += im->c, p2 += im2->c) {
        p2[0] = values[p[0]];
      }
    }

    break;

  case 3:
  case 4:
    for (y = 0; y < im->h; ++y) {
      const uchar* p = im->tt.data + y * im->s;
      uchar* p2 = im2->tt.data + y * im2->s;

      for (x = 0; x < im->w; ++x, p += im->c, p2 += im2->c) {
        p2[0] = values[p[0]];
        p2[1] = values[p[1]];
        p2[2] = values[p[2]];
      }
    }

    break;
  }

  return 0;
}
//---------------------------------------------------------------------------

int test_BrightAndContrast()
{
  img_t im[3] = {0};
  img_t* im1 = im + 1;
  img_t* im2 = im + 2;
  const char* name = "亮度/对比度调整 ";
  int ch = 0, bright = 0, contrast = 255, threshold = 121;
  _chdir("D:/pub/pic");
  imread("source1.jpg", 3, 1, im);
  imshow2(name, im);

  cvCreateTrackbar("亮度", name, &bright, 256, 0);
  cvCreateTrackbar("对比度", name, &contrast, 256, 0);
  cvCreateTrackbar("阀值", name, &threshold, 256, 0);

  for (; 'q' != ch && 'Q' != ch;) {
    BrightAndContrast(im, bright, contrast, threshold, im1);
    imcath(2, im, im2);
    imshow2(name, im1);
    ch = cvWaitKey(50);
  }

  imfree(im);
  return 0;
}
//---------------------------------------------------------------------------

