# CreateDIBSection、BITMAPINFO、BITMAPINFOHEADER、RGBQUAD - [Daliy APIs] - xqhrs232的专栏 - CSDN博客
2013年03月15日 23:10:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：786
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/Lisen/archive/2010/01/21/1653608.html](http://www.cnblogs.com/Lisen/archive/2010/01/21/1653608.html)
CreateDIBSection
**该函数创建应用程序可以直接写入的、与设备无关的位图（DIB）。该函数返回一个位图句柄。**
**原型：**
HBITMAPCreateDIBSection(
HDC                  hdc,
CONSTBITMAPINFO *   pbmi,
UINT                 iUsage,
VOID *               ppvBits,
HANDLE               hSection,
DWORD                dwOffset);
**参数: **
hdc：设备环境句柄。如果iUsage的值是DIB_PAL_COLORS，那么函数使用该设备环境的逻辑调色板对与设备无关位图的颜色进行初始化。
pbmi：指向BITMAPINFO结构的指针，该结构指定了设备无关位图的各种属性，其中包括位图的尺寸和颜色。
iUsage：指定由pbmi参数指定的BITMAPINFO结构中的成员bmiColors数组包含的数据类型（要么是逻辑调色板索引值，要么是原文的RGB值）。下列值是系统定义的，其含义为：
|值|描述|
|----|----|
|**DIB_RGB_COLORS**|根据BITMAPINFOHEADER 的biCompression 成员决定BITMAPINFO 结构包含位掩码还是调色板数组，在呈现位图时使用该数组值。DIB_RGB_COLORS 可以在任何位数的位图上使用。|
|**DIB_PAL_COLORS**|BITMAPINFO.bmiColors 数组被取消，在呈现位图时使用目标调色板。DIB_PAL_COLORS只能在8bpp位图中指定。|
ppvBits：指向一个变量的指针，该变量接收一个指向DIB位数据值的指针。
hSection：该参数设置为NULL。
dwOffset：参数取消。
**返回值：**
   成功，返回值是一个指向刚刚创建的设备无关位图的句柄，并且***ppvBits**指向该位图的位数据值；失败，那么返回值为NULL，并且***ppvBit**也为NULL，若想获得更多错误信息，请调用**GetLastError**函数。
**备注：**
   系统为设备独立位图分配内存。如果在之后调用**DeleteObject**来删除设备独立位图，系统自动关闭内句柄。
   在Windows CE 2.0及其以后版本，如果图像是调色板模式（通常是1，2，4和8格式）的，**BITMAPINFO **结构中必须包含一个颜色表。对于16bpp或32bpp非调色板图像，颜色表必须是3个入口的长度，这3个入口必须指定红、绿、蓝色掩码。 而且，**BITMAPINFOHEADER **结构的**biCompression **成员应该被设置为**BI_BITFIELDS**。
 这些位深不支持BI_RGB。 GDI取消24bpp图像的颜色表，他们的像素必须被存储为 蓝-绿-红 (BGR)格式。
   Windows CE 不支持332位阈设备。
   在Windows CE 1.0 和 1.01版本，pbmi指向的BITMAPINFO结构必须指定每个像素点为1或2位。
BITMAPINFO
**该结构定义了基于Windows的设备独立位图(DIB)的尺寸大小和颜色信息**
**原型：**
typedefstructtagBITMAPINFO { 
BITMAPINFOHEADERbmiHeader; 
RGBQUAD           bmiColors[1]; 
} BITMAPINFO;
**成员：**
bmiHeader： 位图信息头结构，该结构包含了位图的尺寸和颜色格式。
bmiColors： 包含下面中的一种。
- **RGBQUAD **数组。数组元素填充颜色表。
- 16位无符号整形数组，该数组指定索引到当前已实现的逻辑调色板。
允许在使用DIB的函数中使用**bmiColors **。当**bmiColors **包含已实现的逻辑调色板的索引时，必须也要调用**CreateDIBPatternBrushPt **和**CreateDIBSection **。**CreateDIBSection**的**iUsage **成员必须被设置为**DIB_PAL_COLORS **。
**BITMAPINFOHEADER **结构的**biBitCount **和 **biClrUsed **成员的值决定数组的大小。
**bmiColors **表中的颜色根据重要性排序。更多信息，参见备注。
- 如果**bmiHeader.biCompression** 被设置为**BI_RGB **，可以设置**bmiColors **数组大小为0.
**备注：**
   设备独立位图由2部分组成：用于描述位图的尺寸大小和颜色的**BITMAPINFO **结构和定义位图像素的字节数组。字节数组中的所有位封装在一起，但每行扫描必须在行尾补0以确保行尾为**LONG**数据类型边界(*each scan line must be padded with zeroes to end on a ****LONG**** data-type boundary.*)。
   如果位图高度为正数，则位图为从下到上(bottom-up)DIB，它的起点为左下角坐标。如果高度为负数，则位图为从上到下(top-down) DIB，它的起点为左上角。
封装位图时，位图字节数组紧跟在BITMAPINFO 头后面。封装的位图被一个单独指针引用。 
   对于封装的位图，当使用**DIB_PAL_COLORS**模式时，**BITMAPINFOHEADER **结构的**ClrUsed **成员必须设置为偶数，因此，DIB位图数组以**DWORD**边界开始。
   如果位图被存储至文件或传送到其他应用程序，**bmiColors **成员不应包含调色板索引。
   除非程序独占使用和控制位图，否则位图颜色表应该包含明确的RGB值。
安全提示： 一个常见的错误类型包括在内容中发现无效的格式描述。比如，一个BITMAPINFOHEADER结构后跟着一个颜色表。
**   BITMAPINFO **结构被定义为一个**BITMAPINFOHEADER **结构后跟着一个填充颜色表**RGBQUAD **数组。**RGBQUAD **数组大小由**BITMAPINFOHEADER **中的**biClrUsed**值决定。
   在拷贝颜色表到BITMAPINFO前一定要检查为**BITMAPINFO**结构分配的缓冲大小，否则决不能那么做。
BITMAPINFOHEADER
**该结构包含设备独立位图的尺寸大小和颜色格式信息。**
**原型：**
typedefstructtagBITMAPINFOHEADER { 
DWORDbiSize; 
LONGbiWidth; 
LONGbiHeight; 
WORDbiPlanes; 
WORDbiBitCount
DWORDbiCompression; 
DWORDbiSizeImage; 
LONGbiXPelsPerMeter; 
LONGbiYPelsPerMeter; 
DWORDbiClrUsed; 
DWORDbiClrImportant; 
} BITMAPINFOHEADER;
**成员：**
biSize：指定结构体大小，按字节计算，不包括biClrUsed 成员提及的颜色表或掩码的大小。参见备注。
biWidth： 位图的宽度，按像素计算。
biHeight： 位图的高度，按像素计算。
    如果biHeight为正数，位图为从下而上(bottom-up) DIB ，起点为左下角。
    如果biHeight为负数，位图为从上而下(top-down) DIB ，起点为左上角，
    如果biHeight为负数，标识一个从上而下(top-down) DIB，**biCompression **必须为**BI_RGB **或**BI_BITFIELDS**。从上而下(top-down) DIB不能被压缩。
biPlanes：指定目标设备的planes的数量。 必须设定为 1 。
biBitCount：指定每个像素的位数。
    该成员决定位图中定义每个像素的位数和最大颜色数量。 该成员必须为下面值中的一个：
|值|描述|
|----|----|
|**1**|位图为黑白色，bmiColors 成员包含两个入口。位图数组中的每位代表一个像素。如果该位被清除，像素被显示为bmiColors 表的第一个入口的颜色。如果颜色被设置了，像素拥有bmiColors 表第二个入口的颜色。|
|**2**|位图有4个可能的颜色值。|
|**4**|位图有最大16位色，bmiColors 成员最多包含16个入口。颜色表按照4位索引呈现位图中每个像素。例如，如果位图中的第一个字节是Ox1F，该字节代表2个像素。第一个像素包含第二个表入口的颜色，第二个像素包含第16个表入口的颜色。|
|**8**|位图最多有256中颜色，bmiColors 成员包含最多256个入口。这种情况下，数组中的每个字节代表单独一个像素。|
|**16**|位图最多有2^16 种颜色。 如果BITMAPINFOHEADER 的biCompression 成员值为BI_RGB ， bmiColors 为NULL。位图数组中的每个字(WORD)代表单独一个像素。红、绿、蓝的相对强度分别以5位呈现。蓝色值为最低有效5位，接着5位是绿，然后是红。最高有效位不被使用。bmiColors 颜色表用来优化使用在基于调色板的设备上的颜色，而且必须包含BITMAPINFOHEADER的biClrUsed 成员指定的入口数量。|
|**24**|位图最多有2^24种颜色。bmiColors 为NULL。位图数组中每个3字节三元组分别代表每个像素的蓝、绿、红色的相对强度。bmiColors 颜色表用来优化使用在基于调色板的设备上的颜色，而且必须包含BITMAPINFOHEADER的biClrUsed 成员指定的入口数量。|
|**32**|位图最多有2^32种颜色。如果BITMAPINFOHEADER 的biCompression 成员是BI_RGB ， bmiColors 为NULL。位图数组中的每个双字(DWORD)分别代表每个像素的蓝、绿、红色的相对强度。每个双字的高字节不使用。bmiColors 颜色表用来优化使用在基于调色板的设备上的颜色，而且必须包含BITMAPINFOHEADER的biClrUsed 成员指定的入口数量。如果BITMAPINFOHEADER 的biCompression 的值为BI_BITFIELDS，bmiColors 成员包含3个双字颜色掩码来分别指定组成每个像素的红、绿、蓝色。位图数组中的每个双字代表一个像素。|
biCompression：为压缩的bottom-up位图指定压缩类型，而top-down DIBs 不能被压缩。该成员值可能为下表中的一个：|值|描述|
|----|----|
|**BI_RGB**|未压缩的格式|
|**BI_BITFIELDS**|用来说明位图没有被压缩并且颜色表由3个双字颜色掩码组成（3个双字颜色掩码来分别指定组成每个像素的红、绿、蓝值）。当使用16bpp和32bpp位图时该标志可用。该值在Windows CE 2.0 及其以后版本可用。|
|**BI_ALPHABITFIELDS**|用来说明位图没有被压缩并且颜色表由4个双字颜色掩码组成（4个双字颜色掩码来分别指定组成每个像素的红、绿、蓝和alpha值）。当使用16bpp和32bpp位图时该标志可用。该值在Windows CE 4.0 及其以后版本可用。|
    对于Windows CE 5.0 及其以后版本，可有使用上表的任意值与**BI_SRCPREROTATE** 进行布尔**OR** 运算来标明源DIB和目的DIB有相同的旋转角度。
biSizeImage：指定图像的大小，按字节计算。当为**BI_RGB** 时，该值可以设置为0。
biXPelsPerMeter：为位图指定目标设备水平分辨率，按每公尺像素数计算(*in pixels per meter*)。程序使用该值从资源组中选择最符合当前设备特性的位图。
biYPelsPerMeter：为位图指定目标设备垂直分辨率按每公尺像素数计算(*in pixels per meter*)。
biClrUsed：指定实际在位图中使用的颜色表中的颜色索引的数量。
    如果该值为0，位图为biCompression 指定的压缩模式使用biBitCount 值对应的最大颜色数量。
    如果biClrUsed 非0，并且biBitCount 小于16，biClrUsed 指定图形引擎或设备驱动访问的实际颜色数量。
    如果biBitCount 大于等于16，biClrUsed 指定颜色表的数量，用来优化系统颜色调色板性能。
    如果biBitCount 等于16或32，最佳颜色调色板紧跟在3个双字掩码之后开始(the optimal color palette starts immediately following the three DWORD masks.)。
    如果位图被封装了(位图数组紧接着BITMAPINFO头，并被一个单独指针引用)，biClrUsed 必须要么为0，要么为颜色表的实际大小。
biClrImportant：显示位图时指的定颜色索引的数量。如果该值为0，要求使用所有颜色。
**备注：**
**   BITMAPINFO **结构合并**BITMAPINFOHEADER**和一个颜色表来提供对一个DIB的尺寸大小和颜色的完全定义。程序应该使用存储在biSize 中的信息来定位**BITMAPINFO **结构中的颜色表，如下，
pColor = ((LPSTR)pBitmapInfo + (WORD)(pBitmapInfo->bmiHeader.biSize));
   对于Windows CE 1.0 和1.01版本，biBitCount 必须为1或2.
RGBQUAD
**该结构用于描述一种颜色的红、绿、蓝的相对强度。**
**原型：**
typedefstructtagRGBQUAD { 
BYTErgbBlue;
BYTErgbGreen;
BYTErgbRed;
BYTErgbReserved;
} RGBQUAD;
**成员：**
rgbBlue、rgbGreen、rgbRed：分别指定蓝、绿、红色的强度。rgbReserved是保留值，设置为0。
**备注:**
Windows CE 不支持rgbReserved，应该初始化为0。BITMAPINFO结构的bmiColors 成员由RGBQUAD 结构数组组成。
**参考：**
互动百科:  [CreateDIBSection](http://www.hudong.com/wiki/CreateDIBSection)
MSDN:  [CreateDIBSection](http://msdn.microsoft.com/en-us/library/ms908174.aspx)
