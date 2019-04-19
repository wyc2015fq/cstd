# linux下查找某文件的find 命令 - 三少GG - CSDN博客
2012年11月14日 19:52:44[三少GG](https://me.csdn.net/scut1135)阅读数：1089
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
                
通用格式:find pathname -options [-print -exec -ok]
例子:
find / -name filename 再根目录里面搜索文件名为filename的文件
find /etc -name *s*在目录里面搜索带有s的文件
find /etc -name *S 在目录里面搜索以s结尾的文件
find /etc -name s*在目录里面搜索以s开头的文件
find / -amin -10在系统中搜索最后１０分钟访问的文件
find / -atime -2查找在系统中最后４８小时访问的文件
find / -empty 查找在系统中为空的文件或者是文件夹
find / -group groupname 查找在系统中属于groupname的文件
find / -mmin -5查找在系统中最后５分钟修改过的文件
find / -mtime -1查找在系统中最后２４小时修改过的文件
find /－nouser查找在系统中属于费用户的文件
find / -user username 查找在系统中属于username的文件
find / -ctime -1查找在系统中最后２４小时被改变状态的文件
find / -fstype type查找在系统中文件类型为？的文件
find / -user user１name -or -user user２name查找在系统中属于user1name或着属于user2name的文件
find / -user user1name -and -user2name在系统中查找既属于user1name又属于user2name用户的文件．
**find /usr -name "*avcodec*" -print**
///////////
**sudo ln -s i386-linux-gnu/libavcodec.so.53 libavcodec.so**
**硬链接（hardlink）**
命令ln soursedest可以为文件source创建一个名为dest的硬链接。两个文件指向同一块数据块，所以对任意文件修改都会反映到其它所有被链接的文件上。如果source被删除了，dest仍然有效，可以被使用。
**软链接（symboliclink）**
命令ln -s sourcedest可以为文件source创建一个名为dest的软链接。软链接类似于Windows上的快捷方式。一旦source不存在了，那么dest的文件也会失效
///// 解决方案！！！
CC := gcc
CFLAGS :=  -msse2 -D HAVE_SSE2 -D HAVE_LIBJPEG -D HAVE_LIBPNG -D HAVE_GSL -D HAVE_FFTW3 -D HAVE_LIBLINEAR -D HAVE_CBLAS -D USE_OPENMP -fopenmp -D HAVE_AVCODEC -D HAVE_AVFORMAT -D HAVE_SWSCALE
LDFLAGS := -lm -ljpeg -lpng -lz -lgsl -lfftw3f -lfftw3 -llinear **-lgslcblas **-lgomp -lavcodec -lavformat -lswscale 
灵感来自： 
1.Try to write `-lgsl` before `-lgslcblas`. I know that's stupid, but for me it works...
2.原因可能为：
“
但是在某些情况下，我们可能需要安装“旧”版本的工具，或者说安装指定/特定版本的工具，该怎么办呢？还好，Homebrew 已经提供了这类的支持。
今天安装 **gsl** 这个 rubygem ，编译本地库时失败了。我机器的环境是：
- **gsl**-**1.15**
- ruby-1.9.3p125
其中 **gsl** 是使用 
```
brew install **gsl**
```
 安装的，安装了最新的**1.15**版本。执行
`gem install **gsl**` 时的一条错误信息是：
conflicting types for ‘**gsl**_matrix_complex_equal’
于是顺着这条错误信息 Google ，发现很多人都遇到这个问题了。有人说，应该安装 **gsl**-**1.14** 而不是
**gsl**-**1.15** 。而最新的 gem 版本为
**1.14**.7 。看来很可能是版本不兼容。
所以我需要给 **gsl** 降级。 由于
**gsl** 是通过 Homebrew 安装的，所以需要找到安装特定版本工具的方法。于是在 [Stackoverflow](http://stackoverflow.com/questions/3987683/homebrew-install-specific-version-of-formula) 上找到了方法。其实很简单：
”
参考资料：
http://hi.baidu.com/jerry916/item/84b3a5d2368a1ce1b3f77788
**4、BLAS、CBLAS**：Basic Linear Algebra Subprograms，即基础线性代数子程序库，里面拥有大量已经编写好的关于线性代数运算的程序。主要是用于向量和矩阵计算的高性能数学库。本身BLAS是用Fortran写的，为了方便 C/C++程序的使用，就有了BLAS的C接口库CBLAS。BLAS的主页是http://www.netlib.org/blas/，CBLAS的下载地址也可以在这个页面上找到。CBLAS安装需要先装BLAS。
1）从主页上下载blas.tgz，解压
#cd BLAS
#make    //就会生成一个 blas_LINUX.a文件。
2）然后，下载cblas.tgz，解压
在目录下将Makefile.*文件改名或者做一个链接文件为 Makefile.in文件，比如在linux下就是ln -s Makefile.LINUX Makefile.in
根据具体情况修改Makefile.in文件，主要是BLAS的库文件路径BLLIB和CBLAS的安装目录 CBDIR，make help就可以打印出可以使用的make命令，要生成全部文件就是用make all。在$(CBDIR)目录下的$(CBLIBDIR)将生成CBLAS的库文件$(CBLIB)，cblas_LINUX.a。
在CBLAS的安装目录$(CBDIR)下的src目录中有个cblas.h是包括的CBLAS的函数和常量的头文件，使用CBLAS的时候就需要这个头文件，同时还需要BLAS的库文件$(BLLIB )和CBLAS的库文件$(CBLIB)。
CBLAS/BLAS分为3个level，level1是用于向量的计算，level2是用于向量和矩阵之间的计算，level3是矩阵之间的计算。比如计算矩阵的乘法就是属于level3。另外，在GSL下也有BLAS和CBLAS，在boost里有ublas也提供CBLAS/BLAS的功能，有时间也拿来研究研究。
最简单的安装方法：sudo apt-get install libblas-dev
# [CBLAS的安装与使用](http://iysm.net/?p=53)
[2009年10月27日](http://iysm.net/?p=53)[ysm](http://iysm.net/?author=1)[2
 Comments](http://iysm.net/?p=53#comments)
ysm
[cleverysm@gmail.com](mailto:cleverysm@gmail.com)
CBLAS是BLAS的C语言接口。BLAS的全称是Basic Linear Algebra Subprograms，中文大概可以叫做基础线性代数子程序。主要是用于向量和矩阵计算的高性能数学库。本身BLAS是用Fortran写的，为了方便C/C++程序的使用，就有了BLAS的C接口库CBLAS。BLAS的主页是http://www.netlib.org/blas/，CBLAS的下载地址也可以在这个页面上找到。
CBLAS安装需要先装BLAS，从主页上下载blas.tgz，解压，根据系统修改make.inc和Makefile，make，就会生成一个blas_LINUX.a文件。然后，下载cblas.tgz，解压，在目录下将Makefile.*文件改名或者做一个链接文件为Makefile.in文件，比如在linux下就是ln -s Makefile.LINUX Makefile.in，根据具体情况修改Makefile.in文件，主要是BLAS的库文件路径BLLIB和CBLAS的安装目录CBDIR，make help就可以打印出可以使用的make命令，要生成全部文件就是用make
 all。在$(CBDIR)目录下的$(CBLIBDIR)将生成CBLAS的库文件$(CBLIB)，cblas_LINUX.a。
在CBLAS的安装目录$(CBDIR)下的src目录中有个cblas.h是包括的CBLAS的函数和常量的头文件，使用CBLAS的时候就需要这个头文件，同时还需要BLAS的库文件$(BLLIB )和CBLAS的库文件$(CBLIB)。
CBLAS/BLAS分为3个level，level1是用于向量的计算，level2是用于向量和矩阵之间的计算，level3是矩阵之间的计算。比如计算矩阵的乘法就是属于level3，这里就用矩阵乘法来学习使用CBLAS。
计算矩阵乘法的函数之一是 cblas_sgemm，使用单精度实数，另外还有对应双精度实数，单精度复数和双精度复数的函数。在此以 cblas_sgemm为例。
函数定义为：
void cblas_sgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
const int K, const float alpha, const float  *A,
const int lda, const float  *B, const int ldb,
const float beta, float  *C, const int ldc)
关于此函数的详细定义可以在http://www.netlib.org/blas/sgemm.f找到，只不过是fortran语言的,这个C语言版的略有差别。
此函数计算的是 C = alpha*op( A )*op( B ) + beta*C,
const enum CBLAS_ORDER Order，这是指的数据的存储形式，在CBLAS的函数中无论一维还是二维数据都是用一维数组存储，这就要涉及是行主序还是列主序，在C语言中数组是用行主序，fortran中是列主序。我还是习惯于是用行主序，所以这个参数是用CblasRowMajor，如果是列主序的话就是CblasColMajor。
const enum CBLAS_TRANSPOSE TransA和                 const enum CBLAS_TRANSPOSE TransB，这两个参数影响的是op( A )和op( B)，可选参数为CblasNoTrans=111, CblasTrans=112, CblasConjTrans=113，其中TransA = CblasNoTrans,  op( A ) = A，TransA = CblasTrans,  op( A ) = A’，TransA = CblasConjTrans, 
 op( A ) = A’。 TransB类似。
const int M，矩阵A的行，矩阵C的行
const int N，矩阵B的列，矩阵C的列
const int K，矩阵A的列，矩阵B的行
const float alpha， const float beta，计算公式中的两个参数值，如果只是计算C=A*B，则alpha=1,beta=0
const float  *A， const float  *B， const float  *C，矩阵ABC的数据
const int lda， const int ldb， const int ldc，在BLAS的文档里，这三个参数分别为ABC的行数，但是实际使用发现，在CBLAS里应该是列数。
我在这里计算两个简单矩阵的乘法。
A:
1,2,3
4,5,6
7,8,9
8,7,6
B:
5,4
3,2
1,0
程序代码：
```cpp
//因为程序是C++，而CBLAS是C语言写的，所以在此处用extern关键字
extern "C"
{
	#include
}
#includeusing namespace std;
int main(void) {
 
	const enum CBLAS_ORDER Order=CblasRowMajor;
	const enum CBLAS_TRANSPOSE TransA=CblasNoTrans;
	const enum CBLAS_TRANSPOSE TransB=CblasNoTrans;
	const int M=4;//A的行数，C的行数
	const int N=2;//B的列数，C的列数
	const int K=3;//A的列数，B的行数
	const float alpha=1;
	const float beta=0;
	const int lda=K;//A的列
	const int ldb=N;//B的列
	const int ldc=N;//C的列
	const float A[K*M]={1,2,3,4,5,6,7,8,9,8,7,6};
	const float B[K*N]={5,4,3,2,1,0};
	float C[M*N]; 
 
	cblas_sgemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
 
	for(int i=0;i
```
在编译的时候需要带上cblas_LINUX.a和blas_LINUX.a，比如，
g++ main.cpp cblas_LINUX.a blas_LINUX.a -o main
当然，这里假定是这两个.a文件是放在可以直接访问的位置，或者写全路径也可以。
这种做法在CentOS.5下顺利通过，但是在我的Ubuntu.7.10下出了问题，blas_LINUX.a正常编译生成，但在链接的时候出了错误，所以只好从源里安装了atlas，sudo apt-get install atlas3-base，在/usr/lib/atlas/目录下就会有libblas.*和liblapack.*库文件，只需要在链接的时候用这里的blas库文件替换上文安装的BLAS就可以正常编译通过。
另外，在GSL下也有BLAS和CBLAS，在boost里有ublas也提供CBLAS/BLAS的功能，有时间也拿来研究研究。
