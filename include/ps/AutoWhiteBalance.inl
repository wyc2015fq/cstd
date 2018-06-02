
// 基于灰度世界、完美反射、动态阈值等图像自动白平衡算法的原理、实现及效果
// Posted on 2013-04-20 11:03 Imageshop 阅读(1452) 评论(23) 编辑 收藏
//      白平衡是电视摄像领域一个非常重要的概念，通过它可以解决色彩还原和色调处理的一系列问题。白平衡是随着电子影像再现色彩真实而产生的，在专业摄像领域白平衡应用的较早，现在家用电子产品（家用摄像机、数码照相机）中也广泛地使用，然而技术的发展使得白平衡调整变得越来越简单容易，但许多使用者还不甚了解白平衡的工作原理，理解上存在诸多误区。它是实现摄像机图像能精确反映被摄物的色彩状况，有手动白平衡和自动白平衡等方式，本文简要的介绍了几种自动白平衡算法。
// 一、原始的灰度世界算法
// 　　灰度世界算法（Gray World)是以灰度世界假设为基础的,该假设认为对于一幅有着大量色彩变化的图像, R、 G、 B 三个分量的平均值趋于同一个灰度K。一般有两种方法来确定该灰度。
//
//        （1)直接给定为固定值, 取其各通道最大值的一半,即取为127或128；
//        （2)令 K = (Raver+Gaver+Baver)/3,其中Raver,Gaver,Baver分别表示红、 绿、 蓝三个通道的平均值。
//          算法的第二步是分别计算各通道的增益：
//
//              Kr=K/Raver;
//  　　　　　　Kg=K/Gaver;
//              Kb=K/Baver;
//
//          算法第三步为根据Von Kries 对角模型,对于图像中的每个像素R、G、B，计算其结果值：
//
//              Rnew = R * Kr;
// 　　　　　　 Gnew = G * Kg;
// 　　　　　　 Bnew = B * Kb;
//
//          对于上式，计算中可能会存在溢出（>255,不会出现小于0的)现象，处理方式有两种。
//          a、 直接将像素设置为255，这可能会造成图像整体偏白。
//          b、 计算所有Rnew、Gnew、Bnew的最大值，然后利用该最大值将将计算后数据重新线性映射到[0,255]内。实践证明这种方式将会使图像整体偏暗，建议采用第一种方案。
//         一般来说，灰度世界算法的效果还是比较好的呢，并且该算法的执行速度非常之快，目前也存在了不少对该算法进行改进的效果，有时间我在整理一下。

int Gray_World_RGB(int h, int w, const unsigned char* a, int al, unsigned char* b, int bl, int cn)
{
  int K = 0, Raver = 0, Gaver = 0, Baver = 0, Kr, Kg, Kb;
  int i, j;

  for (i = 0; i < h; ++i) {
    const unsigned char* a1 = a + i * al;

    for (j = 0; j < w; ++j) {
      Raver += a1[j * cn + 0];
      Gaver += a1[j * cn + 1];
      Baver += a1[j * cn + 2];
    }
  }

  K = ((Raver + Gaver + Baver) << 8) / 3;
  Kr = K / Raver;
  Kg = K / Gaver;
  Kb = K / Baver;

  for (i = 0; i < h; ++i) {
    const unsigned char* a1 = a + i * al;
    unsigned char* b1 = b + i * bl;

    for (j = 0; j < w; ++j) {
      int Rnew = (a1[j * cn + 0] * Kr) >> 8;
      int Gnew = (a1[j * cn + 1] * Kg) >> 8;
      int Bnew = (a1[j * cn + 2] * Kb) >> 8;

      b1[j * cn + 0] = MIN(Rnew, 255);
      b1[j * cn + 1] = MIN(Gnew, 255);
      b1[j * cn + 2] = MIN(Bnew, 255);
    }
  }

  return 0;
}

