# RGB

[RGB色彩模式](https://baike.baidu.com/item/RGB%E8%89%B2%E5%BD%A9%E6%A8%A1%E5%BC%8F)是工业界的一种颜色标准，是通过对红(R)、绿(G)、蓝(B)三个颜色[通道](https://baike.baidu.com/item/%E9%80%9A%E9%81%93)的变化以及它们相互之间的叠加来得到各式各样的颜色的，RGB即是代表红、绿、蓝三个通道的颜色，这个标准几乎包括了人类视力所能感知的所有颜色，是目前运用最广的[颜色系统](https://baike.baidu.com/item/%E9%A2%9C%E8%89%B2%E7%B3%BB%E7%BB%9F/3177592)之一。



- 中文名

  RGB色彩模式

- 强度值

  0~255

- 图像颜色

  红、绿、蓝三色

- 每像素颜色

  16777216(256 * 256 * 256)种

- 原    理

  颜色发光

- 调色板RGB

  RGB1、RGB4、RGB8

## 目录

1. 1 [RGB色彩模式](https://baike.baidu.com/item/RGB/342517?fr=aladdin#1)
2. ▪ [应用](https://baike.baidu.com/item/RGB/342517?fr=aladdin#1_1)
3. ▪ [原理](https://baike.baidu.com/item/RGB/342517?fr=aladdin#1_2)
4. ▪ [RGB格式](https://baike.baidu.com/item/RGB/342517?fr=aladdin#1_3)

1. ▪ [信号获取](https://baike.baidu.com/item/RGB/342517?fr=aladdin#1_4)
2. ▪ [色彩空间](https://baike.baidu.com/item/RGB/342517?fr=aladdin#1_5)
3. ▪ [常见颜色](https://baike.baidu.com/item/RGB/342517?fr=aladdin#1_6)
4. ▪ [颜色选择器](https://baike.baidu.com/item/RGB/342517?fr=aladdin#1_7)
5. 2 [RGB宏](https://baike.baidu.com/item/RGB/342517?fr=aladdin#2)

1. 3 [RGB颜色函数](https://baike.baidu.com/item/RGB/342517?fr=aladdin#3)
2. ▪ [语法](https://baike.baidu.com/item/RGB/342517?fr=aladdin#3_1)
3. ▪ [函数说明](https://baike.baidu.com/item/RGB/342517?fr=aladdin#3_2)



## RGB色彩模式

编辑



### 应用

目前的显示器大都是采用了RGB颜色标准，

[![img](https://gss3.bdstatic.com/7Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=0737033009f79052eb1f403c3cf2d738/0dd7912397dda1448aa71c91b2b7d0a20cf48662.jpg)](https://baike.baidu.com/pic/RGB/342517/0/7aad4ae711426a3db9382083?fr=lemma&ct=single)

在显示器上，是通过

电子枪

打在屏幕的红、绿、蓝三色发光极上来产生色彩的，目前的电脑一般都能显示32位颜色，有一千万种以上的颜色。

电脑屏幕上的所有颜色，都由这红色绿色蓝色三种色光按照不同的比例[混合](https://baike.baidu.com/item/%E6%B7%B7%E5%90%88)而成的。一组红色绿色蓝色就是一个最小的显示单位。屏幕上的任何一个颜色都可以由一组RGB值来记录和表达。

因此这红色绿色蓝色又称为三原色光，用英文表示就是R(red)、G(green)、B(blue)。

在电脑中，RGB的所谓“多少”就是指亮度，并使用整数来表示。通常情况下，RGB各有256级亮度，用数字表示为从0、1、2...直到255。注意虽然数字最高是255，但0也是数值之一，因此共256级。如同2000年到2010年共是11年一样。

按照计算，256级的RGB色彩总共能组合出约1678万种色彩，即256×256×256=16777216。通常也被简称为1600万色或千万色。也称为24位色(2的24次方)。

在[led](https://baike.baidu.com/item/led)领域利用三合一点阵全彩技术， 即在一个发光单元里由RGB三色晶片组成全彩像素。随着这一技术的不断成熟，led显示技术会给人们带来更加丰富真实的色彩感受。 [1] 



### 原理

RGB是从颜色发光的原理来设计定的，通俗点说它的颜色混合方式就好像有红、绿、蓝三盏灯，当它们的光相互叠合的时候，色彩相混，而亮度却等于两者亮度之总和，越混合亮度越高，即加法混合。

[![img](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=db252f948c5494ee8322081b1df4e0e1/3ac79f3df8dcd100925a560a728b4710b8122fd2.jpg)](https://baike.baidu.com/pic/RGB/342517/0/ac2fc3c479211ad038db4933?fr=lemma&ct=single)

红、绿、蓝三盏灯的叠加情况，中心三色最亮的叠加区为白色，加法混合的特点：越叠加越明亮。

红、绿、蓝三个[颜色通道](https://baike.baidu.com/item/%E9%A2%9C%E8%89%B2%E9%80%9A%E9%81%93)每种色各分为256阶亮度，在0时“灯”最弱——是关掉的，而在255时“灯”最亮。当三色灰度数值相同时，产生不同灰度值的灰色调，即三色灰度都为0时，是最暗的黑色调；三色灰度都为255时，是最亮的白色调。

RGB 颜色称为加成色，因为您通过将 R、G 和 B 添加在一起（即所有光线反射回眼睛）可产生白色。加成色用于照明光、电视和计算机显示器。例如，显示器通过红色、绿色和蓝色荧光粉发射光线产生颜色。绝大多数可视光谱都可表示为红、绿、蓝 (RGB) 三色光在不同比例和强度上的混合。这些颜色若发生重叠，则产生青、[洋红](https://baike.baidu.com/item/%E6%B4%8B%E7%BA%A2)和黄。



### RGB格式

对一种颜色进行编码的方法统称为“[颜色空间](https://baike.baidu.com/item/%E9%A2%9C%E8%89%B2%E7%A9%BA%E9%97%B4)”或“[色域](https://baike.baidu.com/item/%E8%89%B2%E5%9F%9F)”。用最简单的话说，世界上任何一种颜色的“颜色空间”都可定义成一个固定的数字或变量。RGB（红、绿、蓝）只是众多颜色空间的一种。采用这种编码方法，每种颜色都可用三个变量来表示-红色绿色以及蓝色的强度。记录及显示彩色图像时，RGB是最常见的一种方案。但是，它缺乏与早期黑白显示系统的良好兼容性。因此，许多电子电器厂商普遍采用的做法是，将RGB转换成[YUV](https://baike.baidu.com/item/YUV)颜色空间，以维持兼容，再根据需要换回RGB格式，以便在电脑显示器上显示彩色图形。

**网页格式**

由于网页(WEB)是基于计算机浏览器开发的媒体，所以颜色以光学颜色RGB（红、绿、蓝）为主。网页颜色是以16进制代码表示，一般格式为#DEFABC （字母范围从A-F,数字从0-9 ）；如黑色，在[网页代码](https://baike.baidu.com/item/%E7%BD%91%E9%A1%B5%E4%BB%A3%E7%A0%81)中便是：#000000(在css编写中可简写为#000)。当颜色代码为#AABB11时，可以简写为#AB1表示，如#135与#113355表示同样的颜色。

RGB1、RGB4、RGB8都是[调色板](https://baike.baidu.com/item/%E8%B0%83%E8%89%B2%E6%9D%BF)类型的RGB格式，在描述这些媒体类型的格式细节时，通常会在BITMAPINFOHEADER数据结构后面跟着一个调色板（定义一系列颜色）。它们的图像数据并不是真正的颜色值，而是当前像素颜色值在调色板中的索引。以RGB1（2色[位图](https://baike.baidu.com/item/%E4%BD%8D%E5%9B%BE)）为例，比如它的调色板中定义的两种颜色值依次为0x000000（黑色）和0xFFFFFF（白色）…（每个像素用1位表示）表示对应各像素的颜色为：黑黑白白黑白黑白黑白白白…。

**RGB555**

RGB555是另一种16位的RGB格式，RGB分量都用5位表示（剩下的1位不用）。使用一个字读出一个像素后，这个字的各个位意义如下：

高字节 低字节

X R R R R R G G G G G B B B B B （X表示不用，可以忽略）

可以组合使用屏蔽字和移位操作来得到RGB各分量的值：

```
`#define RGB555_MASK_RED 0x7C00``#define RGB555_MASK_GREEN 0x03E0``#define RGB555_MASK_BLUE 0x001F``R = (wPixel & RGB555_MASK_RED) >> 10; ``// 取值范围0-31``G = (wPixel & RGB555_MASK_GREEN) >> 5; ``// 取值范围0-31``B = wPixel & RGB555_MASK_BLUE; ``// 取值范围0-31`
```

**RGB565**

RGB565使用16位表示一个像素，这16位中的5位用于R，6位用于G，5位用于B。程序中通常使用一个字（WORD，一个字等于两个字节）来操作一个像素。当读出一个像素后，这个字的各个位意义如下：

高字节 低字节

R R R R R G G G G G G B B B B B

可以组合使用屏蔽字和移位操作来得到RGB各分量的值：

```
`#define RGB565_MASK_RED 0xF800``#define RGB565_MASK_GREEN 0x07E0``#define RGB565_MASK_BLUE 0x001F``R = (wPixel & RGB565_MASK_RED) >> 11; ``// 取值范围0-31``G = (wPixel & RGB565_MASK_GREEN) >> 5; ``// 取值范围0-63``B = wPixel & RGB565_MASK_BLUE; ``// 取值范围0-31``#define RGB(r,g,b) (unsigned int)( (r|0x08 << 11) | (g|0x08 << 6) | b|0x08 )``#define RGB(r,g,b) (unsigned int)( (r|0x08 << 10) | (g|0x08 << 5) | b|0x08 )`
```

该代码可以解决24位与16位相互转换的问题

**RGB24**

RGB24使用24位来表示一个像素，RGB分量都用8位表示，取值范围为0-255。注意在内存中RGB各分量的排列顺序为：BGR BGR BGR…。通常可以使用RGBTRIPLE数据结构来操作一个像素，它的定义为：

```
`typedef` `struct` `tagRGBTRIPLE {``BYTE` `rgbtBlue; ``// 蓝色分量``BYTE` `rgbtGreen; ``// 绿色分量``BYTE` `rgbtRed; ``// 红色分量``} RGBTRIPLE;`
```

**RGB32**

RGB32使用32位来表示一个像素，RGB分量各用去8位，剩下的8位用作Alpha[通道](https://baike.baidu.com/item/%E9%80%9A%E9%81%93)或者不用。（ARGB32就是带Alpha通道的RGB24。）注意在内存中RGB各分量的排列顺序为：BGRA BGRA BGRA…。通常可以使用RGBQUAD数据结构来操作一个像素，它的定义为：

```
`typedef` `struct` `tagRGBQUAD {``BYTE` `rgbBlue; ``// 蓝色分量``BYTE` `rgbGreen; ``// 绿色分量``BYTE` `rgbRed; ``// 红色分量``BYTE` `rgbReserved; ``// 保留字节（用作Alpha通道或忽略）``} RGBQUAD。`
```



### 信号获取

**技术特点**

● 采集计算机VGA输出屏幕、各种非标准相机的输出采集设备、标准或非标的RGB[分量信号](https://baike.baidu.com/item/%E5%88%86%E9%87%8F%E4%BF%A1%E5%8F%B7)

● 采集的信号种类按照接口可为复合非标准[模拟信号](https://baike.baidu.com/item/%E6%A8%A1%E6%8B%9F%E4%BF%A1%E5%8F%B7)，绿路带同步的/行场分离的RGB分量信号

● 高分辨率高帧率：1280×1024/40帧；1024×768/60帧；800×600/120帧；

● 最高点频可达170M

● 支持硬件任意开窗，二级缩放，硬件翻转

● 有类似内存映射的功能，多个应用程序/进程可以共享其采集的图像数据；

● 信号接入丢失感知，无信号不蓝屏、[死机](https://baike.baidu.com/item/%E6%AD%BB%E6%9C%BA)

● 硬件控制帧率流量，可在实际使用中和其它[采集卡](https://baike.baidu.com/item/%E9%87%87%E9%9B%86%E5%8D%A1)配合，更有效提高PCI[带宽](https://baike.baidu.com/item/%E5%B8%A6%E5%AE%BD)的利用

● 支持RGB32、RGB24、YUV422、RGB8等采集格式

● 全自动行[场频](https://baike.baidu.com/item/%E5%9C%BA%E9%A2%91)检测：具有全自动行场频自适应能力和信号自检测能力，信源端信号的变化不需要用户调节，完全适合无人值守应用

●编程完全使用微软提供DirectShow /VFW接口，也可提供基于VC、VB、Delphi等的二次开发包演示程序和源代码，

●可使用微软的AmCap,VidCap,Windows Media Encode,Window Movie Maker、第三方提供的LabView等应用软件

**信号介绍**

VGA[采集卡](https://baike.baidu.com/item/%E9%87%87%E9%9B%86%E5%8D%A1)/RGB信号采集卡可采集VGA信号、标准和非标准RGB分量等信号源，适用于高精度、高分辨率的图像采集、高清VGA视频图像的存储、编码传输等要求。

**开发工具**

● 操作系统支持：Windows 2000、XP、Vista、7 linux unix等主流操作系统.

● SDK支持：VC、VB、Delphi，提供演示程序及演示程序源代码

● 驱动支持：DirectX、OpenCV、LabView、



### 色彩空间

RGB色彩空间根据实际使用设备系统能力的不同，有各种不同的实现方法。截至2006年，最常用的是24-位实现方法，也就是红绿蓝每个[通道](https://baike.baidu.com/item/%E9%80%9A%E9%81%93)有8位或者256色级。基于这样的24-位RGB 模型的色彩空间可以表现 256×256×256 ≈ 1670万色。一些实现方法采用每原色16位，能在相同范围内实现更高更精确的色彩密度。这在宽域色彩空间中尤其重要，因为大部分通常使用的颜色排列的相对更紧密。

印刷技术的当中的RGB色彩空间主要是指加色法当中的三度色彩空间，通过使用不同强度的三原色，红、绿、蓝色的光线来组合成不同的色彩，就好像说，如果平时我们利用扫描仪从印刷品上扫描图像，原理就是扫描仪阅读了图像上面的红、绿、蓝三色的光亮度，然后把这些量度转换成数据，当显示器收到这些数据的时候就可以按照程序设定转换成制定的红、绿、蓝三原色，其实他们当中是有很多不同颜色的小色块的，由于这些色块的像素非常非常的小而且密密麻麻的，所以我们眼睛没法分辨出来。



### 常见颜色

| 颜色名称 | 红色值 Red | 绿色值 Green | 蓝色值 Blue |
| -------- | ---------- | ------------ | ----------- |
| 黑色     | 0          | 0            | 0           |
| 蓝色     | 0          | 0            | 255         |
| 绿色     | 0          | 255          | 0           |
| 青色     | 0          | 255          | 255         |
| 红色     | 255        | 0            | 0           |
| 洋红色   | 255        | 0            | 255         |
| 黄色     | 255        | 255          | 0           |
| 白色     | 255        | 255          | 255         |

以上颜色为常用的基本颜色。 [2] 



### 颜色选择器

颜色选择器是指利用程序实现R、G、B三色的分配，来实现颜色的选择，利用颜色选择器可以选择出所有的颜色，总共有256*256*256种，同时可以实时的预览颜色，这就使得颜色的选择多种多样，并且非常方便。



## RGB宏

编辑

\#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

这是个带三个参数的宏，
　　首先将r,g,b强制转换成BYTE型，之后g左移8位，b左移16位，并把结果分别强制转换成[DWORD](https://baike.baidu.com/item/DWORD)型，最后将r,左移8位后的g，还有左移16位后的b三者做按位或，所得的结果强制转换成COLORREF类型。 [1] 

The**RGB**macro selects a red, green, blue (RGB) color based on the arguments supplied and the color capabilities of the output device.

COLORREF RGB( BYTE byRed, BYTE byGreen, BYTE byBlue );

The return value is the resultant RGB color as a[**COLORREF**](https://baike.baidu.com/item/COLORREF)value.

*byRed*

The intensity of the red color.

*byGreen*

The intensity of the green color.

*byBlue*

The intensity of the blue color.



## RGB颜色函数

编辑

RGB函数执行成功时返回由指定分量确定的颜色，用长整数表示。用于表示一个RGB（红绿蓝）颜色值.



### 语法

RGB (**Red**As Integer ,**Green**As Integer ,**Blue**As Integer )

| **部分**  | **描述**                                                     |
| --------- | ------------------------------------------------------------ |
| **red**   | 必要参数；**Integer类型**。数值范围从 0 到 255，表示颜色的红色成份。 |
| **green** | 必要参数；**Integer类型**。数值范围从 0 到 255，表示颜色的绿色成份。 |
| **blue**  | 必要参数；**Integer类型**。数值范围从 0 到 255，表示颜色的蓝色成份。 |

注意: 如果其中有一个参数的值超过 255 ，不会显示任何错误，但这个参数会被当做 255。



### 函数说明

可以接受颜色说明的应用程序的方法和属性期望这个说明是一个代表 RGB 颜色值的数值。一个 RGB 颜色值指定红、绿、蓝三原色的相对亮度，生成一个用于显示的特定颜色。

用法RGB()函数使用下述公式计算表示颜色的长整数：Red+ 256 * Green+65536 *Blue其中，Blue代表蓝色分量，Green代表绿色分量，Red代表红色分量。各分量中，数值越小，亮度越低，数值越大，亮度越高。

- 参考资料

  1.[  ](https://baike.baidu.com/item/RGB/342517?fr=aladdin#ref_[1]_17423)[RGB宏详解 ](https://baike.baidu.com/reference/342517/3c334G4hlkC5OXE3qDco5hEe0Q8ymcKXOfasZAjWm1tj0uwrL8VRuuU7qVkhd9ZhvyAesPIjSuupCqAVd75ySAkcpYFqC4wXGzmoFlILbErUMA) ．CSDN[引用日期2013-03-01]2.[  ](https://baike.baidu.com/item/RGB/342517?fr=aladdin#ref_[2]_17423)[在线颜色选择器 | RGB颜色查询对照表 - aTool在线工具 ](https://baike.baidu.com/reference/342517/b2facHjkMCa7akvbONQbyDEf9eieLma3rV3FftFBHorfAL3YklXLCMVhinC8D27a1A62xQvvrg9HSVxe9A3xzLs) ．aTool在线工具[引用日期2013-12-15]

词条标签：

科学 ， 学科