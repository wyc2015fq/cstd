# zlib 与 libpng 的配置与使用 - xqhrs232的专栏 - CSDN博客
2010年03月02日 11:25:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2824
原文地址：：[http://ailantian.bokee.com/2400186.html](http://ailantian.bokee.com/2400186.html)
zlib 与 libpng 的配置与使用
Solstice2003/04/3
说明：本文节选自我主页上的一篇文章，原文介绍了1) Wave 文件的格式、2)读取 Wave 文件内容，并显示文件的基本信息、3)压缩库 zlib 的安装与简单应用、4)PNG 库 libpng 的安装、5)使用 libpng 生成 PNG 文件、6)绘制 Wave 文件的波形，这里只截取3、4、5这三部分内容。本文涉及的源码可从我的主页下载([http://www.chenshuo.com](http://www.chenshuo.com/))。
PNG 格式的图片在网络上非常流行，几乎所有浏览器都支持这种格式。PNG 代表 Portable Network Graphics——可移植网络图形格式。我偏爱 PNG 图片的另一个原因是，在 LaTeX 生成的 PDF 文件中，可以直接嵌入 PNG 文件。PNG 与 GIF 类似，是无损压缩的光栅图形格式。与 GIF 文件不同，编写生成 PNG 文件的软件不需要支付任何版权费用。因此，PNG 的非官方名称为*Png's Not Gif*，够搞笑，是吧？
尽管 PNG 文件的格式并不复杂，我还是决定用一套现成的程序库来读写它，不要总是自己重新发明轮子嘛。我们先来看看怎么安装使用 PNG 文件的官方程序库——libpng 和 zlib。常见的 Linux 系统都配备了这两个程序库，因此我只打算介绍在 Windows 下的安装方法。
以下的操作以免费的 Borland C++ Compiler 5.5.1 free 编译器为例，Microsoft Visual C++ 安装方法大致与此类似，但我没有条件测试。
zlib 的安装
libpng 是一套免费的、公开源代码的程序库，支持对 PNG 图形文件的创建、读写等操作。libpng 使用 zlib 程序库作为压缩引擎，zlib 也是著名的*gzip* (GNU zip) 所采用的压缩引擎。
我们首先安装zlib，从其官方网站下载最新的源程序，不妨假设文件名是zlib-1.1.4.tar.gz。网址：http://www.gzip.org/zlib/。
在 D:/ 建立libpng目录，将zlib-1.1.4.tar.gz释放到这个目录。尽管没有合适的makefile，我们仍然可以直接编译链接zlib.lib：
D:/libpng/zlib-1.1.4>**bcc32 -c -O2 -6 -w-8004 -w-8057 -w-8012 *.c**
Borland C++ 5.5.1 for Win32 Copyright (c) 1993, 2000 Borland
adler32.c:
compress.c:
crc32.c:
deflate.c:
example.c:
gzio.c:
infblock.c:
infcodes.c:
inffast.c:
inflate.c:
inftrees.c:
infutil.c:
maketree.c:
minigzip.c:
trees.c:
uncompr.c:
zutil.c:
D:/libpng/zlib-1.1.4>**tlib zlib.lib +adler32.obj +compress.obj +crc32.obj**
**+deflate.obj +gzio.obj +infblock.obj +infcodes.obj +inffast.obj**
**+inflate.obj +inftrees.obj +infutil.obj +maketree.obj +trees.obj**
**+uncompr.obj +zutil.obj**
TLIB 4.5 Copyright (c) 1987, 1999 Inprise Corporation
注意，在 tlib 的命令行中，没有 example.obj 和 minigzip.obj。接下来，测试 zlib.lib 是否编译成功，执行：
D:/libpng/zlib-1.1.4>**bcc32 minigzip.obj zlib.lib**
Borland C++ 5.5.1 for Win32 Copyright (c) 1993, 2000 Borland
Turbo Incremental Link 5.00 Copyright (c) 1997, 2000 Borland
D:/libpng/zlib-1.1.4>**bcc32 example.obj zlib.lib**
Borland C++ 5.5.1 for Win32 Copyright (c) 1993, 2000 Borland
Turbo Incremental Link 5.00 Copyright (c) 1997, 2000 Borland
D:/libpng/zlib-1.1.4>**example**
uncompress(): hello, hello!
gzread(): hello, hello!
gzgets() after gzseek: hello!
inflate(): hello, hello!
large_inflate(): OK
after inflateSync(): hello, hello!
inflate with dictionary: hello, hello!
执行example.exe，看见“hello, hello!”，表明生成的zlib.lib是好的。
zlib 是通用的压缩库，提供了一套 in-memory 压缩和解压函数，并能检测解压出来的数据的完整性(integrity)。zlib 也支持读写 gzip (.gz) 格式的文件。下面介绍两个最有用的函数——**compress**和**uncompress**。
int compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
compress函数将source缓冲区中的内容压缩到dest缓冲区。sourceLen表示source缓冲区的大小(以字节计)。注意函数的第二个参数destLen是传址调用。当调用函数时，destLen表示 dest 缓冲区的大小，destLen > (sourceLen + 12)*100.1%。当函数退出后，destLen表示压缩后缓冲区的实际大小。此时destLen / sourceLen正好是压缩率。
compress若成功，则返回 Z_OK；若没有足够内存，则返回Z_MEM_ERROR；若输出缓冲区不够大，则返回Z_BUF_ERROR。
int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
uncompress函数将source缓冲区的内容解压缩到dest缓冲区。sourceLen是source缓冲区的大小(以字节计)。注意函数的第二个参数destLen是传址调用。当调用函数时，destLen表示dest缓冲区的大小，dest缓冲区要足以容下解压后的数据。在进行解压缩时，需要提前知道被压缩的数据解压出来会有多大。这就要求在进行压缩之前，保存原始数据的大小(也就是解压后的数据的大小)。这不是 zlib 函数库的功能，需要我们做额外的工作。当函数退出后，destLen是解压出来的数据的实际大小。
uncompress若成功，则返回 Z_OK；若没有足够内存，则返回Z_MEM_ERROR；若输出缓冲区不够大，则返回Z_BUF_ERROR。若输入数据有误，则返回Z_DATA_ERROR。
zlib 带的example.c是个很好的学习范例，值得一观。我们写个程序，验证 zlib 的压缩功能。所写的测试程序保存为 testzlib.cpp，放在zlib-1.1.4 目录下。程序源代码：
// testzlib.cpp简单测试 zlib 的压缩功能
#include 
#include 
#include 
#include "zlib.h"
using namespace std;
int main()
{
int err;
Byte compr[200], uncompr[200];// big enough
uLong comprLen, uncomprLen;
const char* hello = "12345678901234567890123456789012345678901234567890";
uLong len = strlen(hello) + 1;
comprLen= sizeof(compr) / sizeof(compr[0]);
err = compress(compr, &comprLen, (const Bytef*)hello, len);
if (err != Z_OK) {
cerr << "compess error: " << err << '/n';
exit(1);
}
cout << "orignal size: " << len
<< " , compressed size : " << comprLen << '/n';
strcpy((char*)uncompr, "garbage");
err = uncompress(uncompr, &uncomprLen, compr, comprLen);
if (err != Z_OK) {
cerr << "uncompess error: " << err << '/n';
exit(1);
}
cout << "orignal size: " << len
<< " , uncompressed size : " << uncomprLen << '/n';
if (strcmp((char*)uncompr, hello)) {
cerr << "BAD uncompress!!!/n";
exit(1);
} else {
cout << "uncompress() succeed: /n" << (char *)uncompr;
}
}
编译执行这个程序，输出应该是
D:/libpng/zlib-1.1.4>**bcc32 testzlib.cpp zlib.lib**
D:/libpng/zlib-1.1.4>**testzlib**
orignal size: 51 , compressed size : 22
orignal size: 51 , uncompressed size : 51
uncompress() succeed:
12345678901234567890123456789012345678901234567890
至此， zlib的安装任务算是完成了。为了以后使用方便，我将zlib.lib、zlib.h、zconf.h拷贝到D:/mylibs/。
libpng 的安装
接下来，安装 libpng 的过程要稍微轻松些。先下载最新的 libpng 程序库源文件。网址是http://sourceforge.net/projects/libpng/或http://www.libpng.org/pub/png/。不妨设下载的文件是libpng-1.2.5.tar.gz，将这个文件释放到D:/libpng/。
修改D:/libpng/libpng-1.2.5/scripts/makefile.bc32，这是为Borland C++ 32-bit 版准备的 makefile。将第12行的ZLIB_DIR=../zlib改为ZLIB_DIR=D:/mylibs，再将第20行的#TARGET_CPU=6前的井号(#)去掉。然后执行
D:/libpng/libpng-1.2.5>**make -fscripts/makefile.bc32**
MAKE Version 5.2Copyright (c) 1987, 2000 Borland
D:/libpng/libpng-1.2.5>**pngtest**
Testing libpng version 1.2.5
with zlibversion 1.1.4
. . .
PASS (9782 zero samples)
. . .
libpng passes test
看到“9782 zero samples”字样，表明 libpng 安装成功。新生成的pngout.png应该与原有的pngtest.png完全一样。将 png.h、pngconf.h连同编译生成的libpng.lib一起拷贝到D:/mylibs/。
生成 PNG 文件
我们自己写一两个程序来测试 libpng 生成 PNG 文件的功能。testpng1.cpp生成灰度(gray)图象；testpng2.cpp生成256色图象，其中调色盘(palette)有多种配置。这两个程序生成的图片文件分别展示于图1和图2中。编译参数为：
**bcc32 -Id:/mylibs -Ld:/mylibs testpng1.cpp libpng.lib zlib.lib**
为了日后使用方便，我把D:/mylibs/ 加入到 BCC 5.5 的 include 搜索路径和 lib 搜索路径中。
// testpng1.cpp
// test for black & white pictures.
// 请下载文章的配套源码[http://www.chenshuo.com](http://www.chenshuo.com/)
testpng1.cpp 和testpng2.cpp的差别在png_set_IHDR这行，前一个是png_set_IHDR(png_ptr, info_ptr, width, height, 8, **PNG_COLOR_TYPE_GRAY**, ...);，
后一个是
png_set_IHDR(png_ptr, info_ptr, width, height, 8, **PNG_COLOR_TYPE_PALETTE**,...);。
// testpng2.cpp
// test for colorful pictures
// 请下载文章的配套源码[http://www.chenshuo.com](http://www.chenshuo.com/)
|![](http://www.csdn.net/develop/article/images/test1-a.png)![](http://www.csdn.net/develop/article/images/test1-b.png)|![](http://www.csdn.net/develop/article/images/test2-a.png)![](http://www.csdn.net/develop/article/images/test2-b.png)|
|----|----|
|图1 灰度 PNG 图片|图2 彩色 PNG 图片|
libpng 带的example.c 是很好的学习范例。使用 libpng 时，先要 include png.h。这个头文件包含了 libpng 自定义的许多类型，如程序中出现的png_struct、png_info 等等，前者是 libpng 内部使用的结构体，后者用来表示某个 PNG 文件的相关信息。
编写生成PNG的程序先声明几个必要的变量，其中png_structp就是png_struct*：
FILE *fp;
png_structp png_ptr;
png_infop info_ptr;
png_colorp palette;
然后以 binary write 方式打开欲写入的 PNG 文件。
fp = fopen(filename.c_str(), "wb");
欲使用 libpng ，先分配并初始化png_struct：
png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
然后以 png_ptr 为参数初始化info_ptr。以后用png_get_*()或png_set_*()函数来读取或设置 PNG 文件的属性。
info_ptr = png_create_info_struct(png_ptr);
设置错误处理方式，也可以在第6行指定处理错误的 callback 函数。
if (setjmp(png_jmpbuf(png_ptr)))
{
// . . .
}
接下来告诉 libpng 用fwrite来写入 PNG 文件，并传给它已按二进制方式打开的FILE* fp ：
png_init_io(png_ptr, fp);
设置 PNG 文件的基本属性，如高度、宽度、色深(单色、16色、256色或真彩色)、色彩类型(灰度、调色板、RGB)等等。这里我们生成一个256 (28=256)色、采用调色板(PNG_COLOR_TYPE_PALETTE )的 PNG 文件。
const int width= 120;
const int height = 512;
png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_PALETTE,
PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
接下来设置调色板，先分配空间。常数 PNG_MAX_PALETTE_LENGTH 的值是256：
palette = (png_colorp)png_malloc(png_ptr,
PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
这里的 png_color 表示调色板中某一种颜色的 RGB 分量。
typedef struct png_color_struct
{
png_byte red;
png_byte green;
png_byte blue;
} png_color;
然后用自己写的 set_palette 函数设置调色板。
set_palette(palette, RED_BLACK);
set_palette 函数能生成三种类型的调色板，GRAY灰度、RED_BLACK红与黑、SPECTRUM频谱。
其中生成 GRAY 调色板的代码是：
for (int i = 0; i < PNG_MAX_PALETTE_LENGTH; ++i ) {
palette[i].red = palette[i].green = palette[i].blue = i;
}
告诉 libpng 采用我们准备好的调色板，并写 PNG 文件的头部。
png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
png_write_info(png_ptr, info_ptr);
在这些准备工作做完之后，进入最关键的一步——绘制图片内容，这里我们只是依次使用调色板的各种颜色来绘制水平直线。我们准备足够大的一块内存image来表示整幅图像中的所有点，这些点按从左到右，从上到下的顺序排列。注意，PNG 文件是行优先，在写入 PNG 文件时，不用告诉它整幅图形在哪，只要告诉它每一行(由数组 row_pointers表示)在哪就行了。所以如果图像中某些行是相同的，就可以让行指针row_pointer重复指向这些行的地址，这样能节省内存空间。我写的这个程序没有采用这个办法，必尽内存不是什么大问题。
png_uint_32 k;
png_byte image[height][width];
png_bytep row_pointers[height];
for (k = 0; k < height; k++) {
memset(image[k], k / 2, width);
row_pointers[k] = image[k];
}
接下来一次写入整幅图像，是最省力的办法：
png_write_image(png_ptr, row_pointers);
末了，进行必要的扫尾工作：
png_write_end(png_ptr, info_ptr);
png_free(png_ptr, palette);
png_destroy_write_struct(&png_ptr, &info_ptr);
fclose(fp);
至此，大功告成。
