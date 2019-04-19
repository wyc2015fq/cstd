# CUDA：在GPU上实现核函数的嵌套以及编译运行 - Koma Hub - CSDN博客
2017年11月28日 21:59:01[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：970
该源程序来自《CUDA C语言编程中文译文版》，如有侵权，联系删除。此处只为学习交流。
程序如下：
```cpp
#include "../common/common.h"
#include <stdio.h>
#include <cuda_runtime.h>
/*
 * A simple example of nested kernel launches from the GPU. Each thread displays
 * its information when execution begins, and also diagnostics when the next
 * lowest nesting layer completes.
 */
__global__ void nestedHelloWorld(int const iSize, int iDepth)
{
    int tid = threadIdx.x;
    printf("Recursion=%d: Hello World from thread %d block %d\n", iDepth, tid,
           blockIdx.x);
    // condition to stop recursive execution
    if (iSize == 1) return;
    // reduce block size to half
    int nthreads = iSize >> 1;
    // thread 0 launches child grid recursively
    if(tid == 0 && nthreads > 0)
    {
        nestedHelloWorld<<<1, nthreads>>>(nthreads, ++iDepth);
        printf("-------> nested execution depth: %d\n", iDepth);
    }
}
int main(int argc, char **argv)
{
    int size = 8;
    int blocksize = 8;   // initial block size
    int igrid = 1;
    if(argc > 1)
    {
        igrid = atoi(argv[1]);
        size = igrid * blocksize;
    }
    dim3 block (blocksize, 1);
    dim3 grid  ((size + block.x - 1) / block.x, 1);
    printf("%s Execution Configuration: grid %d block %d\n", argv[0], grid.x,
           block.x);
    nestedHelloWorld<<<grid, block>>>(block.x, 0);
    CHECK(cudaGetLastError());
    CHECK(cudaDeviceReset());
    return 0;
}
```
编译与运行：
```
-bash-4.1$ nvcc -o a nestedHelloWorld.cu -arch=sm_35 -rdc=true -lcudadevrt
-bash-4.1$ ./a 2
./a Execution Configuration: grid 2 block 8
Recursion=0: Hello World from thread 0 block 1
Recursion=0: Hello World from thread 1 block 1
Recursion=0: Hello World from thread 2 block 1
Recursion=0: Hello World from thread 3 block 1
Recursion=0: Hello World from thread 4 block 1
Recursion=0: Hello World from thread 5 block 1
Recursion=0: Hello World from thread 6 block 1
Recursion=0: Hello World from thread 7 block 1
Recursion=0: Hello World from thread 0 block 0
Recursion=0: Hello World from thread 1 block 0
Recursion=0: Hello World from thread 2 block 0
Recursion=0: Hello World from thread 3 block 0
Recursion=0: Hello World from thread 4 block 0
Recursion=0: Hello World from thread 5 block 0
Recursion=0: Hello World from thread 6 block 0
Recursion=0: Hello World from thread 7 block 0
-------> nested execution depth: 1
-------> nested execution depth: 1
Recursion=1: Hello World from thread 0 block 0
Recursion=1: Hello World from thread 1 block 0
Recursion=1: Hello World from thread 2 block 0
Recursion=1: Hello World from thread 3 block 0
Recursion=1: Hello World from thread 0 block 0
Recursion=1: Hello World from thread 1 block 0
Recursion=1: Hello World from thread 2 block 0
Recursion=1: Hello World from thread 3 block 0
-------> nested execution depth: 2
-------> nested execution depth: 2
Recursion=2: Hello World from thread 0 block 0
Recursion=2: Hello World from thread 1 block 0
Recursion=2: Hello World from thread 0 block 0
Recursion=2: Hello World from thread 1 block 0
-------> nested execution depth: 3
-------> nested execution depth: 3
Recursion=3: Hello World from thread 0 block 0
Recursion=3: Hello World from thread 0 block 0
-bash-4.1$
```