// 二、完美反射算法
//       当初写这个代码的时候的一些参考文献一下子也找不到了，就从已经写好的代码中描述下该算法的过程吧。
//       原理：完美全反射理论perfect Reflector假设图像上最亮点就是白点，并以此白点为参考对图像进行自动白平衡,最亮点定义为R+G+B的最大值,具体编码步骤如下：
//       （1）计算每个像素的R\G\B之和，并保存到一临时内存块中。

int Perfect_Reflector(int Height, int Width, const unsigned char* bmp_Pointer, int Stride, unsigned char* b, int bl, int cn)
{
  int X, Y, Sum = 0, Threshold = 0;
  int HistRGB[3 * (1 << 8)] = {0};
  const unsigned char* Pointer;
  short* SumP0 = MALLOC(short, Height * Width);
  short* SumP = SumP0;
  int Ratio = 10; // 10%
  int AvgR = 0, AvgG = 0, AvgB = 0, Amount = 0, MaxValue = 0;

  for (Y = 0; Y < Height; Y++) {
    Pointer = bmp_Pointer + Y * Stride;

    for (X = 0; X < Width; X++) {
      Sum = (*(Pointer) + * (Pointer + 1) + * (Pointer + 2));   // R+G+B
      HistRGB[Sum]++;
      *SumP = (short)Sum;
      Pointer += 3;
      SumP++;
    }
  }

  //      （2）按R+G+B值的大小计算出其前10%或其他Ratio的白色参考点的的阈值T。
  for (Y = 767; Y >= 0; Y--) {
    Sum += HistRGB[Y];

    if (Sum > Width * Height * Ratio / 100) {
      Threshold = Y;
      break;
    }
  }

  //     （3）遍历图像中的每个点，计算其中R+G+B值大于T的所有点的R\G\B分量的累积和的平均值。
  for (Y = 0; Y < Height; Y++) {
    Pointer = bmp_Pointer + Y * Stride;

    for (X = 0; X < Width; X++) {
      if (*SumP > Threshold) {
        AvgB += *Pointer;
        AvgG += *(Pointer + 1);
        AvgR += *(Pointer + 2);             // 为获得增益做准备
        Amount++;
        MaxValue = MAX(MaxValue, *(Pointer + 0));
        MaxValue = MAX(MaxValue, *(Pointer + 1));
        MaxValue = MAX(MaxValue, *(Pointer + 2));
      }

      Pointer += 3;
      SumP++;
    }
  }

  AvgB /= Amount;
  AvgG /= Amount;
  AvgR /= Amount;
  // 　　 （4）对每个点将像素量化到[0,255]之间。

  for (Y = 0; Y < Height; Y++) {
    unsigned char* b1 = b + Y * bl;
    Pointer = bmp_Pointer + Y * Stride;

    for (X = 0; X < Width; X++) {
      // 另外一种算法需要先计算不抑制重新计算的RGB的范围，然后求RGB的最大值，如果最大值大于255，则所有的结果都要除以最大值在乘以255，但实际表明该算法、   不合适；
      int Blue = *Pointer * MaxValue / AvgB;
      int Green = *(Pointer + 1) * MaxValue / AvgG;
      int Red = *(Pointer + 2) * MaxValue / AvgR;

      // 这里需要判断，因为RGB空间所有的颜色转换到YCbCr后，并不是填充满了0-255的范围的，反转过去就会存在一些溢出的点。
      // 编译后应该比三目运算符的效率高

      *b1 = (byte)CLAMP255(Blue);
      *(b1 + 1) = (byte)CLAMP255(Green);
      *(b1 + 2) = (byte)CLAMP255(Red);
      Pointer += 3;
      b1 += 3;
    }
  }

  free(SumP0);
  return 0;
}

