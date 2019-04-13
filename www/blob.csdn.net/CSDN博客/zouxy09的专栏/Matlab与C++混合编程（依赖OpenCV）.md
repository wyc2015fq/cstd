
# Matlab与C++混合编程（依赖OpenCV） - zouxy09的专栏 - CSDN博客


2014年03月05日 16:20:48[zouxy09](https://me.csdn.net/zouxy09)阅读数：53377


**Matlab与C++混合编程（依赖OpenCV）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
之前在运行别人论文的代码的时候，经常有遇到Matlab与C++混合编程的影子。实际上就是通过Matlab的Mex工具将C++的代码编译成Matlab支持调用的可执行文件和函数接口。这样一方面可以在Matlab中利用已经编写好的函数，尽管这个函数是用C++编写的。实现了交流无国界，没有江山一统的谁，只有四海之内皆兄弟的豪气。另一方面，取C++所长补己之短。Matlab擅长矩阵运算，但对循环操作的效率不及C++来得高效，例如Hilbert矩阵的创建。所以对于具有大循环的运算，可以借C++之力来完成。
看到它的魅力，之前也一直想学下，可惜机缘不对。但在昨天缘分就到了。我需要用到一个论文给出来的代码，但是它的代码是C++的，而且还依赖了OpenCV的库，基于Linux平台。这与实验室给我定出来的平台有很大的不同，我们是得统一基于Windows + Matlab来实现的，这样组内各个同学的工作才好统一。所以没办法了，就得把这个原作者的代码编译成Matlab支持的可执行文件。
**一、初级**
在使用MATLAB编译C/C++代码时，我们需要修改C/C++代码，在里面添加Matlab能支持的函数接口。这样Matlab才能调用它。然后再通过Matlab的Mex工具来编译它。下面就具体的举例子说明这两个步骤。
假设我们有一个很简单的C++代码，实现的就是两个double型数的加法：
mexAdd.cpp

```cpp
#include <iostream>
using namespace std;
double add(double x, double y)
{
    return x + y;
}
```
**1、修改代码文件**
1）添加头文件mex.h
在我们的c++文件开头处添加头文件：
\#include"mex.h"
2）添加接口函数mexFunction()
mexFunction的定义为：
void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
}
首先，这个函数是没有返回值的。它不是通过返回值把c++代码的计算结果传回Matlab的，而是通过对参数plhs的赋值。例如我们在Matlab中，调用这个add函数一般是这样：
>> a = 0.5; b = 0.8;
>> c = add(a, b);
那mexFunction怎么将输入参数a和b传入给c++的add函数，然后就怎么把计算结果返回给c呢？这些粗重活全部通过mexFunction的四个参数来实现：
nlhs: 感觉是number of left hand size parameters，也就是Matlab调用语句左边的变量个数，实际上就是需要返回给Matlab的返回值变量有多少个。例如上面c = add(a, b);就只有一个返回参数c，所以nlhs就是1；
plhs: 感觉是pointer of left hand size parameters，也就是函数返回参数的指针。但它是一个指针数组。换句话说，它是一个数组，每个元素是个指针，每个指针指向一个数据类型为mxArray的返回参数。例如上面c = add(a, b);就只有一个返回参数c，所以该数组只有一个指针，plhs[0]指向的结果会赋值给c。
nrhs: 这个是number of right hand size parameters，也就是Matlab调用语句右边的变量个数。例如上面c = add(a, b)，它给c++代码传入了两个参数a和b，所以nrhs为2；
prhs:这个是pointer of right hand size parameters，和plhs类似，因为右手面有两个自变量，即该数组有两个指针，prhs[0]指向了a，prhs[1]指向了b。要注意prhs是const的指针数组，即不能改变其指向内容。
因为Matlab最基本的单元为array，无论是什么类型也好，如有doublearray、 cell array、struct array……所以a,b,c都是array，b = 1.1便是一个1x1的double array。而在C语言中，Matlab的array使用mxArray类型来表示。所以就不难明白为什么plhs和prhs都是指向mxArray类型的指针数组（参考资料[1]）。
那mexFunction函数的函数体要怎么写呢？怎么样通过这个接口函数将Matlab的参数和c++代码中的相对应的参数联系起来呢？我们先把这个代码全部展现出来。
最后的mexAdd.cpp是这样：
mexAdd.cpp

