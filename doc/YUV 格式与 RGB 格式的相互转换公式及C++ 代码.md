# YUV 格式与 RGB 格式的相互转换公式及C++ 代码

2017年04月02日 13:00:43 [liyuanbhu](https://me.csdn.net/liyuanbhu) 阅读数：17897



# YUV 格式与 RGB 格式的相互转换公式

最近在用的一个工业相机，输出的图像格式是 YUY2 格式。而在电脑上显示时需要 RGB 格式，所以就花了些时间在网上查了些相关的资料。说实话，网上关于 YUV 与 RGB 格式变换的文章挺多的，本来不需要我再多写这么一篇。但是网上那些文章中给出了各种各样的变换公式，公式的系数又各不相同，让人看了之后越看越糊涂。其实那些文章的公式基本都是对的，只不过因为作者忘记给出变换公式的定义域与值域，所以给读者使用造成了很大的麻烦。

为此我就写了这篇文章，来梳理一下各种网文中的各种公式。

在 Keith Jack’s 的书 “Video Demystified” (ISBN 1-878707-09-4) 给出的公式是这样的。

RGB to YUV 
$$
\begin{eqnarray}
Y  &=&      0.257  R + 0.504  G + 0.098  B + 16\\
C_r = V &=&  0.439  R - 0.368  G - 0.071  B + 128\\
C_b = U &=& -0.148  R - 0.291  G + 0.439  B + 128
\end{eqnarray}
$$


YUV to RGB 
$$
\begin{eqnarray}
B &=& 1.164(Y - 16)                   + 2.018(U - 128)\\
G &=& 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)\\
R &=& 1.164(Y - 16) + 1.596(V - 128)
\end{eqnarray}
$$


注意在上面的式子中，RGB 的范围是 $[0,255]$，Y 的范围是$ [16,235]$ ，UV 的范围是$ [16,239]$。 如果计算结果超出这个范围就截断处理。

CCIR 601 定义的转换公式是：

$$
\begin{eqnarray}
Y &=& 0.299R+0.587G+0.114B \\
C_r = V &=& 0.713(R - Y) = 0.500R-0.419G-0.081B \\
C_b = U &=& 0.564(B - Y) = -0.169R-0.331G+0.500B \\
\end{eqnarray}
$$

$$
\begin{eqnarray}
R &=& Y + 1.403V\\
G &=& Y - 0.344U - 0.714V\\
B &=& Y + 1.770U
\end{eqnarray}
$$

这里 RGB 的取值范围是 $[0,1]$。 Y 的范围是 $[0,1]$， Cr 和 Cb 的范围是 $[−0.5,0.5]$。

大家仔细看，这两个来源给出的公式系数有些细微的差别，如果将公式中的 YUV 和 RGB 的取值范围统一成相同的，计算出的结果也略有不同，但是差异很小，基本上眼睛看不出区别来。所以大家不用计较这两个公式的区别。

如果把 RGB 和YUV 的范围都放缩到 $[0,255]$，那么常用的转换公式是这样的。 

$$
\begin{eqnarray}
R &=& Y + 1.403 \times (V - 128)\\
G &=& Y – 0.343 \times(U – 128) – 0.714 \times(V – 128)\\
B &=& Y + 1.770 \times(U – 128)
\end{eqnarray}
$$


RGB 到 YUV 的转换公式变化很小，只是VU 的值做了个平移。 

$$
\begin{eqnarray}
Y &=& 0.299R+0.587G+0.114B \\
C_r = V &=& 0.500R-0.419G-0.081B + 128 \\
C_b = U &=& -0.169R-0.331G+0.500B  + 128\\
\end{eqnarray}
$$


上面的公式涉及到浮点运算，可以转换成整数运算来加速：

```c
u = U - 128; 
v = V - 128;
R = qRound(0, Y + v + (v * 103) >> 8, 255);
G = qRound(0, Y – (u * 88) >> 8 – (v * 183) >> 8, 255);
B = qRound(0, Y + u + (u * 198) >> 8, 255);12345
```

