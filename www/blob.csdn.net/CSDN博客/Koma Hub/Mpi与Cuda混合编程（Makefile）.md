# Mpi与Cuda混合编程（Makefile） - Koma Hub - CSDN博客
2017年09月06日 21:38:59[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：818
需要建立一个.c的MPI程序和一个.cu的CUDA程序，MPI程序中调用CUDA中的函数来完成并行与GPU的混合编程，我查询了很多资料和博客，最终得出结论，还是Google比较强大，百度什么的还是搜不到完整的讲解
MPI程序如下（文件名test.c）
```cpp
#include<stdio.h>
#include<malloc.h>
#include<math.h>
#include<stdlib.h>
#include "mpi.h"
void fun ( float * vp_device , int  n , int  nz , int  is) ;
void cudaFun ( int is , FILE *fp, int nx , int nz ) ;
main( int argc, char  *argv[ ] )
{
        int myid , numprocs , count , is , nx , nz ;
        float * vp;
        nx = 1000 ; nz = 1000 ;
        MPI_Init(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD,&myid);
        MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
        MPI_Barrier(MPI_COMM_WORLD);
        FILE *fp;
        fp=fopen( "test.dat" , "wb" );
        for ( is = myid ; is < 10 ; is = is    +    numprocs )
        {
                printf( " is== %d  \n "  , is ) ;
                cudaFun( is , fp , nx , nz );
        }
        MPI_Finalize( );
}
```
CUDA程序如下（文件名test_cuda.cu）
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cuda_runtime.h>
__device__  volatile  int  vint = 0;
//a#########################
extern "C"  __global__ void fun ( float  * vp_device , int n, int nz, int  is )
{
        int it=threadIdx.x blockDim.x*blockIdx.x;
        if  ( it < n ) {
                vp_device[it]=2000;
                if ( ( it > nz * 40 && it < 40 && it % nz < 60 ) ) 
                        vp_device [ it ] = 2500 * is * 100 ;
        }
}
//a########################
extern "C" void cudaFun ( int is , FILE  *fp ,  int  nx , int nz )
{
        int i ;
        float  * vp_device , * vp_host;
        cudaMalloc(&vp_device, nx*nz*sizeof(float));  
        cudaMemset(vp_device, 0, nx*nz*sizeof(float));
        vp_host=(float*)malloc(nx*nz*sizeof(float));
        float mstimer;
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);
        fun <<<(  nx * nz   +   511 ) / 512, 512>>> ( vp_device ,  nx*nz , nz , is ) ;
        cudaMemcpy(vp_host, vp_device, nx*nz*sizeof(float),cudaMemcpyDeviceToHost); 
        fseek(fp,is*nx*nz*sizeof(float),0);
        for (  i  =  0  ;  i  <  nx  *  nz   ;  i   ++   )
                fwrite( &vp_host[i] , sizeof(float) , 1 , fp);
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&mstimer, start, stop);
        printf( "CUDA : is = %d, time = %g (s)\ n " ,is, mstimer*1.e-3);
        cudaEventDestroy(start);
        cudaEventDestroy(stop);
        cudaFree(vp_device);
        free(vp_host);
}
```
Makefile如下（文件名Makefile）
```cpp
CUDA_INSTALL_PATH = /usr/local/cuda-7.5
MPI_INSTALL_PATH = /home/leonvel/software/mpi/mpich
NVCC = $(CUDA_INSTALL_PATH)/bin/nvcc
MPICC = $(MPI_INSTALL_PATH)/bin/mpicc
LDFLAGS = -L$(CUDA_INSTALL_PATH)/lib64
LIB = -lcudart -lcurand
CFILES = test.c
CUFILES = test_cuda.cu
OBJECTS = test.o test_cuda.o
EXECNAME = test
all:
        $(MPICC) -c $(CFILES)
        $(NVCC) -c $(CUFILES)
        $(MPICC) -o $(EXECNAME) $(LDFLAGS) $(LIB) $(OBJECTS)
clean:
        rm -f *.o $(EXECNAME)
```
我的环境是Linux，所以接下来make一下吧，
```cpp
[Toa@AMAX test]$ make
/home/leonvel/software/mpi/mpich/bin/mpicc -c test.c
/usr/local/cuda-7.5/bin/nvcc -c test_cuda.cu
/home/leonvel/software/mpi/mpich/bin/mpicc -o test -L/usr/local/cuda-7.5/lib64 -lcudart -lcurand test.o test_cuda.o
```
然后我们在运行一下：
```
[Toa@AMAX test]$ ./test
is==0 
CUDA:is=0, time= 0 (s)
is==1 
CUDA:is=1, time= 0 (s)
is==2 
CUDA:is=2, time= 0 (s)
is==3 
CUDA:is=3, time= 0 (s)
is==4 
CUDA:is=4, time= 0 (s)
is==5 
CUDA:is=5, time= 0 (s)
is==6 
CUDA:is=6, time= 0 (s)
is==7 
CUDA:is=7, time= 0 (s)
is==8 
CUDA:is=8, time= 0 (s)
is==9 
CUDA:is=9, time= 0 (s)
[Toa@AMAX test]$
```
没错，你没看错，运行时间就是零秒钟，就是这么快，就是这么尿性。好的，我今天的工作就是这么多，等以后继续学习的时候我会继续更新的，拜拜了，伙计们。。。。。
![](https://img-blog.csdn.net/20180202172943993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
