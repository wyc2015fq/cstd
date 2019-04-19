# CUDA内存分配、释放、传输，固定内存 - Koma Hub - CSDN博客
2017年11月29日 15:06:13[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：3239
```cpp
/*
 * 内存分配
 * if failed, return cudaErrorMemoryAllocation */
    cudaError_t cudaMalloc(void **devPtr, size_t count);
/*
 * 内存初始化
 */
    cudaError_t cudaMemset(void *devPtr, int value, size_t count);
/*
 * 内存释放
 * if failed, return cudaErrorInvalidDevicePointer */
    cudaError_t cudaFree(void *devPtr);
/*
 * 内存传输
 * 
 * enum cudaMemcpyKind{
 *      cudaMemcpyHostToHost,
 *      cudaMemcpyHostToDevice,
 *      cudaMemcpyDeviceToHost,
 *      cudaMemcpyDeviceToDevice
 *  };
 */
    cudaError_t cudaMemcpy(void *dst, const void *src, size_t count, 
                                enum cudaMemcpyKind kind);
```
给出书中代码，以便参考：[点击打开链接](http://www.wrox.com/WileyCDA/WroxTitle/Professional-CUDA-C-Programming.productCd-1118739329,descCd-DOWNLOAD.html)（如有侵权，联系删除）
```cpp
#include "../common/common.h"
#include <cuda_runtime.h>
#include <stdio.h>
/*
 * An example of using CUDA's memory copy API to transfer data to and from the
 * device. In this case, cudaMalloc is used to allocate memory on the GPU and
 * cudaMemcpy is used to transfer the contents of host memory to an array
 * allocated using cudaMalloc.
 */
int main(int argc, char **argv)
{
    // set up device
    int dev = 0;
    CHECK(cudaSetDevice(dev));
    // memory size
    unsigned int isize = 1 << 22;
    unsigned int nbytes = isize * sizeof(float);
    // get device information
    cudaDeviceProp deviceProp;
    CHECK(cudaGetDeviceProperties(&deviceProp, dev));
    printf("%s starting at ", argv[0]);
    printf("device %d: %s memory size %d nbyte %5.2fMB\n", dev,
           deviceProp.name, isize, nbytes / (1024.0f * 1024.0f));
    // allocate the host memory
    float *h_a = (float *)malloc(nbytes);
    // allocate the device memory
    float *d_a;
    //. if cudaMalloc failed, then return cudaErrorMemoryAllocation
    CHECK(cudaMalloc((float **)&d_a, nbytes));
    // initialize the host memory
    for(unsigned int i = 0; i < isize; i++) h_a[i] = 0.5f;
    // transfer data from the host to the device
    CHECK(cudaMemcpy(d_a, h_a, nbytes, cudaMemcpyHostToDevice));
    // transfer data from the device to the host
    CHECK(cudaMemcpy(h_a, d_a, nbytes, cudaMemcpyDeviceToHost));
    /* free memory
     * cudaFree(a), "a" must allocated by cudaMalloc() 
     * if failed , then return cudaErrorInvalidDevicePointer */
    CHECK(cudaFree(d_a));
    free(h_a);
    // reset device
    CHECK(cudaDeviceReset());
    return EXIT_SUCCESS;
}
```
编译与运行：
```cpp
-bash-4.1$ nvcc -o a memTransfer.cu
-bash-4.1$ nvprof ./a
==21180== NVPROF is profiling process 21180, command: ./a
./a starting at device 0: Tesla K40c memory size 4194304 nbyte 16.00MB
==21180== Profiling application: ./a
==21180== Profiling result:
Time(%)      Time     Calls       Avg       Min       Max  Name
 52.17%  5.7688ms         1  5.7688ms  5.7688ms  5.7688ms  [CUDA memcpy HtoD]
 47.83%  5.2886ms         1  5.2886ms  5.2886ms  5.2886ms  [CUDA memcpy DtoH]
==21180== API calls:
Time(%)      Time     Calls       Avg       Min       Max  Name
 63.57%  291.24ms         1  291.24ms  291.24ms  291.24ms  cudaMalloc
 33.04%  151.39ms         1  151.39ms  151.39ms  151.39ms  cudaDeviceReset
  2.62%  12.009ms         2  6.0043ms  5.9410ms  6.0675ms  cudaMemcpy
  0.48%  2.2154ms       332  6.6720us     222ns  277.55us  cuDeviceGetAttribute
  0.11%  506.96us         1  506.96us  506.96us  506.96us  cudaGetDeviceProperties
  0.06%  281.43us         1  281.43us  281.43us  281.43us  cudaFree
  0.06%  263.35us         4  65.838us  47.937us  103.86us  cuDeviceGetName
  0.05%  236.14us         4  59.035us  55.887us  64.487us  cuDeviceTotalMem
  0.00%  18.748us         1  18.748us  18.748us  18.748us  cudaSetDevice
  0.00%  3.8790us         8     484ns     314ns     724ns  cuDeviceGet
  0.00%  3.5800us         2  1.7900us     625ns  2.9550us  cuDeviceGetCount
-bash-4.1$
```
**固定内存（pinned）**
在CPU与GPU协同计算过程中，主机内存默认为pageable（分页）内存，分页内存需要先转换为固定内存，然后再进行主机与设备之间的内存拷贝，CUDA允许直接开辟固定内存：
```cpp
/*
 * 固定内存（Pinned Memory）
 */
    cudaError_t cudaMallocHost(void **devPtr, size_t count);
    cudaError_t cudaFreeHost(void *ptr);
    cudaError_t status cudaMallocHost((void**)h_aPinned, bytes);
    if(status != cudaSuccess){
        fprintf(stderr,"Error returned from pinned host memory allocation\n");
        exit(1);
    }
```
依然给出源代码：[点击打开链接](http://www.wrox.com/WileyCDA/WroxTitle/Professional-CUDA-C-Programming.productCd-1118739329,descCd-DOWNLOAD.html)
```cpp
#include "../common/common.h"
#include <cuda_runtime.h>
#include <stdio.h>
/*
 * An example of using CUDA's memory copy API to transfer data to and from the
 * device. In this case, cudaMalloc is used to allocate memory on the GPU and
 * cudaMemcpy is used to transfer the contents of host memory to an array
 * allocated using cudaMalloc. Host memory is allocated using cudaMallocHost to
 * create a page-locked host array.
 */
int main(int argc, char **argv)
{
    // set up device
    int dev = 0;
    CHECK(cudaSetDevice(dev));
    // memory size
    unsigned int isize = 1 << 22;
    unsigned int nbytes = isize * sizeof(float);
    // get device information
    cudaDeviceProp deviceProp;
    CHECK(cudaGetDeviceProperties(&deviceProp, dev));
    if (!deviceProp.canMapHostMemory)
    {
        printf("Device %d does not support mapping CPU host memory!\n", dev);
        CHECK(cudaDeviceReset());
        exit(EXIT_SUCCESS);
    }
    printf("%s starting at ", argv[0]);
    printf("device %d: %s memory size %d nbyte %5.2fMB canMap %d\n", dev,
           deviceProp.name, isize, nbytes / (1024.0f * 1024.0f),
           deviceProp.canMapHostMemory);
    // allocate pinned host memory
    float *h_a;
    CHECK(cudaMallocHost ((float **)&h_a, nbytes));
    // allocate device memory
    float *d_a;
    CHECK(cudaMalloc((float **)&d_a, nbytes));
    // initialize host memory
    memset(h_a, 0, nbytes);
    for (int i = 0; i < isize; i++) h_a[i] = 100.10f;
    // transfer data from the host to the device
    CHECK(cudaMemcpy(d_a, h_a, nbytes, cudaMemcpyHostToDevice));
    // transfer data from the device to the host
    CHECK(cudaMemcpy(h_a, d_a, nbytes, cudaMemcpyDeviceToHost));
    // free memory
    CHECK(cudaFree(d_a));
    CHECK(cudaFreeHost(h_a));
    // reset device
    CHECK(cudaDeviceReset());
    return EXIT_SUCCESS;
}
```
