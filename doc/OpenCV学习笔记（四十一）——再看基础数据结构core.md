# OpenCV学习笔记（四十一）——再看基础数据结构core

2011年12月28日 12:51:40

yang_xian521

阅读数：39874

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

记得我在[OpenCV学习笔记（四）——新版本的数据结构core](http://http//blog.csdn.net/yang_xian521/article/details/6905244)里面讲过新版本的数据结构了，可是我再看这部分的时候，我发现我当时实在是看得太马虎了。对于新版本的数据结构，我再说说。

Point_类不用多言，里面两个成员变量x，y。Point_<int>就是Point2i，也是Point，Point_<float>就是Point2f，Point_<double>就是Point2d。

Point3_类不太常用，跟Point_类差不太多，成员变量x，y，z。

Size_类成员变量width、height。Size_<int>就是Size2i，也是Size，Size_<float>就是Size2f，大家就要不臆断出来个Size2d啥的让编译器发懵了。

Rect_类有些意思，成员变量x、y、width、height，分别为左上角点的坐标和矩形的宽和高。常用的成员函数有Size()返回值为一个Size，area()返回矩形的面积，contains(Point)用来判断点是否在矩形内，inside(Rect)函数判断矩形是否在该矩形内，tl()返回左上角点坐标，br()返回右下角点坐标。值得注意的是，如果想求两个矩形的交集，并集，可以用如下格式

```cpp
Rect rect = rect1 & rect2;
Rect rect = rect1 | rect2;
```

如果想

让矩形平移操作，缩放操作

，甚至可以这样写 



```cpp
Rect rectShift = rect + point;
Rect rectScale = rect + size;
```

是不是很妙，可以大大减轻你的代码量。 

Matx其实是个轻量级的Mat，必须在使用前规定好大小，比如一个2*3的float型的可以声明为Matx23f。我想很容易理解的。

Vec是Matx的一个派生类，就是一个1维的Matx，跟vector很相似。比如想声明一个10个数据的float数组，可以写成Vec2f。

这样就很容易引出一个大家经常使用的数据结构了，Scalar_类，这个类其实就是一个Vec4x的一个变种，大家常用的Scalar其实就是Scalar_<double>。这样一说，大家是不是就很容易理解了，为啥很多函数的参数输入可以是Mat，也可以是Scalar了。其实OpenCV定义的InputMat，outputMat参数格式，以上的这几种数据结构都可以作为参数的。

接下来介绍一个有意思的类Range，大家可能用的不多，对它不熟悉，其实它就是为了使OpenCV的使用更像Matlab而产生的。比如Range::all()其实就是Matlab里的符号：或者...。而Range(a, b)其实就是Matlab中的a：b。有趣吧，注意a，b都需要是int型的哦，亲。

Ptr类我就不太敢介绍了，是智能指针，我也没有用过，文档里说是很类似大名鼎鼎的Boost库里的shared_ptr。希望以后有机会用一下再拿出来分享心得。

最后出场的还是最重量级的Mat，介绍几个比较重要的成员变量flag（就是我之前说过的header里的结构信息，深度信息，通道数），dims是Mat的维数，要求大于等于2，rows和cols参数代表2维矩阵的行数列数（对于更高维的矩阵，这两个参数都是-1），还有个比较常用的参数应该uchar* data，是Mat的数据指针（比较暴力的同学可以直接调用它好了，不推荐），还有个参数refconst，我理解应该就是我上一讲提到的释放内存的时候要判断这个矩阵是否是最后一个被使用的，这个参数应该就是控制跟当前矩阵结构相关的个数的。