//     从效果上看，该算法应该比灰度世界的效果要好些，但是也还是受到Ratio这个参数的影像。特别是第二个图片，过高的Ration导致图片过于泛白。这个问题可以还是最后量化的哪一步引起的，我会抽空再研究一下其他的量化方式，尽量降低Ration的影响。
//
//     针对上述的第二步，看到很多matlab和VC的代码，有很多人居然先用快速排序对累加后的数据进行排序，然后再取其10%的最大值，对图像的数据进行排序，可能就是再快速的排序都快不起来吧，看到这，也许全国人民都笑了。
//
// 三、动态阈值算法
//       参考论文：A Novel Automatic White Balance Method For Digital Still Cameras
//       同经典的一些算法相同，算法分为两个步骤：白点检测和白点调整。
//       白点检测：
//            （1）为了增强算法的鲁棒性，原文将图像分成12部分，其中宽高比为4：3，关于这一点，我认为不合理，对图像不是通用的，后文再说。
//             （2）计算每个区域的Cb\Cr分量的平均值Mb/Mr。
//             （3）按下式计算每个区域的Cb\Cr分量的绝对差的累积值Db/Dr：
//                   上式中N为每个区域的像素数。
//
//          （4）如果Db/Dr的值偏小，则我们忽略这一块，因为这表明这一块的颜色分布比较均匀，而这样的局部对于白平衡不好。这个偏小的准则我们稍微再谈。
//
//          （5）统计对于除了符合第四条的的其他区域的Mb/Mr/Db/Dr的平均值作为整幅图像的Mb/Mr/Db/Dr值。
//
// 　　　　　　　　关于这一条，原文的话是：The final Mb、Mr、Db、Dr are obtained by taking the average of those regions that pass this additional step。
//
// 　　　　　　　我在实际中做的时候就是分别对每块进行的，似乎效果也还不错。
//
//          （6）按下述规则初步确定哪些点是属于白色参考点：
//
//
//
//
//
//          （7）对于初步判断已经属于白色参考点的像素，按大小取其亮度值为前10%的位最终确定的白色参考点。
//
//       白点调整：
//
// 　　　　　（1）计算白色参考点亮度值的平均值Raver,Gaver,Baver,（各通道分开计算)。
//
//           （2）按照以下各式计算每个通道的增益：
//
//  　　　　　　　　　　
//
//
//
//
//
//               式中，Ymax就是YCbCr颜色空间中Y分量的在整幅图像中的最大值。
//
//           （3）按照以下各式计算最终每个通道的颜色值：
//
//
//
//
//
//
//
//           其中R/G/B为在原始的颜色空间中的值，注意这里要进行溢出检测的。
//
// 　　      简单的谈下白点检测的分块操作吧，原文把图像分成4*3的12快，这样做事针对于我们很多数码照片是这个比例的，如果通用，我觉得应该用每个块的大小来控制，比如每块为 100*100个像素。
//
//           这个算法的效果如下：
//
//
//
//
//
//
//
//                            原图                                              块大小50*50　　　　　　　　　　　　　　　　　　  块大小100*100
//
//       上三图表明：1、该算法效果非常好；2、对块大小不太敏感，因此非常适合于自动化操作。
//
//        关于RGB到YCbCr的快速转换，可以参考：颜色空间系列3: RGB和YCbCr颜色空间的转换及优化算法
//
//       由于在上述链接的文章中，YCbCr颜色空间已经被转换到[0,255]，因此白色检测部分的第（6）条中的sign函数就不需要了。
//
//       同样，提供个编译好的文件给有兴趣研究该算法的朋友看看效果：
//
//       http://files.cnblogs.com/Imageshop/AutoWhiteBalance.zip
//
// 　　 后记：
//
// 　    针对动态阈值法，很多朋友反映如果YCbCr的值量化在0到255之间的话会出现所有的像素都会被式（6）初步判断为白色参考点。这样前期的工作就失去了意义，算法就变成了类似于完美反射算法那了，稍微有点不同的地方就是两者选择两点的准则有所不同。虽然这样做的最终结果还算不错，但确实和论文的本意像违背了，后面经过实践，如果把YCbCr的值量化在-127到128之间，式6中的Sgn同样适用，则初步判断为白点的数会大量的减少，对于同一个图片，同一个参数两个算法的最终的效果比较如下：
//
// 　　 更新后的下载链接依旧如下，以增加了修正后的功能。
//
