# 视频与图像RGB/YUV格式详解   - 深之JohnChen的专栏 - CSDN博客

2006年02月05日 11:21:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2066标签：[数据结构																[byte																[struct																[存储																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
个人分类：[图像处理与识别](https://blog.csdn.net/byxdaz/article/category/144582)


 作者：中华视频网 

 计算机彩色显示器显示色彩的原理与彩色电视机一样，都是采用R（Red）、G（Green）、B（Blue）相加混色的原理：通过发射出三种不同强度的电子束，使屏幕内侧覆盖的红、绿、蓝磷光材料发光而产生色彩。这种色彩的表示方法称为RGB色彩空间表示（它也是多媒体计算机技术中用得最多的一种色彩空间表示方法）。

  根据三基色原理，任意一种色光F都可以用不同分量的R、G、B三色相加混合而成。

F = r [ R ] + g [ G ] + b [ B ]

  其中，r、g、b分别为三基色参与混合的系数。当三基色分量都为0（最弱）时混合为黑色光；而当三基色分量都为k（最强）时混合为白色光。调整r、g、b三个系数的值，可以混合出介于黑色光和白色光之间的各种各样的色光。
  那么YUV又从何而来呢？在现代彩色电视系统中，通常采用三管彩色摄像机或彩色CCD摄像机进行摄像，然后把摄得的彩色图像信号经分色、分别放大校正后得到RGB，再经过矩阵变换电路得到亮度信号Y和两个色差信号R－Y（即U）、B－Y（即V），最后发送端将亮度和色差三个信号分别进行编码，用同一信道发送出去。这种色彩的表示方法就是所谓的YUV色彩空间表示。
  采用YUV色彩空间的重要性是它的亮度信号Y和色度信号U、V是分离的。如果只有Y信号分量而没有U、V分量，那么这样表示的图像就是黑白灰度图像。彩色电视采用YUV空间正是为了用亮度信号Y解决彩色电视机与黑白电视机的兼容问题，使黑白电视机也能接收彩色电视信号。

  YUV与RGB相互转换的公式如下（RGB取值范围均为0-255）：

Y = 0.299R + 0.587G + 0.114B
U = -0.147R - 0.289G + 0.436B
V = 0.615R - 0.515G - 0.100B

R = Y + 1.14V
G = Y - 0.39U - 0.58V
B = Y + 2.03U

  在DirectShow中，常见的RGB格式有RGB1、RGB4、RGB8、RGB565、RGB555、RGB24、RGB32、ARGB32等；常见的YUV格式有YUY2、YUYV、YVYU、UYVY、AYUV、Y41P、Y411、Y211、IF09、IYUV、YV12、YVU9、YUV411、YUV420等。作为视频媒体类型的辅助说明类型（Subtype），它们对应的GUID见表2.3。

表2.3 常见的RGB和YUV格式

      GUID                                         格式描述
MEDIASUBTYPE_RGB1    2色，每个像素用1位表示，需要调色板
MEDIASUBTYPE_RGB4    16色，每个像素用4位表示，需要调色板
MEDIASUBTYPE_RGB8    256色，每个像素用8位表示，需要调色板
MEDIASUBTYPE_RGB565    每个像素用16位表示，RGB分量分别使用5位、6位、5位
MEDIASUBTYPE_RGB555    每个像素用16位表示，RGB分量都使用5位（剩下的1位不用）
MEDIASUBTYPE_RGB24    每个像素用24位表示，RGB分量各使用8位
MEDIASUBTYPE_RGB32    每个像素用32位表示，RGB分量各使用8位（剩下的8位不用）
MEDIASUBTYPE_ARGB32    每个像素用32位表示，RGB分量各使用8位（剩下的8位用于表示Alpha通道值）
MEDIASUBTYPE_YUY2    YUY2格式，以4:2:2方式打包
MEDIASUBTYPE_YUYV    YUYV格式（实际格式与YUY2相同）
MEDIASUBTYPE_YVYU    YVYU格式，以4:2:2方式打包
MEDIASUBTYPE_UYVY    UYVY格式，以4:2:2方式打包
MEDIASUBTYPE_AYUV    带Alpha通道的4:4:4 YUV格式
MEDIASUBTYPE_Y41P    Y41P格式，以4:1:1方式打包
MEDIASUBTYPE_Y411    Y411格式（实际格式与Y41P相同）
MEDIASUBTYPE_Y211    Y211格式
MEDIASUBTYPE_IF09    IF09格式
MEDIASUBTYPE_IYUV    IYUV格式
MEDIASUBTYPE_YV12    YV12格式
MEDIASUBTYPE_YVU9    YVU9格式

下面分别介绍各种RGB格式。

¨ RGB1、RGB4、RGB8都是调色板类型的RGB格式，在描述这些媒体类型的格式细节时，通常会在BITMAPINFOHEADER数据结构后面跟着一个调色板（定义一系列颜色）。它们的图像数据并不是真正的颜色值，而是当前像素颜色值在调色板中的索引。以RGB1（2色位图）为例，比如它的调色板中定义的两种颜色值依次为0x000000（黑色）和0xFFFFFF（白色），那么图像数据001101010111…（每个像素用1位表示）表示对应各像素的颜色为：黑黑白白黑白黑白黑白白白…。

¨ RGB565使用16位表示一个像素，这16位中的5位用于R，6位用于G，5位用于B。程序中通常使用一个字（WORD，一个字等于两个字节）来操作一个像素。当读出一个像素后，这个字的各个位意义如下：
     高字节              低字节
R R R R R G G G     G G G B B B B B
可以组合使用屏蔽字和移位操作来得到RGB各分量的值：

#define RGB565_MASK_RED    0xF800
#define RGB565_MASK_GREEN  0x07E0
#define RGB565_MASK_BLUE   0x001F
R = (wPixel & RGB565_MASK_RED) >> 11;   // 取值范围0-31
G = (wPixel & RGB565_MASK_GREEN) >> 5;  // 取值范围0-63
B =  wPixel & RGB565_MASK_BLUE;         // 取值范围0-31

¨ RGB555是另一种16位的RGB格式，RGB分量都用5位表示（剩下的1位不用）。使用一个字读出一个像素后，这个字的各个位意义如下：
     高字节             低字节
X R R R R G G       G G G B B B B B       （X表示不用，可以忽略）
可以组合使用屏蔽字和移位操作来得到RGB各分量的值：

#define RGB555_MASK_RED    0x7C00
#define RGB555_MASK_GREEN  0x03E0
#define RGB555_MASK_BLUE   0x001F
R = (wPixel & RGB555_MASK_RED) >> 10;   // 取值范围0-31
G = (wPixel & RGB555_MASK_GREEN) >> 5;  // 取值范围0-31
B =  wPixel & RGB555_MASK_BLUE;         // 取值范围0-31

¨ RGB24使用24位来表示一个像素，RGB分量都用8位表示，取值范围为0-255。注意在内存中RGB各分量的排列顺序为：BGR BGR BGR…。通常可以使用RGBTRIPLE数据结构来操作一个像素，它的定义为：

typedef struct tagRGBTRIPLE { 
  BYTE rgbtBlue;    // 蓝色分量
  BYTE rgbtGreen;   // 绿色分量
  BYTE rgbtRed;     // 红色分量
} RGBTRIPLE;

¨ RGB32使用32位来表示一个像素，RGB分量各用去8位，剩下的8位用作Alpha通道或者不用。（ARGB32就是带Alpha通道的RGB32。）注意在内存中RGB各分量的排列顺序为：BGRA BGRA BGRA…。通常可以使用RGBQUAD数据结构来操作一个像素，它的定义为：

typedef struct tagRGBQUAD {
  BYTE    rgbBlue;      // 蓝色分量
  BYTE    rgbGreen;     // 绿色分量
  BYTE    rgbRed;       // 红色分量
  BYTE    rgbReserved;  // 保留字节（用作Alpha通道或忽略）
} RGBQUAD;

  下面介绍各种YUV格式。YUV格式通常有两大类：打包（packed）格式和平面（planar）格式。前者将YUV分量存放在同一个数组中，通常是几个相邻的像素组成一个宏像素（macro-pixel）；而后者使用三个数组分开存放YUV三个分量，就像是一个三维平面一样。表2.3中的YUY2到Y211都是打包格式，而IF09到YVU9都是平面格式。（注意：在介绍各种具体格式时，YUV各分量都会带有下标，如Y0、U0、V0表示第一个像素的YUV分量，Y1、U1、V1表示第二个像素的YUV分量，以此类推。）

¨ YUY2（和YUYV）格式为每个像素保留Y分量，而UV分量在水平方向上每两个像素采样一次。一个宏像素为4个字节，实际表示2个像素。（4:2:2的意思为一个宏像素中有4个Y分量、2个U分量和2个V分量。）图像数据中YUV分量排列顺序如下：
Y0 U0 Y1 V0    Y2 U2 Y3 V2 …

¨ YVYU格式跟YUY2类似，只是图像数据中YUV分量的排列顺序有所不同：
Y0 V0 Y1 U0    Y2 V2 Y3 U2 …

¨ UYVY格式跟YUY2类似，只是图像数据中YUV分量的排列顺序有所不同：
U0 Y0 V0 Y1    U2 Y2 V2 Y3 …

¨ AYUV格式带有一个Alpha通道，并且为每个像素都提取YUV分量，图像数据格式如下：
A0 Y0 U0 V0    A1 Y1 U1 V1 …

¨ Y41P（和Y411）格式为每个像素保留Y分量，而UV分量在水平方向上每4个像素采样一次。一个宏像素为12个字节，实际表示8个像素。图像数据中YUV分量排列顺序如下：
U0 Y0 V0 Y1    U4 Y2 V4 Y3    Y4 Y5 Y6 Y8 … 

¨ Y211格式在水平方向上Y分量每2个像素采样一次，而UV分量每4个像素采样一次。一个宏像素为4个字节，实际表示4个像素。图像数据中YUV分量排列顺序如下：
Y0 U0 Y2 V0    Y4 U4 Y6 V4 …

¨ YVU9格式为每个像素都提取Y分量，而在UV分量的提取时，首先将图像分成若干个4 x 4的宏块，然后每个宏块提取一个U分量和一个V分量。图像数据存储时，首先是整幅图像的Y分量数组，然后就跟着U分量数组，以及V分量数组。IF09格式与YVU9类似。

¨ IYUV格式为每个像素都提取Y分量，而在UV分量的提取时，首先将图像分成若干个2 x 2的宏块，然后每个宏块提取一个U分量和一个V分量。YV12格式与IYUV类似。

¨ YUV411、YUV420格式多见于DV数据中，前者用于NTSC制，后者用于PAL制。YUV411为每个像素都提取Y分量，而UV分量在水平方向上每4个像素采样一次。YUV420并非V分量采样为0，而是跟YUV411相比，在水平方向上提高一倍色差采样频率，在垂直方向上以U/V间隔的方式减小一半色差采样，如下图所示。

![Image](http://www.chinavideo.org/images/stories/news/200601/20060122_04.jpg)


