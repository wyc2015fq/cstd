# 图像处理之快速均值模糊(Box Blur) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年01月14日 19:25:51[gloomyfish](https://me.csdn.net/jia20003)阅读数：17882
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









**图像处理之快速均值模糊(Box Blur)**

****

图像模糊的本质, 从数字信号处理的角度看，图像模糊就要压制高频信号保留低频信号，

压制高频的信号的一个可选择的方法就是卷积滤波。选择一个低频滤波器，对图像上的

每个像素实现低频滤波，这样整体效果就是一张数字图像更加的模糊，显示更少的细节信息。

 传统的卷积模糊计算量巨大，程序效率比较低，基于滑动窗口的Box Blur是一种快速模糊方法，

其结果近似于卷积模糊的结果。我没证明过！



**一：Box Blur数学原理**

根据输入的半径R，计算起始2*R +1个像素的平均值, 作为第一个输出像素的结果, 

公式可以表示为

像素 X0 =  其中K代表输入像素集合, i的取值范围为 i∈[-R, R]

然后计算每一行输出像素的值根据Xi = X0 + (K[index + R + 1] – K[index - R])



**二：Box Blur的特征**

Box Blur是一种快速的图像模糊技术, 相比于传统的卷积模糊,Box Blur可以更有效率的

完成对图像模糊, 模糊的程度取决一下三个输入参数,

1.      X方向上半径 H Radius

2.      Y方向上半径 V Radius

3.      迭代次数 Iteration number

在半径相同的情况下, 迭代次数越多,输出的图像就越模糊

在迭代次数相同的情况下, 像素半径越大, 输出的图像就越模糊

上述两者之间的不同是对图像的拉伸效果, 半径越大，对图像的拉伸效果越显著



Box模糊利用滑动窗口算法，从而简化了每次计算平均值带来额外开销。



从数字图像和信号处理的角度看, Box Blur是一种不折不扣的低通滤波, 但是它并不

是真正的高斯低通滤波, 不是卷积实现, 因而速度更快。

 当水平和垂直半径分别为1 时，是典型的3*3 的矩阵卷积

1, 1, 1

1, 1, 1

1, 1, 1

计算, 相比于传统的卷积计算之后，要进行归一化处理，box计算过程中已经完成像素平均，

无需归一化处理。

![](http://hi.csdn.net/attachment/201201/14/0_13265402518AA3.gif)


**三：基于滑动窗口算法的Box模糊效果**



水平和垂直方向

![](http://hi.csdn.net/attachment/201201/14/0_1326540304aX5O.gif)

垂直方向：

![](http://hi.csdn.net/attachment/201201/14/0_1326540317dyZ9.gif)



水平方向：

![](http://hi.csdn.net/attachment/201201/14/0_1326540347Oy1t.gif)

四：程序关键代码解析

注释已经很详细的写在代码中，最重要的一个步骤是提前建立index，根据index来找到平均值。

****

```java
public static void blur( int[] in, int[] out, int width, int height, int radius ) {
        int widthMinus1 = width-1;
        int tableSize = 2*radius+1;
        int divide[] = new int[256*tableSize];

        // the value scope will be 0 to 255, and number of 0 is table size
        // will get means from index not calculate result again since 
        // color value must be  between 0 and 255.
        for ( int i = 0; i < 256*tableSize; i++ )
            divide[i] = i/tableSize; 

        int inIndex = 0;
        
        // 
        for ( int y = 0; y < height; y++ ) {
            int outIndex = y;
            int ta = 0, tr = 0, tg = 0, tb = 0; // ARGB -> prepare for the alpha, red, green, blue color value.

            for ( int i = -radius; i <= radius; i++ ) {
                int rgb = in[inIndex + ImageMath.clamp(i, 0, width-1)]; // read input pixel data here. table size data.
                ta += (rgb >> 24) & 0xff;
                tr += (rgb >> 16) & 0xff;
                tg += (rgb >> 8) & 0xff;
                tb += rgb & 0xff;
            }

            for ( int x = 0; x < width; x++ ) { // get output pixel data.
                out[ outIndex ] = (divide[ta] << 24) | (divide[tr] << 16) | (divide[tg] << 8) | divide[tb]; // calculate the output data.

                int i1 = x+radius+1;
                if ( i1 > widthMinus1 )
                    i1 = widthMinus1;
                int i2 = x-radius;
                if ( i2 < 0 )
                    i2 = 0;
                int rgb1 = in[inIndex+i1];
                int rgb2 = in[inIndex+i2];
                
                ta += ((rgb1 >> 24) & 0xff)-((rgb2 >> 24) & 0xff);
                tr += ((rgb1 & 0xff0000)-(rgb2 & 0xff0000)) >> 16;
                tg += ((rgb1 & 0xff00)-(rgb2 & 0xff00)) >> 8;
                tb += (rgb1 & 0xff)-(rgb2 & 0xff);
                outIndex += height; // per column or per row as cycle...
            }
            inIndex += width; // next (i+ column number * n, n=1....n-1)
        }
    }
```





