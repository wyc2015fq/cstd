# C++的Matlab接口 - wishchinYang的专栏 - CSDN博客
2013年09月05日 20:37:48[wishchin](https://me.csdn.net/wishchin)阅读数：892
与 原文 过程有些不同，根据具体环境自行配置即可！
转自于：[http://blog.csdn.net/left_la/article/details/8206645](http://blog.csdn.net/left_la/article/details/8206645)
我的计算机环境是win7 64位系统，VS2010，matlab R2010b。
一、工程配置：
1.C/C++->常规->附加包含目录 添加:
MATLAB\R2010b\extern\include
MATLAB\R2010b\extern\include\win64
2.链接器->常规->附加库目录 添加:
MATLAB\R2010b\extern\lib\win64\microsoft
MATLAB\R2010b\extern\lib\win32\microsoft
3.链接器->输入->附加依赖库 添加:
libmat.lib
libmx.lib
libmex.lib
libeng.lib
由于所安装的matlab为64位，要调用其函数，需要将工程转换为X64
4.顶菜单->生成->配置管理器->平台：X64
5.链接器->高级->目标计算机:
MachineX64(/MACHINE:X64)
6.计算机环境变量->path 添加：
E:\DevTools\MATLAB\R2010b\extern\lib\win64\microsoft;
E:\DevTools\MATLAB\R2010b\bin\win64;
二、所用到的基本读取写入mat文件代码示例：
首先需要包含头文件：
#include <mat.h>
```cpp
MATFile *pmatFile = NULL;  
mxArray *pMxArray = NULL;  
  
// 读取.mat文件（例：mat文件名为"initUrban.mat"，其中包含"initA"）  
double *initA;  
  
pmatFile = matOpen("initUrban.mat","r");  
pMxArray = matGetVariable(pmatFile, "initA");  
initA = (double*) mxGetData(pMxArray);  
M = mxGetM(pMxArray);  
N = mxGetN(pMxArray);  
Matrix<double> A(M,N);  
for (int i=0; i<M; i++)  
    for (int j=0; j<N; j++)  
        A[i][j] = initA[M*j+i];  
  
matClose(pmatFile);  
mxFree(initA);  
  
// 生成.mat文件  
double *outA = new double[M*N];  
    for (int i=0; i<M; i++)  
        for (int j=0; j<N; j++)  
            outA[M*j+i] = A[i][j];  
pmatFile = matOpen("A.mat","w");  
pMxArray = mxCreateDoubleMatrix(M, N, mxREAL);  
mxSetData(pMxArray, outA);  
matPutVariable(pmatFile, "A", pMxArray);  
matClose(pmatFile);
```
三、程序说明
1.使用matOpen函数打开mat文件
MATFile *matOpen(const char *filename,const char *mode)
mode：
r:以只读方式打开
u:更新模式，可读可写，但是如果要打开的数据文件不存在，也不会创建新的文件
w:以写入方式打开，只能写如入，如果要打开的数据文件不存在，则创建一个新的文件
2.使用matGetVariable函数读取mat文件中的变量
mxArray * matGetVariable(MATFile * pMF, const char * name);
读取名为name的变量，返回一个数据阵列指针
3.使用mxGetData函数获取数据阵列中的数据
void *mxGetData(const mxArray *pa );
返回时需要使用强制类型转换。
4.使用mxGetM和mxGetN函数获取数据阵列矩阵的维数
size_t mxGetM(const mxArray *pa);
size_t mxGetN(const mxArray *pa);
5.使用mxCreateDoubleMatrix函数创建数据阵列为双浮点数矩阵
mxArray *mxCreateDoubleMatrix(mwSize m, mwSize n, mxComplexity flag);
6.使用mxSetData函数将变量存入数据阵列
void mxSetData( mxArray *pa, void  *newdata );
7.使用matPutVariable函数将数据阵列存入到mat文件中
int matPutVariable(MATFile * pMF, const char * name, const mxArray * pA);
存入成功返回0，存入出错返回非0
8.由于malab中矩阵的存储是按列存储，与c中不同，所以从mat文件中获取的矩阵需要重新排列。生成mat文件时，同样要注意。
9.完成mat文件操作后需要调用matClose函数关闭文件，并使用mxFree释放内存
