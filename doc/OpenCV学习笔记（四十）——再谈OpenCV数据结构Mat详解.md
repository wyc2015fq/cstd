# OpenCV学习笔记（四十）——再谈OpenCV数据结构Mat详解

2011年12月28日 10:33:32

yang_xian521

阅读数：154022

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									



我记得开始接触OpenCV就是因为一个算法里面需要2维动态数组，那时候看core这部分也算是走马观花吧，随着使用的增多，对Mat这个结构越来越喜爱，也觉得有必要温故而知新，于是这次再看看Mat。

Mat最大的优势跟STL很相似，都是对内存进行动态的管理，不需要之前用户手动的管理内存，对于一些大型的开发，有时候投入的lpImage内存管理的时间甚至比关注算法实现的时间还要多，这显然是不合适的。除了有些嵌入式场合必须使用c语言，我任何时候都强烈像大家推荐Mat。

Mat这个类有两部分数据。一个是matrix header，这部分的大小是固定的，包含矩阵的大小，存储的方式，矩阵存储的地址等等。另一个部分是一个指向矩阵包含像素值的指针。



```cpp
Mat A, C; // creates just the header parts
A = imread(argv[1], CV_LOAD_IMAGE_COLOR); // here we’ll know the method used (allocate matrix)
Mat B(A); // Use the copy constructor
C = A; // Assignment operator
```

 需要注意的是，

copy这样的操作只是copy了矩阵的matrix header和那个指针，而不是矩阵的本身，也就意味着两个矩阵的数据指针指向的是同一个地址，需要开发者格外注意

。比如上面这段程序，A、B、C指向的是同一块数据，他们的header不同，但对于A的操作同样也影响着B、C的结果。刚刚提高了内存自动释放的问题，那么当我不再使用A的时候就把内存释放了，那时候再操作B和C岂不是很危险。不用担心，OpenCV的大神为我们已经考虑了这个问题，是在最后一个Mat不再使用的时候才会释放内存，咱们就放心用就行了。 



如果想建立互不影响的Mat，是真正的复制操作，需要使用函数clone()或者copyTo()。

说到数据的存储，这一直就是一个值得关注的问题，Mat_<uchar>对应的是CV_8U，Mat_<uchar>对应的是CV_8U，Mat_<char>对应的是CV_8S，Mat_<int>对应的是CV_32S，Mat_<float>对应的是CV_32F，Mat_<double>对应的是CV_64F，对应的数据深度如下：

> • CV_8U - 8-bit unsigned integers ( 0..255 )
>
> • CV_8S - 8-bit signed integers ( -128..127 )
>
> • CV_16U - 16-bit unsigned integers ( 0..65535 )
>
> • CV_16S - 16-bit signed integers ( -32768..32767 )
>
> • CV_32S - 32-bit signed integers ( -2147483648..2147483647 )
>
> • CV_32F - 32-bit ﬂoating-point numbers ( -FLT_MAX..FLT_MAX, INF, NAN )
>
> • CV_64F - 64-bit ﬂoating-point numbers ( -DBL_MAX..DBL_MAX, INF, NAN )

这里还需要注意一个问题，很多OpenCV的函数支持的数据深度只有8位和32位的，所以要少使用CV_64F，但是vs的编译器又会把float数据自动变成double型，有些不太爽。

还有个需要注意的问题，就是流操作符<<对于Mat的操作，仅限于Mat是2维的情况。

还有必要说一下Mat的存储是逐行的存储的。

再说说Mat的创建，方式有两种，罗列一下：1.调用create（行，列，类型）2.Mat（行，列，类型（值））。例如：



```cpp
// make a 7x7 complex matrix filled with 1+3j.
Mat M(7,7,CV_32FC2,Scalar(1,3));
// and now turn M to a 100x60 15-channel 8-bit matrix.
// The old content will be deallocated
M.create(100,60,CV_8UC(15));
```

 要是想创建更高维的矩阵，要写成下面的方式 



```cpp
// create a 100x100x100 8-bit array
int sz[] = {100, 100, 100};
Mat bigCube(3, sz, CV_8U, Scalar::all(0));
```

 对于矩阵的行操作或者列操作，方式如下：（

注意对列操作时要新建一个Mat，我想应该跟列地址不连续有关

）

```cpp
// add the 5-th row, multiplied by 3 to the 3rd row
M.row(3) = M.row(3) + M.row(5)*3;
// now copy the 7-th column to the 1-st column
// M.col(1) = M.col(7); // this will not work
Mat M1 = M.col(1);
M.col(7).copyTo(M1);
```

 下面的东西就比较狂暴了，

对于外来的数据，比如你从别的地方接受了一幅图片，但可以不是Mat结构的，而只有一个数据的指针，看看接下来的代码是如何应付的

，重点哦，亲 



```cpp
void process_video_frame(const unsigned char* pixels,
int width, int height, int step)
{
Mat img(height, width, CV_8UC3, pixels, step);
GaussianBlur(img, img, Size(7,7), 1.5, 1.5);
}
```

 亲，有木有很简单！！！ 

还有一种快速初始化数据的办法，如下：



```cpp
double m[3][3] = {{a, b, c}, {d, e, f}, {g, h, i}};
Mat M = Mat(3, 3, CV_64F, m).inv();
```

 也可以把原来的IplImage格式的图片直接用Mat（IplImage）的方式转成Mat结构，也可以像Matlab一样调用zeros()、ones()、eye()这样的函数进行初始化。 

如果你需要提前释放数据的指针和内存，可以调用release()。

对于数据的获取，当然还是调用at<float>(3, 3)这样的格式为最佳。其他的方法我甚少尝试，就不敢介绍了。

最后要提的一点是关于Mat的表达式，这个也非常多，加减乘除，转置求逆，我怎么记得我以前介绍过呢。那就不多说啦~