```cpp
#include "opencv2/opencv.hpp"
#include "mex.h"
double add(double x, double y)
{
    return x + y;
}
 
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	double *a;
    double b, c;
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    a = mxGetPr(plhs[0]);
    b = *(mxGetPr(prhs[0]));
    c = *(mxGetPr(prhs[1]));
    *a = add(b, c);
}
```
mexFunction的内容是什么意思呢？我们知道，如果在Matlab中这样调用函数时：
>> output = add(0.5, 0.8);
在未涉及具体的计算时，output的值是未知的，是未赋值的。所以在具体的程序中，我们建立一个1x1的实double矩阵（使用 mxCreateDoubleMatrix函数，其返回指向刚建立的mxArray的指针），然后令plhs[0]指向它。接着令指针a指向plhs [0]所指向的mxArray的第一个元素（使用mxGetPr函数，返回指向mxArray的首元素的指针）。同样地，我们把prhs[0]和prhs [1]所指向的元素（即0.5和0.8）取出来赋给b和c。于是我们可以把b和c作自变量传给函数add，得出给果赋给指针a所指向的mxArray中的元素。因为a是指向plhs[0]所指向的mxArray的元素，所以最后作输出时，plhs[0]所指向的mxArray赋值给output，则 output便是已计算好的结果了。
实际上mexFunction是没有这么简单的，我们要对用户的输入自变量的个数和类型进行测试，以确保输入正确。如在add函数的例子中，用户输入char array便是一种错误了。
从上面的讲述中我们总结出，MEX文件实现了一种接口，把C语言中的计算结果适当地返回给Matlab罢了。当我们已经有用C编写的大型程序时，大可不必在 Matlab里重写，只写个接口，做成MEX文件就成了。另外，在Matlab程序中的部分计算瓶颈（如循环），可通过MEX文件用C语言实现，以提高计算速度（参考资料[1]）。
**2、编译修改后的c++文件**
文件修改完后，我们需要将他编译，生成Matlab支持的可执行文件。这里需要的是Matlab自带的Mex工具。但在编译器，我们需要配置下这个工具，告诉它你要采用什么编译器来编译我们的c/c++代码。在Matlab中运行：
>> mex -setup
就会出现叫你选择一个默认的编译器。例如我这里是叫选择Matlab自带的Lcc或者我自己在电脑上安装的Microsoft Visual C++ 2010。一般都是选择后者。配置这个就可以编译了。编译也有以下几种情况：
>> mex XXX.cpp
>> mex X1.cpp X2.cpp X3.cpp %多个cpp文件，且有依赖。生成的库名字叫X1
>> mex -O X1.cpp  %大写O选项，优化编译
>> mex -largeArrayDims X1.cpp %对64位系统，通过这个选项来指定使用处理大容量数组的API。因为Matlab与C++之间的接口是以32位系统作为标准的，这就导致了人们在处理大容量数据时没办法利用C和C++语言的速度优势。但对64位系统来说，系统资源一般都比32位系统要充足，所以指定该接口，让它对大容量数据处理更游刃有余。
还有一些编译选项，和gcc一样。例如-I指定额外需要include的目录，-L指定额外需要连接的库的目录，-l指定额外需要链接的库等。
对于我们的程序就简单了。在MATLAB命令窗口输入以下命令：mexmexAdd.cpp，即可编译成功。编译成功后，在同文件夹下会出现一个同名的，但后缀是mexw32（32位的系统）或者mexw64（64位的系统）的文件，例如mexAdd.mexw32。然后在Matlab中就可以直接调用它来运算了：
>> ans = mexAdd(0.5, 0.8);
**二、进阶**
上面我们针对的是处理标量的情况，也就是数a,b或者c。这节我们让它处理二维数组，也就是图像。为了验证，我们很傻瓜地完成以下功能：
>> [grayImage] =RGB2Gray('imageFile.jpeg');
也就是将一个图像文件名，传递给c++的代码，然后c++代码将这个图像读入，再转成灰度图，然后返回给Matlab。而c++代码里面的图像读入和灰度转换的操作通过调用OpenCV的库函数来实现。是不是很傻瓜呢？因为Matlab已经有实现同样功能的函数了。对，没错，就是多此一举。但我们只是为了说明二维数组的传递过程，没有什么用意。不过，如果要计算两个图像的光流的话，Matlab可能就真正需要OpenCV的帮助了。
另外，因为cpp文件要链接OpenCV的库，所以为了统一或者规范编译工程，我写了一个make.m文件，它的功能类似于Makefile，实际上就实现了mex编译这个工程时候的编译规则。具体可以看后面的代码，然后就知道在里面做了什么了。
首先是RGB2Gray.cpp代码：

