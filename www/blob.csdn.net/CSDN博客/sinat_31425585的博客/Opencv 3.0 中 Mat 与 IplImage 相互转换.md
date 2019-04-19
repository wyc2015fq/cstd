# Opencv 3.0 中 Mat 与 IplImage *相互转换 - sinat_31425585的博客 - CSDN博客
2017年08月02日 21:18:45[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：174
     参考opencv官方文档：[点击打开链接](http://lib.csdn.net/article/opencv/32238)
    直接贴过来：
```cpp
(1) IplImage 转 Mat:
IplImage* image = cvLoadImage( "lena.jpg");  
Mat mat=cvarrToMat(image);
(2)Mat转IplImage:
IplImage img = IplImage(mat);
```
