# opencv at类型总结 - KinboSong的博客 - CSDN博客
2017年06月08日 10:05:14[songjinbo3](https://me.csdn.net/KinboSong)阅读数：436
自从[OpenCV](http://lib.csdn.net/base/opencv)推出了Mat后越来越像是Matlab了，使用起来方便了很多，但是，在用at方法访问Mat时，如何选用合适的typename类型来访问相应的Mat元素是个头疼的问题。
比如：
int Height = 100;
int Width = 100;
Mat  ImageMat8U(Height, Width, CV_8UC1);
for(int i=0; i<Height; i++)
{
for(int j=0; j<Width;j++)
{
ImageMat8U.at<uchar>(i, j) = (uchar)0; //编译可以通过，运行也正确
ImageMat8U.at<int>(i, j) = (uchar)0; //编译可以通过，运行时会发生asser错误
}
}
通常如果选错了typename，编译仍然可以通过，但是运行时总会发生assert错误，原因是无法通过depth检查。这里对不同类型的Mat使用at方法时对应的typename做个总结：
CV_8U=0: bool或者uchar 
CV_8S=1: schar或者char
CV_16U=2: ushort
CV_16S=3: short 
CV_32S=4: int或者unsigned
CV_32F=5: float
CV_64F=6: double
实际上数据depth的检查涉及到了一个模板类：
template<typename _Tp> class CV_EXPORTS DataDepth {};
这个类的特化就指定了Mat类型与typename类型之间的对应关系：
template<> class DataDepth<bool> { public: enum { value = CV_8U, fmt=(int)’u’ }; };
template<> class DataDepth<uchar> { public: enum { value = CV_8U, fmt=(int)’u’ }; };
template<> class DataDepth<schar> { public: enum { value = CV_8S, fmt=(int)’c’ }; };
template<> class DataDepth<char> { public: enum { value = CV_8S, fmt=(int)’c’ }; };
template<> class DataDepth<ushort> { public: enum { value = CV_16U, fmt=(int)’w’ }; };
template<> class DataDepth<short> { public: enum { value = CV_16S, fmt=(int)’s’ }; };
template<> class DataDepth<int> { public: enum { value = CV_32S, fmt=(int)’i’ }; };
// this is temporary solution to support 32-bit unsigned integers
template<> class DataDepth<unsigned> { public: enum { value = CV_32S, fmt=(int)’i’ }; };
template<> class DataDepth<float> { public: enum { value = CV_32F, fmt=(int)’f’ }; };
template<> class DataDepth<double> { public: enum { value = CV_64F, fmt=(int)’d’ }; };
template<typename _Tp> class DataDepth<_Tp*> { public: enum { value = CV_USRTYPE1, fmt=(int)’r’ }; };
我是个新手，如果说的不对还希望路过的大大不要笑话。

