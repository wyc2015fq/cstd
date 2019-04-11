# OpenCV学习笔记（五十六）——InputArray和OutputArray的那些事core

2012年07月17日 14:37:49

yang_xian521

阅读数：46167

更多

 								所属专栏： 																[OpenCV与数字图像处理](https://blog.csdn.net/column/details/19251.html) 																 							

 									

看过OpenCV源代码的朋友，肯定都知道很多函数的接口都是InputArray或者OutputArray型的，这个接口类还是很强大的，今个就来说说它们的那些事。

InputArray这个接口类可以是Mat、Mat_<T>、Mat_<T, m, n>、vector<T>、vector<vector<T>>、vector<Mat>。也就意味着当你看refman或者源代码时，如果看见函数的参数类型是InputArray型时，把上诉几种类型作为参数都是可以的。

有时候InputArray输入的矩阵是个空参数，你只需要用cv：：noArray（）作为参数即可，或者很多代码里都用cv：：Mat（）作为空参。

这个类只能作为函数的形参参数使用，不要试图声明一个InputArray类型的变量

如果在你自己编写的函数中形参也想用InputArray，可以传递多类型的参数，在函数的内部可以使用_InputArray：：getMat（）函数将传入的参数转换为Mat的结构，方便你函数内的操作；必要的时候，可能还需要_InputArray：：kind（）用来区分Mat结构或者vector<>结构，但通常是不需要的。例如：



```cpp
void myAffineTransform(InputArray _src, OutputArray _dst, InputArray _m)
{
 
	Mat src = _src.getMat(), m = _m.getMat();
	CV_Assert( src.type() == CV_32FC2 && m.type() == CV_32F && m.size() == Size(3, 2) );
	_dst.create(src.size(), src.type());
	Mat dst = _dst.getMat();
	for( int i = 0; i < src.rows; i++ )
		for( int j = 0; j < src.cols; j++ )
		{
			Point2f pt = src.at<Point2f>(i, j);
			dst.at<Point2f>(i, j) = Point2f(m.at<float>(0, 0) * pt.x +	m.at<float>(0, 1) *	pt.y + m.at<float>(0, 2);
		}
}
```

 至于有的源代码里使用

InputArrayOfArrays

作为形参，不用慌张，其实它和InputArray是一样一样一样的。 

OutputArray是InputArray的派生类。使用时需要注意的问题和InputArray一样。和InputArray不同的是，需要注意在使用_OutputArray：：getMat（）之前一定要调用_OutputArray：：create（）为矩阵分配空间。可以用_OutputArray：：needed（）来检测输出的矩阵是否需要被计算。有时候传进去的参不是空就不需要计算

还有就是OutputArrayOfArrays、InputOutputArray、InputOutputArrayOfArrays都是OutputArray的别名而已