这里的 qRound 是 Qt 里的函数，作用是将值域限制到 [0,255][0,255]。

上面的公式用 C++ 来实现可以写成这样：

```c
/**
 * @brief yuv2rgb
 * @param y [0, 255]
 * @param u [0, 255]
 * @param v [0, 255]
 * @return #ABGR
 */
inline QRgb yuv2rgb(uint8_t y, uint8_t u, uint8_t v)
{
    int R = qRound(y + 1.403 * (v - 128)); // 四舍五入到最近的整数
    int G = qRound( - 0.343 * (u - 128) - 0.714 * (v - 128));
    int B = qRound(y + 1.770 * (u - 128));

    R = qBound(0, R, 255);
    G = qBound(0, y + G, 255);
    B = qBound(0, B, 255);

    return qRgb(R, G, B);
}

/**
 * @brief rgb2yuv
 * @param rgb [0, 255]
 * @param y [0, 255]
 * @param u [0, 255]
 * @param v [0, 255]
 */
inline void rgb2yuv(QRgb rgb, uint8_t &y, uint8_t &u, uint8_t &v)
{
    int R = qRed(rgb);
    int B = qBlue(rgb);
    int G = qGreen(rgb);

    int Y, U, V;
    Y = 0.299 * R + 0.587 * G + 0.114 * B;
    U = -0.169 * R - 0.331 * G + 0.500 * B + 128;
    V = 0.500 * R - 0.419 * G - 0.081 * B + 128;

    y = qBound(0, Y, 255);
    u = qBound(0, U, 255);
    v = qBound(0, V, 255);
}

inline QRgb yuv2rgb_fast(uint8_t y, uint8_t u, uint8_t v)
{
    u = u - 128;
    v = v - 128;

    int r_dif = v + ((v * 103) >> 8);
    int g_dif = -((u * 88) >> 8) - ((v * 183) >> 8);
    int b_dif = u +((u * 198) >> 8);

    int R = y + r_dif;
    int G = y + g_dif;
    int B = y + b_dif;

    R = qBound(0, R, 255);
    G = qBound(0, G, 255);
    B = qBound(0, B, 255);

    return qRgb(R, G, B);
}
```

用移位运算代替浮点数乘法确实可以算的快一些。但是要求更高速度时还是略显的有点慢。这时可以用查表法。

$$
\begin{eqnarray}
R &=& Y + 1.403 \times (V - 128)\\
G &=& Y – 0.343 \times(U – 128) – 0.714 \times(V – 128)\\
B &=& Y + 1.770 \times(U – 128)
\end{eqnarray}
$$
从上式可以看出，要求出 R ，只需要 Y 和 V 的信息。这个计算结果可以事先算好存储在一个 256∗256 的表格中。B 也类似，只需要 Y 和 U 的信息，也可以存储在一个 256∗256 的表格中。

G 的计算稍微复杂点，–0.343×(U–128)–0.714×(V–128) 的计算结果可以存在一个 256∗256 的表格中。这个表格中存放的数从 -135 到 134， 共有 270 个。因此，可以再用一个 256∗270 的表格来存储 YY 和这个值的和。因此，G 的计算需要两层查表。即使这样，也比移位运算快得多。

这一部分的计算可以用个 C++ 的类来封装。

