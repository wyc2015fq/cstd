# VC数字图像处理编程讲座之六 - 深之JohnChen的专栏 - CSDN博客

2005年11月30日 11:16:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：7579


图像文件格式

一般的图像文件结构主要都包含有文件头、文件体和文件尾等三部分

文件头： 软件ID、软件版本号、图像分辨率、图像尺寸、图像深度、彩色类型、编码方式、压缩算法 
文件体：图像数据、彩色变换表 
文件尾：用户名、注释、开发日期、工作时间 
以上是一个大概的图像文件结构说明，实际的结构根据不同的格式其中的条目要细得多，结构也复杂得多，各个条目所占空间及条目间的排列顺序也大不相同。目前还没有非常统一的图像文件格式。但大多数图像处理软件都与数种图像文件格式相兼容，也即可读取多种不同格式的图像文件。这样，不同的图像格式间可相互转换。当然，还有专门的图像格式转换软件，用于各种图像格式间的转换。

图像文件后缀一览表

文件格式是存储文本、图形或者图像数据的一种数据结构。在文字处理中，存储文本文件要使用文件格式。例如，使用微软公司的Word处理器编写的文件，可根据不同的应用环境用不同的格式存储。如果使用多信息文本格式(Rich Text Format，RTF)存储，这个文件就可在其他的平台(如Mac机)或者使用其他的字处理器进行处理。同样，存储图像也需要有存储格式，从20世纪70年代图像开始进入计算机以来，开发了许许多多的图像文件存储格式，而且互相不兼容，需要使用针对特定格式的处理软件。现在都意识到，不兼容的格式给用户造成很多的不便，因此有些格式也逐渐被淘汰。

在计算机中，有两种类型的图：矢量图(vector graphics)和位映象图(bitmapped graphics)。矢量图是用数学方法描述的一系列点、线、弧和其他几何形状，如图6-17(a)所示。因此存放这种图使用的格式称为矢量图格式，存储的数据主要是绘制图形的数学描述；位映象图(bitmapped graphics)也称光栅图(raster graphics)，这种图就像电视图像一样，由象点组成的，如图6-17(b)，因此存放这种图使用的格式称为位映象图格式，经常简称为位图格式，存储的数据是描述像素的数值。

