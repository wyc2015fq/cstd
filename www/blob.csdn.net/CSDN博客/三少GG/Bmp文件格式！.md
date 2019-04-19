# Bmp文件格式！ - 三少GG - CSDN博客
2010年05月10日 03:29:00[三少GG](https://me.csdn.net/scut1135)阅读数：1393
1.BMP文件格式
[http://blog.sina.com.cn/s/blog_4a3c050a0100bex4.html](http://blog.sina.com.cn/s/blog_4a3c050a0100bex4.html)
**1.2 BMP文件结构及其存取**
数字图像在外存储器设备中的存储形式是图像文件，图像必须按照某个已知的、公认的数据存储顺序和结构进行存储，才能使不同的程序对图像文件顺利进行打开或存盘操作，实现数据共享。图像数据在文件中的存储顺序和结构称为图像文件格式。目前广为流传的图像文件格式有许多种，常见的格式包括BMP、GIF、JPEG、TIFF、PSD、DICOM、MPEG等。在各种图像文件格式中，一部分是由某个软硬件厂商提出并被广泛接受和采用的格式，例如BMP、GIF和PSD格式；另一部分是由各种国际标准组织提出的格式，例如JPEG、TIFF和DICOM，其中JPEG是国际静止图像压缩标准组织提出的格式，TIFF是由部分厂商组织提出的格式，DICOM是医学图像国际标准组织提出的医学图像专用格式。
BMP文件是Windows操作系统所推荐和支持的图像文件格式，是一种将内存或显示器的图像数据不经过压缩而直接按位存盘的文件格式，所以称为位图（bitmap）文件，因其文件扩展名为BMP，故称为BMP文件格式，简称BMP文件。本书对图像的算法编程都是针对BMP图像文件的，因此在本章中我们详细介绍BMP文件结构及其读写操作，以加深对图像数据的理解。
**1.2.1 BMP文件结构**
如图1-7所示，BMP图像文件被分成4个部分：位图文件头（Bitmap File Header）、位图信息头（Bitmap Info Header）、颜色表（Color Map）和位图数据（即图像数据，Data Bits或Data Body）。
第1部分为位图文件头BITMAPFILEHEADER，是一个结构体类型，该结构的长度是固定的，为14个字节。其定义如下：
typedef struct tagBITMAPFILEHEADER
{
WORD bfType;  
DWORD bfSize；  
WORD bfReserved1;  
WORD bfReserved2;
DWORD bfOffBits;
} BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER,
*PBITMAPFILEHEADER;
BITMAPFILEHEADER结构的各个域详细说明如下：
bfType：位图文件类型，必须是0x424D，即字符串“BM”，也就是说，所有的“*.bmp”文件的头两个字节都是“BM”。
bfSize：位图文件大小，包括这14个字节。
bfReserved1, bfReserved2：Windows保留字，暂不用。
bfOffBits：从文件头到实际的位图数据的偏移字节数，图1-7中前3个部分的长度之和。
图1-7 BMP文件结构示意图
第2部分为位图信息头BITMAPINFOHEADER，也是一个结构体类型的数据结构，该结构的长度也是固定的，为40个字节（WORD为无符号16位整数，DWORD为无符号32位整数，LONG为32位整数）。其定义如下：
typedef struct tagBITMAPINFOHEADER
{
DWORD biSize;    
LONG biWidth;    
LONG biHeight;    
WORD biPlanes;    
WORD biBitCount   
DWORD biCompression;  
DWORD biSizeImage;   
LONG biXPelsPerMeter;  
LONG biYPelsPerMeter;  
DWORD biClrUsed;  
DWORD biClrImportant; 
} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER,
*PBITMAPINFOHEADER;
BITMAPINFOHEADER结构的各个域的详细说明如下：
biSize：本结构的长度，为40个字节。
biWidth：位图的宽度，以像素为单位。
biHeight：位图的高度，以像素为单位。
biPlanes：目标设备的级别，必须是1。
biBitCount：每个像素所占的位数（bit），其值必须为1（黑白图像）、4（16色图）、8（256色）、24（真彩色图），新的BMP格式支持32位色。
biCompresssion：位图压缩类型，有效的值为BI_RGB（未经压缩）、BI_RLE8、BI_RLE4、BI_BITFILEDS（均为Windows定义常量）。这里只讨论未经压缩的情况，即biCompression=BI_RGB。
biSizeImage：实际的位图数据占用的字节数，该值的大小在第4部分位图数据中有具体解释。
biXPelsPerMeter：指定目标设备的水平分辨率，单位是像素/米。
biYPelsPerMeter：指定目标设备的垂直分辨率，单位是像素/米。
biClrUsed：位图实际用到的颜色数，如果该值为零，则用到的颜色数为2的biBitCount次幂。
biClrImportant：位图显示过程中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。
第3部分为颜色表。颜色表实际上是一个RGBQUAD结构的数组，数组的长度由biClrUsed指定（如果该值为零，则由biBitCount指定，即2的biBitCount次幂个元素）。RGBQUAD结构是一个结构体类型，占4个字节，其定义如下：
typedef struct tagRGBQUAD
{
BYTE rgbBlue;
BYTE rgbGreen;
BYTE rgbRed;
BYTE rgbReserved;
}RGBQUAD;
RGBQUAD结构的各个域的详细说明如下：
rgbBlue：该颜色的蓝色分量；
rgbGreen：该颜色的绿色分量；
rgbRed：该颜色的红色分量；
rgbReserved：保留字节，暂不用。
有些位图需要颜色表；有些位图（如真彩色图）则不需要颜色表，颜色表的长度由BITMAPINFOHEADER结构中biBitCount分量决定。对于biBitCount值为1的二值图像，每像素占1bit，图像中只有两种（如黑白）颜色，颜色表也就有21=2个表项，整个颜色表的大小为![](http://new.51cto.com/files/uploadimg/20080810/134159437.jpg)个字节；对于biBitCount值为8的灰度图像，每像素占8bit，图像中有![](http://new.51cto.com/files/uploadimg/20080810/134246591.jpg)颜色，颜色表也就有256个表项，且每个表项的R、G、B分量相等，整个颜色表的大小为![](http://new.51cto.com/files/uploadimg/20080810/134312795.jpg)个字节；而对于biBitCount=24的真彩色图像，由于每像素3个字节中分别代表了R、G、B三分量的值，此时不需要颜色表，因此真彩色图的BITMAPINFOHEADER结构后面直接就是位图数据。
第4部分是位图数据，即图像数据，其紧跟在位图文件头、位图信息头和颜色表（如果有颜色表的话）之后，记录了图像的每一个像素值。对于有颜色表的位图，位图数据就是该像素颜色在调色板中的索引值；对于真彩色图，位图数据就是实际的R、G、B值（三个分量的存储顺序是B、G、R）。下面分别就2色、16色、256色和真彩色位图的位图数据进行说明：
对于2色位图，用1位就可以表示该像素的颜色，所以1个字节能存储8个像素的颜色值。
 
对于16色位图，用4位可以表示一个像素的颜色。所以一个字节可以存储2个像素的颜色值。
 
对于256色位图，1个字节刚好存储1个像素的颜色值。
 
对于真彩色位图，3个字节才能表示1个像素的颜色值。
需要注意两点：
第一，Windows规定一个扫描行所占的字节数必须是4的倍数，不足4的倍数则要对其进行扩充。假设图像的宽为biWidth个像素、每像素biBitCount个比特，其一个扫描行所占的真实字节数的计算公式如下：
DataSizePerLine = (biWidth * biBitCount /8+ 3) / 4*4
那么，不压缩情况下位图数据的大小（BITMAPINFOHEADER结构中的biSizeImage成员）计算如下：
biSizeImage = DataSizePerLine * biHeight
第二，一般来说，BMP文件的数据是从图像的左下角开始逐行扫描图像的，即从下到上、从左到右，将图像的像素值一一记录下来，因此图像坐标零点在图像左下角。
 2. [http://linux.chinaunix.net/techdoc/system/2009/07/06/1122029.shtml](http://linux.chinaunix.net/techdoc/system/2009/07/06/1122029.shtml)
BMP文件格式分析
简介
    BMP(Bitmap-File)图形文件是Windows采用的图形文件格式，在Windows环境下运行的所有图象处理软件都支持BMP图象文件格式。Windows系统内部各图像绘制操作都是以BMP为基础的。Windows
    3.0以前的BMP图文件格式与显示设备有关，因此把这种BMP图象文件格式称为设备相关位图DDB(device-dependent
    bitmap)文件格式。Windows
    3.0以后的BMP图象文件与显示设备无关，因此把这种BMP图象文件格式称为设备无关位图DIB(device-independent
    bitmap)格式（注：Windows
    3.0以后，在系统中仍然存在DDB位图，象BitBlt()这种函数就是基于DDB位图的，只不过如果你想将图像以BMP格式保存到磁盘文件中时，微软极力推荐你以DIB格式保存），目的是为了让Windows能够在任何类型的显示设备上显示所存储的图象。BMP位图文件默认的文件扩展名是BMP或者 bmp（有时它也会以.DIB或.RLE作扩展名）。
    6.1.2 文件结构
    位图文件可看成由4个部分组成：位图文件头(bitmap-file
    header)、位图信息头(bitmap-information header)、彩色表(color
    table)和定义位图的字节阵列，它具有如下所示的形式。
    位图文件的组成  结构名称  符号 
    位图文件头(bitmap-file header) BITMAPFILEHEADER bmfh
    位图信息头(bitmap-information header) BITMAPINFOHEADER bmih
    彩色表(color table) RGBQUAD aColors[]
    图象数据阵列字节 BYTE aBitmapBits[]
    图象文件 头
     0000h 文件标识 2 bytes 两字节的内容用来识别位图的类型：
    ‘BM’ ： Windows 3.1x, 95, NT, … ‘BA’ ：OS/2 Bitmap Array ‘CI’
    ：OS/2 Color Icon ‘CP’ ：OS/2 Color Pointer ‘IC’ ： OS/2 Icon
    ‘PT’ ：OS/2 Pointer
    注：因为OS/2系统并没有被普及开，所以在编程时，你只需判断第一个标识“BM”就行。
      0002h File Size 1 dword 用字节表示的整个文件的大小
    　 0006h Reserved 1 dword 保留，必须设置为0
    　 000Ah Bitmap Data Offset 1 dword 从文件开始到位图数据开始之间的数据(bitmap
    data)之间的偏移量
    　 000Eh Bitmap Header Size 1 dword 位图信息头(Bitmap Info
    Header)的长度，用来描述位图的颜色、压缩方法等。下面的长度表示：
    28h - Windows 3.1x, 95, NT, … 0Ch - OS/2 1.x F0h - OS/2 2.x
    注：在Windows95、98、2000等操作系统中，位图信息头的长度并不一定是28h，因为微软已经制定出了新的BMP文件格式，其中的信息头结构变化比较大，长度加长。所以最好不要直接使用常数28h，而是应该从具体的文件中读取这个值。这样才能确保程序的兼容性。
      0012h Width 1 dword 位图的宽度，以象素为单位
    　 0016h Height 1 dword 位图的高度，以象素为单位
    　 001Ah Planes 1 word 位图的位面数（注：该值将总是1）
     001Ch Bits Per Pixel 1 word 每个象素的位数
    1 - 单色位图（实际上可有两种颜色，缺省情况下是黑色和白色。你可以自己定义这两种颜色） 4 - 16 色位图 8 -
    256 色位图 16 - 16bit 高彩色位图 24 - 24bit 真彩色位图 32 - 32bit 增强型真彩色位图
     　 001Eh Compression 1 dword 压缩说明：
    0 - 不压缩 (使用BI_RGB表示) 1 - RLE 8-使用8位RLE压缩方式(用BI_RLE8表示) 2 - RLE
    4-使用4位RLE压缩方式(用BI_RLE4表示) 3 -
Bitfields-位域存放方式(用BI_BITFIELDS表示)
    　 0022h Bitmap Data Size 1 dword 用字节数表示的位图数据的大小。该数必须是4的倍数
    　 0026h HResolution 1 dword 用象素/米表示的水平分辨率
    　 002Ah VResolution 1 dword 用象素/米表示的垂直分辨率
    　 002Eh Colors 1 dword 位图使用的颜色数。如8-比特/象素表示为100h或者 256.
    　 0032h Important Colors 1 dword
    指定重要的颜色数。当该域的值等于颜色数时（或者等于0时），表示所有颜色都一样重要
    调色板数据 根据BMP版本的不同而不同 Palette N * 4 byte
    调色板规范。对于调色板中的每个表项，这4个字节用下述方法来描述RGB的值：  1字节用于蓝色分量
     1字节用于绿色分量  1字节用于红色分量  1字节用于填充符(设置为0)
    图象数据 根据BMP版本及调色板尺寸的不同而不同 Bitmap Data xxx bytes
    该域的大小取决于压缩方法及图像的尺寸和图像的位深度，它包含所有的位图数据字节，这些数据可能是彩色调色板的索引号，也可能是实际的RGB值，这将根据图像信息头中的位深度值来决定。
    构件详解
    1. 位图文件头
    位图文件头包含有关于文件类型、文件大小、存放位置等信息，在Windows
    3.0以上版本的位图文件中用BITMAPFILEHEADER结构来定义：
    typedef struct tagBITMAPFILEHEADER { /* bmfh */
    UINT bfType;
    DWORD bfSize;
    UINT bfReserved1;
    UINT bfReserved2;
    DWORD bfOffBits;
    } BITMAPFILEHEADER;
    其中：
    bfType
    说明文件的类型.（该值必需是0x4D42，也就是字符'BM'。我们不需要判断OS/2的位图标识，这么做现在来看似乎已经没有什么意义了，而且如果要支持OS/2的位图，程序将变得很繁琐。所以，在此只建议你检察'BM'标识）
    bfSize
     说明文件的大小，用字节为单位
    bfReserved1
     保留，必须设置为0
    bfReserved2
     保留，必须设置为0
    bfOffBits
    说明从文件头开始到实际的图象数据之间的字节的偏移量。这个参数是非常有用的，因为位图信息头和调色板的长度会根据不同情况而变化，所以你可以用这个偏移值迅速的从文件中读取到位数据。
    2. 位图信息头
    位图信息用BITMAPINFO结构来定义，它由位图信息头(bitmap-information
    header)和彩色表(color
    table)组成，前者用BITMAPINFOHEADER结构定义，后者用RGBQUAD结构定义。BITMAPINFO结构具有如下形式：
    typedef struct tagBITMAPINFO { /* bmi */
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
    } BITMAPINFO;
    其中：
      bmiHeader
     说明BITMAPINFOHEADER结构，其中包含了有关位图的尺寸及位格式等信息
    bmiColors
     说明彩色表RGBQUAD结构的阵列，其中包含索引图像的真实RGB值。
    BITMAPINFOHEADER结构包含有位图文件的大小、压缩类型和颜色格式，其结构定义为：
    typedef struct tagBITMAPINFOHEADER { /* bmih */
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
    } BITMAPINFOHEADER;
    其中：
      biSize
    说明BITMAPINFOHEADER结构所需要的字数。注：这个值并不一定是BITMAPINFOHEADER结构的尺寸，它也可能是 sizeof(BITMAPV4HEADER)的值，或是sizeof(BITMAPV5HEADER)的值。这要根据该位图文件的格式版本来决定，不过，就现在的情况来看，绝大多数的BMP图像都是BITMAPINFOHEADER结构的（可能是后两者太新的缘故吧:-）。
    biWidth
     说明图象的宽度，以象素为单位
    biHeight
    说明图象的高度，以象素为单位。注：这个值除了用于描述图像的高度之外，它还有另一个用处，就是指明该图像是倒向的位图，还是正向的位图。如果该值是一个正数，说明图像是倒向的，如果该值是一个负数，则说明图像是正向的。大多数的BMP文件都是倒向的位图，也就是时，高度值是一个正数。（注：当高度值是一个负数时（正向图像），图像将不能被压缩（也就是说biCompression成员将不能是BI_RLE8或BI_RLE4）。
    biPlanes
     为目标设备说明位面数，其值将总是被设为1
    biBitCount
     说明比特数/象素，其值为1、4、8、16、24、或32
    biCompression
     说明图象数据压缩的类型。其值可以是下述值之一：
     BI_RGB：没有压缩；
     BI_RLE8：每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；
     BI_RLE4：每个象素4比特的RLE压缩编码，压缩格式由2字节组成
     BI_BITFIELDS：每个象素的比特由指定的掩码决定。
    biSizeImage
     说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0
    biXPelsPerMeter
     说明水平分辨率，用象素/米表示
    biYPelsPerMeter
     说明垂直分辨率，用象素/米表示
    biClrUsed
     说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）
    biClrImportant
     说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。
    现就BITMAPINFOHEADER结构作如下说明：
    (1) 彩色表的定位
    应用程序可使用存储在biSize成员中的信息来查找在BITMAPINFO结构中的彩色表，如下所示：
    pColor = ((LPSTR) pBitmapInfo + (WORD)
    (pBitmapInfo->bmiHeader.biSize))
    (2) biBitCount
    biBitCount=1
    表示位图最多有两种颜色，缺省情况下是黑色和白色，你也可以自己定义这两种颜色。图像信息头装调色板中将有两个调色板项，称为索引0和索引1。图象数据阵列中的每一位表示一个象素。如果一个位是0，显示时就使用索引0的RGB值，如果位是1，则使用索引1的RGB值。
    biBitCount=4
    表示位图最多有16种颜色。每个象素用4位表示，并用这4位作为彩色表的表项来查找该象素的颜色。例如，如果位图中的第一个字节为0x1F，它表示有两个象素，第一象素的颜色就在彩色表的第2表项中查找，而第二个象素的颜色就在彩色表的第16表项中查找。此时，调色板中缺省情况下会有16个RGB项。对应于索引0到索引15。
    biBitCount=8
    表示位图最多有256种颜色。每个象素用8位表示，并用这8位作为彩色表的表项来查找该象素的颜色。例如，如果位图中的第一个字节为0x1F，这个象素的颜色就在彩色表的第32表项中查找。此时，缺省情况下，调色板中会有256个RGB项，对应于索引0到索引255。
    biBitCount=16
    表示位图最多有216种颜色。每个色素用16位（2个字节）表示。这种格式叫作高彩色，或叫增强型16位色，或64K色。它的情况比较复杂，当 biCompression成员的值是BI_RGB时，它没有调色板。16位中，最低的5位表示蓝色分量，中间的5位表示绿色分量，高的5位表示红色分量，一共占用了15位，最高的一位保留，设为0。这种格式也被称作555
    16位位图。如果biCompression成员的值是BI_BITFIELDS，那么情况就复杂了，首先是原来调色板的位置被三个DWORD变量占据，称为红、绿、蓝掩码。分别用于描述红、绿、蓝分量在16位中所占的位置。在Windows
    95（或98）中，系统可接受两种格式的位域：555和565，在555格式下，红、绿、蓝的掩码分别是：0x7C00、0x03E0、0x001F，而在565格式下，它们则分别为：0xF800、0x07E0、0x001F。你在读取一个像素之后，可以分别用掩码“与”上像素值，从而提取出想要的颜色分量（当然还要再经过适当的左右移操作）。在NT系统中，则没有格式限制，只不过要求掩码之间不能有重叠。（注：这种格式的图像使用起来是比较麻烦的，不过因为它的显示效果接近于真彩，而图像数据又比真彩图像小的多，所以，它更多的被用于游戏软件）。
    biBitCount=24
    表示位图最多有224种颜色。这种位图没有调色板（bmiColors成员尺寸为0），在位数组中，每3个字节代表一个象素，分别对应于颜色R、G、B。
    biBitCount=32
    表示位图最多有232种颜色。这种位图的结构与16位位图结构非常类似，当biCompression成员的值是BI_RGB时，它也没有调色板，32位中有24位用于存放RGB值，顺序是：最高位—保留，红8位、绿8位、蓝8位。这种格式也被成为888
    32位图。如果
    biCompression成员的值是BI_BITFIELDS时，原来调色板的位置将被三个DWORD变量占据，成为红、绿、蓝掩码，分别用于描述红、绿、蓝分量在32位中所占的位置。在Windows
    95(or
    98)中，系统只接受888格式，也就是说三个掩码的值将只能是：0xFF0000、0xFF00、0xFF。而在NT系统中，你只要注意使掩码之间不产生重叠就行。（注：这种图像格式比较规整，因为它是DWORD对齐的，所以在内存中进行图像处理时可进行汇编级的代码优化（简单））。
    (3) ClrUsed
    BITMAPINFOHEADER结构中的成员ClrUsed指定实际使用的颜色数目。如果ClrUsed设置成0，位图使用的颜色数目就等于 biBitCount成员中的数目。请注意，如果ClrUsed的值不是可用颜色的最大值或不是0，则在编程时应该注意调色板尺寸的计算，比如在4位位图中，调色板的缺省尺寸应该是16＊sizeof(RGBQUAD)，但是，如果ClrUsed的值不是16或者不是0，那么调色板的尺寸就应该是 ClrUsed＊sizeof(RGBQUAD)。
    (4) 图象数据压缩
    ①
    BI_RLE8：每个象素为8比特的RLE压缩编码，可使用编码方式和绝对方式中的任何一种进行压缩，这两种方式可在同一幅图中的任何地方使用。
    编码方式：由2个字节组成，第一个字节指定使用相同颜色的象素数目，第二个字节指定使用的颜色索引。此外，这个字节对中的第一个字节可设置为0，联合使用第二个字节的值表示：
     第二个字节的值为0：行的结束。
     第二个字节的值为1：图象结束。
     第二个字节的值为2：其后的两个字节表示下一个象素从当前开始的水平和垂直位置的偏移量。
    绝对方式：第一个字节设置为0，而第二个字节设置为0x03～0xFF之间的一个值。在这种方式中，第二个字节表示跟在这个字节后面的字节数，每个字节包含单个象素的颜色索引。压缩数据格式需要字边界(word
    boundary)对齐。下面的例子是用16进制表示的8-位压缩图象数据：
    03 04 05 06 00 03 45 56 67 00 02 78 00 02 05 01 02 78 00 00 09
    1E 00 01
    这些压缩数据可解释为 ：
    压缩数据 
     扩展数据
    03 04 04 04 04 
    05 06 06 06 06 06 06 
    00 03 45 56 67 00 45 56 67 
    02 78 78 78 
    00 02 05 01 从当前位置右移5个位置后向下移一行
    02 78 78 78 
    00 00 行结束
    09 1E 1E 1E 1E 1E 1E 1E 1E 1E 1E 
    00 01 RLE编码图象结束 
    ②
    BI_RLE4：每个象素为4比特的RLE压缩编码，同样也可使用编码方式和绝对方式中的任何一种进行压缩，这两种方式也可在同一幅图中的任何地方使用。这两种方式是：
    编码方式：由2个字节组成，第一个字节指定象素数目，第二个字节包含两种颜色索引，一个在高4位，另一个在低4位。第一个象素使用高4位的颜色索引，第二个使用低4位的颜色索引，第3个使用高4位的颜色索引，依此类推。
    绝对方式：这个字节对中的第一个字节设置为0，第二个字节包含有颜色索引数，其后续字节包含有颜色索引，颜色索引存放在该字节的高、低4位中，一个颜色索引对应一个象素。此外，BI_RLE4也同样联合使用第二个字节中的值表示：
     第二个字节的值为0：行的结束。
     第二个字节的值为1：图象结束。
     第二个字节的值为2：其后的两个字节表示下一个象素从当前开始的水平和垂直位置的偏移量。
    下面的例子是用16进制数表示的4-位压缩图象数据：
    03 04 05 06 00 06 45 56 67 00 04 78 00 02 05 01 04 78 00 00 09
    1E 00 01
    这些压缩数据可解释为 ：
    压缩数据
     扩展数据
    03 04 0 4 0
    05 06 0 6 0 6 0 
    00 06 45 56 67 00 4 5 5 6 6 7 
    04 78 7 8 7 8 
    00 02 05 01 从当前位置右移5个位置后向下移一行
    04 78 7 8 7 8 
    00 00 行结束
    09 1E 1 E 1 E 1 E 1 E 1 
    00 01 RLE图象结束 
    3. 彩色表
    彩色表包含的元素与位图所具有的颜色数相同，象素的颜色用RGBQUAD结构来定义。对于24-位真彩色图象就不使用彩色表（同样也包括16位、和32位位图），因为位图中的RGB值就代表了每个象素难丈２噬碇械难丈囱丈闹匾耘判颍饪梢愿ㄖ允厩绦蛭荒芟允咀愎欢嘌丈南允旧璞赶允静噬枷蟆GBQUAD结构描述由R、G、B相对强度组成的颜色，定义如下：
    typedef struct tagRGBQUAD { /* rgbq */
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
    } RGBQUAD;
    其中：
    rgbBlue 指定蓝色强度 rgbGreen 指定绿色强度 rgbRed 指定红色强度 rgbReserved
    保留，设置为0
    4. 位图数据
    紧跟在彩色表之后的是图象数据字节阵列。图象的每一扫描行由表示图象象素的连续的字节组成，每一行的字节数取决于图象的颜色数目和用象素表示的图象宽度。扫描行是由底向上存储的，这就是说，阵列中的第一个字节表示位图左下角的象素，而最后一个字节表示位图右上角的象素。（只针对与倒向DIB，如果是正向 DIB，则扫描行是由顶向下存储的），倒向DIB的原点在图像的左下角，而正向DIB的原点在图像的左上角。同时，每一扫描行的字节数必需是4的整倍数，也就是DWORD对齐的。如果你想确保图像的扫描行DWORD对齐，可使用下面的代码：
    (((width*biBitCount)+31)>>5)<<2
    5. 参考书目
    《图象文件格式(上、下)—Windows编程》
    《图像文件格式大全》
    《Programming Windows by Charles Petzold》
    6. 相关站点
    各种格式：http://www.wotsit.org/
    各种格式：http://www.csdn.net/
    位图格式：http://www.cica.indiana.edu/graphics/image_specs/bmp.format.txt
    〈完〉 
