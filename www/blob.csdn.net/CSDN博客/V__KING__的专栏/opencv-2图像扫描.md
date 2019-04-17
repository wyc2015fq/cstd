# opencv-2图像扫描 - V__KING__的专栏 - CSDN博客





2015年12月08日 17:20:32[v__king__](https://me.csdn.net/V__KING__)阅读数：738








[http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#howtoscanimagesopencv](http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#howtoscanimagesopencv)

# cv::LUT(I, lookUpTable, J);

I 是输入参数， J是输出的结果，返回 Mat 类型 
**使用方法**：

```
Mat lookUpTable(1, 256, CV_8U);
uchar* p = lookUpTable.data;
for( int i = 0; i < 256; ++i)
    p[i] = table[i];

t = (double)getTickCount();

for (int i = 0; i < times; ++i)
    LUT(I, lookUpTable, J);// I is input,  J is output

namedWindow("LUT", WINDOW_AUTOSIZE );
imshow("LUT", J);
```

# Mat::begin();  Mat::end();

```
MatIterator_<Vec3b> it, end;// a pointer it ,end, it changed the I Object
for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
{
    (*it)[0] = table[(*it)[0]];
    (*it)[1] = table[(*it)[1]];
    (*it)[2] = table[(*it)[2]];
}
```

# p = I.ptr(i);

ptr函数也是模板函数，我们将会用到的ptr函数声明。此函数返回指定的数据行的首地址。

```matlab
int i,j;
 uchar* p;
 for( i = 0; i < nRows; ++i)
 {
     p = I.ptr<uchar>(i);//ptr函数也是模板函数，我们将会用到的ptr函数声明如代码段5所示。此函数返回指定的数据行的首地址
     for ( j = 0; j < nCols; ++j)
     {
         p[j] = table[p[j]];
     }
 }
 return I;
```



