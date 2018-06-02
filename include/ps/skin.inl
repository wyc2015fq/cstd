//1、二次多项式混合模型
// 二次多项式混合模型首先有SORIANO提出，此后CHIANG对此进行了改进。改进后的模型由两个R-G平面的二次多项式和一个圆方程构成：
//            l_upper(r)=-1.3767r^2 + 1.0743r+0.1452           (1)
//            l_lower(r)=-0.776r^2 + 0.5601r+0.1766            (2)
//            W(r,g) = (r-0.33)^2 + (g-0.33)^2                 (3)
//    在以上三个方程的基础上，肤色区域可以通过一下规则实现：
//     R1:g>l_lower(r) and g<l_upper(r)
//     R2:W(r,g)>=0.0004
//     R3:R>G>B
//     R4:R-G>=45
//上述算法的参考论文 Adaptive skin color modeling using the skin locus.pdf
//                A novel method for detecting lips,eyes and faces in real time
//以及百度文库相关文章：基于混合肤色模型的快速人脸检测算法
//     上式中，小写r,g,b（未涉及）为对R/G/B(byte类型的数据，0-255）进行归一化后的数据，即：
// r=R/(R+G+B);   g=G/(R+G+B);   b=B/(R+G+B);
//  如上所示，算法中涉及到了不少的浮点运算，以及大量的乘法，如果按照源汁原味的来编写代码，程序的效率可想而知。因此，我们着手于算法的优化。
//  首先，我们来看四个判断条件，由于判断条件是不分先后，需要同时满足的地方才是区域，因此应该把简单的判断条件放在最前面判断。
//  首先看如果符合了判断条件R4，条件R3中的R>G肯定是已经成立的，则只需要判断G是否大于B，这是优化手段1。
//  然后我们来看R2的优化，为方便表达，我们这里令Sum=R+G+B，将判断条件R2展开：
//W(r,g)>=0.0004        => (r-0.33)^2 + (g-0.33)^2>0.0004   =>
//(R/Sum-0.33)^2 + (G/Sum-0.33)^2>0.0004   => (R-0.33Sum)^2 + (G-0.33Sum)^2>0.0004*Sum*Sum   =>
//(156R-52Sum)^2 + (256G-52Sum)^2>=0.062*Sum*Sum
//     上式子最后一步同时乘以156, 理论上说156×0.33=51.48，不应该取52的，不过这个0.33本来就是个经验数据，谁说不能是1/3呢。
//     到此，我们看到在式子的最右侧还有个浮点数0.0624，如果不消除该数据，算法速度依旧会有大的影响，常常研究移位的朋友肯定对0.0625这个数字很熟悉，1/16=0.0625，不是吗，懂了吗，还不懂，看代码吧（这里的式子很多都是经验公式，因此，稍微修改一些参数对结果基本无影响)。
//     上述这样做的目的，无非是将浮点数的运算全部转换为整数的运算。
//    最后来看式R1的优化，R1实际上也是两个条件，把他分开来，分别称为R11及R12，对于R11，同样展开：
// 现在大部分的PC都还是32位的系统，因此，使用32位的整数类数据类型速度是最快的，因此，如果上述放大系数的取夺就必须主要使得计算式两边的值都在int.MinValue和 int.MaxValue之间，比如上式，>号左侧算式的肯能最大取值为10000×255×765，是小于int.MaxValue所能表达的范围的，因此放大系数是合理的。
int skin_Adaptive(int Width, int Height, const unsigned char* Scan0, int Stride, unsigned char* SkinScan0, int SkinStride, int cn)
{
  int X, Y;

  for (Y = 0; Y < Height; Y++) {
    const unsigned char* Pointer = Scan0 + Y * Stride;
    unsigned char* SkinP = SkinScan0 + Y * SkinStride;

    for (X = 0; X < Width; X++) {
      int Blue, Green, Red, Lower, Upper;
      *SkinP = 0;                                                                  // 非皮肤区域为黑色

      Blue = *Pointer;
      Green = *(Pointer + 1);
      Red = *(Pointer + 2);

      if (Red - Green >= 45) {                                            //  符合条件R4
        if (Green > Blue) {                                             //   符合条件R3
          int Sum = Red + Green + Blue;
          int T1 = 156 * Red - 52 * Sum;
          int T2 = 156 * Green - 52 * Sum;

          if (T1 * T1 + T2 * T2 >= (Sum * Sum) >> 4) {                  // 符合条件R2，在32位系统要尽量避免用long类型数据，
            T1 = 10000 * Green * Sum;
            Lower = - 7760 * Red * Red + 5601 * Red * Sum + 1766 * Sum * Sum;         // 把这里的公用的乘法提取出来基本没啥优化的效果

            if (T1 > Lower) {                                       // 符合条件R11
              Upper = - 13767 * Red * Red + 10743 * Red * Sum + 1452 * Sum * Sum ;

              if (T1 < Upper) {                                   //  符合条件R12
                *SkinP = 255;
              }
            }
          }
        }
      }

      Pointer += 3;
      SkinP++;
    }
  }

  return 0;
}