```cpp
class YUV2RGBConverter
{
public:
    YUV2RGBConverter(); 
    QRgb yuv2rgb(uint8_t y, uint8_t u, uint8_t v);
    void yuy2(const uint8_t * yuy2, QRgb * rgb, size_t size);
    void vyuy(const uint8_t * yuy2, QRgb * rgb, size_t size);
    void yuv422(const uint8_t * yuv_y,
                const uint8_t * yuv_u,
                const uint8_t * yuv_v,
                QRgb * rgb,
                size_t size);
private:
    static uint8_t RYV[256][256];
    static uint8_t BYU[256][256];
    static uint16_t TUV[256][256];
    static uint8_t GYT[256][270];
    static bool m_table_init;
};

inline QRgb YUV2RGBConverter::yuv2rgb(uint8_t y, uint8_t u, uint8_t v)
{
    int R = RYV[y][v];
    int G = GYT[y][TUV[u][v]];
    int B = BYU[y][u];
    return qRgb(R, G, B);
}

bool YUV2RGBConverter::m_table_init = false;
uint8_t YUV2RGBConverter::RYV[256][256];
uint8_t YUV2RGBConverter::BYU[256][256];
uint16_t YUV2RGBConverter::TUV[256][256];
uint8_t YUV2RGBConverter::GYT[256][270];


YUV2RGBConverter::YUV2RGBConverter()
{
    int R, G, B, T;
    if(!m_table_init)
    {
        for(int y = 0; y < 256; y++)
        {
            for(int v = 0; v < 256; v++)
            {
                int u = v;
                R = qRound(y + 1.403 * (v - 128));
                B = qRound(y + 1.770 * (u - 128));
                RYV[y][v] = qBound(0, R, 255);
                BYU[y][u] = qBound(0, B, 255);
            }
        }
        for(int u = 0; u < 256; u++)
        {
            for(int v = 0; v < 256; v++)
            {
                T = qRound(-0.343 * (u - 128) - 0.714 * (v - 128));
                // T 的范围 [-134, 135]
                TUV[u][v] = qBound(0, T + 134, 269);
            }
        }
        for(int y = 0; y < 256; y++)
        {
            for(int t = 0; t < 270; t++)
            {
                G = y + (t - 134);
                GYT[y][t] = qBound(0, G, 255);
            }
        }

        m_table_init = true;
    }
}


void YUV2RGBConverter::yuy2(const uint8_t *yuy2, QRgb * rgb, size_t size)
{
    size_t i = 0;
    do
    {
        uint8_t y0 = *yuy2++;
        uint8_t u = *yuy2++;
        uint8_t y1 = *yuy2++;
        uint8_t v = *yuy2++;

        *rgb++ = yuv2rgb(y0, u, v);
        *rgb++ = yuv2rgb(y1, u, v);
        i += 2;
    }while(i < size);
}

void YUV2RGBConverter::vyuy(const uint8_t * yuy2, QRgb * rgb, size_t size)
{
    size_t i = 0;
    do
    {
        uint8_t v = *yuy2++;
        uint8_t y0 = *yuy2++;
        uint8_t u = *yuy2++;
        uint8_t y1 = *yuy2++;

        *rgb++ = yuv2rgb(y0, u, v);
        *rgb++ = yuv2rgb(y1, u, v);
        i += 2;
    }while(i < size);
}

void YUV2RGBConverter::yuv422(const uint8_t * yuv_y,
            const uint8_t * yuv_u,
            const uint8_t * yuv_v,
            QRgb * rgb,
            size_t size)
{
    size_t i = 0;
    do
    {
        uint8_t y0 = *yuv_y++;
        uint8_t y1 = *yuv_y++;
        uint8_t u = *yuv_u++;
        uint8_t v = *yuv_v++;

        *rgb++ = yuv2rgb(y0, u, v);
        *rgb++ = yuv2rgb(y1, u, v);
        i += 2;
    }while(i < size);
}
```

上面的代码中又出现了几种新名词：yuy2、vyuv 和 yuv422，这个是YUV 数据存放的几种常见格式。

YUY2是packed方式的。水平方向两个像素打包到一个DWORD，并且UV采样率只有Y的一半，这符合人的视觉特征能有效的压缩数据,具体布局为[Y0,U0,Y1,V0,Y2,U1,Y3,V1,Y4,U2,Y5,V2,…]。

vyuv 与 YUY2 很类似，只是排列顺序颠倒了：V,Y,U,Y,V,Y,U,Y,V,Y,U,Y,…..

yuv422 格式是把 YUV 这三个通道分开存储到三个数组。UV采样率还是只有Y的一半。