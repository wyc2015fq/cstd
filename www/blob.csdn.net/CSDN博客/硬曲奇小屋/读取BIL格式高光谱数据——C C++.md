# 读取BIL格式高光谱数据——C/C++ - 硬曲奇小屋 - CSDN博客





置顶2017年10月15日 18:03:24[HardCookies](https://me.csdn.net/james_616)阅读数：1165








在[上一篇博客](http://blog.csdn.net/james_616/article/details/78190368%20%E5%8D%9A%E5%AE%A2)中，我们提到如何读取头文件。这篇博客将要谈到如何进一步读取高光谱数据本身，这里以BIL格式为例。 

什么是BIL呢？BIL的全称为Band Interleave by Line，连续保存的是一个波段的一整行数据。如图1所示的三个波段的数据 

![这里写图片描述](http://cn.mathworks.com/help/matlab/ref/fileio_l_p2.gif)

图1 



波段1的第一行为AAAAA，波段2的第一行为BBBBB，波段3的第一行为CCCCCC，那么整个第一行的数据就是AAAAABBBBBCCCCC。同理，第二行的数据为AAAAABBBBBCCCCC，第三行的数据为AAAAABBBBBCCCCC。再把3行的数据整体排开，得到AAAAABBBBBCCCCCAAAAABBBBBCCCCCAAAAABBBBBCCCCC，这也是写到二进制文件的顺序。 

知道排列顺序是读取数据的关键，我们读取数据主要分为两个部分：一是读取空间位置点的光谱信息，二是读取某一波段的空间信息。为此，分别编写两个函数readSpec和readBand，分别返回一维指针和二维指针。读取的数据仍是上次的**cupeff95.int**，头文件的数据作为已知值给出。 

具体代码如下所示：
```cpp
// read_envi.cpp
// C++ 读取ENVI的BIL 2-byte高光谱数据
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    printf("Hello, welcome to use C++!\n");

    FILE *fp;
    int lines = 350;     // 行数
    int samples = 400;   // 列数
    int bands = 50;      // 波段数

    // 定义函数readSpec
    short *readSpec(short *bil_data, int row, int col);     /* 返回的是指针 */
    short *readBand(short *bil_data, int band_num);         /* 返回二维数组的指针 */

    // 读取文件数据
    fp = fopen("cup95eff.int", "rb");
    int size = lines * samples * bands;
    short *data = new short[size];
    fread(data, sizeof(short), size, fp);

    // 读取(i,j)的光谱数据
    int i = 0, j = 0;
    cout << "请输入读取的行、列：" << endl;
    cin >> i >> j;
    if (i > lines-1 || i < 0 || j > samples-1 || j < 0)
    {
        printf("超出范围\n");
        exit(0);
    }

    short *sij = new short[50];
    sij = readSpec(data, i, j);

    // 输出光谱数据
    printf("第%d行，第%d列的光谱数据：\n", i, j);
    for (int i = 0; i < 50; ++i)
    {
        cout << sij[i] << endl;
    }


    // 读取指定波段
    int k;
    printf("请输入读取的波段号：\n");
    cin >> k;
    if (k > bands-1 || k < 0)
    {
        printf("超出范围\n");
        exit(0);
    }

    short *band_k = new short[lines*samples];
    band_k = readBand(data, k);

    // 输出波段影像
    printf("第%d波段影像：\n", k);
    printf("输出左上角20x20区域：\n");
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            cout << *(band_k + i * 400 + j) << " ";
        }
        cout << endl;
    }

    return 0;
}

// readSpec函数：返回指定位置光谱数据
short *readSpec(short *data, int i, int j)
{
    short *spectrum = new short[50];

    for (int k = 0; k < 50; ++k)
    {
        spectrum[k] = data[50 * 400 * i + j + 400 * k];
    }
    return spectrum;
}

// readBand函数：返回指定波段数据
short *readBand(short *data, int k)
{
    short *image = new short[350 * 400];
    for (int i = 0; i < 350; ++i)
    {
        for (int j = 0; j < 400; ++j)
        {
            image[i * 400 + j] = data[400 * 50 * i + 400 * k + j];
        }
    }
    return image;
}
```

其中的关键就是把整个lines*samples*bands*2-byte大小的指针data进行分割，正如readSpec函数和readBand所做的一样。 

编译、运行以上代码，读取并显示第99行，99列的光谱数据。读取第24波段的空间数据，显示左上角20x20区域的数据。 

运行结果分别如图2、图3所示： 

![这里写图片描述](https://img-blog.csdn.net/20171015204604056?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图2 读取光谱 

![这里写图片描述](https://img-blog.csdn.net/20171015204720475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3 读取波段


我们用Matlab读取同样的数据来验证正确与否，matlab代码如下：

```matlab
filename = 'cup95eff.int';

rows = 350;
cols = 400;
bands = 50;

data = multibandread(filename, [rows cols bands],...
        'int16', 0, 'bil', 'ieee-le');

Y = squeeze(data(:,:,25));
Z = squeeze(data(100,200,:));
```

因为matlab的起始是1，C的起始是0，所以这里读的波段号为25，坐标为100。matlab的结果分别如图4、图5所示： 


![这里写图片描述](https://img-blog.csdn.net/20171015205935551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4 光谱数据




![这里写图片描述](https://img-blog.csdn.net/20171015210026592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图5 空间数据


对比可以得出，我们读取的数据正确。 

读取BIL格式高光谱数据的工作基本完成！






