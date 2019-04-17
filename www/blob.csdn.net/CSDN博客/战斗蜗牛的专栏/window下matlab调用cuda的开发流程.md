# window下matlab调用cuda的开发流程 - 战斗蜗牛的专栏 - CSDN博客





2010年10月24日 17:08:00[vbskj](https://me.csdn.net/vbskj)阅读数：8948








    这两天抽点时间研究matlab调用cuda程序的问题，发现网上这方面的资料比较少，而且nvidia提供的白皮书描述的不够详尽，因此对于开发流程进行总结，希望大家有用。

    总的来说，matlab中调用cuda程序有两种方法，第一是利用开勇的cuda wizard创建cuda程序的dll，然后结合matlab的mex编译器在matlab中进行调用；第二种是根据白皮书中所写，将cuda程写成标准的mex文件格式，然后利用mex编译器生成二进制代码，在matlab中进行调用。两种方法生成的都是以.mexw32或.mexw64结尾的二进制程序，直接在m文件中使用即可。



    1. Matlab白皮书方法

        A.  仅仅使用cufft，将cuda代码和C代码函数写在mex标准文件中，则利用下面的编译指令可以完成二进制程序的生成。

            mex fft2_cuda.c -IC:/CUDA/include -LC:/CUDA/lib -lcudart -lcufft

        B.  代码里面包含核函数，readme中说明使用如下指令可以完成，但是总是报错，说nvmex.pl有错误，可能是vs或者matlab版本不对？（我用的vs2005 sp1 and Matlab R2010）



        nvmex -f nvmexopts.bat Szeta.cu -IC:/cuda/include -LC:/cuda/lib -lcufft -lcudart



        C. 从网上下载了一个代码，非常不错，里面包含matlab编译脚本文件my_compile.m, cuda文件test.cu,mex标准文件test.cpp.这是一个非常好的例子，参照即可。首先运行my_compile test.cpp, 然后就会生成二进制程序文件以供matlab调用。

my_compile.m

///////////////////////

function my_compile(varargin)
!"%VS80COMNTOOLS%vsvars32.bat" & nvcc -c -arch compute_13 test.cu
n=getenv('CUDA_LIB_PATH'); if n(1)=='"', n=n(2:end); end, if n(end)=='"', n=n(1:end-1);end
mex(['-L' n],'-lcudart','test.obj',varargin{:});



test.cu

//////////////////

extern "C" void gpuAdd(double* A, double* B, double *C);

__global__ void vecAdd(double* A, double* B, double* C)
{
    int i = threadIdx.x;
    C[i]=A[i]+B[i];
}

void gpuAdd(double* A, double* B, double *C)
{
    double *Ad,*Bd,*Cd;
    cudaMalloc((void**)&Ad,5*sizeof(double)); 
    cudaMalloc((void**)&Bd,5*sizeof(double)); 
    cudaMalloc((void**)&Cd,5*sizeof(double));
    cudaMemcpy(Ad,A,5*sizeof(double),cudaMemcpyHostToDevice);     cudaMemcpy(Bd,B,5*sizeof(double),cudaMemcpyHostToDevice); 
    vecAdd<<<1,5>>>(Ad,Bd,Cd);
    cudaMemcpy(C,Cd,5*sizeof(double),cudaMemcpyDeviceToHost);
    cudaFree(Ad);cudaFree(Bd);cudaFree(Cd);
}



test.cpp

/////////////

#include "mex.h"

extern "C" void gpuAdd(double* A, double* B, double *C);


void mexFunction(
     int nlhs, mxArray *plhs[],
     int nrhs, const mxArray *prhs[])
{
    double *C;
    if (mxGetNumberOfElements(prhs[0])!=5) mexErrMsgTxt("Wrong number of elements in A!");
    if (mxGetNumberOfElements(prhs[1])!=5) mexErrMsgTxt("Wrong number of elements in B!");
    C=(double*)mxGetData(plhs[0]=mxCreateDoubleMatrix(1,5,mxREAL));
    gpuAdd((double*)mxGetData(prhs[0]),(double*)mxGetData(prhs[1]),C);
}




2.   动态库的方法

    首先按照开勇的dll开发方法，完成cuda程序的dll封装。然后创建一个包含mexFunction的标准mex文件，将调用的cuda函数写入其中即可。编译的方法类似：

    mex -L. -ltest test.c



3.  直接把cuda程序编译成exe文件，在matlab中通过system函数来调用也可，（懒人办法）



