# BMP位图格式详解 - 战斗蜗牛的专栏 - CSDN博客





2012年01月16日 11:28:38[vbskj](https://me.csdn.net/vbskj)阅读数：795标签：[struct																[数据结构																[bi																[windows																[vb																[图形](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
个人分类：[C/C++/Visual C++](https://blog.csdn.net/vbskj/article/category/630758)





查了半天，才发现位图有一个行的字节数必须是4的整数倍的死规定，nnd 




列举图片:C:/WINDOWS/Blue Lace 16.bmp

位图文件头的格式：

typedef struct{

int bfType;//bfType(2字节)，这里恒定等于&H4D42,ASCII字符'BM'

long bfSize;//文件大小，以4字节为单位

int bfReserve1;//备用

int bfReserve2;//备用

long bfoffBits;//数据区在文件中的位置偏移量

}BITMAPFILEHEADER;//文件头结构体，14字节

typedef struct{

long bitSize;//位图信息头大小

long biWidth;//图象宽度，像素单位

long biHeight;//图象高度，像素单位

int biPlanes;//位平面树=1

int biBitCount;//单位像素的位数，表示bmp图片的颜色位数，即24位图、32位图

long biCompression;//图片的压缩属性，bmp图片是不压缩的，等于0

long biSizeImage;表示bmp图片数据区的大小，当上一个属性biCompression等于0时，这里的值可以省略不填

long biXPlosPerMeter;//水平分辨率，可省略

long biYPlosPerMeter;//垂直分辨率，可省略

long biClrUsed;//表示使用了多少个颜色索引表，一般biBitCount属性小于16才会用到，等于0时表示有2^biBitCount个颜色索引表

long biClrImportant;//表示有多少个重要的颜色，等于0时表示所有颜色都很重要

}BITMAPINFOHEADER;//位图信息头，40字节




BMP文件详解（2）2007-03-21 12:26一个bmp文件以BITMAPFILEHEADER结构体开始，BITMAPFILEHEADER的第1个属性是bfType(2字节)，这里 恒定等于&H4D42。由于内存中的数据排列高位在左，低位在右，所以内存中从左往右看就显示成(42 4D)，所以在UltraEdit中头两个 字节显示为(42 4D)就是这样形成的，以后的数据都是这个特点，不再作重复说明。

BITMAPFILEHEADER的第2个属性是bfSize(4字节)，表示整个bmp文件的大小，这里等于&H000004F8=1272字节。

BITMAPFILEHEADER的第3个、第4个属性分别是bfReserved1、bfReserved2(各2字节)，这里是2个保留属性，都为0，这里等于&H0000、&H0000。

BITMAPFILEHEADER的第5个属性是bfOffBits(4字节)，表示DIB数据区在bmp文件中的位置偏移量，这里等于&H00000076=118，表示数据区从文件开始往后数的118字节开始。 

BITMAPFILEHEADER结构体这里就讲完了，大家会发现BITMAPFILEHEADER只占了bmp文件开始的14字节长度，但需要 特别说明的是在vb中定义一个BITMAPFILEHEADER结构体变量，其长度占了16个字节，原因就是第1个属性本来应该只分配2个字节，但实际被 分配了4个字节，多出来2个字节，所以如果想保存一张bmp图片，写入BITMAPFILEHEADER结构体时一定要注意这一点。

接下来是BITMAPINFO结构体部分。BITMAPINFO段由两部分组成：BITMAPINFOHEADER结构体和RGBQUAD结构 体。其中RGBQUAD结构体表示图片的颜色信息，有些时候可以省略，一般的24位图片和32位图片都不带RGBQUAD结构体，因为DIB数据区直接表 示的RGB值，一般4位图片和8位图片才带有RGBQUAD结构体。(多少位的图片就是用多少位来表示一个颜色信息，例如4位图片表示用4个bit来表示 一个颜色信息。)一个bmp文件中有没有RGBQUAD结构体，可以根据前面BITMAPFILEHEADER结构体的第5个属性bfOffBits来判
 断，因为BITMAPINFOHEADER结构体长度为40bit，如果BITMAPINFOHEADER结构体结束后还未到DIB数据区的偏移量，就说 明接下来的数据是RGBQUAD结构体部分。这里讲的C:/WINDOWS/Blue Lace 16.bmp是一个4bit图片，所以它带有 RGBQUAD结构体。

下面进入正题BITMAPINFOHEADER部分。

BITMAPINFOHEADER的第1个属性是biSize(4字节)，表示BITMAPINFOHEADER结构体的长度，最常见的长度是40字节，UltraEdit中可以看到紧接着的4个字节等于&H00000028=40字节。

BITMAPINFOHEADER的第2个属性是biWidth(4字节)，表示bmp图片的宽度，这里等于&H00000030=48像素。

BITMAPINFOHEADER的第3个属性是biHeight(4字节)，表示bmp图片的高度，这里等于&H00000030=48像素。

BITMAPINFOHEADER的第4个属性是biPlanes(2字节)，表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1，这里等于&H0001。

BITMAPINFOHEADER的第5个属性是biBitCount(2字节)，表示bmp图片的颜色位数，即24位图、32位图等等。这里等于&H0004，表示该图片为4位图。

BITMAPINFOHEADER的第6个属性是biCompression(4字节)，表示图片的压缩属性，bmp图片是不压缩的，等于0，所以这里为&H00000000。

BITMAPINFOHEADER的第7个属性是biSizeImage(4字节)，表示bmp图片数据区的大小，当上一个熟悉biCompression等于0时，这里的值可以省略不填，所以这里等于&H00000000。

BITMAPINFOHEADER的第8个属性是biXPelsPerMeter(4字节)，表示图片X轴每米多少像素，可省略，这里等于&H00000EC3=3779像素/米。

BITMAPINFOHEADER的第9个属性是biYPelsPerMeter(4字节)，表示图片Y轴每米多少像素，可省略，这里等于&H00000EC3=3779像素/米。

BITMAPINFOHEADER的第10个属性是biClrUsed(4字节)，表示使用了多少个颜色索引表，一般biBitCount属性小于16才会用到，等于0时表示有2^biBitCount个颜色索引表，所以这里仍等于&H00000000。

BITMAPINFOHEADER的第11个属性是biClrImportant(4字节)，表示有多少个重要的颜色，等于0时表示所有颜色都很重要，所以这里等于&H00000000。

至此BITMAPINFOHEADER结构体结束。

由于这个图片到这里还未到达DIB数据区的偏移量，所以接下来的部分是RGBQUAD结构体。RGBQUAD结构体由4个字节型数据组成，所以一 个RGBQUAD结构体只占用4字节空间，从左到右每个字节依次表示(蓝色，绿色，红色，未使用)。举例的这个图片我数了数总共有16个RGBQUAD结 构体，由于该图片是4位图，2^4正好等于16，所以它把16种颜色全部都枚举出来了，这些颜色就是一个颜色索引表。颜色索引表编号从0开始，总共16个 颜色，所以编号为0-15。从UltraEdit中可以看到按照顺序，这16个RGBQUAD结构体依次为：

编号：(蓝，绿，红，空)

0号：(00，00，00，00)

1号：(00，00，80，00)

2号：(00，80，00，00)

3号：(00，80，80，00)

4号：(80，00，00，00)

5号：(80，00，80，00)

6号：(80，80，00，00)

7号：(80，80，80，00)

8号：(C0，C0，C0，00)

9号：(00，00，FF，00)

10号：(00，FF，00，00)

11号：(00，FF，FF，00)

12号：(FF，00，00，00)

13号：(FF，00，FF，00)

14号：(FF，FF，00，00)

15号：(FF，FF，FF，00)

为了更直观的表示这些颜色，可以见后面的图片。

到这里，正好满足DIB数据区的偏移量，所以后面的字节就是图片内容了。这里需要提醒的是所有的DIB数据扫描行是上下颠倒的，也就是说一幅图片先绘制底部的像素，再绘制顶部的像素，所以这些DIB数据所表示的像素点就是从图片的左下角开始，一直表示到图片的右上角。

由于这里的图片是4位图片，也就是说4bit就表示一个像素，一个字节有8个bit，所以一个字节能表示2个像素。

从UltraEdit中可以看到，DIB数据区第一个字节是&H44，16进制正好是将2进制数每4个一组书写的，跟4bit图片正好吻 合，所以&H44表示两个像素，高位的4表示第一个像素，低位的4表示第二个像素。这里的4不是表示RGB颜色，而是表示颜色索引号为4，由于索 引号从0开始编号的，所以4表示索引表中第5个颜色，从附图中可以看到索引号为4的是蓝色。这是第一字节，表示的是图片左下角开始2个像素，如果有 PhotoShop打开这个图片可以看到，左下角2个像素取出来的颜色RGB值正好等于索引表中第5个颜色的RGB值。后面的DIB数据以此类推。

至此一个bmp图片就全部解析完了，根据这些信息就可以完整的绘制一张bmp图片来。


============================================

如果你还不明白,还有:

1. BMP文件组成 

BMP文件由文件头、位图信息头、颜色信息和图形数据四部分组成。 

2. BMP文件头 

BMP文件头数据结构含有BMP文件的类型、文件大小和位图起始位置等信息。 


其结构定义如下: 


typedef struct tagBITMAPFILEHEADER

{

WORDbfType; // 位图文件的类型，必须为BM

DWORD bfSize; // 位图文件的大小，以字节为单位

WORDbfReserved1; // 位图文件保留字，必须为0

WORDbfReserved2; // 位图文件保留字，必须为0

DWORD bfOffBits; // 位图数据的起始位置，以相对于位图

// 文件头的偏移量表示，以字节为单位

} BITMAPFILEHEADER;


3. 位图信息头 


BMP位图信息头数据用于说明位图的尺寸等信息。

typedef struct tagBITMAPINFOHEADER{

DWORD biSize; // 本结构所占用字节数

LONGbiWidth; // 位图的宽度，以像素为单位

LONGbiHeight; // 位图的高度，以像素为单位

WORD biPlanes; // 目标设备的级别，必须为1

WORD biBitCount// 每个像素所需的位数，必须是1(双色),

// 4(16色)，8(256色)或24(真彩色)之一

DWORD biCompression; // 位图压缩类型，必须是 0(不压缩),

// 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一

DWORD biSizeImage; // 位图的大小，以字节为单位

LONGbiXPelsPerMeter; // 位图水平分辨率，每米像素数

LONGbiYPelsPerMeter; // 位图垂直分辨率，每米像素数

DWORD biClrUsed;// 位图实际使用的颜色表中的颜色数

DWORD biClrImportant;// 位图显示过程中重要的颜色数

} BITMAPINFOHEADER;


4. 颜色表 

颜色表用于说明位图中的颜色，它有若干个表项，每一个表项是一个RGBQUAD类型的结构，定义一种颜色。RGBQUAD结构的定义如下: 


typedef struct tagRGBQUAD {

BYTErgbBlue;// 蓝色的亮度(值范围为0-255)

BYTErgbGreen; // 绿色的亮度(值范围为0-255)

BYTErgbRed; // 红色的亮度(值范围为0-255)

BYTErgbReserved;// 保留，必须为0

} RGBQUAD;

颜色表中RGBQUAD结构数据的个数有biBitCount来确定:

当biBitCount=1,4,8时，分别有2,16,256个表项;

当biBitCount=24时，没有颜色表项。

位图信息头和颜色表组成位图信息，BITMAPINFO结构定义如下:

typedef struct tagBITMAPINFO {

BITMAPINFOHEADER bmiHeader; // 位图信息头

RGBQUAD bmiColors[1]; // 颜色表

} BITMAPINFO;



5. 位图数据 

位图数据记录了位图的每一个像素值，记录顺序是在扫描行内是从左到右,扫描行之间是从下到上。位图的一个像素值所占的字节数: 


当biBitCount=1时，8个像素占1个字节;

当biBitCount=4时，2个像素占1个字节;

当biBitCount=8时，1个像素占1个字节;

当biBitCount=24时,1个像素占3个字节;

Windows规定一个扫描行所占的字节数必须是

4的倍数(即以long为单位),不足的以0填充，

一个扫描行所占的字节数计算方法:

DataSizePerLine= (biWidth* biBitCount+31)/8; 

// 一个扫描行所占的字节数

DataSizePerLine= DataSizePerLine/4*4; // 字节数必须是4的倍数

位图数据的大小(不压缩情况下):

DataSize= DataSizePerLine* biHeight](https://so.csdn.net/so/search/s.do?q=vb&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=bi&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)




