# 常见图片格式：PNG，JPG/JPEG，BMP - Koma Hub - CSDN博客
2018年06月14日 10:04:35[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：363

## `PNG` - Portable Network Graphics - 便携式网络图形
百度百科:[https://baike.baidu.com/item/png/174154?fr=aladdin](https://baike.baidu.com/item/png/174154?fr=aladdin)
- 便携式网络图形（`Portable Network Graphics`）是一种无损压缩的位图片形格式.
- 其设计目的是试图替代GIF和TIFF文件格式，同时增加一些GIF文件格式所不具备的特性。
- PNG的名称来源于“可移植网络图形格式(Portable Network Graphic Format，PNG)”，也有一个非官方解释`“PNG's Not GIF”`。
- PNG使用从`LZ77`派生的无损数据压缩算法，一般应用于JAVA程序、网页或S60程序中，原因是它压缩比高，生成文件体积小。
8字节的PNG文件署名域用来识别该文件是不是PNG文件。该域的值是：
十进制数: `137 80 78 71 13 10 26 10` 十六进制数: `89 50 4e 47 0d 0a 1a 0a`
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/png-jpeg-bmp.md#jpgjpeg---joint-photographic-experts-group---%E8%81%94%E5%90%88%E6%91%84%E5%BD%B1%E4%B8%93%E5%AE%B6%E7%BB%84)`JPG/JPEG` - Joint Photographic Experts GROUP - 联合摄影专家组
百度百科:[https://baike.baidu.com/item/jpg](https://baike.baidu.com/item/jpg)
- jpg全名是`JPEG`，是图片的一种格式。
- JPEG图片以24位颜色存储单个位图。
- JPEG是与平台无关的格式，支持最高级别的压缩，不过，这种压缩是有损耗的。
- 渐近式 JPEG 文件支持交错。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/png-jpeg-bmp.md#bmp---bitmap---%E4%BD%8D%E5%9B%BE)`BMP` - Bitmap - 位图
百度百科:[https://baike.baidu.com/item/BMP/35116?fr=aladdin](https://baike.baidu.com/item/BMP/35116?fr=aladdin)
典型的`BMP`图像文件由四部分组成：
- 1：位图头文件数据结构，它包含BMP图像文件的类型、显示内容等信息；
- 2：位图信息数据结构，它包含有BMP图像的宽、高、压缩方法，以及定义颜色等信息；
- 3：调色板，这个部分是可选的，有些位图需要调色板，有些位图，比如真彩色图（24位的BMP）就不需要调色板；
- 4：位图数据，这部分的内容根据BMP位图使用的位数不同而不同，在24位图中直接使用`RGB`，而其他的小于24位的使用调色板中颜色索引值。