//2、基于RGB颜色空间的简单阈值肤色识别
//       在human skin color clustering for face detection一文中提出如下简单的判别算式：
//            R>95 And G>40 And B>20 And R>G And R>B And Max(R,G,B)-Min(R,G,B)>15 And Abs(R-G)>15
//      算法非常之简单，同样主要把复杂的判断条件放到后面去判断，能有效的降低程序的执行时间，参考代码：
int skin_rgb(int Width, int Height, const unsigned char* Scan0, int Stride, unsigned char* SkinScan0, int SkinStride, int cn)
{
  int X, Y;

  for (Y = 0; Y < Height; Y++) {
    const unsigned char* Pointer = Scan0 + Y * Stride;
    unsigned char* SkinP = SkinScan0 + Y * SkinStride;

    for (X = 0; X < Width; X++) {
      int Blue = *Pointer;
      int Green = *(Pointer + 1);
      int Red = *(Pointer + 2);
      int Max, Min;

      if (Red > 95 && Green > 40 && Blue > 20 && Red > Blue && Red > Green && abs(Red - Green) > 15) {
        if (Blue >= Green) {
          Max = Blue;
          Min = Green;
        }
        else {
          Max = Green;
          Min = Blue;
        }

        if (Red > Max) {
          Max = Red;
        }
        else if (Red < Min) {
          Min = Red;
        }

        if (Max - Min > 15) {
          *SkinP = 255;
        }
      }

      Pointer += 3;
      SkinP++;
    }
  }

  return 0;
}
//基于YCbCr颜色空间的简单阈值肤色识别
//    该算法则更为简单，将图像转换到YCbCr颜色空间，然后按下述计算式判断是否属于皮肤区域：
//        (Cb > 77 And Cb < 127)  And (Cr > 133 And Cr < 173)
//      关于RGB和YCbCr颜色空间的转换的优化算法，可参考本博客相关文章。
//      由于当初写这方面的时候没有注明该算法的出处，现在也没从提起了。

int skin_YCbCr(int Width, int Height, const unsigned char* Scan0, int Stride, unsigned char* SkinScan0, int SkinStride, int cn)
{
  int X, Y;

  for (Y = 0; Y < Height; Y++) {
    const unsigned char* Pointer = Scan0 + Y * Stride;
    unsigned char* SkinP = SkinScan0 + Y * SkinStride;

    for (X = 0; X < Width; X++) {
      int Blue = *Pointer;
      int Green = *(Pointer + 1);
      int Red = *(Pointer + 2);
      int Cb = (-176933 * Red - 347355 * Green + 524288 * Blue + 134217728) >> 20;

      if (Cb > 77 && Cb < 127) {
        int Cr = (524288 * Red - 439026 * Green - 85262 * Blue + 134217728) >> 20;

        if (Cr > 133 && Cr < 173) {
          *SkinP = 255;
        }
      }

      Pointer += 3;
      SkinP++;
    }
  }

  return 0;
}