![](http://www.dxrtvu.net/wlkj/dmtzzsyrj-xwd/mxzy/MultiMedia/multimedia/course/course/img/Imgc6265.gif)

图6-1 矢量图与位映象图

除了本章介绍的4种常用格式之外，在我们的工作中还会遇到其他图像格式。为方便查阅，现将部分图形与图像文件的后缀和名称列在表6-11和表6-12中。如果编写程序需要很专业的图像格式资源，包括一些源程序(source code)，可以访问站点：http://www.wotsit.org/，你可饱览多媒体世界中的各种媒体的存储格式。

表6-11位映象图格式/光栅图光栅(bitmapped formats / raster graphics)

|后缀|文件名称|后缀|文件名称|
|----|----|----|----|
|AG4|Access G4 document imaging|JFF|JPEG (JFIF)|
|ATT|AT&T Group IV|JPG|JPEG|
|BMP|Windows & OS/2|KFX|Kofax Group IV|
|CAL|CALS Group IV|MAC|MacPaint|
|CIT|Intergraph scanned images|MIL|Same as GP4 extension|
|CLP|Windows Clipboard|MSP|Microsoft Paint|
|CMP|Photomatrix G3/G4 scanner format|NIF|Navy Image File|
|CMP|LEAD Technologies|PBM|Portable bitmap|
|CPR|Knowledge Access|PCD|PhotoCD|
|CT|Scitex Continuous Tone|PCX|PC Paintbrush|
|CUT|Dr. Halo|PIX|Inset Systems (HiJaak)|
|DBX|DATABEAM|PNG|Portable Network Graphics|
|DX|Autotrol document imaging|PSD|Photoshop native format|
|ED6|EDMICS (U.S. DOD)|RAS|Sun|
|EPS|Encapsulated PostScript|RGB|SGI|
|FAX|Fax|RIA|Alpharel Group IV document imaging|
|FMV|FrameMaker|RLC|Image Systems|
|GED|Arts & Letters|RLE|Various RLE-compressed formats|
|GDF|IBM GDDM format|RNL|GTX Runlength|
|GIF|CompuServe|SBP|IBM StoryBoard|
|GP4|CALS Group IV - ITU Group IV|SGI|Silicon Graphics RGB|
|GX1|Show Partner|SUN|Sun|
|GX2|Show Partner|TGA|Targa|
|ICA|IBM IOCA (see MO:DCA)|TIF|TIFF|
|ICO|Windows icon|WPG|WordPerfect image|
|IFF|Amiga ILBM|XBM|X Window bitmap|
|IGF|Inset Systems (HiJaak)|XPM|X Window pixelmap|
|IMG|GEM Paint|XWD|X Window dump|

表6-12 矢量图格式(vector graphics formats)

|后缀|文件名称|后缀|文件名称|
|----|----|----|----|
|3DS|3D Studio|GEM|GEM proprietary|
|906|Calcomp plotter|G4|GTX RasterCAD - scanned imagesinto vectors for AutoCAD|
|AI|Adobe Illustrator|IGF|Inset Systems (HiJaak)|
|CAL|CALS subset of CGM|IGS|IGES|
|CDR|CorelDRAW|MCS|MathCAD|
|CGM|Computer Graphics Metafile|MET|OS/2 metafile|
|CH3|Harvard Graphics chart|MRK|Informative Graphics markup file|
|CLP|Windows clipboard|P10|Tektronix plotter (PLOT10)|
|CMX|Corel Metafile Exchange|PCL|HP LaserJet|
|DG|Autotrol|PCT|Macintosh PICT drawings|
|DGN|Intergraph drawing format|PDW|HiJaak|
|DRW|Micrografx Designer 2.x, 3.x|PGL|HP plotter|
|DS4|Micrografx Designer 4.x|PIC|Variety of picture formats|
|DSF|Micrografx Designer 6.x|PIX|Inset Systems (HiJaak)|
|DXF|AutoCAD|PLT|HPGL Plot File (HPGL2 has raster format)|
|DWG|AutoCAD|PS|PostScript Level 2|
|EMF|Enhanced metafile|RLC|Image Systems "CAD Overlay ESP"vector files overlaid onto raster images|
|EPS|Encapsulated PostScript|SSK|SmartSketch|
|ESI|Esri plot file (GIS mapping)|WMF|Windows Metafile|
|FMV|FrameMaker|WPG|WordPerfect graphics|
|GCA|IBM GOCA|WRL|VRML|
|||后缀|文件名称|
|||||

文件格式是存储文本、图形或者图像数据的一种数据结构。在文字处理中，存储文本文件要使用文件格式。例如，使用微软公司的Word处理器编写的文件，可根据不同的应用环境用不同的格式存储。如果使用多信息文本格式(Rich Text Format，RTF)存储，这个文件就可在其他的平台(如Mac机)或者使用其他的字处理器进行处理。同样，存储图像也需要有存储格式，从20世纪70年代图像开始进入计算机以来，开发了许许多多的图像文件存储格式，而且互相不兼容，需要使用针对特定格式的处理软件。现在都意识到，不兼容的格式给用户造成很多的不便，因此有些格式也逐渐被淘汰。

在计算机中，有两种类型的图：矢量图(vector graphics)和位映象图(bitmapped graphics)。矢量图是用数学方法描述的一系列点、线、弧和其他几何形状，如图6-17(a)所示。因此存放这种图使用的格式称为矢量图格式，存储的数据主要是绘制图形的数学描述；位映象图(bitmapped graphics)也称光栅图(raster graphics)，这种图就像电视图像一样，由象点组成的，如图6-17(b)，因此存放这种图使用的格式称为位映象图格式，经常简称为位图格式，存储的数据是描述像素的数值。

![](http://www.dxrtvu.net/wlkj/dmtzzsyrj-xwd/mxzy/MultiMedia/multimedia/course/course/img/Imgc6265.gif)

图6-1 矢量图与位映象图

除了本章介绍的4种常用格式之外，在我们的工作中还会遇到其他图像格式。为方便查阅，现将部分图形与图像文件的后缀和名称列在表6-11和表6-12中。如果编写程序需要很专业的图像格式资源，包括一些源程序(source code)，可以访问站点：http://www.wotsit.org/，你可饱览多媒体世界中的各种媒体的存储格式。

表6-11位映象图格式/光栅图光栅(bitmapped formats / raster graphics)

|后缀|文件名称|后缀|文件名称|
|----|----|----|----|
|AG4|Access G4 document imaging|JFF|JPEG (JFIF)|
|ATT|AT&T Group IV|JPG|JPEG|
|BMP|Windows & OS/2|KFX|Kofax Group IV|
|CAL|CALS Group IV|MAC|MacPaint|
|CIT|Intergraph scanned images|MIL|Same as GP4 extension|
|CLP|Windows Clipboard|MSP|Microsoft Paint|
|CMP|Photomatrix G3/G4 scanner format|NIF|Navy Image File|
|CMP|LEAD Technologies|PBM|Portable bitmap|
|CPR|Knowledge Access|PCD|PhotoCD|
|CT|Scitex Continuous Tone|PCX|PC Paintbrush|
|CUT|Dr. Halo|PIX|Inset Systems (HiJaak)|
|DBX|DATABEAM|PNG|Portable Network Graphics|
|DX|Autotrol document imaging|PSD|Photoshop native format|
|ED6|EDMICS (U.S. DOD)|RAS|Sun|
|EPS|Encapsulated PostScript|RGB|SGI|
|FAX|Fax|RIA|Alpharel Group IV document imaging|
|FMV|FrameMaker|RLC|Image Systems|
|GED|Arts & Letters|RLE|Various RLE-compressed formats|
|GDF|IBM GDDM format|RNL|GTX Runlength|
|GIF|CompuServe|SBP|IBM StoryBoard|
|GP4|CALS Group IV - ITU Group IV|SGI|Silicon Graphics RGB|
|GX1|Show Partner|SUN|Sun|
|GX2|Show Partner|TGA|Targa|
|ICA|IBM IOCA (see MO:DCA)|TIF|TIFF|
|ICO|Windows icon|WPG|WordPerfect image|
|IFF|Amiga ILBM|XBM|X Window bitmap|
|IGF|Inset Systems (HiJaak)|XPM|X Window pixelmap|
|IMG|GEM Paint|XWD|X Window dump|

表6-12 矢量图格式(vector graphics formats)

|后缀|文件名称|后缀|文件名称|
|----|----|----|----|
|3DS|3D Studio|GEM|GEM proprietary|
|906|Calcomp plotter|G4|GTX RasterCAD - scanned imagesinto vectors for AutoCAD|
|AI|Adobe Illustrator|IGF|Inset Systems (HiJaak)|
|CAL|CALS subset of CGM|IGS|IGES|
|CDR|CorelDRAW|MCS|MathCAD|
|CGM|Computer Graphics Metafile|MET|OS/2 metafile|
|CH3|Harvard Graphics chart|MRK|Informative Graphics markup file|
|CLP|Windows clipboard|P10|Tektronix plotter (PLOT10)|
|CMX|Corel Metafile Exchange|PCL|HP LaserJet|
|DG|Autotrol|PCT|Macintosh PICT drawings|
|DGN|Intergraph drawing format|PDW|HiJaak|
|DRW|Micrografx Designer 2.x, 3.x|PGL|HP plotter|
|DS4|Micrografx Designer 4.x|PIC|Variety of picture formats|
|DSF|Micrografx Designer 6.x|PIX|Inset Systems (HiJaak)|
|DXF|AutoCAD|PLT|HPGL Plot File (HPGL2 has raster format)|
|DWG|AutoCAD|PS|PostScript Level 2|
|EMF|Enhanced metafile|RLC|Image Systems "CAD Overlay ESP"vector files overlaid onto raster images|
|EPS|Encapsulated PostScript|SSK|SmartSketch|
|ESI|Esri plot file (GIS mapping)|WMF|Windows Metafile|
|FMV|FrameMaker|WPG|WordPerfect graphics|
|GCA|IBM GOCA|WRL|VRML|
|||后缀|文件名称|
|||||

几种常见的文件格式：
1>JPEG格式

微处理机中的存放顺序有正序(big endian)和逆序(little endian)之分。正序存放就是高字节存放在前低字节在后，而逆序存放就是低字节在前高字节在后。例如，十六进制数为A02B，正序存放就是A02B，逆序存放就是2BA0。摩托罗拉(Motorola)公司的微处理器使用正序存放，而英特尔(Intel)公司的微处理器使用逆序。JPEG文件中的字节是按照正序排列的。

JPEG委员会在制定JPEG标准时，定义了许多标记(marker)用来区分和识别图像数据及其相关信息，但笔者没有找到JPEG委员会对JPEG文件交换格式的明确定义。直到1998年12月从分析网上具体的JPG图像来看，使用比较广泛的还是JPEG文件交换格式(JPEG File Interchange Format，JFIF)版本号为1.02。这是1992年9月由在C-Cube Microsystems公司工作的Eric Hamilton提出的。此外还有TIFF JPEG等格式，但由于这种格式比较复杂，因此大多数应用程序都支持JFIF文件交换格式。 

JPEG文件使用的颜色空间是CCIR 601推荐标准进行的彩色空间(参看第7章)。在这个彩色空间中，每个分量、每个像素的电平规定为255级，用8位代码表示。从RGB转换成YCbCr空间时，使用下面的精确的转换关系：

　　　　　　 Y = 256 * E'y

　　　　　 Cb = 256 * [E'Cb] + 128

　　　　　 Cr = 256 * [E'Cr] + 128
其中亮度电平E'y和色差电平E'Cb和E'Cb分别是CCIR 601定义的参数。由于E'y的范围是0～1，E'Cb和E'Cb的范围是-0.5～+0.5，因此Y, Cb和Cr的最大值必须要箝到255。于是RGB和YCbCr之间的转换关系需要按照下面的方法计算。

(1) 从RGB转换成YCbCr

YCbCr(256级)分量可直接从用8位表示的RGB分量计算得到：

　 　　　　 Y =　　 0.299 R + 0.587　G　 + 0.114 B

　　 　 Cb = - 0.1687R - 0.3313G　 + 0.5　　　B + 128

　　　 Cr = 0.5 R - 0.4187G - 0.0813 B + 128

需要注意的是不是所有图像文件格式都按照R0，G0，B0，…… Rn，Gn，Bn的次序存储样本数据，因此在RGB文件转换成JFIF文件时需要首先验证RGB的次序。

(2) 从YCbCr转换成RGB 

RGB分量可直接从YCbCr(256级)分量计算得到：

　　　　 R = Y　　　　　　　　　　　　　　　　 + 1.402 (Cr-128)

　　　　　 G = Y - 0.34414 (Cb-128) - 0.71414 (Cr-128)

　　　　　 B = Y + 1.772 (Cb-128)

在JFIF文件格式中，图像样本的存放顺序是从左到右和从上到下。这就是说JFIF文件中的第一个图像样本是图像左上角的样本。
**文件结构文件结构**

JPEG文件使用的颜色空间是CCIR 601推荐标准进行的彩色空间(参看第7章)。在这个彩色空间中，每个分量、每个像素的电平规定为255级，用8位代码表示。从RGB转换成YCbCr空间时，使用下面的精确的转换关系：

　　　　　　 Y = 256 * E'y

　　　　　 Cb = 256 * [E'Cb] + 128

　　　　　 Cr = 256 * [E'Cr] + 128
其中亮度电平E'y和色差电平E'Cb和E'Cb分别是CCIR 601定义的参数。由于E'y的范围是0～1，E'Cb和E'Cb的范围是-0.5～+0.5，因此Y, Cb和Cr的最大值必须要箝到255。于是RGB和YCbCr之间的转换关系需要按照下面的方法计算。

(1) 从RGB转换成YCbCr

YCbCr(256级)分量可直接从用8位表示的RGB分量计算得到：

　 　　　　 Y =　　 0.299 R + 0.587　G　 + 0.114 B

　　 　 Cb = - 0.1687R - 0.3313G　 + 0.5　　　B + 128

　　　 Cr = 0.5 R - 0.4187G - 0.0813 B + 128

需要注意的是不是所有图像文件格式都按照R0，G0，B0，…… Rn，Gn，Bn的次序存储样本数据，因此在RGB文件转换成JFIF文件时需要首先验证RGB的次序。

(2) 从YCbCr转换成RGB 

RGB分量可直接从YCbCr(256级)分量计算得到：

　　　　 R = Y　　　　　　　　　　　　　　　　 + 1.402 (Cr-128)

　　　　　 G = Y - 0.34414 (Cb-128) - 0.71414 (Cr-128)

　　　　　 B = Y + 1.772 (Cb-128)

在JFIF文件格式中，图像样本的存放顺序是从左到右和从上到下。这就是说JFIF文件中的第一个图像样本是图像左上角的样本。
**文件结构文件结构**
JFIF文件格式直接使用JPEG标准为应用程序定义的许多标记，因此JFIF格式成了事实上JPEG文件交换格式标准。JPEG的每个标记都是由2个字节组成，其前一个字节是固定值0xFF。每个标记之前还可以添加数目不限的0xFF填充字节(fill byte)。下面是其中的8个标记：
- 
SOI　 0xD8　　　　　　　　　　　 图像开始 

- 
APP0 0xE0　　　　　　　　　　　 JFIF应用数据块 

- 
APPn 0xE1 - 0xEF　　　 其他的应用数据块(n, 1～15) 

- 
DQT　 0xDB　　　　　　　　　　　量化表 

- 
SOF0 0xC0　　　　　　　　　　　 帧开始 

- 
DHT　 0xC4　　　　　　　　　　　霍夫曼(Huffman)表 

- 
SOS　 0xDA　　　　　　　　　　　扫描线开始 

- 
EOI　 0xD9　　　　　　　　　　　 图像结束

为使读者对JPEG定义的标记一目了然，现将JPEG的标记码列于表6-05，并保留英文解释。
**表6-05 JPEG定义的标记**

|**Symbol (符号)**|**Code Assignment(标记代码)**|**Description(说明)**|
|----|----|----|
|**Start Of Frame markers, non-hierarchical Huffman coding**| | |
|SOF0|0xFFC0|Baseline DCT|
|SOF1|0xFFC1|Extended sequential DCT|
|SOF2|0xFFC2|Progressive DCT|
|SOF3|0xFFC3|Spatial (sequential) lossless|
|**Start Of Frame markers, hierarchical Huffman coding**| | |
|SOF5|0xFFC5|Differential sequential DCT|
|SOF6|0xFFC6|Differential progressive DCT|
|SOF7|0xFFC7|Differential spatial lossless|
|**Start Of Frame markers, non-hierarchical arithmetic coding**| | |
|JPG|0xFFC8|Reserved for JPEG extensions|
|SOF9|0xFFC9|Extended sequential DCT|
|SOF10|0xFFCA|Progressive DCT|
|SOF11|0xFFCB|Spatial (sequential) Lossless|
|**Start Of Frame markers, hierarchical arithmetic coding**| | |
|SOF13|0xFFCD|Differential sequential DCT|
|SOF14|0xFFCE|Differential progressive DCT|
|SOF15|0xFFCF|Differential spatial Lossless|
|**Huffman table specification**| | |
|DHT|0xFFC4|Define Huffman table(s)|
|**arithmetic coding conditioning specification**| | |
|DAC|0xFFCC|Define arithmetic conditioning table|
|**Restart interval termination**| | |
|RSTm|0xFFD0～0xFFD7|Restart with modulo 8 counter m|
|**Other marker**| | |
|SOI|0xFFD8|Start of image|
|EOI|0xFFD9|End of image|
|SOS|0xFFDA|Start of scan|
|DQT|0xFFDB|Define quantization table(s)|
|DNL|0xFFDC|Define number of lines|
|DRI|0xFFDD|Define restart interval|
|DHP|0xFFDE|Define hierarchical progression|
|EXP|0xFFDF|Expand reference image(s)|
|APPn|0xFFE0～0xFFEF|Reserved for application use|
|JPGn|0xFFF0～0xFFFD|Reserved for JPEG extension|
|COM|0xFFFE|Comment|
|**Reserved markers**| | |
|TEM|0xFF01|For temporary use in arithmetic coding|
|RES|0xFF02～0xFFBF|Reserved|

JPEG文件由下面的8个部分组成：

(1) 图像开始SOI(Start of Image)标记

(2) APP0标记(Marker)

① APP0长度(length)

② 标识符(identifier)

③ 版本号(version)

④ X和Y的密度单位(units=0：无单位；units=1：点数/英寸；units=2：点数/厘米)

⑤ X方向像素密度(X density)

⑥ Y方向像素密度(Y density)

⑦ 缩略图水平像素数目(thumbnail horizontal pixels)

⑧ 缩略图垂直像素数目(thumbnail vertical pixels)

⑨ 缩略图RGB位图(thumbnail RGB bitmap)

(3) APPn标记(Markers)，其中n=1～15(任选)

① APPn长度(length)

② 由于详细信息(application specific information)

(4) 一个或者多个量化表DQT(difine quantization table)

① 量化表长度(quantization table length)

② 量化表数目(quantization table number)

③ 量化表(quantization table)

(5) 帧图像开始SOF0(Start of Frame)

① 帧开始长度(start of frame length)

② 精度(precision)，每个颜色分量每个像素的位数(bits per pixel per color component)

③ 图像高度(image height)

④ 图像宽度(image width)

⑤ 颜色分量数(number of color components)

⑥ 对每个颜色分量(for each component)
- 
ID 

- 
垂直方向的样本因子(vertical sample factor) 

- 
水平方向的样本因子(horizontal sample factor) 

- 
量化表号(quantization table#)

(6) 一个或者多个霍夫曼表DHT(Difine Huffman Table)

① 霍夫曼表的长度(Huffman table length)

② 类型、AC或者DC(Type, AC or DC)

③ 索引(Index)

④ 位表(bits table)

⑤ 值表(value table)

(7) 扫描开始SOS(Start of Scan)

① 扫描开始长度(start of scan length)

② 颜色分量数(number of color components)

③ 每个颜色分量
- 
ID 

- 
交流系数表号(AC table #) 

- 
直流系数表号(DC table #)

④ 压缩图像数据(compressed image data)

(8) 图像结束EOI(End of Image)

表6-06表示了APP0域的详细结构。有兴趣的读者可通过UltraEdit或者PC TOOLS等工具软件打开一个JPG图像文件，对APP0的结构进行分析和验证。
**表6-06 JFIF格式中APP0域的详细结构**

|**偏移**|**长度**|**内容**|**块的名称**|**说明**|
|----|----|----|----|----|
|0|2 byte|0xFFD8|(Start of Image,SOI)|图像开始|
|2|2 byte|0xFFE0|APP0(JFIF application segment)|JFIF应用数据块|
|4|2 bytes||length of APP0 block|APP0块的长度|
|6|5 bytes||"JFIF"+"0"|识别APP0标记|
|11|1 byte||<Major version>|主要版本号(如版本1.02中的1)|
|12|1 byte||<Minor version>|次要版本号(如版本1.02中的02)|
|13|1 byte||<Units for the Xand Y densities>|X和Y的密度单位units=0：无单位units=1：点数/英寸units=2：点数/厘米|
|14|2 bytes||<Xdensity>|水平方向像素密度|
|16|2 bytes||<Ydensity>|垂直方向像素密度|
|18|1 byte||<Xthumbnail>|缩略图水平像素数目|
|19|1 byte||<Ythumbnail>|缩略图垂直像素数目|
||3n||< Thumbnail RGB bitmap>|缩略RGB位图(n为缩略图的像素数)|
||||Optional JFIF extension APP0 marker segment(s)|任选的JFIF扩展APP0标记段|
||……||……||
||2 byte|0xFFD9|(EOI) end-of-file|图像文件结束标记|

2>GIF文件格式

GIF(**G**raphics **I**nterchange **F**ormat)是CompuServe公司开发的图像文件存储格式，1987年开发的GIF文件格式版本号是GIF87a，1989年进行了扩充，扩充后的版本号定义为GIF89a。

GFI图像文件以数据块(block)为单位来存储图像的相关信息。一个GIF文件由表示图形/图像的数据块、数据子块以及显示图形/图像的控制信息块组成，称为GIF数据流(Data Stream)。数据流中的所有控制信息块和数据块都必须在文件头(Header)和文件结束块(Trailer)之间。

GIF文件格式采用了LZW(**L**empel-**Z**iv **W**alch)压缩算法来存储图像数据，定义了允许用户为图像设置背景的透明(transparency)属性。此外，GIF文件格式可在一个文件中存放多幅彩色图形/图像。如果在GIF文件中存放有多幅图，它们可以像演幻灯片那样显示或者像动画那样演示。
** 文件结构**
GIF文件结构的典型结构如图6-01所示。为下文说明方便，在构件左边加了编号。

|1|Header||GIF文件头||
|----|----|----|----|----|
|2|Logical Screen Descriptor||逻辑屏幕描述块||
|3|Global Color Table||全局彩色表||
||… 扩展模块(任选) …||||
|4|Image Descriptor||图形描述块||
|5|Local Color Table||局部彩色表(可重复n次)|可|
|6|Table Based Image Data||表式压缩图像数据|重|
|7|Graphic Control Extension||图像控制扩展块|复|
|8|Plain Text Extension||无格式文本扩展块|n|
|9|Comment Extension||注释扩展块|个|
|10|Applicaton Extension||应用程序扩展块||
||… 扩展模块(任选) …||||
|11|GIF Trailer||GIF文件结束块||

图6-01 GIF文件结构

数据块可分成3类：控制块(Control Block)，图形描绘块(Graphic-Rendering Block)和专用块(Special Purpose Block)。
**(1) 控制块：控制块包含有用来控制数据流(Data Stream)或者设置硬件参数的信息，其成员包括：**控制块包含有用来控制数据流(Data Stream)或者设置硬件参数的信息，其成员包括：- 
GIF文件头(Header) 

- 
逻辑屏幕描述块(Logical Screen Descriptor) 

- 
图形控制扩展块(Graphic Control Extension) 

- 
文件结束块(Trailer)

**(2) 图形描绘块：包含有用来描绘在显示设备上显示图形的信息和数据，其成员包括：**包含有用来描绘在显示设备上显示图形的信息和数据，其成员包括：- 
图像描述块(Image Descriptor) 

- 
无格式文本扩展块(Plain Text Extension)

**(3) 特殊用途数据块；包含有与图像处理无关的信息，其成员包括：**包含有与图像处理无关的信息，其成员包括：- 
注释扩展块(Comment Extension) 

- 
应用扩展块(Application Extension)

除了在控制块中的逻辑屏幕描述块(Logical Screen Descriptor)和全局彩色表(Global Color Table)的作用范围是整个数据流(Data Stream)之外, 所有其他控制块仅控制跟在它们后面的图形描绘块。
** 构件详解1. GIF文件头**** 构件详解1. GIF文件头**
文件头描述块(Header)定义GIF数据流(GIF Data Stream)，它的结构如图6-02所示。文件头描述块(Header)由GIF标记域(Signature)和版本号(Version)域组成，是一个由6个固定字节组成的数据块，它们用来说明使用的文件格式是GIF格式及当前所用的版本号。GIF标记域(Signature)存放的是“GIF”，版本号域存放的是1987年5月发布的“87a”或者1989年7月发布的“89a”，或者更加新的版本号。

|7|6|5|4|3|2|1|0|字节号|域的名称|数据类型|
|----|----|----|----|----|----|----|----|----|----|----|
||0||| | | | | | | |
|Signature|1|GIF标记|3 Bytes| | | | | | | |
||2||| | | | | | | |
||3||| | | | | | | |
|Version|4|版本号|3 Bytes| | | | | | | |
||5||| | | | | | | |

图6-02 标记/版本数据块的结构

**2. 逻辑屏幕描述块**
逻辑屏幕描述块(Logical Screen Descriptor)包含定义图像显示区域的参数，包括背景颜色信息。这个数据块中的坐标相对于虚拟屏幕的左上角，不一定是指显示屏的绝对坐标，这就意味可以参照窗口软件环境下的窗口坐标或者打印机坐标来设计图像显示程序。逻辑屏幕描述块的结构如图6-03所示：

|7|6|5|4|3|2|1|0|字节号|域的名称|类型|
|----|----|----|----|----|----|----|----|----|----|----|
|Logical Screen Width|0|逻辑屏幕宽度|Unsigned| | | | | | | |
||1|(以像素为定单位)|| | | | | | | |
|Logical Screen Height|2|逻辑屏幕高度|Unsigned| | | | | | | |
||3|(以像素为定单位)|| | | | | | | |
|G|CR|S|Size|4|包装域|见图6-04| | | | |
|Background Color Index|5|背景颜色索引|Byte| | | | | | | |
|Pixel Aspect Ratio|6|像素宽高比|Byte| | | | | | | |

图6-03 屏幕描述块的结构

逻辑描述块包含7个字节。字节0和字节1用来说明逻辑显示屏的宽度，字节3和字节4用来说明逻辑显示屏的高度，字节4用来描述彩色表的属性，字节5用来指定背景颜色索引，字节6用来计算像素的宽高比。现作如下说明：

(1) 屏幕描述块中的第5个字节称为包装域(Packed Fields)，它的位结构如图6-04所示，它由4个子域组成：

① 全局彩色表标志(Global Color Table Flag )域G用来说明是否有全局彩色表存在。如果G＝1，表示有一个全局彩色表(Global Color Table)将紧跟在这个逻辑屏幕描述块(Logical Screen Descriptor)之后；这个标志也用来选择背景颜色索引(Background Color Index)。如果G＝1，背景颜色索引(Background Color Index)域中的值就用作背景颜色的索引。

② 彩色分辨率(Color Resolution)域CR用来表示原始图像可用的每种基色的位数(实际值减1)。这个位数表示整个调色板的大小，而不是这幅图像使用的实际的颜色数。例如，如果该域的值CR＝3，说明原始图像可用每个基色有4位的调色板来生成彩色图像。

③ 彩色表排序标志(Sort Flag)域S用来表示全局彩色表(Global Color Table)中的颜色是否按重要性(或者称使用率)排序。如果S＝0，表示没有重要性排序；如果S＝1表示最重要的颜色排在前。这样做的目的是辅助颜色数比较少的解码器能够选择最好的颜色子集，在这种情况下解码器就可选择彩色表中开始段的彩色来显示图像。

④ 全局彩色表大小(Size of Global Color Table)域Size表示表示每个像素的位数，它用来计算全局彩色表(Global Color Table)中包含的字节数。在全局彩色表标志(Global Color Table Flag)域G＝0时就不需要计算，G＝1时就要计算彩色表的大小，具体计算见下文的**“3. 全局彩色表”**。

|7|6|5|4|3|2|1|0|
|----|----|----|----|----|----|----|----|
|Global Color Table Flag|Color Resolution|Sort Flag|Size of Global Color Table| | | | |

图6-04 逻辑屏幕描述块中的包装域结构

(2) 屏幕描述块中的第6个字节是背景颜色索引(Background Color Index)，它是彩色表的一个索引值，用来指定背景颜色。如果全局彩色表标志(Global Color Table Flag)域G＝0，这个域的值也设置为0。

(3) 像素宽高比(Pixel Aspect Ratio)域中的值是一个因数，是计算原始图像像素的宽高比的一个近似值。如果该域的值范围为1～255，如果不等于0，宽高比的近似值按下式计算：

Aspect Ratio = (Pixel Aspect Ratio + 15) / 64
像素宽高比(Pixel Aspect Ratio)定义成像素的宽度与高度之比，比值的范围在4:1～1:4之间，其增量为1/64。

**3. 全局彩色表**
由于一个GIF文件可以包含多幅彩色图像，每幅彩色图像也许都包含适合自身特点的彩色表，所以一个GIF文件可以有好几个彩色表。但归纳起来只有两类：全局彩色表(Global Color Table)或局部彩色表(Local Color Table)。全局彩色表可用于图像本身没有带彩色表的所有图像和无格式文本扩展块(Plain Text Extension)，而局部彩色表只用于紧跟在它后面的一幅图像。在处理全局彩色表和局部彩色表时需要注意下面一些规则。

① 如果GIF文件包含全局彩色表(Global Color Table)，而且要显示的图像本身又带有局部彩色表，那末显示该幅彩色图像时就用它自己的彩色表，而不用全局彩色表。在这种情况下，解码器就首先保存全局彩色表(Global Color Table)，然后使用局部彩色表(Local Color Table)来显示图像，最后再回复全局彩色表(Global Color Table)。

② 全局彩色表(Global Color Table)和局部彩色表(Local Color Table)都是可选择的。由于这个原因，解码器最好要保存全局彩色表(Global Color Table)，一直到出现另一个全局彩色表(Global Color Table)为止。这样做之后，对于包含完全没有彩色表的一幅或者多幅彩色图像的GIF文件就可以使用最后保存的全局彩色表(Global Color Table)进行处理。

③ 如果同类型的图像能够使用相同的彩色表来显示，编码器就要尽可能使用一个全局彩色表(Global Color Table)；如果没有彩色表可用，解码器就可以使用计算机系统提供的彩色表或者解码器自身的彩色表。

④ 全局彩色表(Global Color Table)存在与否由逻辑屏幕描述块(Logical Screen Descriptor)中字节5的全局彩色表标志(Global Color Table Flag )域G的值确定。如果存在，彩色表就紧跟在逻辑屏幕描述块(Logical Screen Descriptor)之后。彩色表的表项数目等于2(*n *+1)，其中*n*=b2b1b0，每个表项由3个字节组成，分别代表R、G、B的相对强度，因此彩色表的字节数就等于3×2(*n *+1)。彩色表的结构如图6-05所示。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|red intensity|0|红色索引 000|Byte|
|green intensity|1|绿色索引 000|Byte|
|blue intensity|2|蓝色索引 000|Byte|
|red intensity|3|红色索引 001|Byte|
|green intensity|4|绿色索引 001|Byte|
|blue intensity|5|蓝色索引 001|Byte|
|…|…|…||
|…|…|…||
|red intensity|745|红色索引 255|Byte|
|green intensity|746|绿色索引 255|Byte|
|blue intensity|767|蓝色索引 255|Byte|

**图6-05 彩色表结构**
局部彩色表与全局彩色表有相同的存储格式。
**4. 图像描述块**
GIF图像文件格式可包含数量不限的图像，而且也没有一个固定的存放顺序，仅用一个字节的图像分隔符(Image Separator)来判断是不是图像描述块。每一幅图像都由一个图像描述块(Image Descriptor)、可有可无的局部彩色表(Local Color Table)和图像数据组成。每幅图像必须要落在逻辑屏幕描述块(Logical Screen Descriptor)中定义的逻辑屏(Logical Screen)尺寸范围里。

图像描述块(Image Descriptor)之前可以有一个或者多个控制块，例如图形控制扩展块(Graphic Control Extension)，其后可以跟着一个局部彩色表(Local Color Table)。无论前后是否有各种数据块，图像描述块(Image Descriptor)总是带有图像数据。

图像描述块(Image Descriptor)的结构如图6-06所示。

|7|6|5|4|3|2|1|0|字节号|域的名称|类型|
|----|----|----|----|----|----|----|----|----|----|----|
|Image Separator|0|图像分隔符|Byte| | | | | | | |
|Image Left Position|1|图像左边位置|Unsigned| | | | | | | |
||2|(以像素为定单位)|| | | | | | | |
|Image Top Position|3|图像顶部位置|Unsigned| | | | | | | |
||4|(以像素为定单位)|| | | | | | | |
|Image Width|5|图像宽度|Unsigned| | | | | | | |
||6|(以像素为定单位)|| | | | | | | |
|Image Height|7|图像高度|Unsigned| | | | | | | |
||8|(以像素为定单位)|| | | | | | | |
||9|包装域|见图6-07| | | | | | | |

图6-06 图像描述块的结构

在图6-06中，图像分隔符(Image Separator)用来标识图像描述块的开始，该域包含固定的值：0x2C；图像左边位置(Image Left Position)是相对于逻辑屏幕(Logical Screen)最左边的列号，逻辑屏幕最左边的列好定义为0；图像顶部位置(Image Top Position) 是相对于逻辑屏幕(Logical Screen)顶部的行号，逻辑屏幕顶部的行号定义为0。

|7|6|5|4|3|2|1|0|
|----|----|----|----|----|----|----|----|
|Local Color Table Flag|Interlace Flag|Sort Flag|Reserved|Size of Local Color Table| | | |

图6-07 图像描述块中的包装域结构

图像描述块(Image Descriptor)中的第9个字节称为包装域(Packed Fields)字节，它的位结构如图6-07所示，它由5个子域组成：

① 局部彩色表标志(Local Color Table Flag )域L用来说明是否有局部彩色表存在。如果L＝1，表示有一个局部彩色表(Local Color Table)将紧跟在这个图像描述块(Image Descriptor)之后;如果G＝0，表示图像描述块(Image Descriptor)后面没有局部彩色表(Local Color Table)，该图像要使用全局彩色表(Global Color Table)。

② 交插显示标志(Interlace Flag)域I用来表示该图像是不是交插图像(Interlaced Images)。如果I＝0，表示该图像不是交插图像，如果I＝1表示该图像是交插图像。使用该位标志可知道图像数据是如何存放的。GIF文件格式定义了两种数据存储方式：一种是按图像行连续顺序存储，这个顺序与显示器上显示行的顺序相同；另一种按交插方式存储。交插图像按行分成如下所示的4组(Group)：

Group 1：每隔8行组成一组，从第0行开始显示 /第1遍交插

Group 2：每隔8行组成一组，从第4行开始显示 /第2遍交插

Group 3：每隔4行组成一组，从第2行开始显示 /第3遍交插

Group 4：每隔2行组成一组，从第1行开始显示 /第4遍交插

由于显示图像需要较长的时间，使用这种方法存放和显示图像数据，人们就可以在图像显示完成之前看到这幅图像的概貌，而不觉得显示时间长。图6-08说明了这种交插图像的存储和显示顺序。

|行号|像 点|交插遍次| | | |
|----|----|----|----|----|----|
|0|……………………………………|1||||
|1|……………………………………||||4|
|2|……………………………………|||3||
|3|……………………………………||||4|
|4|……………………………………||2|||
|5|……………………………………||||4|
|6|……………………………………|||3||
|7|……………………………………||||4|
|8|……………………………………|1||||
|9|……………………………………||||4|
|10|……………………………………|||3||
|11|……………………………………||||4|
|12|……………………………………||2|||
|13|……………………………………||||4|
|14|……………………………………|||3||
|15|……………………………………||||4|
|16|……………………………………|1||||
|17|……………………………………||||4|
|18|……………………………………|||3||
|19|……………………………………||||4|

图6-08 交插图像显示顺序

③ 彩色表排序标志(Sort Flag)域的含义与全局彩色表(Global Color Table)中(Sort Flag)域的含义相同。

④ 保留(Reserved)。

⑤ 局部彩色表大小(Size of Local Color Table)域的值用来计算局部彩色表(Global Color Table)中包含的字节数。
**5. 局部彩色表**
局部彩色表(Local Color Table)用于紧跟在它后面的图像。彩色表是否存在取决于图像描述块(Image Descriptor)中局部彩色表标志(Local Color Table Flag)位的设置。彩色表的结构和大小与全局彩色表(Global Color Table)完全相同。
**6. 表基图像数据**
GIF图像采用了LZW算法对实际的图像数据进行压缩。为了提高压缩编码的效率，对LZW编码器输出的代码采用可变长度码VLC(**v**ariable-length-**c**ode)，不是用位数高度的代码来表示输出，而且代表码字的位数是可变的。

表基图像数据(Table Based Image Data)由LZW最小代码长度(LZW Minimum Code Size)和图像数据(Image Data)组成，如图6-09所示。LZW最小代码长度域的值用来确定图像数据中LZW代码使用的初始位数。图像数据(Image Data)由数据子块(Data Sub-blocks)序列组成。

|7|6|5|4|3|2|1|0||域的名称|类型|
|----|----|----|----|----|----|----|----|----|----|----|
|LZW Minimum Code Size||LZW最小代码长度|Byte| | | | | | | |


|Image Data||图像数据|Data Sub-blocks|

图6-09 图像数据的存储格式

数据子块(Data Sub-blocks)的结构如图6-10所示，这是一个可变长度的数据块，其长度由块大小域(Block Size)域中的值确定，字节数在0～255之间。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|Block Size|0|块大小|Byte|
||1||Byte|
||||Byte|
|Data Values||数值|Byte|
||||Byte|
||…||…|
||…||…|
||||Byte|
||多||Byte|
||到||Byte|
||255||Byte|

图6-10 数据子块的结构

**7. 图形控制扩展块**
图形控制扩展块(Graphic Control Extension)包含处理图形描绘块时要使用的参数，它的结构如图6-11所示。现说明如下：

(1) 扩展导入符Extension Introducer)用于识别扩展块的开始，域中的值是一个数值等于0x21的固定值。

(2) 图形控制标签(Graphic Control Label)用于标识当前块是一个图形控制扩展块，域中的值是一个数值等于0xF9的固定值。

(3) 块大小(Block Size)用来说明该扩展块所包含字节数，该字节数是从这个块大小(Block Size)域之后到块结束符之间的字节数。

|7|6|5|4|3|2|1|0|字节号|域的名称|类型|
|----|----|----|----|----|----|----|----|----|----|----|
|Extension Introducer|0|扩展导入符|Byte| | | | | | | |
|Graphic Control Label|1|图形扩展标签|Byte| | | | | | | |
||||| | | | | | | |
|Block Size|0|块大小|Byte| | | | | | | |
|<Packed Fields>|1|包装域|See below| | | | | | | |
|Delay Time|2|延时时间|Unsigned| | | | | | | |
||||| | | | | | | |
|Transparent Color Index|3|透明彩色索引|Byte| | | | | | | |
||||| | | | | | | |
|Block Terminator|0|块结束符|Byte| | | | | | | |

图6-11 图像描述块的结构

(4) 包装域的结构如图6-12所示。处理方法(Disposal Method)规定图形显示之后译码器要用表6-03中所述方法进行处理。
**表6-03 包装域规定的处理方法**

|域值|处理方法|
|----|----|
|0|没有指定要做任何处理|
|1|不处理，图形留在原处|
|2|显示图形的区域必须要恢复成背景颜色|
|3|恢复成以前显示的图形|
|4～7|(未定义)|

用户输入标志(User Input Flag)域表示在继续处理之前是否需要用户输入响应。在延时时间(Delay Time)和用户输入标志(User Input Flag)都设置为1的情况下，继续处理的开始时间取决于用户响应输入在前还是延时时间结束在前。

|7|6|5|4|3|2|1|0|
|----|----|----|----|----|----|----|----|
|Reserved(保留)|Disposal Method(处理方法)|User Input Flag|Transparent Color Flag| | | | |

图6-12 图形控制扩展块的包装结构

(5) 透明(Transparency Flag)表示是否给出透明索引(transparency index)

(6) 延时时间(Delay Time)用来指定在图形显示之后继续处理数据流之前的等待时间，一百分之一秒为单位。

(7) 当且仅当透明标志位设置为1时，透明索引(Transparency Index)用来指示处理程序是否要修改显示设备上的相应象点。当且仅当透明标志位设置为1时，就要修改。

(8) 块结束符(Block Terminator)表示该图形控制扩展块(Graphic Control Extension)结束，它是由一个字节组成的数据块，该域的值是一个固定的值：0x00，因此称为零长度数据子块(zero-length Data Sub-block)。

**8. 无格式文本扩展块**
无格式文本扩展块(Plain Text Extension)包含文本数据和描绘文本所须的参数。文本数据用7位的ASCII字符编码并以图形形式显示。扩展块的结构如图6-13所示。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|Extension Introducer (0x21)|0|扩展导入符|Byte|
|Plain Text Label (0x01)|1|无格式文本标签|Byte|

|Block Size|0|块大小|Byte|
|----|----|----|----|
|Text Grid Left Position|1|文本网格左列位置|Unsigned|
||2|||
|Text Grid Top Position|3|文本网格顶行位置|Unsigned|
||4|||
|Text Grid Width|5|文本网格宽度|Unsigned|
||6|||
|Text Grid Height|7|文本网格高度|Unsigned|
||8|||
|Character Cell Width|9|字符单元宽度|Byte|
|Character Cell Height|10|字符单元高度|Byte|
|Text Foreground Color Index|11|文本颜色索引|Byte|
|Text Background Color Index|12|文本背景颜色索引|Byte|

|||||
|----|----|----|----|
|Plain Text Data||无格式文本数据|Data Sub-blocks|
|||||

图6-13 无格式文本扩展块结构

**9. 注释扩展块**
注释扩展块(Comment Extension)域的内容用来说明图形、作者或者其他任何非图形数据和控制信息的文本信息。

注释扩展块的结构如图6-14所示。其中的注释数据是序列数据子块(Data Sub-blocks)，每块最多255个字节，最少1个字节。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|Extension Introducer (0x21)|0|扩展导入符|Byte|
|Comment Label (0xFE)|1|注释标签|Byte|

|Comment Data|0|注释数据||
|----|----|----|----|
||||Data Sub-blocks|
||…|||
||N－1|||

|Block Terminator||块结束符|Byte|

图6-14 注释扩展块

**10. 应用扩展块**
应用扩展块(Application Extension)包含制作该图像文件的应用程序的相关信息，它的结构如图6-15所示。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|Extension Introducer (0x21)|0|扩展导入符|Byte|
|Extension Label (0xFF)|1|扩展标签|Byte|

|Block Size|0|块大小|Byte|
|----|----|----|----|
||1|||
||2|||
||3|||
|Application Identifier|4|应用程序标识符|8 Bytes|
||5|(程序名称)||
||6|||
||7|||
||8|||
||9|||
|Appl. Authentication Code|10|应用程序识别码|3 Bytes|
||11|||

|||||
|----|----|----|----|
|Application Data||应用数据|Data Sub-blocks|
|||||

|Block Terminator|0||Byte|

图6-15 应用扩展块

**11. GIF文件结束块**
结束块(GIF Trailer)表示GIF文件的结尾，它包含一个固定的数值：0x3B。它具有如图6-16所示的结构。

|7 6 5 4 3 2 1 0|域的名称|数据类型|
|----|----|----|
|GIF Trailer = 0x3B|GFI文件结束块|Byte|

图6-16 GIF文件结束块
** 速差表表6-04 GIF文件格式**** 速差表表6-04 GIF文件格式**

|块的名称|需要|标签|扩展|版本号.|
|----|----|----|----|----|
|Application Extension(应用扩展)|Opt. (*)|0xFF (255)|yes|89a|
|Comment Extension(注释扩展)|Opt. (*)|0xFE (254)|yes|89a|
|Global Color Table(全局彩色表)|Opt. (1)|none|no|87a|
|Graphic Control Extension(图形控制扩展)|Opt. (*)|0xF9 (249)|yes|89a|
|Header(文件头)|Req. (1)|none|no|N/A|
|Image Descriptor(图像描述)|Opt. (*)|0x2C (044)|no|87a (89a)|
|Local Color Table(局部彩色表)|Opt. (*)|none|no|87a|
|Logical Screen Descriptor(逻辑屏幕描述块)|Req. (1)|none|no|87a (89a)|
|Plain Text Extension(无格式文本扩展)|Opt. (*)|0x01 (001)|yes|89a|
|Trailer(文件结束)|Req. (1)|0x3B (059)|no|87a|

Unlabeled Blocks(无标号块)

|Header(文件头)|Req. (1)|none|no|N/A|
|----|----|----|----|----|
|Logical Screen Descriptor(逻辑屏幕描述块)|Req. (1)|none|no|87a (89a)|
|Global Color Table(全局彩色表)|Opt. (1)|none|no|87a|
|Local Color Table(局部彩色表)|Opt. (*)|none|no|87a|

Graphic-Rendering Blocks(图像描绘块)

|Plain Text Extension(无格式文本扩展)|Opt. (*)|0x01 (001)|yes|89a|
|----|----|----|----|----|
|Image Descriptor(图像描述块)|Opt. (*)|0x2C (044)|no|87a (89a)|

Control Blocks(控制块)

|Graphic Control Extension(图形控制扩展)|Opt. (*)|0xF9 (249)|yes|89a|

Special Purpose Blocks(专用块)

|Trailer(结束)|Req. (1)|0x3B (059)|no|87a|
|----|----|----|----|----|
|Comment Extension(注释扩展)|Opt. (*)|0xFE (254)|yes|89a|
|Application Extension(应用程序扩展)|Opt. (*)|0xFF (255)|yes|89a|

表中：Req. (1) 表示最多出现一次

Opt. (*) 出现次数大于等于0

3>PNG格式

PNG是20世纪90年代中期开始开发的图像文件存储格式，其目的是企图替代GIF和TIFF文件格式，同时增加一些GIF文件格式所不具备的特性。流式网络图形格式(Portable Network Graphic Format，PNG)名称来源于非官方的“PNG's Not GIF”，是一种位图文件(bitmap file)存储格式，读成“ping”。PNG用来存储灰度图像时，灰度图像的深度可多到16位，存储彩色图像时，彩色图像的深度可多到48位，并且还可存储多到16位的α通道数据。PNG使用从LZ77派生的无损数据压缩算法。

PNG文件格式保留GIF文件格式的下列特性：
- 
使用彩色查找表或者叫做调色板可支持256种颜色的彩色图像。 

- 
流式读/写性能(streamability)：图像文件格式允许连续读出和写入图像数据，这个特性很适合于在通信过程中生成和显示图像。 

- 
逐次逼近显示(progressive display)：这种特性可使在通信链路上传输图像文件的同时就在终端上显示图像，把整个轮廓显示出来之后逐步显示图像的细节，也就是先用低分辨率显示图像，然后逐步提高它的分辨率。 

- 
透明性(transparency)：这个性能可使图像中某些部分不显示出来，用来创建一些有特色的图像。 

- 
辅助信息(ancillary information)：这个特性可用来在图像文件中存储一些文本注释信息。 

- 
独立于计算机软硬件环境。 

- 
使用无损压缩。

PNG文件格式中要增加下列GIF文件格式所没有的特性：
- 
每个像素为48位的真彩色图像。 

- 
每个像素为16位的灰度图像。 

- 
可为灰度图和真彩色图添加α通道。 

- 
添加图像的γ信息。 

- 
使用循环冗余码(**c**yclic **r**edundancy **c**ode，CRC)检测损害的文件。 

- 
加快图像显示的逐次逼近显示方式。 

- 
标准的读/写工具包。 

- 
可在一个文件中存储多幅图像。

**文件结构**
PNG图像格式文件(或者称为数据流)由一个8字节的PNG文件署名(PNG file signature)域和按照特定结构组织的3个以上的数据块(chunk)组成。

PNG定义了两种类型的数据块，一种是称为关键数据块(critical chunk)，这是标准的数据块，另一种叫做辅助数据块(ancillary chunks)，这是可选的数据块。关键数据块定义了4个标准数据块，每个PNG文件都必须包含它们，PNG读写软件也都必须要支持这些数据块。虽然PNG文件规范没有要求PNG编译码器对可选数据块进行编码和译码，但规范提倡支持可选数据块。
**(1) PNG文件署名域**
8字节的PNG文件署名域用来识别该文件是不是PNG文件。该域的值是：

|十进制数|137|80|78|71|13|10|26|10|
|----|----|----|----|----|----|----|----|----|
|十六进制数|89|50|4e|47|0d|0a|1a|0a|

**(2) 数据块的结构**
每个数据块都由表6-07所示的的4个域组成。
**表6-07 PNG文件数据块的结构**

|**名称**|**字节数**|**说明**|
|----|----|----|
|Length(长度)|4字节|指定数据块中数据域的长度，其长度不超过(231－1)字节|
|Chunk Type Code(数据块类型码)|4字节|数据块类型码由ASCII字母(A-Z和a-z)组成|
|Chunk Data(数据块数据)|可变长度|存储按照Chunk Type Code指定的数据|
|CRC(循环冗余检测)|4字节|存储用来检测是否有错误的循环冗余码|

在表6-07中，CRC(**c**yclic **r**edundancy **c**heck)域中的值是对Chunk Type Code域和Chunk Data域中的数据进行计算得到的。CRC具体算法定义在ISO 3309和ITU-T V.42中，其值按下面的CRC码生成多项式进行计算：

x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
**数据块结构1. 关键数据块****数据块结构1. 关键数据块**
关键数据块中的4个标准数据块是：

(1) 文件头数据块IHDR(header chunk)：它包含有PNG文件中存储的图像数据的基本信息，并要作为第一个数据块出现在PNG数据流中，而且一个PNG数据流中只能有一个文件头数据块。

文件头数据块由13字节组成，它的格式如表6-08所示。
**表6-08 PNG文件头键数据块的结构**

|域的名称|字节数|说明|
|----|----|----|
|Width|4 bytes|图像宽度，以像素为单位|
|Height|4 bytes|图像高度，以像素为单位|
|Bit depth|1 byte|图像深度：索引彩色图像：1，2，4或8灰度图像：1，2，4，8或16真彩色图像：8或16|
|ColorType|1 byte|颜色类型：0：灰度图像, 1，2，4，8或162：真彩色图像，8或163：索引彩色图像，1，2，4或84：带α通道数据的灰度图像，8或166：带α通道数据的真彩色图像，8或16|
|Compression method|1 byte|压缩方法(LZ77派生算法)|
|Filter method|1 byte|滤波器方法|
|Interlace method|1 byte|隔行扫描方法：0：非隔行扫描1： Adam7(由Adam M. Costello开发的7 遍隔行扫描方法)|

(2) 调色板数据块PLTE(palette chunk)：它包含有与索引彩色图像((indexed-color image))相关的彩色变换数据，它仅与索引彩色图像有关，而且要放在图像数据块(image data chunk)之前。真彩色的PNG数据流也可以有调色板数据块，目的是便于非真彩色显示程序用它来量化图像数据，从而显示该图像。调色板数据块结构如表6-09所示。
**表6-09 调色板数据块结构**

|域的名称|字节数|说明|
|----|----|----|
|Red|1 byte|0 = 黑，255 = 红|
|Green|"> 0 = 黑，255 = 绿| |
|Blue|1 byte|0 = 黑，255 = 蓝|

调色板实际是一个彩色索引查找表，它的表项数目可以是1～256中的一个数，每个表项有3字节，因此调色板数据块所包含的最大字节数为768。

(3) 图像数据块IDAT(image data chunk)：它存储实际的数据，在数据流中可包含多个连续顺序的图像数据块。

(4) 图像结束数据IEND(image trailer chunk)：它用来标记PNG文件或者数据流已经结束，并且必须要放在文件的尾部。

除了表示数据块开始的IHDR必须放在最前面， 表示PNG文件结束的IEND数据块放在最后面之外，其他数据块的存放顺序没有限制。
**2. 辅助数据块**
PNG文件格式规范制定的10个辅助数据块是：

(1) 背景颜色数据块bKGD(background color)。

(2) 基色和白色度数据块cHRM(primary chromaticities and white point)。所谓白色度是指当R＝G＝B＝最大值时在显示器上产生的白色度。

(3) 图像γ数据块gAMA(image gamma)。

(4) 图像直方图数据块hIST(image histogram)。

(5) 物理像素尺寸数据块pHYs(physical pixel dimensions)。

(6) 样本有效位数据块sBIT(significant bits)。

(7) 文本信息数据块tEXt(textual data)。

(8) 图像最后修改时间数据块tIME (image last-modification time)。

(9) 图像透明数据块tRNS (transparency)。

(10) 压缩文本数据块zTXt (compressed textual data)。
**3. 数据块摘要**
关键数据块、辅助数据块和专用公共数据块(special-purpose public chunks)综合在表6-10中。
**表6-10 PNG文件格式中的数据块**

|**数据块符号**|**数据块名称**|**多数据块**|**可选否**|**位置限制**|
|----|----|----|----|----|
|IHDR|文件头数据块|否|否|第一块|
|cHRM|基色和白色点数据块|否|是|在PLTE和IDAT之前|
|gAMA|图像γ数据块|否|是|在PLTE和IDAT之前|
|sBIT|样本有效位数据块|否|是|在PLTE和IDAT之前|
|PLTE|调色板数据块|否|是|在IDAT之前|
|bKGD|背景颜色数据块|否|是|在PLTE之后IDAT之前|
|hIST|图像直方图数据块|否|是|在PLTE之后IDAT之前|
|tRNS|图像透明数据块|否|是|在PLTE之后IDAT之前|
|oFFs|(专用公共数据块)|否|是|在IDAT之前|
|pHYs|物理像素尺寸数据块|否|是|在IDAT之前|
|sCAL|(专用公共数据块)|否|是|在IDAT之前|
|IDAT|图像数据块|是|否|与其他IDAT连续|
|tIME|图像最后修改时间数据块|否|是|无限制|
|tEXt|文本信息数据块|是|是|无限制|
|zTXt|压缩文本数据块|是|是|无限制|
|fRAc|(专用公共数据块)|是|是|无限制|
|gIFg|(专用公共数据块)|是|是|无限制|
|gIFt|(专用公共数据块)|是|是|无限制|
|gIFx|(专用公共数据块)|是|是|无限制|
|IEND|图像结束数据|否|否|最后一个数据块|

5>PCX格式
是PC 画笔的图像文件格式。PCX的图像深度可选为1、4、8 bit。由于这种文件格式出现较早，它不支持真彩色。PCX文件采用RLE行程编码，文件体中存放的是压缩后的图像数据。因此，将采集到的图像数据写成PCX文件格式时，要对其进行RLE编码；而读取一个PCX文件时首先要对其进行RLE解码，才能进一步显示和处理。

PCX
格式用于写是相对较简单的，但用于读就比较棘手，除非知道被解码图像的很多细节内
容（如位深度和调色板等）。因此，以下的阐述都是基于最坏的情况，即读取一个其特
性和年代都未确定的PCX文件，所有的数均是little－endian（Intel）格式，即LSB在先
。
字节0，Zsoft标志
总是十进制值160，即十六进制A0。
字节1，版本号，
一定程度上不可靠的文件内容指南，见前面一节“变体”的讨论。
字节2，编码
到目前为止，总是为1。当前编码（压缩）方法只有一种，即在下面“位图数据），一节
中讲述的运行长度法。
字节3，每像素位数
实际上是每个位平面的每像素位数，可能的值是1、2、4或8。
字节4-11，图像大小
图像大小由最小的和最大的极限给出。通常的下限是0。所有的极限均用16位无符号整数
表示，单位为像素。图像大小可以这样计算：XSIZE＝Xmax―Xmin＋l；
YSIZE＝Ymax－Ymin＋l，单位为像素。
字节12-15，以每英寸点数为单位的水平和垂直分辨率
这两个16位的数字有点古怪，它们对于定义所存储图像不起任何作用，但是，当它们与
图像大小组合起来加以考虑时，能产生出被扫描图像的原始大小，或者被打印图像的希
望大小，以英寸为单位。
字节16-63，头调色板
这一字段看上去只适用于带有单一位平面、16种或更少的颜色以及版本号为2的文件（参
见下面“解释数据的关键”口节）。使用时，调色板拥有16组三元组的单字节调色板值
。
字节65，色彩平面
PCX图像可以是单色彩，也可以是多个色彩平面的（参见第一章）。头的这个字节给出色
彩平面数，它是正确翻译PCX文件的关键。
字节66，每行字节数
实际是每个平面的每行的字节数—存储末压缩图像一个扫描行的一个色彩平面所需要内
存字节数，它总是偶数。
字节68，头调色板翻译
1=彩色／单色；2=灰度。Paintbrush IV或Paintbrush IV PIus中不使用它。
字节70~73，视屏屏幕大小，X和Y
只被Paintbrush IV和Paintbrush IV PIus使用；并不是必不可少的，但是对于产生正确
的外观比例（防止压缩型失真）可能有用。
三、位图数据
如果没有使用调色板，那么数据是实际的像素值；否则，它们是指向调色板值的指针。
在后一种情况下，数据给出的是相对于所使用的调色板的起始处的偏移（比如在三字节
的三元组值中，1=字节3）。
当数据是实际的像素值时，它们按色彩平面和扫描行存储。例如，对于三种颜色红、绿
和蓝（RGB），数据格式为：
（第0行：）RRRRRR．．．GGGGGG．．．BBBBBB．．．
（第1行：）RRRRRR．．．GGGGGG．．．BBBBBB．．．
如果有两个平面，那么色彩是任选的；如果有三个平面，则颜色为RGB；如果使用四个平
面，则它们是符合IBM
CGA／EGA标准的单个位的平面：红、绿、蓝和光强（RGBI）。光强位只是给像素以一种
名义上较高的亮度。
当数据是指向某调色板指针时，它们就组成一个完整的图像平面（也就是说，它们不会
分解成单独色彩平面）。然后数据按如下方式简单地编排（字符P代表各种指针值）：
（行0：）PPPPPP
（行1：）PPPPPPP
P的长度取决于深度，以每平面的每像素位数表示。例如，如果深度为4位，则P就是半个
字节长。
所有情况下，在扫描行之间都有编码隔断标志。但是，在一个扫描行中的色彩平面间没
有编码隔断标志。同样，也没有分隔符可用来标识扫描行的结束（虽然一个扫描行可能
是也可能不是用额外的零作为结束）。也就是说，在扫描行之间不会有行号（虽然这里
写出来了），也不会有空字符、空格、回车、换行或其他的字符。
不论要记录的是何种类型的位图数据，都使用同样的运行长度压缩方法，下面给出恢复
算法（当前普遍使用的基于调色板的图像只有一个平面）。


PCX图象文件格式最早出现于Zsoft公司开发的PC Paintbrush绘图软件，由于该绘图软件功能强大并成功移植到Windows操作系统，加上PCX是最早支持彩色的图象格式之一，PCX成为目前比较流行的图象格式。对于开发图象浏览、处理软件的程序员来讲，如何读取、保存PCX图象格式是最为基本的话题,根据自己对PCX图象格式理解，开发了相应的代码，希望对读者有用，由于篇幅限制，在此对文件格式不予介绍，读者可以参考相关数目。
/***********************************************************************
* 函数名称：LoadPCXLine(PPCXHEAD ppcxHdr, LPBYTE ppcxImg, LPBYTE ppcxBits) const
*
* 参数：PPCXHEAD ppcxHdr －指向PCXHEAD结构的指针!NULL,导入BitPlane,BytePerLine,=>clScanLineSize
* LPBYTE ppcxImg －指向PCX图象区指针!NULL,RLE压缩编码,位置递增+=rec。
*     调用之前获得首地址指针: 
* LPBYTE ppcxBits －指向DIB数据区的指针,按扫描行(scanline)长度递增
*
* 返回：UINT rec －返回每行解压以后的字节数目
*
* 说明：根据PCX图象数据指针，对RLE进行解码
***********************************************************************/
UINT CPcxImage::LoadPCXLine(PPCXHEAD ppcxHdr, LPBYTE ppcxImg, LPBYTE ppcxBits) const
{
 ASSERT(ppcxHdr!=NULL&&ppcxImg!=NULL&&ppcxBits!=NULL);China It Power . ComYHzsF

 // because in bitmap bits order, it's blue=>green=>red
 // however pcx is red=>green=>blue so use decrease order
 //－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
 UINT lPos(0),  // 记录存入ppcxBits的总数
   iX(0), // 记录每个位平面字节序号
   rec(0); // 读取_ppcxImg_ 字节序号
 for ( int bp=ppcxHdr->BitPlane-1; bp >= 0;  bp-- )
 {
  // RLE 解码=======
  iX=0;

  while ( iX<ppcxHdr->BytePerLine )
  {
   BYTE uiValue = ppcxImg[rec++];
   if ( (uiValue & 0xc0) == 0xc0 )  // 判断高位字节是否设置 0xc0
   {
    uiValue = uiValue & 0x3f ;  // 计算重复
    BYTE Color = ppcxImg[rec++]; // 提取颜色

    // 存放到内存DIB
    for ( BYTE bRepeat=0; bRepeat < uiValue; bRepeat++ )
    {
     ppcxBits[(iX++)*ppcxHdr->BitPlane+bp] = Color;
     lPos++;
    }
   }
   else
   {
    ppcxBits[(iX++)*ppcxHdr->BitPlane+bp] = uiValue;
    lPos++;
   }
  }

 }

 return rec;
}

/****************************************************************************
* 函数名称：PackPCXLine(PPCXHEAD ppcxHdr, LPBYTE ppcxImg, LPBYTE ppcxBits) const
*
* 参数：PPCXHEAD ppcxHdr －指向PCXHEAD结构的指针!NULL,导入BitPlane,BytePerLine,=>clScanLineSize
* LPBYTE ppcxBits －指向DIB数据区的指针,按扫描行(scanline)长度递增
* LPBYTE ppcxImg －指向PCX图象区指针!NULL,RLE压缩编码。
*     调用之前声明: LPBYTE ppcxImg=new BYTE[2*BitPlane*BytePerLine]
*
* 返回：UINT rec －返回每行压缩以后的字节数目
*
* 说明：根据DIB图象数据指针，进行RLE编码(经过测试算法很完善,支持256和24bit真彩色)
****************************************************************************/
UINT CPcxImage::PackPCXLine(PPCXHEAD ppcxHdr, LPBYTE ppcxBits, LPBYTE ppcxImg) const
{
 //----------------------------------------
 // RLE压缩
 ASSERT(ppcxHdr!=NULL && ppcxBits!=NULL && ppcxImg!=NULL);China It Power . ComYHzsF

 BYTE i(1);
 UINT lPos(0), rec(0);China It Power . ComYHzsF

 // RLE编码，最大重复<=63
 for(int bp=ppcxHdr->BitPlane-1; bp>=0; bp--)
 {
  lPos=0;  // 处理到的RGB序列

  while(lPos<ppcxHdr->BytePerLine)  // 等价小于图象宽度
  {
   i=1; // 重置步长-1

   //----------------->以下代码检查
   while((ppcxBits[(i-1+lPos)*ppcxHdr->BitPlane+bp]==ppcxBits[(i+lPos)*ppcxHdr->BitPlane+bp])
    &&((lPos+i)<ppcxHdr->BytePerLine)&&(i<63)) i++;
   if(i>1 && i<64)
   {
    // 表明当前象素位置开始存在i个重复象素值，依次写入PCX图象数据Buffer
    // 1.重复次数
    ppcxImg[rec++]=i|0xc0;
    // 2.象素值
    ppcxImg[rec++]=ppcxBits[lPos*ppcxHdr->BitPlane+bp];

    lPos+=i; // lPos－记录当前扫描行中已经处理的字节数
    // rec －记录当前已经写入PCX文件的字节数
   }
   else
   {
    // 表明当前象素位置开始不存在重复象素值
    // 象素值大于0xc0(192)，写标志0xc1
    if((ppcxBits[lPos*ppcxHdr->BitPlane+bp]&0xc0)==0xc0) ppcxImg[rec++]=0xc1;
    ppcxImg[rec++]=ppcxBits[lPos*ppcxHdr->BitPlane+bp]; lPos++;
   }
  }
 }

 // 写图象数据结束
 return rec;
}

＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
调用如下：
 1.// RLE解码－－－－－－－－－－－－－> 已经包含8、24bit图象
 for( int iY=0; iY<=ppcxHdr->YMax; iY++ )
 {
  ZeroMemory(ppcxBits, clScanLineSize);
  ppcxImg+=LoadPCXLine(ppcxHdr, ppcxImg, ppcxBits); // 读取扫描行数据
  ppcxBits+=clScanLineSize;
 }
 ppcxHdr是指向PCXHEAD结构(128BYTE)的指针，ppcxBits是存放解码后图象数据的buffer，ppcxImg是指向pcx图象文件
中图象数据的指针，此处随扫描行递增。完成功能是从pcx文件中解码图象数据到windows位图格式的图象数据。

 2.// RLE压缩－－－－－－－－－－－－－> 已经包含8、24bit图象
 // 最坏情况下申请2倍的缓冲,相邻都不重复,并且都大于0xc0
 LPBYTE ppcxImg=new BYTE[2*pcxHdr.BitPlane*pcxHdr.BytePerLine]; // 存放临时扫描行
 UINT rec(0); // 计数器，写如PCX文件字节数
 for( int iY=0; iY<=pcxHdr.YMax; iY++ )
 {
  ZeroMemory(ppcxImg,2*pcxHdr.BitPlane*pcxHdr.BytePerLine);
  rec=PackPCXLine(&pcxHdr, ppcxBits, ppcxImg);

  // DIB 扫描行递增
  ppcxBits+=clScanLineSize;

  pFile->Write(ppcxImg,rec);
 }
 delete []ppcxImg;
具体参数大致同1.ppcxImg为临时RLE压缩后的buffer。


6> TIFF格式
TIFF（Tag Image File Format）文件是由Aldus和Microsoft公司为扫描仪和桌上出版系统研制开发的一种较为通用的图像文件格式。TIFF格式灵活易变，它又定义了四类不同的格式：TIFF－B适用于二值图像；TIFF－G适用于黑白灰度图像；TIFF－P适用于带调色板的彩色图像；TIFF－R适用于RGB真彩图像。TIFF支持多种编码方法，其中包括RGB无压缩、RLE压缩及JPEG压缩等。

TIF是一种国际上非常流行的适于各种电脑和操作系统的图形文件格式，目前国际上流行
的很多软件都支持TIF格式。一般扫描器所配备的软件都直接或间接地用到TIF图形文件
格式，还有一些软件系统，例如：
Windows系统下的PhotoStyler都把TIF作为图形的存取格式，TIF格式也可以转换为Win
dows的BMP等其它格式。
TIFF格式有三级体系，从高到低依次为：文件头，一个或多个称为IFD的包含桥记指针的
目录以及数据。体系的最高层是文件头，只包含三个表项：
（1）一个代码，指明字节顺序（低字节在先还是高字节在先）。
（2）一个把文件标识为TIFF文件的代码号。
（3）一个指向图像文件目录（Image FileDirectory，IFD）的指针。
这里是一个tif文件的文件头:
4D 4D 2A 00 00 00 00 08
IFD提供一系列的指针（索引），这些指针告诉我们各种有关的数据字段在文件中的开始
位置，并给出每个字母的数据类型（例如，1字节整型）及长度。这种方法允许数据字段
定位在文件的任何地方，可以是差不多任意长度，并可以包含大量信息。例如，一个指
针可能指向关于彩色调色板数据的一个786字节字段；另一个可能指向扫描仪的一条64字
节灰度修正曲线。在一个文件中可能有几个相关的图像，这时可以有几种IFD。IFD的最
后一个表项指向任何一个后续的IFD。
每个指针都有一个标记值指明所指向的数据字段类型的一个代码号。TIFF
规范列出了所有正式的、非专用的标记号，给予它们有用的名字（如SamplesPerPixel，
十进制代码为277），并描述这个指针所识别的数据，告知数据的组织方法。
1．优点一种有竞争力的位元映射格式，为以后扩充说明留有余地。
2．缺点有许多衍生格式，但井非所有的衍生格式都得到所有应用程序支持。
3．变体最早的1984年1．0版格式和1989年2．0版格式，应用程序设计师可以注册自己
私有的衍生格式。
TGA格式由AT&T首先引用，用于支持他们的Targa 和Truevision 公司的 ATVIS
TA
图形捕捉卡。此格式已经成为数字化图形以及由光跟踪和其它应用程序所产生的高质量
图形常用格式。TrueVision公司的TGA文件格式已广泛地被国际上的图形工业所接受。有
许多图形工作人员喜欢按个人习惯储存图形文件，并且只存数据，不存格式，使得大量
的图形文件成为个人的专用产品，无法交流，因此，有必要按照标准格式来储存图形文
件，
TGA文件格式就是目前国际上比较流行的图形文件储存格式。
Targa文件以一个固定大小的文件头开始，然后是可变长度的图像标识符（ID）、色彩对
应表和图像。ID字段的偏移为18，紧跟在文件头的后面。
多字节值的存储是低字节（tss）为先，即Intel格式。除了字节对齐之外，值和段均没
有填充或对齐。
图像可以彩色映射的。有两种类型的色彩对应表图像：“伪彩色”（pseudo
color），这种图像中每个像素值从色彩对应表中选取一个单独值，“直接彩色”（dir
ect
color），这种图像中每个像素包含分别查询的红、绿和蓝值。像素为实际的红、绿和蓝
值的图像称为“真色彩图”（True
Color），为灰度值的称为“黑白图”（black-and-white）。
图像总是按行存储，但行可以按从上到下或从下到上的顺序，而且对像素可队从左到右
或从右到左存储，在实际图像中，像素几乎总是从左到右存储。有些型号的扫描仪从上
到下扫描，而另外一些扫描仪则从下到上扫描，所以至今还没有占统治地位的行顺序。

1．优点与设备无关，文件可以很好地组织结构；由于使用向量描述图形的性能，文件
可以比相应的位元映射小很多。
2．缺点语意结构与Windows图形模型关系太密切；文件比较复杂。
3．变体 Windows 3．x将Windows 2．x转换文件格式中加入了新的记录类型，但不使用
新类型的转换文件应该与早期版本相容。





微处理机中的存放顺序有正序(big endian)和逆序(little endian)之分。正序存放就是高字节存放在前低字节在后，而逆序存放就是低字节在前高字节在后。例如，十六进制数为A02B，正序存放就是A02B，逆序存放就是2BA0。摩托罗拉(Motorola)公司的微处理器使用正序存放，而英特尔(Intel)公司的微处理器使用逆序。JPEG文件中的字节是按照正序排列的。

JPEG委员会在制定JPEG标准时，定义了许多标记(marker)用来区分和识别图像数据及其相关信息，但笔者没有找到JPEG委员会对JPEG文件交换格式的明确定义。直到1998年12月从分析网上具体的JPG图像来看，使用比较广泛的还是JPEG文件交换格式(JPEG File Interchange Format，JFIF)版本号为1.02。这是1992年9月由在C-Cube Microsystems公司工作的Eric Hamilton提出的。此外还有TIFF JPEG等格式，但由于这种格式比较复杂，因此大多数应用程序都支持JFIF文件交换格式。 

JPEG文件使用的颜色空间是CCIR 601推荐标准进行的彩色空间(参看第7章)。在这个彩色空间中，每个分量、每个像素的电平规定为255级，用8位代码表示。从RGB转换成YCbCr空间时，使用下面的精确的转换关系：

　　　　　　 Y = 256 * E'y

　　　　　 Cb = 256 * [E'Cb] + 128

　　　　　 Cr = 256 * [E'Cr] + 128
其中亮度电平E'y和色差电平E'Cb和E'Cb分别是CCIR 601定义的参数。由于E'y的范围是0～1，E'Cb和E'Cb的范围是-0.5～+0.5，因此Y, Cb和Cr的最大值必须要箝到255。于是RGB和YCbCr之间的转换关系需要按照下面的方法计算。

(1) 从RGB转换成YCbCr

YCbCr(256级)分量可直接从用8位表示的RGB分量计算得到：

　 　　　　 Y =　　 0.299 R + 0.587　G　 + 0.114 B

　　 　 Cb = - 0.1687R - 0.3313G　 + 0.5　　　B + 128

　　　 Cr = 0.5 R - 0.4187G - 0.0813 B + 128

需要注意的是不是所有图像文件格式都按照R0，G0，B0，…… Rn，Gn，Bn的次序存储样本数据，因此在RGB文件转换成JFIF文件时需要首先验证RGB的次序。

(2) 从YCbCr转换成RGB 

RGB分量可直接从YCbCr(256级)分量计算得到：

　　　　 R = Y　　　　　　　　　　　　　　　　 + 1.402 (Cr-128)

　　　　　 G = Y - 0.34414 (Cb-128) - 0.71414 (Cr-128)

　　　　　 B = Y + 1.772 (Cb-128)

在JFIF文件格式中，图像样本的存放顺序是从左到右和从上到下。这就是说JFIF文件中的第一个图像样本是图像左上角的样本。
**文件结构文件结构**

JPEG文件使用的颜色空间是CCIR 601推荐标准进行的彩色空间(参看第7章)。在这个彩色空间中，每个分量、每个像素的电平规定为255级，用8位代码表示。从RGB转换成YCbCr空间时，使用下面的精确的转换关系：

　　　　　　 Y = 256 * E'y

　　　　　 Cb = 256 * [E'Cb] + 128

　　　　　 Cr = 256 * [E'Cr] + 128
其中亮度电平E'y和色差电平E'Cb和E'Cb分别是CCIR 601定义的参数。由于E'y的范围是0～1，E'Cb和E'Cb的范围是-0.5～+0.5，因此Y, Cb和Cr的最大值必须要箝到255。于是RGB和YCbCr之间的转换关系需要按照下面的方法计算。

(1) 从RGB转换成YCbCr

YCbCr(256级)分量可直接从用8位表示的RGB分量计算得到：

　 　　　　 Y =　　 0.299 R + 0.587　G　 + 0.114 B

　　 　 Cb = - 0.1687R - 0.3313G　 + 0.5　　　B + 128

　　　 Cr = 0.5 R - 0.4187G - 0.0813 B + 128

需要注意的是不是所有图像文件格式都按照R0，G0，B0，…… Rn，Gn，Bn的次序存储样本数据，因此在RGB文件转换成JFIF文件时需要首先验证RGB的次序。

(2) 从YCbCr转换成RGB 

RGB分量可直接从YCbCr(256级)分量计算得到：

　　　　 R = Y　　　　　　　　　　　　　　　　 + 1.402 (Cr-128)

　　　　　 G = Y - 0.34414 (Cb-128) - 0.71414 (Cr-128)

　　　　　 B = Y + 1.772 (Cb-128)

在JFIF文件格式中，图像样本的存放顺序是从左到右和从上到下。这就是说JFIF文件中的第一个图像样本是图像左上角的样本。
**文件结构文件结构**
JFIF文件格式直接使用JPEG标准为应用程序定义的许多标记，因此JFIF格式成了事实上JPEG文件交换格式标准。JPEG的每个标记都是由2个字节组成，其前一个字节是固定值0xFF。每个标记之前还可以添加数目不限的0xFF填充字节(fill byte)。下面是其中的8个标记：
- 
SOI　 0xD8　　　　　　　　　　　 图像开始 

- 
APP0 0xE0　　　　　　　　　　　 JFIF应用数据块 

- 
APPn 0xE1 - 0xEF　　　 其他的应用数据块(n, 1～15) 

- 
DQT　 0xDB　　　　　　　　　　　量化表 

- 
SOF0 0xC0　　　　　　　　　　　 帧开始 

- 
DHT　 0xC4　　　　　　　　　　　霍夫曼(Huffman)表 

- 
SOS　 0xDA　　　　　　　　　　　扫描线开始 

- 
EOI　 0xD9　　　　　　　　　　　 图像结束

为使读者对JPEG定义的标记一目了然，现将JPEG的标记码列于表6-05，并保留英文解释。
**表6-05 JPEG定义的标记**

|**Symbol (符号)**|**Code Assignment(标记代码)**|**Description(说明)**|
|----|----|----|
|**Start Of Frame markers, non-hierarchical Huffman coding**| | |
|SOF0|0xFFC0|Baseline DCT|
|SOF1|0xFFC1|Extended sequential DCT|
|SOF2|0xFFC2|Progressive DCT|
|SOF3|0xFFC3|Spatial (sequential) lossless|
|**Start Of Frame markers, hierarchical Huffman coding**| | |
|SOF5|0xFFC5|Differential sequential DCT|
|SOF6|0xFFC6|Differential progressive DCT|
|SOF7|0xFFC7|Differential spatial lossless|
|**Start Of Frame markers, non-hierarchical arithmetic coding**| | |
|JPG|0xFFC8|Reserved for JPEG extensions|
|SOF9|0xFFC9|Extended sequential DCT|
|SOF10|0xFFCA|Progressive DCT|
|SOF11|0xFFCB|Spatial (sequential) Lossless|
|**Start Of Frame markers, hierarchical arithmetic coding**| | |
|SOF13|0xFFCD|Differential sequential DCT|
|SOF14|0xFFCE|Differential progressive DCT|
|SOF15|0xFFCF|Differential spatial Lossless|
|**Huffman table specification**| | |
|DHT|0xFFC4|Define Huffman table(s)|
|**arithmetic coding conditioning specification**| | |
|DAC|0xFFCC|Define arithmetic conditioning table|
|**Restart interval termination**| | |
|RSTm|0xFFD0～0xFFD7|Restart with modulo 8 counter m|
|**Other marker**| | |
|SOI|0xFFD8|Start of image|
|EOI|0xFFD9|End of image|
|SOS|0xFFDA|Start of scan|
|DQT|0xFFDB|Define quantization table(s)|
|DNL|0xFFDC|Define number of lines|
|DRI|0xFFDD|Define restart interval|
|DHP|0xFFDE|Define hierarchical progression|
|EXP|0xFFDF|Expand reference image(s)|
|APPn|0xFFE0～0xFFEF|Reserved for application use|
|JPGn|0xFFF0～0xFFFD|Reserved for JPEG extension|
|COM|0xFFFE|Comment|
|**Reserved markers**| | |
|TEM|0xFF01|For temporary use in arithmetic coding|
|RES|0xFF02～0xFFBF|Reserved|

JPEG文件由下面的8个部分组成：

(1) 图像开始SOI(Start of Image)标记

(2) APP0标记(Marker)

① APP0长度(length)

② 标识符(identifier)

③ 版本号(version)

④ X和Y的密度单位(units=0：无单位；units=1：点数/英寸；units=2：点数/厘米)

⑤ X方向像素密度(X density)

⑥ Y方向像素密度(Y density)

⑦ 缩略图水平像素数目(thumbnail horizontal pixels)

⑧ 缩略图垂直像素数目(thumbnail vertical pixels)

⑨ 缩略图RGB位图(thumbnail RGB bitmap)

(3) APPn标记(Markers)，其中n=1～15(任选)

① APPn长度(length)

② 由于详细信息(application specific information)

(4) 一个或者多个量化表DQT(difine quantization table)

① 量化表长度(quantization table length)

② 量化表数目(quantization table number)

③ 量化表(quantization table)

(5) 帧图像开始SOF0(Start of Frame)

① 帧开始长度(start of frame length)

② 精度(precision)，每个颜色分量每个像素的位数(bits per pixel per color component)

③ 图像高度(image height)

④ 图像宽度(image width)

⑤ 颜色分量数(number of color components)

⑥ 对每个颜色分量(for each component)
- 
ID 

- 
垂直方向的样本因子(vertical sample factor) 

- 
水平方向的样本因子(horizontal sample factor) 

- 
量化表号(quantization table#)

(6) 一个或者多个霍夫曼表DHT(Difine Huffman Table)

① 霍夫曼表的长度(Huffman table length)

② 类型、AC或者DC(Type, AC or DC)

③ 索引(Index)

④ 位表(bits table)

⑤ 值表(value table)

(7) 扫描开始SOS(Start of Scan)

① 扫描开始长度(start of scan length)

② 颜色分量数(number of color components)

③ 每个颜色分量
- 
ID 

- 
交流系数表号(AC table #) 

- 
直流系数表号(DC table #)

④ 压缩图像数据(compressed image data)

(8) 图像结束EOI(End of Image)

表6-06表示了APP0域的详细结构。有兴趣的读者可通过UltraEdit或者PC TOOLS等工具软件打开一个JPG图像文件，对APP0的结构进行分析和验证。
**表6-06 JFIF格式中APP0域的详细结构**

|**偏移**|**长度**|**内容**|**块的名称**|**说明**|
|----|----|----|----|----|
|0|2 byte|0xFFD8|(Start of Image,SOI)|图像开始|
|2|2 byte|0xFFE0|APP0(JFIF application segment)|JFIF应用数据块|
|4|2 bytes||length of APP0 block|APP0块的长度|
|6|5 bytes||"JFIF"+"0"|识别APP0标记|
|11|1 byte||<Major version>|主要版本号(如版本1.02中的1)|
|12|1 byte||<Minor version>|次要版本号(如版本1.02中的02)|
|13|1 byte||<Units for the Xand Y densities>|X和Y的密度单位units=0：无单位units=1：点数/英寸units=2：点数/厘米|
|14|2 bytes||<Xdensity>|水平方向像素密度|
|16|2 bytes||<Ydensity>|垂直方向像素密度|
|18|1 byte||<Xthumbnail>|缩略图水平像素数目|
|19|1 byte||<Ythumbnail>|缩略图垂直像素数目|
||3n||< Thumbnail RGB bitmap>|缩略RGB位图(n为缩略图的像素数)|
||||Optional JFIF extension APP0 marker segment(s)|任选的JFIF扩展APP0标记段|
||……||……||
||2 byte|0xFFD9|(EOI) end-of-file|图像文件结束标记|

2>GIF文件格式

GIF(**G**raphics **I**nterchange **F**ormat)是CompuServe公司开发的图像文件存储格式，1987年开发的GIF文件格式版本号是GIF87a，1989年进行了扩充，扩充后的版本号定义为GIF89a。

GFI图像文件以数据块(block)为单位来存储图像的相关信息。一个GIF文件由表示图形/图像的数据块、数据子块以及显示图形/图像的控制信息块组成，称为GIF数据流(Data Stream)。数据流中的所有控制信息块和数据块都必须在文件头(Header)和文件结束块(Trailer)之间。

GIF文件格式采用了LZW(**L**empel-**Z**iv **W**alch)压缩算法来存储图像数据，定义了允许用户为图像设置背景的透明(transparency)属性。此外，GIF文件格式可在一个文件中存放多幅彩色图形/图像。如果在GIF文件中存放有多幅图，它们可以像演幻灯片那样显示或者像动画那样演示。
** 文件结构**
GIF文件结构的典型结构如图6-01所示。为下文说明方便，在构件左边加了编号。

|1|Header||GIF文件头||
|----|----|----|----|----|
|2|Logical Screen Descriptor||逻辑屏幕描述块||
|3|Global Color Table||全局彩色表||
||… 扩展模块(任选) …||||
|4|Image Descriptor||图形描述块||
|5|Local Color Table||局部彩色表(可重复n次)|可|
|6|Table Based Image Data||表式压缩图像数据|重|
|7|Graphic Control Extension||图像控制扩展块|复|
|8|Plain Text Extension||无格式文本扩展块|n|
|9|Comment Extension||注释扩展块|个|
|10|Applicaton Extension||应用程序扩展块||
||… 扩展模块(任选) …||||
|11|GIF Trailer||GIF文件结束块||

图6-01 GIF文件结构

数据块可分成3类：控制块(Control Block)，图形描绘块(Graphic-Rendering Block)和专用块(Special Purpose Block)。
**(1) 控制块：控制块包含有用来控制数据流(Data Stream)或者设置硬件参数的信息，其成员包括：**控制块包含有用来控制数据流(Data Stream)或者设置硬件参数的信息，其成员包括：- 
GIF文件头(Header) 

- 
逻辑屏幕描述块(Logical Screen Descriptor) 

- 
图形控制扩展块(Graphic Control Extension) 

- 
文件结束块(Trailer)

**(2) 图形描绘块：包含有用来描绘在显示设备上显示图形的信息和数据，其成员包括：**包含有用来描绘在显示设备上显示图形的信息和数据，其成员包括：- 
图像描述块(Image Descriptor) 

- 
无格式文本扩展块(Plain Text Extension)

**(3) 特殊用途数据块；包含有与图像处理无关的信息，其成员包括：**包含有与图像处理无关的信息，其成员包括：- 
注释扩展块(Comment Extension) 

- 
应用扩展块(Application Extension)

除了在控制块中的逻辑屏幕描述块(Logical Screen Descriptor)和全局彩色表(Global Color Table)的作用范围是整个数据流(Data Stream)之外, 所有其他控制块仅控制跟在它们后面的图形描绘块。
** 构件详解1. GIF文件头**** 构件详解1. GIF文件头**
文件头描述块(Header)定义GIF数据流(GIF Data Stream)，它的结构如图6-02所示。文件头描述块(Header)由GIF标记域(Signature)和版本号(Version)域组成，是一个由6个固定字节组成的数据块，它们用来说明使用的文件格式是GIF格式及当前所用的版本号。GIF标记域(Signature)存放的是“GIF”，版本号域存放的是1987年5月发布的“87a”或者1989年7月发布的“89a”，或者更加新的版本号。

|7|6|5|4|3|2|1|0|字节号|域的名称|数据类型|
|----|----|----|----|----|----|----|----|----|----|----|
||0||| | | | | | | |
|Signature|1|GIF标记|3 Bytes| | | | | | | |
||2||| | | | | | | |
||3||| | | | | | | |
|Version|4|版本号|3 Bytes| | | | | | | |
||5||| | | | | | | |

图6-02 标记/版本数据块的结构

**2. 逻辑屏幕描述块**
逻辑屏幕描述块(Logical Screen Descriptor)包含定义图像显示区域的参数，包括背景颜色信息。这个数据块中的坐标相对于虚拟屏幕的左上角，不一定是指显示屏的绝对坐标，这就意味可以参照窗口软件环境下的窗口坐标或者打印机坐标来设计图像显示程序。逻辑屏幕描述块的结构如图6-03所示：

|7|6|5|4|3|2|1|0|字节号|域的名称|类型|
|----|----|----|----|----|----|----|----|----|----|----|
|Logical Screen Width|0|逻辑屏幕宽度|Unsigned| | | | | | | |
||1|(以像素为定单位)|| | | | | | | |
|Logical Screen Height|2|逻辑屏幕高度|Unsigned| | | | | | | |
||3|(以像素为定单位)|| | | | | | | |
|G|CR|S|Size|4|包装域|见图6-04| | | | |
|Background Color Index|5|背景颜色索引|Byte| | | | | | | |
|Pixel Aspect Ratio|6|像素宽高比|Byte| | | | | | | |

图6-03 屏幕描述块的结构

逻辑描述块包含7个字节。字节0和字节1用来说明逻辑显示屏的宽度，字节3和字节4用来说明逻辑显示屏的高度，字节4用来描述彩色表的属性，字节5用来指定背景颜色索引，字节6用来计算像素的宽高比。现作如下说明：

(1) 屏幕描述块中的第5个字节称为包装域(Packed Fields)，它的位结构如图6-04所示，它由4个子域组成：

① 全局彩色表标志(Global Color Table Flag )域G用来说明是否有全局彩色表存在。如果G＝1，表示有一个全局彩色表(Global Color Table)将紧跟在这个逻辑屏幕描述块(Logical Screen Descriptor)之后；这个标志也用来选择背景颜色索引(Background Color Index)。如果G＝1，背景颜色索引(Background Color Index)域中的值就用作背景颜色的索引。

② 彩色分辨率(Color Resolution)域CR用来表示原始图像可用的每种基色的位数(实际值减1)。这个位数表示整个调色板的大小，而不是这幅图像使用的实际的颜色数。例如，如果该域的值CR＝3，说明原始图像可用每个基色有4位的调色板来生成彩色图像。

③ 彩色表排序标志(Sort Flag)域S用来表示全局彩色表(Global Color Table)中的颜色是否按重要性(或者称使用率)排序。如果S＝0，表示没有重要性排序；如果S＝1表示最重要的颜色排在前。这样做的目的是辅助颜色数比较少的解码器能够选择最好的颜色子集，在这种情况下解码器就可选择彩色表中开始段的彩色来显示图像。

④ 全局彩色表大小(Size of Global Color Table)域Size表示表示每个像素的位数，它用来计算全局彩色表(Global Color Table)中包含的字节数。在全局彩色表标志(Global Color Table Flag)域G＝0时就不需要计算，G＝1时就要计算彩色表的大小，具体计算见下文的**“3. 全局彩色表”**。

|7|6|5|4|3|2|1|0|
|----|----|----|----|----|----|----|----|
|Global Color Table Flag|Color Resolution|Sort Flag|Size of Global Color Table| | | | |

图6-04 逻辑屏幕描述块中的包装域结构

(2) 屏幕描述块中的第6个字节是背景颜色索引(Background Color Index)，它是彩色表的一个索引值，用来指定背景颜色。如果全局彩色表标志(Global Color Table Flag)域G＝0，这个域的值也设置为0。

(3) 像素宽高比(Pixel Aspect Ratio)域中的值是一个因数，是计算原始图像像素的宽高比的一个近似值。如果该域的值范围为1～255，如果不等于0，宽高比的近似值按下式计算：

Aspect Ratio = (Pixel Aspect Ratio + 15) / 64
像素宽高比(Pixel Aspect Ratio)定义成像素的宽度与高度之比，比值的范围在4:1～1:4之间，其增量为1/64。

**3. 全局彩色表**
由于一个GIF文件可以包含多幅彩色图像，每幅彩色图像也许都包含适合自身特点的彩色表，所以一个GIF文件可以有好几个彩色表。但归纳起来只有两类：全局彩色表(Global Color Table)或局部彩色表(Local Color Table)。全局彩色表可用于图像本身没有带彩色表的所有图像和无格式文本扩展块(Plain Text Extension)，而局部彩色表只用于紧跟在它后面的一幅图像。在处理全局彩色表和局部彩色表时需要注意下面一些规则。

① 如果GIF文件包含全局彩色表(Global Color Table)，而且要显示的图像本身又带有局部彩色表，那末显示该幅彩色图像时就用它自己的彩色表，而不用全局彩色表。在这种情况下，解码器就首先保存全局彩色表(Global Color Table)，然后使用局部彩色表(Local Color Table)来显示图像，最后再回复全局彩色表(Global Color Table)。

② 全局彩色表(Global Color Table)和局部彩色表(Local Color Table)都是可选择的。由于这个原因，解码器最好要保存全局彩色表(Global Color Table)，一直到出现另一个全局彩色表(Global Color Table)为止。这样做之后，对于包含完全没有彩色表的一幅或者多幅彩色图像的GIF文件就可以使用最后保存的全局彩色表(Global Color Table)进行处理。

③ 如果同类型的图像能够使用相同的彩色表来显示，编码器就要尽可能使用一个全局彩色表(Global Color Table)；如果没有彩色表可用，解码器就可以使用计算机系统提供的彩色表或者解码器自身的彩色表。

④ 全局彩色表(Global Color Table)存在与否由逻辑屏幕描述块(Logical Screen Descriptor)中字节5的全局彩色表标志(Global Color Table Flag )域G的值确定。如果存在，彩色表就紧跟在逻辑屏幕描述块(Logical Screen Descriptor)之后。彩色表的表项数目等于2(*n *+1)，其中*n*=b2b1b0，每个表项由3个字节组成，分别代表R、G、B的相对强度，因此彩色表的字节数就等于3×2(*n *+1)。彩色表的结构如图6-05所示。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|red intensity|0|红色索引 000|Byte|
|green intensity|1|绿色索引 000|Byte|
|blue intensity|2|蓝色索引 000|Byte|
|red intensity|3|红色索引 001|Byte|
|green intensity|4|绿色索引 001|Byte|
|blue intensity|5|蓝色索引 001|Byte|
|…|…|…||
|…|…|…||
|red intensity|745|红色索引 255|Byte|
|green intensity|746|绿色索引 255|Byte|
|blue intensity|767|蓝色索引 255|Byte|

**图6-05 彩色表结构**
局部彩色表与全局彩色表有相同的存储格式。
**4. 图像描述块**
GIF图像文件格式可包含数量不限的图像，而且也没有一个固定的存放顺序，仅用一个字节的图像分隔符(Image Separator)来判断是不是图像描述块。每一幅图像都由一个图像描述块(Image Descriptor)、可有可无的局部彩色表(Local Color Table)和图像数据组成。每幅图像必须要落在逻辑屏幕描述块(Logical Screen Descriptor)中定义的逻辑屏(Logical Screen)尺寸范围里。

图像描述块(Image Descriptor)之前可以有一个或者多个控制块，例如图形控制扩展块(Graphic Control Extension)，其后可以跟着一个局部彩色表(Local Color Table)。无论前后是否有各种数据块，图像描述块(Image Descriptor)总是带有图像数据。

图像描述块(Image Descriptor)的结构如图6-06所示。

|7|6|5|4|3|2|1|0|字节号|域的名称|类型|
|----|----|----|----|----|----|----|----|----|----|----|
|Image Separator|0|图像分隔符|Byte| | | | | | | |
|Image Left Position|1|图像左边位置|Unsigned| | | | | | | |
||2|(以像素为定单位)|| | | | | | | |
|Image Top Position|3|图像顶部位置|Unsigned| | | | | | | |
||4|(以像素为定单位)|| | | | | | | |
|Image Width|5|图像宽度|Unsigned| | | | | | | |
||6|(以像素为定单位)|| | | | | | | |
|Image Height|7|图像高度|Unsigned| | | | | | | |
||8|(以像素为定单位)|| | | | | | | |
||9|包装域|见图6-07| | | | | | | |

图6-06 图像描述块的结构

在图6-06中，图像分隔符(Image Separator)用来标识图像描述块的开始，该域包含固定的值：0x2C；图像左边位置(Image Left Position)是相对于逻辑屏幕(Logical Screen)最左边的列号，逻辑屏幕最左边的列好定义为0；图像顶部位置(Image Top Position) 是相对于逻辑屏幕(Logical Screen)顶部的行号，逻辑屏幕顶部的行号定义为0。

|7|6|5|4|3|2|1|0|
|----|----|----|----|----|----|----|----|
|Local Color Table Flag|Interlace Flag|Sort Flag|Reserved|Size of Local Color Table| | | |

图6-07 图像描述块中的包装域结构

图像描述块(Image Descriptor)中的第9个字节称为包装域(Packed Fields)字节，它的位结构如图6-07所示，它由5个子域组成：

① 局部彩色表标志(Local Color Table Flag )域L用来说明是否有局部彩色表存在。如果L＝1，表示有一个局部彩色表(Local Color Table)将紧跟在这个图像描述块(Image Descriptor)之后;如果G＝0，表示图像描述块(Image Descriptor)后面没有局部彩色表(Local Color Table)，该图像要使用全局彩色表(Global Color Table)。

② 交插显示标志(Interlace Flag)域I用来表示该图像是不是交插图像(Interlaced Images)。如果I＝0，表示该图像不是交插图像，如果I＝1表示该图像是交插图像。使用该位标志可知道图像数据是如何存放的。GIF文件格式定义了两种数据存储方式：一种是按图像行连续顺序存储，这个顺序与显示器上显示行的顺序相同；另一种按交插方式存储。交插图像按行分成如下所示的4组(Group)：

Group 1：每隔8行组成一组，从第0行开始显示 /第1遍交插

Group 2：每隔8行组成一组，从第4行开始显示 /第2遍交插

Group 3：每隔4行组成一组，从第2行开始显示 /第3遍交插

Group 4：每隔2行组成一组，从第1行开始显示 /第4遍交插

由于显示图像需要较长的时间，使用这种方法存放和显示图像数据，人们就可以在图像显示完成之前看到这幅图像的概貌，而不觉得显示时间长。图6-08说明了这种交插图像的存储和显示顺序。

|行号|像 点|交插遍次| | | |
|----|----|----|----|----|----|
|0|……………………………………|1||||
|1|……………………………………||||4|
|2|……………………………………|||3||
|3|……………………………………||||4|
|4|……………………………………||2|||
|5|……………………………………||||4|
|6|……………………………………|||3||
|7|……………………………………||||4|
|8|……………………………………|1||||
|9|……………………………………||||4|
|10|……………………………………|||3||
|11|……………………………………||||4|
|12|……………………………………||2|||
|13|……………………………………||||4|
|14|……………………………………|||3||
|15|……………………………………||||4|
|16|……………………………………|1||||
|17|……………………………………||||4|
|18|……………………………………|||3||
|19|……………………………………||||4|

图6-08 交插图像显示顺序

③ 彩色表排序标志(Sort Flag)域的含义与全局彩色表(Global Color Table)中(Sort Flag)域的含义相同。

④ 保留(Reserved)。

⑤ 局部彩色表大小(Size of Local Color Table)域的值用来计算局部彩色表(Global Color Table)中包含的字节数。
**5. 局部彩色表**
局部彩色表(Local Color Table)用于紧跟在它后面的图像。彩色表是否存在取决于图像描述块(Image Descriptor)中局部彩色表标志(Local Color Table Flag)位的设置。彩色表的结构和大小与全局彩色表(Global Color Table)完全相同。
**6. 表基图像数据**
GIF图像采用了LZW算法对实际的图像数据进行压缩。为了提高压缩编码的效率，对LZW编码器输出的代码采用可变长度码VLC(**v**ariable-length-**c**ode)，不是用位数高度的代码来表示输出，而且代表码字的位数是可变的。

表基图像数据(Table Based Image Data)由LZW最小代码长度(LZW Minimum Code Size)和图像数据(Image Data)组成，如图6-09所示。LZW最小代码长度域的值用来确定图像数据中LZW代码使用的初始位数。图像数据(Image Data)由数据子块(Data Sub-blocks)序列组成。

|7|6|5|4|3|2|1|0||域的名称|类型|
|----|----|----|----|----|----|----|----|----|----|----|
|LZW Minimum Code Size||LZW最小代码长度|Byte| | | | | | | |


|Image Data||图像数据|Data Sub-blocks|

图6-09 图像数据的存储格式

数据子块(Data Sub-blocks)的结构如图6-10所示，这是一个可变长度的数据块，其长度由块大小域(Block Size)域中的值确定，字节数在0～255之间。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|Block Size|0|块大小|Byte|
||1||Byte|
||||Byte|
|Data Values||数值|Byte|
||||Byte|
||…||…|
||…||…|
||||Byte|
||多||Byte|
||到||Byte|
||255||Byte|

图6-10 数据子块的结构

**7. 图形控制扩展块**
图形控制扩展块(Graphic Control Extension)包含处理图形描绘块时要使用的参数，它的结构如图6-11所示。现说明如下：

(1) 扩展导入符Extension Introducer)用于识别扩展块的开始，域中的值是一个数值等于0x21的固定值。

(2) 图形控制标签(Graphic Control Label)用于标识当前块是一个图形控制扩展块，域中的值是一个数值等于0xF9的固定值。

(3) 块大小(Block Size)用来说明该扩展块所包含字节数，该字节数是从这个块大小(Block Size)域之后到块结束符之间的字节数。

|7|6|5|4|3|2|1|0|字节号|域的名称|类型|
|----|----|----|----|----|----|----|----|----|----|----|
|Extension Introducer|0|扩展导入符|Byte| | | | | | | |
|Graphic Control Label|1|图形扩展标签|Byte| | | | | | | |
||||| | | | | | | |
|Block Size|0|块大小|Byte| | | | | | | |
|<Packed Fields>|1|包装域|See below| | | | | | | |
|Delay Time|2|延时时间|Unsigned| | | | | | | |
||||| | | | | | | |
|Transparent Color Index|3|透明彩色索引|Byte| | | | | | | |
||||| | | | | | | |
|Block Terminator|0|块结束符|Byte| | | | | | | |

图6-11 图像描述块的结构

(4) 包装域的结构如图6-12所示。处理方法(Disposal Method)规定图形显示之后译码器要用表6-03中所述方法进行处理。
**表6-03 包装域规定的处理方法**

|域值|处理方法|
|----|----|
|0|没有指定要做任何处理|
|1|不处理，图形留在原处|
|2|显示图形的区域必须要恢复成背景颜色|
|3|恢复成以前显示的图形|
|4～7|(未定义)|

用户输入标志(User Input Flag)域表示在继续处理之前是否需要用户输入响应。在延时时间(Delay Time)和用户输入标志(User Input Flag)都设置为1的情况下，继续处理的开始时间取决于用户响应输入在前还是延时时间结束在前。

|7|6|5|4|3|2|1|0|
|----|----|----|----|----|----|----|----|
|Reserved(保留)|Disposal Method(处理方法)|User Input Flag|Transparent Color Flag| | | | |

图6-12 图形控制扩展块的包装结构

(5) 透明(Transparency Flag)表示是否给出透明索引(transparency index)

(6) 延时时间(Delay Time)用来指定在图形显示之后继续处理数据流之前的等待时间，一百分之一秒为单位。

(7) 当且仅当透明标志位设置为1时，透明索引(Transparency Index)用来指示处理程序是否要修改显示设备上的相应象点。当且仅当透明标志位设置为1时，就要修改。

(8) 块结束符(Block Terminator)表示该图形控制扩展块(Graphic Control Extension)结束，它是由一个字节组成的数据块，该域的值是一个固定的值：0x00，因此称为零长度数据子块(zero-length Data Sub-block)。

**8. 无格式文本扩展块**
无格式文本扩展块(Plain Text Extension)包含文本数据和描绘文本所须的参数。文本数据用7位的ASCII字符编码并以图形形式显示。扩展块的结构如图6-13所示。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|Extension Introducer (0x21)|0|扩展导入符|Byte|
|Plain Text Label (0x01)|1|无格式文本标签|Byte|

|Block Size|0|块大小|Byte|
|----|----|----|----|
|Text Grid Left Position|1|文本网格左列位置|Unsigned|
||2|||
|Text Grid Top Position|3|文本网格顶行位置|Unsigned|
||4|||
|Text Grid Width|5|文本网格宽度|Unsigned|
||6|||
|Text Grid Height|7|文本网格高度|Unsigned|
||8|||
|Character Cell Width|9|字符单元宽度|Byte|
|Character Cell Height|10|字符单元高度|Byte|
|Text Foreground Color Index|11|文本颜色索引|Byte|
|Text Background Color Index|12|文本背景颜色索引|Byte|

|||||
|----|----|----|----|
|Plain Text Data||无格式文本数据|Data Sub-blocks|
|||||

图6-13 无格式文本扩展块结构

**9. 注释扩展块**
注释扩展块(Comment Extension)域的内容用来说明图形、作者或者其他任何非图形数据和控制信息的文本信息。

注释扩展块的结构如图6-14所示。其中的注释数据是序列数据子块(Data Sub-blocks)，每块最多255个字节，最少1个字节。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|Extension Introducer (0x21)|0|扩展导入符|Byte|
|Comment Label (0xFE)|1|注释标签|Byte|

|Comment Data|0|注释数据||
|----|----|----|----|
||||Data Sub-blocks|
||…|||
||N－1|||

|Block Terminator||块结束符|Byte|

图6-14 注释扩展块

**10. 应用扩展块**
应用扩展块(Application Extension)包含制作该图像文件的应用程序的相关信息，它的结构如图6-15所示。

|7 6 5 4 3 2 1 0|字节号|域的名称|数据类型|
|----|----|----|----|
|Extension Introducer (0x21)|0|扩展导入符|Byte|
|Extension Label (0xFF)|1|扩展标签|Byte|

|Block Size|0|块大小|Byte|
|----|----|----|----|
||1|||
||2|||
||3|||
|Application Identifier|4|应用程序标识符|8 Bytes|
||5|(程序名称)||
||6|||
||7|||
||8|||
||9|||
|Appl. Authentication Code|10|应用程序识别码|3 Bytes|
||11|||

|||||
|----|----|----|----|
|Application Data||应用数据|Data Sub-blocks|
|||||

|Block Terminator|0||Byte|

图6-15 应用扩展块

**11. GIF文件结束块**
结束块(GIF Trailer)表示GIF文件的结尾，它包含一个固定的数值：0x3B。它具有如图6-16所示的结构。

|7 6 5 4 3 2 1 0|域的名称|数据类型|
|----|----|----|
|GIF Trailer = 0x3B|GFI文件结束块|Byte|

图6-16 GIF文件结束块
** 速差表表6-04 GIF文件格式**** 速差表表6-04 GIF文件格式**

|块的名称|需要|标签|扩展|版本号.|
|----|----|----|----|----|
|Application Extension(应用扩展)|Opt. (*)|0xFF (255)|yes|89a|
|Comment Extension(注释扩展)|Opt. (*)|0xFE (254)|yes|89a|
|Global Color Table(全局彩色表)|Opt. (1)|none|no|87a|
|Graphic Control Extension(图形控制扩展)|Opt. (*)|0xF9 (249)|yes|89a|
|Header(文件头)|Req. (1)|none|no|N/A|
|Image Descriptor(图像描述)|Opt. (*)|0x2C (044)|no|87a (89a)|
|Local Color Table(局部彩色表)|Opt. (*)|none|no|87a|
|Logical Screen Descriptor(逻辑屏幕描述块)|Req. (1)|none|no|87a (89a)|
|Plain Text Extension(无格式文本扩展)|Opt. (*)|0x01 (001)|yes|89a|
|Trailer(文件结束)|Req. (1)|0x3B (059)|no|87a|

Unlabeled Blocks(无标号块)

|Header(文件头)|Req. (1)|none|no|N/A|
|----|----|----|----|----|
|Logical Screen Descriptor(逻辑屏幕描述块)|Req. (1)|none|no|87a (89a)|
|Global Color Table(全局彩色表)|Opt. (1)|none|no|87a|
|Local Color Table(局部彩色表)|Opt. (*)|none|no|87a|

Graphic-Rendering Blocks(图像描绘块)

|Plain Text Extension(无格式文本扩展)|Opt. (*)|0x01 (001)|yes|89a|
|----|----|----|----|----|
|Image Descriptor(图像描述块)|Opt. (*)|0x2C (044)|no|87a (89a)|

Control Blocks(控制块)

|Graphic Control Extension(图形控制扩展)|Opt. (*)|0xF9 (249)|yes|89a|

Special Purpose Blocks(专用块)

|Trailer(结束)|Req. (1)|0x3B (059)|no|87a|
|----|----|----|----|----|
|Comment Extension(注释扩展)|Opt. (*)|0xFE (254)|yes|89a|
|Application Extension(应用程序扩展)|Opt. (*)|0xFF (255)|yes|89a|

表中：Req. (1) 表示最多出现一次

Opt. (*) 出现次数大于等于0

3>PNG格式

PNG是20世纪90年代中期开始开发的图像文件存储格式，其目的是企图替代GIF和TIFF文件格式，同时增加一些GIF文件格式所不具备的特性。流式网络图形格式(Portable Network Graphic Format，PNG)名称来源于非官方的“PNG's Not GIF”，是一种位图文件(bitmap file)存储格式，读成“ping”。PNG用来存储灰度图像时，灰度图像的深度可多到16位，存储彩色图像时，彩色图像的深度可多到48位，并且还可存储多到16位的α通道数据。PNG使用从LZ77派生的无损数据压缩算法。

PNG文件格式保留GIF文件格式的下列特性：
- 
使用彩色查找表或者叫做调色板可支持256种颜色的彩色图像。 

- 
流式读/写性能(streamability)：图像文件格式允许连续读出和写入图像数据，这个特性很适合于在通信过程中生成和显示图像。 

- 
逐次逼近显示(progressive display)：这种特性可使在通信链路上传输图像文件的同时就在终端上显示图像，把整个轮廓显示出来之后逐步显示图像的细节，也就是先用低分辨率显示图像，然后逐步提高它的分辨率。 

- 
透明性(transparency)：这个性能可使图像中某些部分不显示出来，用来创建一些有特色的图像。 

- 
辅助信息(ancillary information)：这个特性可用来在图像文件中存储一些文本注释信息。 

- 
独立于计算机软硬件环境。 

- 
使用无损压缩。

PNG文件格式中要增加下列GIF文件格式所没有的特性：
- 
每个像素为48位的真彩色图像。 

- 
每个像素为16位的灰度图像。 

- 
可为灰度图和真彩色图添加α通道。 

- 
添加图像的γ信息。 

- 
使用循环冗余码(**c**yclic **r**edundancy **c**ode，CRC)检测损害的文件。 

- 
加快图像显示的逐次逼近显示方式。 

- 
标准的读/写工具包。 

- 
可在一个文件中存储多幅图像。

**文件结构**
PNG图像格式文件(或者称为数据流)由一个8字节的PNG文件署名(PNG file signature)域和按照特定结构组织的3个以上的数据块(chunk)组成。

PNG定义了两种类型的数据块，一种是称为关键数据块(critical chunk)，这是标准的数据块，另一种叫做辅助数据块(ancillary chunks)，这是可选的数据块。关键数据块定义了4个标准数据块，每个PNG文件都必须包含它们，PNG读写软件也都必须要支持这些数据块。虽然PNG文件规范没有要求PNG编译码器对可选数据块进行编码和译码，但规范提倡支持可选数据块。
**(1) PNG文件署名域**
8字节的PNG文件署名域用来识别该文件是不是PNG文件。该域的值是：

|十进制数|137|80|78|71|13|10|26|10|
|----|----|----|----|----|----|----|----|----|
|十六进制数|89|50|4e|47|0d|0a|1a|0a|

**(2) 数据块的结构**
每个数据块都由表6-07所示的的4个域组成。
**表6-07 PNG文件数据块的结构**

|**名称**|**字节数**|**说明**|
|----|----|----|
|Length(长度)|4字节|指定数据块中数据域的长度，其长度不超过(231－1)字节|
|Chunk Type Code(数据块类型码)|4字节|数据块类型码由ASCII字母(A-Z和a-z)组成|
|Chunk Data(数据块数据)|可变长度|存储按照Chunk Type Code指定的数据|
|CRC(循环冗余检测)|4字节|存储用来检测是否有错误的循环冗余码|

在表6-07中，CRC(**c**yclic **r**edundancy **c**heck)域中的值是对Chunk Type Code域和Chunk Data域中的数据进行计算得到的。CRC具体算法定义在ISO 3309和ITU-T V.42中，其值按下面的CRC码生成多项式进行计算：

x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
**数据块结构1. 关键数据块****数据块结构1. 关键数据块**
关键数据块中的4个标准数据块是：

(1) 文件头数据块IHDR(header chunk)：它包含有PNG文件中存储的图像数据的基本信息，并要作为第一个数据块出现在PNG数据流中，而且一个PNG数据流中只能有一个文件头数据块。

文件头数据块由13字节组成，它的格式如表6-08所示。
**表6-08 PNG文件头键数据块的结构**

|域的名称|字节数|说明|
|----|----|----|
|Width|4 bytes|图像宽度，以像素为单位|
|Height|4 bytes|图像高度，以像素为单位|
|Bit depth|1 byte|图像深度：索引彩色图像：1，2，4或8灰度图像：1，2，4，8或16真彩色图像：8或16|
|ColorType|1 byte|颜色类型：0：灰度图像, 1，2，4，8或162：真彩色图像，8或163：索引彩色图像，1，2，4或84：带α通道数据的灰度图像，8或166：带α通道数据的真彩色图像，8或16|
|Compression method|1 byte|压缩方法(LZ77派生算法)|
|Filter method|1 byte|滤波器方法|
|Interlace method|1 byte|隔行扫描方法：0：非隔行扫描1： Adam7(由Adam M. Costello开发的7 遍隔行扫描方法)|

(2) 调色板数据块PLTE(palette chunk)：它包含有与索引彩色图像((indexed-color image))相关的彩色变换数据，它仅与索引彩色图像有关，而且要放在图像数据块(image data chunk)之前。真彩色的PNG数据流也可以有调色板数据块，目的是便于非真彩色显示程序用它来量化图像数据，从而显示该图像。调色板数据块结构如表6-09所示。
**表6-09 调色板数据块结构**

|域的名称|字节数|说明|
|----|----|----|
|Red|1 byte|0 = 黑，255 = 红|
|Green|"> 0 = 黑，255 = 绿| |
|Blue|1 byte|0 = 黑，255 = 蓝|

调色板实际是一个彩色索引查找表，它的表项数目可以是1～256中的一个数，每个表项有3字节，因此调色板数据块所包含的最大字节数为768。

(3) 图像数据块IDAT(image data chunk)：它存储实际的数据，在数据流中可包含多个连续顺序的图像数据块。

(4) 图像结束数据IEND(image trailer chunk)：它用来标记PNG文件或者数据流已经结束，并且必须要放在文件的尾部。

除了表示数据块开始的IHDR必须放在最前面， 表示PNG文件结束的IEND数据块放在最后面之外，其他数据块的存放顺序没有限制。
**2. 辅助数据块**
PNG文件格式规范制定的10个辅助数据块是：

(1) 背景颜色数据块bKGD(background color)。

(2) 基色和白色度数据块cHRM(primary chromaticities and white point)。所谓白色度是指当R＝G＝B＝最大值时在显示器上产生的白色度。

(3) 图像γ数据块gAMA(image gamma)。

(4) 图像直方图数据块hIST(image histogram)。

(5) 物理像素尺寸数据块pHYs(physical pixel dimensions)。

(6) 样本有效位数据块sBIT(significant bits)。

(7) 文本信息数据块tEXt(textual data)。

(8) 图像最后修改时间数据块tIME (image last-modification time)。

(9) 图像透明数据块tRNS (transparency)。

(10) 压缩文本数据块zTXt (compressed textual data)。
**3. 数据块摘要**
关键数据块、辅助数据块和专用公共数据块(special-purpose public chunks)综合在表6-10中。
**表6-10 PNG文件格式中的数据块**

|**数据块符号**|**数据块名称**|**多数据块**|**可选否**|**位置限制**|
|----|----|----|----|----|
|IHDR|文件头数据块|否|否|第一块|
|cHRM|基色和白色点数据块|否|是|在PLTE和IDAT之前|
|gAMA|图像γ数据块|否|是|在PLTE和IDAT之前|
|sBIT|样本有效位数据块|否|是|在PLTE和IDAT之前|
|PLTE|调色板数据块|否|是|在IDAT之前|
|bKGD|背景颜色数据块|否|是|在PLTE之后IDAT之前|
|hIST|图像直方图数据块|否|是|在PLTE之后IDAT之前|
|tRNS|图像透明数据块|否|是|在PLTE之后IDAT之前|
|oFFs|(专用公共数据块)|否|是|在IDAT之前|
|pHYs|物理像素尺寸数据块|否|是|在IDAT之前|
|sCAL|(专用公共数据块)|否|是|在IDAT之前|
|IDAT|图像数据块|是|否|与其他IDAT连续|
|tIME|图像最后修改时间数据块|否|是|无限制|
|tEXt|文本信息数据块|是|是|无限制|
|zTXt|压缩文本数据块|是|是|无限制|
|fRAc|(专用公共数据块)|是|是|无限制|
|gIFg|(专用公共数据块)|是|是|无限制|
|gIFt|(专用公共数据块)|是|是|无限制|
|gIFx|(专用公共数据块)|是|是|无限制|
|IEND|图像结束数据|否|否|最后一个数据块|

5>PCX格式
是PC 画笔的图像文件格式。PCX的图像深度可选为1、4、8 bit。由于这种文件格式出现较早，它不支持真彩色。PCX文件采用RLE行程编码，文件体中存放的是压缩后的图像数据。因此，将采集到的图像数据写成PCX文件格式时，要对其进行RLE编码；而读取一个PCX文件时首先要对其进行RLE解码，才能进一步显示和处理。

PCX
格式用于写是相对较简单的，但用于读就比较棘手，除非知道被解码图像的很多细节内
容（如位深度和调色板等）。因此，以下的阐述都是基于最坏的情况，即读取一个其特
性和年代都未确定的PCX文件，所有的数均是little－endian（Intel）格式，即LSB在先
。
字节0，Zsoft标志
总是十进制值160，即十六进制A0。
字节1，版本号，
一定程度上不可靠的文件内容指南，见前面一节“变体”的讨论。
字节2，编码
到目前为止，总是为1。当前编码（压缩）方法只有一种，即在下面“位图数据），一节
中讲述的运行长度法。
字节3，每像素位数
实际上是每个位平面的每像素位数，可能的值是1、2、4或8。
字节4-11，图像大小
图像大小由最小的和最大的极限给出。通常的下限是0。所有的极限均用16位无符号整数
表示，单位为像素。图像大小可以这样计算：XSIZE＝Xmax―Xmin＋l；
YSIZE＝Ymax－Ymin＋l，单位为像素。
字节12-15，以每英寸点数为单位的水平和垂直分辨率
这两个16位的数字有点古怪，它们对于定义所存储图像不起任何作用，但是，当它们与
图像大小组合起来加以考虑时，能产生出被扫描图像的原始大小，或者被打印图像的希
望大小，以英寸为单位。
字节16-63，头调色板
这一字段看上去只适用于带有单一位平面、16种或更少的颜色以及版本号为2的文件（参
见下面“解释数据的关键”口节）。使用时，调色板拥有16组三元组的单字节调色板值
。
字节65，色彩平面
PCX图像可以是单色彩，也可以是多个色彩平面的（参见第一章）。头的这个字节给出色
彩平面数，它是正确翻译PCX文件的关键。
字节66，每行字节数
实际是每个平面的每行的字节数—存储末压缩图像一个扫描行的一个色彩平面所需要内
存字节数，它总是偶数。
字节68，头调色板翻译
1=彩色／单色；2=灰度。Paintbrush IV或Paintbrush IV PIus中不使用它。
字节70~73，视屏屏幕大小，X和Y
只被Paintbrush IV和Paintbrush IV PIus使用；并不是必不可少的，但是对于产生正确
的外观比例（防止压缩型失真）可能有用。
三、位图数据
如果没有使用调色板，那么数据是实际的像素值；否则，它们是指向调色板值的指针。
在后一种情况下，数据给出的是相对于所使用的调色板的起始处的偏移（比如在三字节
的三元组值中，1=字节3）。
当数据是实际的像素值时，它们按色彩平面和扫描行存储。例如，对于三种颜色红、绿
和蓝（RGB），数据格式为：
（第0行：）RRRRRR．．．GGGGGG．．．BBBBBB．．．
（第1行：）RRRRRR．．．GGGGGG．．．BBBBBB．．．
如果有两个平面，那么色彩是任选的；如果有三个平面，则颜色为RGB；如果使用四个平
面，则它们是符合IBM
CGA／EGA标准的单个位的平面：红、绿、蓝和光强（RGBI）。光强位只是给像素以一种
名义上较高的亮度。
当数据是指向某调色板指针时，它们就组成一个完整的图像平面（也就是说，它们不会
分解成单独色彩平面）。然后数据按如下方式简单地编排（字符P代表各种指针值）：
（行0：）PPPPPP
（行1：）PPPPPPP
P的长度取决于深度，以每平面的每像素位数表示。例如，如果深度为4位，则P就是半个
字节长。
所有情况下，在扫描行之间都有编码隔断标志。但是，在一个扫描行中的色彩平面间没
有编码隔断标志。同样，也没有分隔符可用来标识扫描行的结束（虽然一个扫描行可能
是也可能不是用额外的零作为结束）。也就是说，在扫描行之间不会有行号（虽然这里
写出来了），也不会有空字符、空格、回车、换行或其他的字符。
不论要记录的是何种类型的位图数据，都使用同样的运行长度压缩方法，下面给出恢复
算法（当前普遍使用的基于调色板的图像只有一个平面）。


PCX图象文件格式最早出现于Zsoft公司开发的PC Paintbrush绘图软件，由于该绘图软件功能强大并成功移植到Windows操作系统，加上PCX是最早支持彩色的图象格式之一，PCX成为目前比较流行的图象格式。对于开发图象浏览、处理软件的程序员来讲，如何读取、保存PCX图象格式是最为基本的话题,根据自己对PCX图象格式理解，开发了相应的代码，希望对读者有用，由于篇幅限制，在此对文件格式不予介绍，读者可以参考相关数目。
/***********************************************************************
* 函数名称：LoadPCXLine(PPCXHEAD ppcxHdr, LPBYTE ppcxImg, LPBYTE ppcxBits) const
*
* 参数：PPCXHEAD ppcxHdr －指向PCXHEAD结构的指针!NULL,导入BitPlane,BytePerLine,=>clScanLineSize
* LPBYTE ppcxImg －指向PCX图象区指针!NULL,RLE压缩编码,位置递增+=rec。
*     调用之前获得首地址指针: 
* LPBYTE ppcxBits －指向DIB数据区的指针,按扫描行(scanline)长度递增
*
* 返回：UINT rec －返回每行解压以后的字节数目
*
* 说明：根据PCX图象数据指针，对RLE进行解码
***********************************************************************/
UINT CPcxImage::LoadPCXLine(PPCXHEAD ppcxHdr, LPBYTE ppcxImg, LPBYTE ppcxBits) const
{
 ASSERT(ppcxHdr!=NULL&&ppcxImg!=NULL&&ppcxBits!=NULL);China It Power . ComYHzsF

 // because in bitmap bits order, it's blue=>green=>red
 // however pcx is red=>green=>blue so use decrease order
 //－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
 UINT lPos(0),  // 记录存入ppcxBits的总数
   iX(0), // 记录每个位平面字节序号
   rec(0); // 读取_ppcxImg_ 字节序号
 for ( int bp=ppcxHdr->BitPlane-1; bp >= 0;  bp-- )
 {
  // RLE 解码=======
  iX=0;

  while ( iX<ppcxHdr->BytePerLine )
  {
   BYTE uiValue = ppcxImg[rec++];
   if ( (uiValue & 0xc0) == 0xc0 )  // 判断高位字节是否设置 0xc0
   {
    uiValue = uiValue & 0x3f ;  // 计算重复
    BYTE Color = ppcxImg[rec++]; // 提取颜色

    // 存放到内存DIB
    for ( BYTE bRepeat=0; bRepeat < uiValue; bRepeat++ )
    {
     ppcxBits[(iX++)*ppcxHdr->BitPlane+bp] = Color;
     lPos++;
    }
   }
   else
   {
    ppcxBits[(iX++)*ppcxHdr->BitPlane+bp] = uiValue;
    lPos++;
   }
  }

 }

 return rec;
}

/****************************************************************************
* 函数名称：PackPCXLine(PPCXHEAD ppcxHdr, LPBYTE ppcxImg, LPBYTE ppcxBits) const
*
* 参数：PPCXHEAD ppcxHdr －指向PCXHEAD结构的指针!NULL,导入BitPlane,BytePerLine,=>clScanLineSize
* LPBYTE ppcxBits －指向DIB数据区的指针,按扫描行(scanline)长度递增
* LPBYTE ppcxImg －指向PCX图象区指针!NULL,RLE压缩编码。
*     调用之前声明: LPBYTE ppcxImg=new BYTE[2*BitPlane*BytePerLine]
*
* 返回：UINT rec －返回每行压缩以后的字节数目
*
* 说明：根据DIB图象数据指针，进行RLE编码(经过测试算法很完善,支持256和24bit真彩色)
****************************************************************************/
UINT CPcxImage::PackPCXLine(PPCXHEAD ppcxHdr, LPBYTE ppcxBits, LPBYTE ppcxImg) const
{
 //----------------------------------------
 // RLE压缩
 ASSERT(ppcxHdr!=NULL && ppcxBits!=NULL && ppcxImg!=NULL);China It Power . ComYHzsF

 BYTE i(1);
 UINT lPos(0), rec(0);China It Power . ComYHzsF

 // RLE编码，最大重复<=63
 for(int bp=ppcxHdr->BitPlane-1; bp>=0; bp--)
 {
  lPos=0;  // 处理到的RGB序列

  while(lPos<ppcxHdr->BytePerLine)  // 等价小于图象宽度
  {
   i=1; // 重置步长-1

   //----------------->以下代码检查
   while((ppcxBits[(i-1+lPos)*ppcxHdr->BitPlane+bp]==ppcxBits[(i+lPos)*ppcxHdr->BitPlane+bp])
    &&((lPos+i)<ppcxHdr->BytePerLine)&&(i<63)) i++;
   if(i>1 && i<64)
   {
    // 表明当前象素位置开始存在i个重复象素值，依次写入PCX图象数据Buffer
    // 1.重复次数
    ppcxImg[rec++]=i|0xc0;
    // 2.象素值
    ppcxImg[rec++]=ppcxBits[lPos*ppcxHdr->BitPlane+bp];

    lPos+=i; // lPos－记录当前扫描行中已经处理的字节数
    // rec －记录当前已经写入PCX文件的字节数
   }
   else
   {
    // 表明当前象素位置开始不存在重复象素值
    // 象素值大于0xc0(192)，写标志0xc1
    if((ppcxBits[lPos*ppcxHdr->BitPlane+bp]&0xc0)==0xc0) ppcxImg[rec++]=0xc1;
    ppcxImg[rec++]=ppcxBits[lPos*ppcxHdr->BitPlane+bp]; lPos++;
   }
  }
 }

 // 写图象数据结束
 return rec;
}

＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
调用如下：
 1.// RLE解码－－－－－－－－－－－－－> 已经包含8、24bit图象
 for( int iY=0; iY<=ppcxHdr->YMax; iY++ )
 {
  ZeroMemory(ppcxBits, clScanLineSize);
  ppcxImg+=LoadPCXLine(ppcxHdr, ppcxImg, ppcxBits); // 读取扫描行数据
  ppcxBits+=clScanLineSize;
 }
 ppcxHdr是指向PCXHEAD结构(128BYTE)的指针，ppcxBits是存放解码后图象数据的buffer，ppcxImg是指向pcx图象文件
中图象数据的指针，此处随扫描行递增。完成功能是从pcx文件中解码图象数据到windows位图格式的图象数据。

 2.// RLE压缩－－－－－－－－－－－－－> 已经包含8、24bit图象
 // 最坏情况下申请2倍的缓冲,相邻都不重复,并且都大于0xc0
 LPBYTE ppcxImg=new BYTE[2*pcxHdr.BitPlane*pcxHdr.BytePerLine]; // 存放临时扫描行
 UINT rec(0); // 计数器，写如PCX文件字节数
 for( int iY=0; iY<=pcxHdr.YMax; iY++ )
 {
  ZeroMemory(ppcxImg,2*pcxHdr.BitPlane*pcxHdr.BytePerLine);
  rec=PackPCXLine(&pcxHdr, ppcxBits, ppcxImg);

  // DIB 扫描行递增
  ppcxBits+=clScanLineSize;

  pFile->Write(ppcxImg,rec);
 }
 delete []ppcxImg;
具体参数大致同1.ppcxImg为临时RLE压缩后的buffer。


6> TIFF格式
TIFF（Tag Image File Format）文件是由Aldus和Microsoft公司为扫描仪和桌上出版系统研制开发的一种较为通用的图像文件格式。TIFF格式灵活易变，它又定义了四类不同的格式：TIFF－B适用于二值图像；TIFF－G适用于黑白灰度图像；TIFF－P适用于带调色板的彩色图像；TIFF－R适用于RGB真彩图像。TIFF支持多种编码方法，其中包括RGB无压缩、RLE压缩及JPEG压缩等。

TIF是一种国际上非常流行的适于各种电脑和操作系统的图形文件格式，目前国际上流行
的很多软件都支持TIF格式。一般扫描器所配备的软件都直接或间接地用到TIF图形文件
格式，还有一些软件系统，例如：
Windows系统下的PhotoStyler都把TIF作为图形的存取格式，TIF格式也可以转换为Win
dows的BMP等其它格式。
TIFF格式有三级体系，从高到低依次为：文件头，一个或多个称为IFD的包含桥记指针的
目录以及数据。体系的最高层是文件头，只包含三个表项：
（1）一个代码，指明字节顺序（低字节在先还是高字节在先）。
（2）一个把文件标识为TIFF文件的代码号。
（3）一个指向图像文件目录（Image FileDirectory，IFD）的指针。
这里是一个tif文件的文件头:
4D 4D 2A 00 00 00 00 08
IFD提供一系列的指针（索引），这些指针告诉我们各种有关的数据字段在文件中的开始
位置，并给出每个字母的数据类型（例如，1字节整型）及长度。这种方法允许数据字段
定位在文件的任何地方，可以是差不多任意长度，并可以包含大量信息。例如，一个指
针可能指向关于彩色调色板数据的一个786字节字段；另一个可能指向扫描仪的一条64字
节灰度修正曲线。在一个文件中可能有几个相关的图像，这时可以有几种IFD。IFD的最
后一个表项指向任何一个后续的IFD。
每个指针都有一个标记值指明所指向的数据字段类型的一个代码号。TIFF
规范列出了所有正式的、非专用的标记号，给予它们有用的名字（如SamplesPerPixel，
十进制代码为277），并描述这个指针所识别的数据，告知数据的组织方法。
1．优点一种有竞争力的位元映射格式，为以后扩充说明留有余地。
2．缺点有许多衍生格式，但井非所有的衍生格式都得到所有应用程序支持。
3．变体最早的1984年1．0版格式和1989年2．0版格式，应用程序设计师可以注册自己
私有的衍生格式。
TGA格式由AT&T首先引用，用于支持他们的Targa 和Truevision 公司的 ATVIS
TA
图形捕捉卡。此格式已经成为数字化图形以及由光跟踪和其它应用程序所产生的高质量
图形常用格式。TrueVision公司的TGA文件格式已广泛地被国际上的图形工业所接受。有
许多图形工作人员喜欢按个人习惯储存图形文件，并且只存数据，不存格式，使得大量
的图形文件成为个人的专用产品，无法交流，因此，有必要按照标准格式来储存图形文
件，
TGA文件格式就是目前国际上比较流行的图形文件储存格式。
Targa文件以一个固定大小的文件头开始，然后是可变长度的图像标识符（ID）、色彩对
应表和图像。ID字段的偏移为18，紧跟在文件头的后面。
多字节值的存储是低字节（tss）为先，即Intel格式。除了字节对齐之外，值和段均没
有填充或对齐。
图像可以彩色映射的。有两种类型的色彩对应表图像：“伪彩色”（pseudo
color），这种图像中每个像素值从色彩对应表中选取一个单独值，“直接彩色”（dir
ect
color），这种图像中每个像素包含分别查询的红、绿和蓝值。像素为实际的红、绿和蓝
值的图像称为“真色彩图”（True
Color），为灰度值的称为“黑白图”（black-and-white）。
图像总是按行存储，但行可以按从上到下或从下到上的顺序，而且对像素可队从左到右
或从右到左存储，在实际图像中，像素几乎总是从左到右存储。有些型号的扫描仪从上
到下扫描，而另外一些扫描仪则从下到上扫描，所以至今还没有占统治地位的行顺序。

1．优点与设备无关，文件可以很好地组织结构；由于使用向量描述图形的性能，文件
可以比相应的位元映射小很多。
2．缺点语意结构与Windows图形模型关系太密切；文件比较复杂。
3．变体 Windows 3．x将Windows 2．x转换文件格式中加入了新的记录类型，但不使用
新类型的转换文件应该与早期版本相容。





