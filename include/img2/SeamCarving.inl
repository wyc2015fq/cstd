//#include "img/color.inl"
// Seam Carving 算法是2007年siggraph上的一篇paper，
// 名为《Seam Carving for Content-Aware Image Resizing》，这篇paper提出了一种内容感知的图像缩放算法，继这篇paper的思想，
//后来近几年siggraph上面也有好几篇都是内容感知的图像缩放算法。内容感知的图像缩放算法一般用于图像的裁剪，就是有的时候，
//你觉得一张照片有点大，你希望把它裁剪的小一些，但是你又想保留照片中的物体，这个时候就要用到内容感知的突刺昂缩放算法了。
//该开始学习这个算法的时候，看起来好像很高深莫测的样子，理论看起来好像牛逼哄哄，什么能量，动态规划，都把我吓到了，最后理论的东西我实在是受不鸟了，于是果断直接解读代码，看了代码以后，才知道这个算法其实非常简单，难度就比普通的图像缩放难那么一点点，其实算法的原理很简单。废话不多说，这边只介绍缩小算法，且是图像宽度缩小的算法，对于图像高度缩小，只需对图像转置处理，总之paper中其它功能的实现，大同小异，整个过程说的简单一点就是通过寻找一条线，如上面的图1所示，找到了线以后，把那条线上的像素点全部删除，从而实现缩小操作。
//该过程是通过迭代实现的，每次只能移除一列，迭代直到宽度满足要求。下面介绍寻找移除能量线的方法，需要注意的是，每次移除一列，都要重新计算能量图，也就是说，梯度图要重新计算，才能进行下一轮的移除工作。
// http://www.faculty.idc.ac.il/arik/SCWeb/imret/
// http://www.inf.ufrgs.br/~eslgastal/DomainTransform/
#include "img/imgopt.inl"
//#include "img/imedge.inl"
int imSobel_mag(const img_t* im, img_t* im2)
{
  img_t im1[1] = {0};
  imreplicate_border(im, im1, iRECT(1, 1, 1, 1));
  im = im1;
  {
    int x, y, w = im->w - 2, h = im->h - 2, s = im->s;
    imsetsize(im2, h, w, sizeof(ushort), 1);
    for (y = 0; y < h; ++y) {
      const uchar* src = im->tt.data + (y + 1) * im->s + 1;
      ushort* dst = (ushort*)(im2->tt.data + y * im2->s);
      for (x = 0; x < w; ++x, ++src, ++dst) {
        int gx = (src[1] << 1) + src[-s + 1] + src[s + 1] - (src[-1] << 1) - src[-s - 1] - src[s - 1];
        int gy = (src[s] << 1) + src[s - 1] + src[s + 1] - (src[-s] << 1) - src[-s - 1] - src[-s + 1];
        *dst = (IABS(gx) + IABS(gy));
      }
    }
  }
  imfree(im1);
  return 0;
}
//算法流程
int SeamCarving(img_t* im, int new_w)
{
  //先找出图片最底部的一行中，能量最小的像素点作为起始点
  int x, y, w = im->w, h = im->h;
  int c = im->c, i = 0;
  img_t ims[10] = {0};
  img_t* gry = ims + i++;
  img_t* energy = ims + i++;
  int* Path = NULL;
  MYREALLOC(Path, h);
  if (new_w > im->w) {
    imreplicate_border(im, im, iRECT(0, 0, new_w - im->w, 0));
    im->w = w;
    //imshow(im);cvWaitKey(-1);
  }
  while (im->w != new_w) {
    im2gry(im, gry);
    //1、算法第一步：计算图像能量图
    //能量图一般是图像像素的梯度模值，为了简化计算可先转换成灰度图像，然后直接采用如下公式(直接用x、y方向上的差分取绝对值，然后相加)，其实这一步就是相当于边缘检测算法一样：
    imSobel_mag(gry, energy);
    //imshowfmt(energy, PF_16bppGray);//cvWaitKey(-1);
    //2、算法第二步：寻找最小能量线
    //最小能量线指的是需要被移除的那一列：首先需要以图像第一行或最后一行为开始行进行迭代。下面实现的为从图像最后一行开始，往上迭代，
    //min_x是图片最底部的一行的能量值最小的像素点 因此遍历的时候，要往上寻找
    {
      int min_x = 0;
      int minx, Maxy;
      ushort* en = (ushort*)(energy->tt.data + (h - 1) * energy->s);
      for (x = 0; x < w; x++) {
        if (en[x] < en[min_x]) {
          min_x = x;
        }
      }
      Path[h - 1] = min_x;
      x = min_x;//起始行的能量最小点
      //从底部往上寻找最小能量线
      for (y = h - 2; y >= 0; y--) { //从下往上
        ushort* en = (ushort*)(energy->tt.data + (y) * energy->s);
        minx = x; //首个顶点迭代更新
        //计算上一行中 三个邻接像素中能量值最小的像素点 作为上一行的能量最小点
        Maxy = en[x];
        if (en[x - 1] < Maxy) { //
          minx = x - 1;
        }
        if (en[x + 1] < Maxy) { //up-right
          minx = x + 1;
        }
        Path[y] = minx;
        x = minx;
      }
    }
    //3、算法第三步：移除得到的最小能量线，让图片的宽度缩小一个像素
    // 移除最小能量线，同时所有位于最小能量线右边的像素点左移一个单位，从而实现图像缩小宽度缩小一个单位。
    // 移除的时候 为了让图像看起来自然，需要在移除缝线的地方进行平均，假设移除坐标为P(x,y),那么移除后P(x-1,y)的像素值为P(x-1,y)与P(x,y)的像素值的平均。P(x+1,y)的像素值为P(x-1,y)与P(x,y)的像素值的平均，然后才能把P(x+1,y)移动到P(x,y)的位置。
    if (im->w > new_w) {
      for (y = 0; y < h; ++y) {
        int k = BOUND(Path[y], 1, w - 2);
        //移除的时候 为了让图像看起来自然，需要在移除缝线的地方进行平均，假设移除坐标为P(x,y),那么
        //移除后P(x-1,y)的像素值为P(x-1,y)与P(x,y)的像素值的平均
        //P(x+1,y)的像素值为P(x-1,y)与P(x,y)的像素值的平均，然后才能把P(x+1,y)移动到P(x,y)的位置
        uchar* src = im->tt.data + y * im->s + k * im->c;
        for (i = 0; i < im->c; ++i) {
          src[-c + i] = (src[-c + i] + src[i]) >> 1;
          src[i] = (src[i] + src[c + i]) >> 1;
        }
        memmove(src + c, src + 2 * c, (w - k - 3)*c);
      }
      im->w--;
    }
    else {
      //对于图像的放大算法原理一样，先找到最小能量线，设能量线上点的坐标为P(x,y),则在P(x,y)、P(x+1,y)中心位置插入新的像素，像素值为P(x,y)与P(x+1,y)的平均。
      for (y = 0; y < h; ++y) {
        int k = BOUND(Path[y], 1, w - 2);
        uchar* src = im->tt.data + y * im->s + k * im->c;
        memmove(src + 2 * c, src + c, (w - k - 1)*c);
        for (i = 0; i < im->c; ++i) {
          src[c + i] = (src[i] + src[c + i]) >> 1;
        }
      }
      im->w++;
    }
    //imshow(gry); imshow(im);cvWaitKey(-1);
  }
  FREE(Path);
  imfrees(ims, 10);
  return 0;
}

