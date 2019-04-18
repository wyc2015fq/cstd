# 图像文件格式之PNG（转载） - 深之JohnChen的专栏 - CSDN博客

2005年11月30日 09:36:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2623


图像文件格式之PNG

1．概述

PNG是Portable Network Graphics的缩写，是一种采用无损压缩、轻量级的静态图像文件格式。PNG不仅支持基于调色板的图像格式，也支持灰度图，以及真彩色图像，还有可选的Alpha通道；与GIF（Graphics Interchange Format）文件相比，PNG不受专利权的困扰更是它一大优势。PNG是开放的，是自由的，是免费的，从它诞生之日起就扮演着GIF替代者的角色；PNG文件格式在设计上的先进性，加上它的可流化以及支持渐进显示的能力，这种图像文件格式在互联网上越来越受欢迎也就不足为奇了！

2．文件结构

给定一个图像文件，如何判断它是不是PNG文件呢？除了看它的文件扩展名是不是.png外，还可以分析文件开头的部分数据，这部分数据我们一般称之为文件签名（File Signature）。每个标准的PNG文件开头都有如下格式的数字签名（数字以16进制表示，总共8个字节）：
89  50  4e  47  0d  0a  1a  0a

在文件签名之后，就开始PNG数据了。PNG数据由一个一个数据块（Data Chunk）组成。每个数据块都有一个4字节长度的标识，典型的有IHDR、IDAT、IEND等。完整的PNG数据总是以一个IHDR块开头，最终以一个IEND块结束的，而真正表示图像数据的IDAT块则嵌在IHDR块和IEND块之间（注意这3种数据块在文件中出现的顺序不可以颠倒，并且IDAT块可以多次出现）。因此，一个典型的PNG文件大致如图1.1所示：

