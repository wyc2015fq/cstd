# CUDA线程分配 - Koma Hub - CSDN博客
2017年11月28日 15:30:15[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：386
在cuda中，blockDim表示一个块中线程的维度信息，例如blockDim（2,1,1）代表一个块中有2个thread，gridDim表示一个网格中块的维度。
下面给出一个示例程序，这个程序来自CUDA C语言编程中文译本，如有侵权，联系删除。
```cpp
#include "../common/common.h"
#include <cuda_runtime.h>
#include <stdio.h>
/*
 * Display the dimensionality of a thread block and grid from the host and
 * device.
 */
__global__ void checkIndex(void)
{
    printf("threadIdx:(%d, %d, %d)\n", threadIdx.x, threadIdx.y, threadIdx.z);
    printf("---blockIdx:(%d, %d, %d)\n", blockIdx.x, blockIdx.y, blockIdx.z);
    printf("------blockDim:(%d, %d, %d)\n", blockDim.x, blockDim.y, blockDim.z);
    printf("---------gridDim:(%d, %d, %d)\n", gridDim.x, gridDim.y, gridDim.z);
}
int main(int argc, char **argv)
{
    // define total data element
    int nElem = 6;
    // define grid and block structure
    dim3 block(3);
    dim3 grid((nElem + block.x - 1) / block.x);
    // check grid and block dimension from host side
    printf("grid.x %d, grid.y %d, grid.z %d,\n", grid.x, grid.y, grid.z);
    printf("block.x %d, block.y %d, block.z %d,\n", block.x, block.y, block.z);
    // check grid and block dimension from device side
    checkIndex<<<grid, block>>>();
    // reset device before you leave
    CHECK(cudaDeviceReset());
    return(0);
}
```
编译与运行：
```
-bash-4.1$ nvcc -o a checkDimension.cu
-bash-4.1$ ./a
grid.x 2, grid.y 1, grid.z 1,
block.x 3, block.y 1, block.z 1,
threadIdx:(0, 0, 0)
threadIdx:(1, 0, 0)
threadIdx:(2, 0, 0)
threadIdx:(0, 0, 0)
threadIdx:(1, 0, 0)
threadIdx:(2, 0, 0)
---blockIdx:(1, 0, 0)
---blockIdx:(1, 0, 0)
---blockIdx:(1, 0, 0)
---blockIdx:(0, 0, 0)
---blockIdx:(0, 0, 0)
---blockIdx:(0, 0, 0)
------blockDim:(3, 1, 1)
------blockDim:(3, 1, 1)
------blockDim:(3, 1, 1)
------blockDim:(3, 1, 1)
------blockDim:(3, 1, 1)
------blockDim:(3, 1, 1)
---------gridDim:(2, 1, 1)
---------gridDim:(2, 1, 1)
---------gridDim:(2, 1, 1)
---------gridDim:(2, 1, 1)
---------gridDim:(2, 1, 1)
---------gridDim:(2, 1, 1)
-bash-4.1$
```
为了更加浅显地展示grid，block和thread之间的关系，我画了几个图：
![](https://img-blog.csdn.net/20171128152926384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171128152931068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171128154011821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
