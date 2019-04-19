# opencv其他常用数据结构 - LC900730的博客 - CSDN博客
2017年09月28日 16:54:34[lc900730](https://me.csdn.net/LC900730)阅读数：113
### 定义和输出二维点
```cpp
Point2f p(6,2);
cout<<"[二维点]p="<<p<<";\n"<<endl;
//[二维点]p=[6,2]
```
### 定义和输出三维点
```cpp
Point3f p3f(8,2，0);
cout<<"[二维点]p3f="<<p3f<<";\n"<<endl;
//[二维点]p=[8,2,0];
```
### 定义和输出基于Mat的std::vector
```cpp
vector<float> v;
v.push_back(3);
v.push_back(5);
v.push_back(7);
cout<<"[基于Mat的vector]shortvec="<<p3f<<";\n"<<endl;
//[基于Mat的vector]p=[3;5;7];
```
#### 定义和输出std::vector点
vector存放二维点Point2f为例：
```cpp
vector<Point2f> points(20);
for(size_t i=0;i<points.size();++i)
points[i]=Point2f((float)(i*5),float(i%7));
cout<<"[二维点向量]points="<<points<<";";
```
## 点：Point类
```
1.Point point;
        point.x=10;
        point.y=8;
    2.Point point=Point(10,8);
在opencv中：
    typedef Point_<int> Point2i;
    typedef  Point2i Point;
    typedef Point_<float> Point2f;
```
## 颜色类：Scalar
Scalar()表示具有4个元素的数组。在OpenCV中被大量用于传递像素值，如RGB颜色值。而RGB颜色值为3个参数值，其实对于Scalar函数来说，如果用不到第四个参数，不需要写出来。若只写三个参数，OpenCV会认为我们就想表示三个参数。 
Scalar(a,b,c);
## 尺寸表示：Size类
```
typedef Size_<int> Size2i;
typedef Size2i Size;
```
Size(5,5); //构造出来的宽高都为5
## 矩形的表示：Rect类
Rect类成员变量有x y width height，左上角、右下角、矩形的宽、高。
- Size()返回值为Size；
- area()返回值为面积；
- contains(Point)判断点是否在矩形内；
- inside(Rect)判断矩形是否在该矩形内；
- tl()左上角坐标；
- br()右下角坐标
## 颜色空间变换：cvtColor()函数
可以实现RGB颜色向HSV、HSI等颜色空间的转换，也可以转换为灰度图像。