![说明 PNGFileStruct.jpg](http://www.vchelp.net/ASP/ibr_upload/1589.jpg)

图1.1 PNG文件的总体结构

3．格式细节

PNG文件中的数据块都有一个固定的格式，即依次由4部分组成：Length、Chunk Type、Chunk Data和CRC，如表1.1：

表1.1 PNG数据块的基本结构

域名    域长度    描述
Length    4字节    一个无符号整数，表示数据块实际数据的长度（Length、Chunk Type、CRC这3个域均不计算在内）；其值可能为0，表示Chunk Data域根本不存在。
Chunk Type    4字节    数据块的标识，由A-Z或a-z中的4个字母组合而成。数据块标识的选择另有一套约定，区分大小写，典型如IHDR、PLTE、tRNS、gAMA、IDAT、IEND等。
Chunk Data    由Length域指定    具有实际意义的块数据，长度由Length域指定，数据的意义因Chunk Type不同而不同。
CRC    4字节    对Chunk Type和Chunk Data两个域进行计算得出的校验码，用于检查块数据的合法性（未曾被篡改过）。

根据数据块的功能及其重要程度，PNG数据块可以大致分成两类：关键数据块和辅助数据块。常见的关键数据块有IHDR、PLTE、IDAT、IEND等；常见的辅助数据块有tRNS、gAMA、cHRM、sRGB、iCCP、iTXt、tEXt、zTXt、bKGD、pHYs、sBIT、sPLT、hIST、tIME等。下面再详细介绍一下几个关键数据块。

IHDR数据块定义了图像头信息，是PNG文件中必须第一个出现的数据块。这个数据块定义了如下的一个数据结构：

表1.2 IHDR数据块定义的数据结构

域名    域长度    描述
Width    4字节    图像的宽度，以像素为单位
Height    4字节    图像的高度，以像素为单位
Bit depth    1字节    用于表达一个采样点使用的Bit数，可能的值为1、2、4、8、16
Color type    1字节    表示图像数据的颜色类型，用二进制位来表示，1：使用调色板，2：使用RGB颜色，4：使用Alpha通道。经过组合，这个域可能的取值为0、2、3、4、6（另外，Bit depth和Color type的组合必须符合表1.3的约束）
Compression method    1字节    图像数据的压缩方法，目前指定为0，即使用deflate/inflate方法（这是一种通用的无损压缩方法，支持这种方法的开源代码集为ZLib，更多信息参见 [http://www.zlib.net/）](http://www.zlib.net/%EF%BC%89)
Filter method    1字节    压缩前进行的预处理过滤类型，目前指定为0，
Interlace method    1字节    图像数据的传输方式，0：顺序模式（no interlace），1：交错模式（Adam7 interlace）

表1.3 Color type和Bit depth组合的约束

Color type    允许的Bit depth    代表的图像类型
0    1、2、4、8、16    灰度图，每个像素的值都是一个灰度值
2    8、16    每个像素都直接用RGB分量来表示
3    1、2、4、8    使用调色板（这种PNG文件必然有PLTE数据块的存在），每个像素的值实际上是该像素的颜色在调色板中的索引值
4    8、16    灰度图，并且每个像素都带有一个Alpha通道值
6    8、16    每个像素都直接用RGB分量来表示，并且都带有一个Alpha通道值

提示：对于IHDR数据块来说，Chunk Type就是IHDR，Chunk Data就可以用这个数据结构来诠释。另外，PNG使用的网络字节顺序，也就是Big-endian.

PLTE数据块定义了调色板的颜色矩阵。这个矩阵可以有1个元素，最多有256个元素；每个矩阵元素都用3个字节来表示，分别为这种颜色的红、绿、蓝3个分量。这种数据块在Color type为3时必须出现，在Color type为2或6时也可能出现；但如果出现，必须在IDAT数据块之前！

IDAT数据块携带的是压缩过的图像数据。这种数据块可以在一个PNG文件中多次出现，且数据块的大小不固定。

IEND数据块必须出现在一个PNG文件的最后，表示PNG文件的结束。注意，这个数据块的Chunk data是空的。

表1.4 PNG文件的标准数据块一览表

数据块类型    是否允许多次出现？    出现次序上的约束    备注
关键数据块（必须按下列顺序依次出现，不过PLTE是可选的）
IHDR    否    必须是第1个    描述图像头信息
PLTE    否    在IDAT之前    记录调色板颜色矩阵
IDAT    是    多个IDAT出现时必须是连续的    记录图像数据
IEND    否    必须是最后1个    表征PNG文件的结束
辅助数据块（不必按下列顺序依次出现）
cHRM    否    必须在PLTE和IDAT之前    使用设备无关的颜色
gAMA    否    必须在PLTE和IDAT之前    图像再现时进行Gama校正
iCCP    否    必须在PLTE和IDAT之前    进行特殊的颜色处理
sBIT    否    必须在PLTE和IDAT之前    编解码器在Bit depth转换时用到的辅助信息
sRGB    否    必须在PLTE和IDAT之前    指定一种图像显示模式：知觉的、相对色度、饱和度、绝对色度
bKGD    否    必须在PLTE之后且在IDAT之前    指定图像的背景色
hIST    否    必须在PLTE之后且在IDAT之前    对每种颜色的使用频率进行大致的统计
tRNS    否    必须在PLTE之后且在IDAT之前    使用简单的（相对于使用Alpha通道来说）图像透明处理
pHYs    否    必须在IDAT之前    为了正确显示该图像，指定像素的大小、宽高比率
sPLT    是    必须在IDAT之前    如果目标显示设备不能显示图像的所有颜色，则推荐使用该调色板
tIME    否    无约束    记录该图像最后的修改时间
iTXt    是    无约束    用于携带文本信息（使用UTF-8编码）
tEXt    是    无约束    用于携带文本信息（Latin-1）
zTXt    是    无约束    用于携带压缩过的文本信息（Latin-1）

4．参考资源

官方网站：
[http://www.w3.org/](http://www.w3.org/)
[http://www.libpng.org/pub/png/](http://www.libpng.org/pub/png/)

推荐开源项目——CxImage：
[http://sourceforge.net/projects/cximage](http://sourceforge.net/projects/cximage)

