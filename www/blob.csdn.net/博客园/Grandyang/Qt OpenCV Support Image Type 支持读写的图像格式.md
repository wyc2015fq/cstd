# Qt OpenCV Support Image Type 支持读写的图像格式 - Grandyang - 博客园







# [Qt OpenCV Support Image Type 支持读写的图像格式](https://www.cnblogs.com/grandyang/p/4285233.html)







Qt 支持的图片格式如下：
|Format|Description|Qt's support|
|----|----|----|
|BMP|Windows Bitmap|Read/write|
|GIF|Graphic Interchange Format (optional)|Read|
|JPG|Joint Photographic Experts Group|Read/write|
|JPEG|Joint Photographic Experts Group|Read/write|
|PNG|Portable Network Graphics|Read/write|
|PBM|Portable Bitmap|Read|
|PGM|Portable Graymap|Read|
|PPM|Portable Pixmap|Read/write|
|TIFF|Tagged Image File Format|Read/write|
|XBM|X11 Bitmap|Read/write|
|XPM|X11 Pixmap|Read/write|



OpenCV 支持的图片格式如下：

```
Windows bitmaps - *.bmp, *.dib (always supported)
    JPEG files - *.jpeg, *.jpg, *.jpe (see the Notes section)
    JPEG 2000 files - *.jp2 (see the Notes section)
    Portable Network Graphics - *.png (see the Notes section)
    Portable image format - *.pbm, *.pgm, *.ppm (always supported)
    Sun rasters - *.sr, *.ras (always supported)
    TIFF files - *.tiff, *.tif (see the Notes section)
```



下面是1024X768分辨率的图片在各种格式下的尺寸，参见[这里](http://www.cnblogs.com/xrwang/archive/2008/08/26/1276353.html)。
|图片格式|色彩位数|图片质量|文件大小（KB）|备注|
|----|----|----|----|----|
|bmp|8|-|769|-|
|bmp|24|-|2359|-|
|jpg|-|0%|104|-|
|jpg|-|25%|124|-|
|jpg|-|50%|156|-|
|jpg|-|75%|220|-|
|jpg|-|100%|639|-|
|gif|8|-|225|-|
|tiff|-|-|1880|-|
|png|-|-|868|用ACDSEE保存|
|png|1|-|73|用PhotoShop保存|
|png|6|-|194|用PhotoShop保存|
|png|7|-|267|用PhotoShop保存|
|png|8|-|332|用PhotoShop保存|
|png|24|-|895|用PhotoShop保存|



从上表可以看出，jpg格式在图片大小和质量之间得到了很好的平衡；即使选择0%的图片质量，仍然可以接受，而图片大小只有104KB。