```cpp
// Interface: convert an image to gray and return to Matlab
// Author : zouxy
// Date   : 2014-03-05
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
#include "opencv2/opencv.hpp"
#include "mex.h"
using namespace cv;
/*******************************************************
Usage: [imageMatrix] = RGB2Gray('imageFile.jpeg');
Input: 
	a image file
OutPut: 
	a matrix of image which can be read by Matlab
**********************************************************/

void exit_with_help()
{
	mexPrintf(
	"Usage: [imageMatrix] = DenseTrack('imageFile.jpg');\n"
	);
}
static void fake_answer(mxArray *plhs[])
{
	plhs[0] = mxCreateDoubleMatrix(0, 0, mxREAL);
}
void RGB2Gray(char *filename, mxArray *plhs[])
{
	// read the image
	Mat image = imread(filename);
	if(image.empty()) {
		mexPrintf("can't open input file %s\n", filename);
		fake_answer(plhs);
		return;
	}
	
	// convert it to gray format
	Mat gray;
	if (image.channels() == 3)
		cvtColor(image, gray, CV_RGB2GRAY);
	else
		image.copyTo(gray);
	
	// convert the result to Matlab-supported format for returning
	int rows = gray.rows;
	int cols = gray.cols;
	plhs[0] = mxCreateDoubleMatrix(rows, cols, mxREAL);
	double *imgMat;
    imgMat = mxGetPr(plhs[0]);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			*(imgMat + i + j * rows) = (double)gray.at<uchar>(i, j);
	
	return;
}
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs == 1)
	{
		char filename[256];
		mxGetString(prhs[0], filename, mxGetN(prhs[0]) + 1);
		if(filename == NULL)
		{
			mexPrintf("Error: filename is NULL\n");
			exit_with_help();
			return;
		}
		RGB2Gray(filename, plhs);
	}
	else
	{
		exit_with_help();
		fake_answer(plhs);
		return;
	}
}
```
和上面的相比，里面多了几个东西。第一个就是传入参数的测试，看看Matlab传入的参数是否存在错误，还包括了些异常处理。第二个就是帮助信息。第三个就是主要的实现函数了。只有OpenCV的读图像和灰度转换这里就不讲了，就是两个函数的调用。关键的地方还是如果把一个图像，也就是二维数组，传递给mexFunction的参数，让它返回给Matlab。实际上，我们只要清楚一点：
plhs[0] = mxCreateDoubleMatrix(2, 3,mxREAL);
这个函数建立的矩阵的指针plhs[0]是按照列的方式来存储的。假设imgMat是它的指针，那么*(imgMat+1)就是矩阵元素[1, 0]，*(imgMat+2)就是矩阵元素[0, 1]，*(imgMat+4)就是矩阵元素[0, 2]。上面的代码就是按照这个方式，将图像gray中像素值赋值给参数plhs[0]相应的位置（实际上也许可以直接内存拷贝，但因为里面是指针操作，涉及到局部变量gray的销毁问题，所以就简单的用上面的笨但稳当的方式来实现了）。
好了，下面是make.m文件。里面需要获取你的电脑的系统版本是32还是64位的，来选择编译选项。然后添加OpenCV的相关配置。如果您需要使用使用，请修改成您的OpenCV的相关目录。然后给出一个需要编译的文件的列表。最后分析这个列表，加上编译选项，用mex来编译列表里面的所有文件。

```cpp
%// This make.m is for MATLAB
%// Function: compile c++ files which rely on OpenCV for Matlab using mex
%// Author : zouxy
%// Date   : 2014-03-05
%// HomePage : http://blog.csdn.net/zouxy09
%// Email  : zouxy09@qq.com
%% Please modify your path of OpenCV
%% If your have any question, please contact Zou Xiaoyi
% Notice: first use "mex -setup" to choose your c/c++ compiler
clear all;
%-------------------------------------------------------------------
%% get the architecture of this computer
is_64bit = strcmp(computer,'MACI64') || strcmp(computer,'GLNXA64') || strcmp(computer,'PCWIN64');

%-------------------------------------------------------------------
%% the configuration of compiler
% You need to modify this configuration according to your own path of OpenCV
% Notice: if your system is 64bit, your OpenCV must be 64bit!
out_dir='./';
CPPFLAGS = ' -O -DNDEBUG -I.\ -ID:\OpenCV_64\include'; % your OpenCV "include" path
LDFLAGS = ' -LD:\OpenCV_64\lib';					   % your OpenCV "lib" path
LIBS = ' -lopencv_core240 -lopencv_highgui240 -lopencv_video240 -lopencv_imgproc240';
if is_64bit
	CPPFLAGS = [CPPFLAGS ' -largeArrayDims'];
end
%% add your files here!
compile_files = { 
	% the list of your code files which need to be compiled
	'RGB2Gray.cpp'
};

%-------------------------------------------------------------------
%% compiling...
for k = 1 : length(compile_files)
    str = compile_files{k};
    fprintf('compilation of: %s\n', str);
    str = [str ' -outdir ' out_dir CPPFLAGS LDFLAGS LIBS];
    args = regexp(str, '\s+', 'split');
    mex(args{:});
end
fprintf('Congratulations, compilation successful!!!\n');
```
**三、使用方法和结果**
**1、编译**
直接在Matlab中运行make.m。即可生成RGB2Gray.mexw64。然后在Matlab中运行：
>> img = RGB2Gray(‘d:\test.jpg’);
>> imshow(uint8(img));
即可显示转换结果，如图：
![](https://img-blog.csdn.net/20140305162005921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**注：**以上Matlab的说明都是在你的cpp文件所在目录下。
**四、参考资料**
[1][如何写mexFunction函数](http://blog.sina.com.cn/s/blog_6ec1700e0100rqlh.html)
[2][matlab用mex编译cpp文件](http://blog.renren.com/share/1197609117/6969417628)


