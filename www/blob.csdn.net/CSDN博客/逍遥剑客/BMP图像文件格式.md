# BMP图像文件格式 - 逍遥剑客 - CSDN博客
2007年03月31日 00:15:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：5020标签：[数据结构																[struct																[存储																[byte																[算法																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
个人分类：[图像处理](https://blog.csdn.net/xoyojank/article/category/337625)
对于现存的所有的图像文件格式，我们在这里主要介绍BMP图像文件格式，并且文件里的图像数据是未压缩的，因为图像的数字化处理主要是对图像中的各个像素进行相应的处理，而未压缩的BMP图像中的像素数值正好与实际要处理的数字图像相对应，这种格式的文件最合适我们对之进行数字化处理。请读者记住，压缩过的图像是无法直接进行数字化处理的，如JPEG、GIF等格式的文件，此时首先要对图像文件解压缩，这就要涉及到一些比较复杂的压缩算法。后续章节中我们将针对特殊的文件格式如何转换为BMP格式的文件问题作专门的论述，经过转换，我们就可以利用得到的未压缩的BMP文件格式进行后续处理。对于JPEG、GIF等格式，由于涉及到压缩算法，这要求读者掌握一定的信息论方面的知识，如果展开的话，可以写一本书，限于篇幅原因，我们只作一般性的讲解，有兴趣的朋友可以参考相关书籍资料。
一、BMP文件结构 
1. BMP文件组成 
BMP文件由文件头、位图信息头、颜色信息和图形数据四部分组成。文件头主要包含文件的大小、文件类型、图像数据偏离文件头的长度等信息；位图信息头包含图象的尺寸信息、图像用几个比特数值来表示一个像素、图像是否压缩、图像所用的颜色数等信息。颜色信息包含图像所用到的颜色表，显示图像时需用到这个颜色表来生成调色板，但如果图像为真彩色，既图像的每个像素用24个比特来表示，文件中就没有这一块信息，也就不需要操作调色板。文件中的数据块表示图像的相应的像素值，需要注意的是：图像的像素值在文件中的存放顺序为从左到右，从下到上，也就是说，在BMP文件中首先存放的是图像的最后一行像素，最后才存储图像的第一行像素，但对与同一行的像素，则是按照先左边后右边的的顺序存储的；另外一个需要读者朋友关注的细节是：文件存储图像的每一行像素值时，如果存储该行像素值所占的字节数为4的倍数，则正常存储，否则，需要在后端补0，凑足4的倍数。 
2. BMP文件头 
BMP文件头数据结构含有BMP文件的类型、文件大小和位图起始位置等信息。其结构定义如下: typedef struct tagBITMAPFILEHEADER
{
WORD bfType; // 位图文件的类型，必须为“BM”
DWORD bfSize; // 位图文件的大小，以字节为单位
WORD bfReserved1; // 位图文件保留字，必须为0
WORD bfReserved2; // 位图文件保留字，必须为0
DWORD bfOffBits; // 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位
} BITMAPFILEHEADER；该结构占据14个字节。
3. 位图信息头 
BMP位图信息头数据用于说明位图的尺寸等信息。其结构如下：
typedef struct tagBITMAPINFOHEADER{
DWORD biSize; // 本结构所占用字节数
LONG biWidth; // 位图的宽度，以像素为单位
LONG biHeight; // 位图的高度，以像素为单位
WORD biPlanes; // 目标设备的平面数不清，必须为1
WORD biBitCount// 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
DWORD biCompression; // 位图压缩类型，必须是 0(不压缩),1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
DWORD biSizeImage; // 位图的大小，以字节为单位
LONG biXPelsPerMeter; // 位图水平分辨率，每米像素数
LONG biYPelsPerMeter; // 位图垂直分辨率，每米像素数
DWORD biClrUsed;// 位图实际使用的颜色表中的颜色数
DWORD biClrImportant;// 位图显示过程中重要的颜色数
} BITMAPINFOHEADER；该结构占据40个字节。
注意：对于BMP文件格式，在处理单色图像和真彩色图像的时候，无论图象数据多么庞大，都不对图象数据进行任何压缩处理，一般情况下，如果位图采用压缩格式，那么16色图像采用RLE4压缩算法，256色图像采用RLE8压缩算法。
4. 颜色表 
颜色表用于说明位图中的颜色，它有若干个表项，每一个表项是一个RGBQUAD类型的结构，定义一种颜色。RGBQUAD结构的定义如下: 
typedef struct tagRGBQUAD {
BYTErgbBlue;// 蓝色的亮度(值范围为0-255)
BYTErgbGreen; // 绿色的亮度(值范围为0-255)
BYTErgbRed; // 红色的亮度(值范围为0-255)
BYTErgbReserved;// 保留，必须为0
} RGBQUAD;
颜色表中RGBQUAD结构数据的个数由BITMAPINFOHEADER 中的biBitCount项来确定，当biBitCount=1,4,8时，分别有2,16,256个颜色表项，当biBitCount=24时，图像为真彩色，图像中每个像素的颜色用三个字节表示，分别对应R、G、B值，图像文件没有颜色表项。位图信息头和颜色表组成位图信息，BITMAPINFO结构定义如下:
typedef struct tagBITMAPINFO {
BITMAPINFOHEADER bmiHeader; // 位图信息头
RGBQUAD bmiColors[1]; // 颜色表
} BITMAPINFO;
注意：RGBQUAD数据结构中，增加了一个保留字段rgbReserved，它不代表任何颜色，必须取固定的值为“0”，同时，RGBQUAD结构中定义的颜色值中，红色、绿色和蓝色的排列顺序与一般真彩色图像文件的颜色数据排列顺序恰好相反，既：若某个位图中的一个像素点的颜色的描述为“00，00，ff，00”，则表示该点为红色，而不是蓝色。
5. 位图数据 
位图数据记录了位图的每一个像素值或该对应像素的颜色表的索引值，图像记录顺序是在扫描行内是从左到右,扫描行之间是从下到上。这种格式我们又称为Bottom_Up位图，当然与之相对的还有Up_Down形式的位图，它的记录顺序是从上到下的，对于这种形式的位图，也不存在压缩形式。位图的一个像素值所占的字节数：当biBitCount=1时，8个像素占1个字节；当biBitCount=4时，2个像素占1个字节；当biBitCount=8时，1个像素占1个字节；当biBitCount=24时,1个像素占3个字节，此时图像为真彩色图像。当图像不是为真彩色时，图像文件中包含颜色表，位图的数据表示对应像素点在颜色表中相应的索引值，当为真彩色时，每一个像素用三个字节表示图像相应像素点彩色值，每个字节分别对应R、G、B分量的值，这时候图像文件中没有颜色表。上面我已经讲过了，Windows规定图像文件中一个扫描行所占的字节数必须是4的倍数(即以字为单位),不足的以0填充，图像文件中一个扫描行所占的字节数计算方法：
DataSizePerLine= (biWidth* biBitCount+31)/8；// 一个扫描行所占的字节数
位图数据的大小按下式计算(不压缩情况下)：
DataSize= DataSizePerLine* biHeight。
上述是BMP文件格式的说明，搞清楚了以上的结构，就可以正确的操作图像文件，对它进行读或写操作了。
GIF图像文件格式
GIF图象格式的全称为Graphics Interchange Format，从这个名字可以看出，这种图像格式主要是为了通过网络传输图像而设计的。GIF文件不支持24位真彩色图像，最多只能存储256色的图像或灰度图像；GIF格式文件也无法存储CMY和HIS模型的图像数据；另外，GIF图像文件的各种数据区域一般没有固定的数据长度和存储顺序，所以为了方便程序寻找数据区，将数据区中的第一个字节作为标志符；最后需要读者注意的是GIF文件存储图像数据是有二种排列顺序：顺序排列或交叉排列。交叉排列的方式适合网络传输，这样一来允许用户在不完全掌握图像数据之前，获取当前图像的轮廓数据。
GIF文件格式分为87和89两个版本，对于87这个版本，该文件主要是有五个部分组成，它，们是按顺序出现的：文件头块、逻辑屏幕描述块、可选择的调色板块、图像数据块、最后是标志文件结束的尾块，该块总是取固定的值3BH。其中第一和第二两个块用GIF图像文件头结构描述：
GIFHEADER：{
DB Signature; //该字段占六个字节，为了用于指明图像为GIF格式，前三个字符必须为“GIF”，后三字符用于指定是哪个版本，87或89。
DW ScreenWidth;//
DW ScreenDepth;//占两个字节，以像素为单位表示图像的宽、高
DB GlobalFlagByte;//该字节的各个位用于调色版的描述
DB BackGroundColor;//代表图象的背景颜色的索引
DB AspectRatio;图像的长宽比
}
GIF格式中的调色板有通用调色板和局部调色板之分，因为GIF格式允许一个文件中存储多个图像，因此有这两种调色板，其中通用调色板适于文件中的所有图像，而局部调色板只适用于某一个图像。格式中的数据区域一般分为四个部分，图像数据识别区域，局部调色板数据，采用压缩算法得到的图象数据区域和结束标志区域。
在GIF89版本中，它包含七个部分，分别是文件头、通用调色板数据、图像数据区和四个补充数据区，它们主要是用于提示程序如何处理图像的。
下面分别介绍各种RGB格式。
¨ RGB1、RGB4、RGB8都是调色板类型的RGB格式，在描述这些媒体类型的格式细节时，通常会在BITMAPINFOHEADER数据结构后面跟着一个调色板（定义一系列颜色）。它们的图像数据并不是真正的颜色值，而是当前像素颜色值在调色板中的索引。以RGB1（2色位图）为例，比如它的调色板中定义的两种颜色值依次为0x000000（黑色）和0xFFFFFF（白色），那么图像数据001101010111…（每个像素用1位表示）表示对应各像素的颜色为：黑黑白白黑白黑白黑白白白…。
¨ RGB565使用16位表示一个像素，这16位中的5位用于R，6位用于G，5位用于B。程序中通常使用一个字（WORD，一个字等于两个字节）来操作一个像素。当读出一个像素后，这个字的各个位意义如下：
     高字节              低字节
R R R R R G G G     G G G B B B B B
可以组合使用屏蔽字和移位操作来得到RGB各分量的值：
#define RGB565_MASK_RED    0xF800
#define RGB565_MASK_GREEN  0x07E0
#define RGB565_MASK_BLUE   0x001F
R = (wPixel & RGB565_MASK_RED) >> 11;   // 取值范围0-31
G = (wPixel & RGB565_MASK_GREEN) >> 5;  // 取值范围0-63
B =  wPixel & RGB565_MASK_BLUE;         // 取值范围0-31
¨ RGB555是另一种16位的RGB格式，RGB分量都用5位表示（剩下的1位不用）。使用一个字读出一个像素后，这个字的各个位意义如下：
     高字节             低字节
X R R R R G G       G G G B B B B B       （X表示不用，可以忽略）
可以组合使用屏蔽字和移位操作来得到RGB各分量的值：
#define RGB555_MASK_RED    0x7C00
#define RGB555_MASK_GREEN  0x03E0
#define RGB555_MASK_BLUE   0x001F
R = (wPixel & RGB555_MASK_RED) >> 10;   // 取值范围0-31
G = (wPixel & RGB555_MASK_GREEN) >> 5;  // 取值范围0-31
B =  wPixel & RGB555_MASK_BLUE;         // 取值范围0-31
¨ RGB24使用24位来表示一个像素，RGB分量都用8位表示，取值范围为0-255。注意在内存中RGB各分量的排列顺序为：BGR BGR BGR…。通常可以使用RGBTRIPLE数据结构来操作一个像素，它的定义为：
typedef struct tagRGBTRIPLE { 
  BYTE rgbtBlue;    // 蓝色分量
  BYTE rgbtGreen;   // 绿色分量
  BYTE rgbtRed;     // 红色分量
} RGBTRIPLE;
¨ RGB32使用32位来表示一个像素，RGB分量各用去8位，剩下的8位用作Alpha通道或者不用。（ARGB32就是带Alpha通道的RGB32。）注意在内存中RGB各分量的排列顺序为：BGRA BGRA BGRA…。通常可以使用RGBQUAD数据结构来操作一个像素，它的定义为：
typedef struct tagRGBQUAD {
  BYTE    rgbBlue;      // 蓝色分量
  BYTE    rgbGreen;     // 绿色分量
  BYTE    rgbRed;       // 红色分量
  BYTE    rgbReserved;  // 保留字节（用作Alpha通道或忽略）
} RGBQUAD;
