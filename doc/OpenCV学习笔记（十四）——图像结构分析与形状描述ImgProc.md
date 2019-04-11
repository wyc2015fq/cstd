# OpenCV学习笔记（十四）——图像结构分析与形状描述ImgProc

2011年11月01日 09:19:30

yang_xian521

阅读数：31777

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

OpenCV支持大量的轮廓、边缘、边界的相关函数，相应的函数有moments、HuMoments、findContours、drawContours、approxPolyDP、arcLength、boundingRect、contourArea、convexHull、fitEllipse、fitLine、isContourConvex、minAreaRect、minEnclosingCircle、mathcShapes、pointPolygonTest。还有一些c版本的针对老版本的数据结构的函数比如cvApproxChains、cvConvexityDefects。这里先介绍一些我用过的函数，以后用到再陆续补充。

OpenCV里支持很多边缘提取的办法，可是如何在一幅图像里得到轮廓区域的参数呢，这就需要用到findContours函数，这个函数的原型为：



```cpp
//C++: 
void findContours(InputOutputArray image, OutputArrayOfArrays contours, OutputArray hierarchy, int mode, int method, Point offset=Point())
void findContours(InputOutputArray image, OutputArrayOfArrays contours, int mode, int method, Point offset=Point())
```

 这里介绍下该函数的各个参数： 



输入图像image必须为一个2值单通道图像

contours参数为检测的轮廓数组，每一个轮廓用一个point类型的vector表示

hiararchy参数和轮廓个数相同，每个轮廓contours[ i ]对应4个hierarchy元素hierarchy[ i ][ 0 ]  ~hierarchy[ i ][ 3 ]，分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，该值设置为负数。

mode表示轮廓的检索模式

> CV_RETR_EXTERNAL表示只检测外轮廓
>
> CV_RETR_LIST检测的轮廓不建立等级关系
>
> CV_RETR_CCOMP建立两个等级的轮廓，上面的一层为外边界，里面的一层为内孔的边界信息。如果内孔内还有一个连通物体，这个物体的边界也在顶层。
>
> CV_RETR_TREE建立一个等级树结构的轮廓。具体参考contours.c这个demo









method为轮廓的近似办法

> CV_CHAIN_APPROX_NONE存储所有的轮廓点，相邻的两个点的像素位置差不超过1，即max（abs（x1-x2），abs（y2-y1））==1
>
> CV_CHAIN_APPROX_SIMPLE压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标，例如一个矩形轮廓只需4个点来保存轮廓信息
>
> CV_CHAIN_APPROX_TC89_L1，CV_CHAIN_APPROX_TC89_KCOS使用teh-Chinl chain 近似算法







offset表示代表轮廓点的偏移量，可以设置为任意值。对ROI图像中找出的轮廓，并要在整个图像中进行分析时，这个参数还是很有用的。

具体应用参考sample文件夹下面的squares.cpp这个demo

findContours后会对输入的2值图像改变，所以如果不想改变该2值图像，需创建新mat来存放，findContours后的轮廓信息contours可能过于复杂不平滑，可以用approxPolyDP函数对该多边形曲线做适当近似

contourArea函数可以得到当前轮廓包含区域的大小，方便轮廓的筛选

findContours经常与drawContours配合使用，用来将轮廓绘制出来。其中第一个参数image表示目标图像，第二个参数contours表示输入的轮廓组，每一组轮廓由点vector构成，第三个参数contourIdx指明画第几个轮廓，如果该参数为负值，则画全部轮廓，第四个参数color为轮廓的颜色，第五个参数thickness为轮廓的线宽，如果为负值或CV_FILLED表示填充轮廓内部，第六个参数lineType为线型，第七个参数为轮廓结构信息，第八个参数为maxLevel

得到了复杂轮廓往往不适合特征的检测，这里再介绍一个点集凸包络的提取函数convexHull，输入参数就可以是contours组中的一个轮廓，返回外凸包络的点集

还可以得到轮廓的外包络矩形，使用函数boundingRect，如果想得到旋转的外包络矩形，使用函数minAreaRect，返回值为RotatedRect；也可以得到轮廓的外包络圆，对应的函数为minEnclosingCircle；想得到轮廓的外包络椭圆，对应的函数为fitEllipse，返回值也是RotatedRect，可以用ellipse函数画出对应的椭圆

如果想根据多边形的轮廓信息得到多边形的多阶矩，可以使用类moments，这个类可以得到多边形和光栅形状的3阶以内的所有矩，类内有变量m00，m10，m01，m20，m11，m02，m30，m21，m12，m03，比如多边形的质心为 x = m10 / m00，y = m01 / m00。

如果想获得一点与多边形封闭轮廓的信息，可以调用pointPolygonTest函数，这个函数返回值为该点距离轮廓最近边界的距离，为正值为在轮廓内部，负值为在轮廓外部，0表示在边界上。