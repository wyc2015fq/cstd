# 结构体数组（SoA）与数组结构体（AoS） - Koma Hub - CSDN博客
2017年11月29日 19:32:22[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：641
**1.结构体数组（SoA）**
```cpp
/*
 * SoA 结构体数组定义
 */
struct InnerArray
{
    float x[LEN];
    float y[LEN];
};
/*
 * CPU -> SoA 结构体数组的CPU计算形式
 */
void testInnerArrayHost(InnerArray *A, InnerArray *C, const int n)
{
    for (int idx = 0; idx < n; idx++)
    {
        C->x[idx] = A->x[idx] + 10.f;
        C->y[idx] = A->y[idx] + 20.f;
    }
    return;
}
/*
 * GPU -> SoA 结构体数组的CUDA计算模式
 */
__global__ void testInnerArrayDevice(InnerArray *data, InnerArray * result,
                               const int n)
{
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        float tmpx = data->x[i];
        float tmpy = data->y[i];
        tmpx += 10.f;
        tmpy += 20.f;
        result->x[i] = tmpx;
        result->y[i] = tmpy;
    }
}
```
**2.数组结构体（AoS）**
```cpp
/*
 * AoS
 */
struct innerStruct
{
    float x;
    float y;
};
/*
 * CPU -> AoS
 */
void testInnerStructHost(innerStruct *A, innerStruct *C, const int n)
{
    for (int idx = 0; idx < n; idx++)
    {
        C[idx].x = A[idx].x + 10.f;
        C[idx].y = A[idx].y + 20.f;
    }
    return;
}
/*
 * GPU -> AoS
 */
__global__ void testInnerStructDevice(innerStruct *data, innerStruct * result,
                                const int n)
{
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        innerStruct tmp = data[i];
        tmp.x += 10.f;
        tmp.y += 20.f;
        result[i] = tmp;
    }
}
```
**3.结构体数组（SoA）与数组结构体（AoS）二者的区别**
许多并行编程范式，尤其是SIMD（单指令多数据）型范式，更倾向于使用SoA。在CUDA C编程中也普遍倾向于SoA，一维数据元素是为全局内存的有效合并访问而预先准备好的，而相同内存操作引用的同字段元素在存储时时彼此相邻的。
4.给出源代码示例，（《CUDA C编程中文翻译版本》，如有侵权，联系删除）源码网址：[点击打开链接](http://www.wrox.com/WileyCDA/WroxTitle/Professional-CUDA-C-Programming.productCd-1118739329,descCd-DOWNLOAD.html)
SoA
```cpp
#include "../common/common.h"
#include <cuda_runtime.h>
#include <stdio.h>
/*
 * A simple example of using a structore of arrays to store data on the device.
 * This example is used to study the impact on performance of data layout on the
 * GPU.
 *
 * SoA: contiguous reads for x and y
 */
#define LEN 1<<22
struct InnerArray
{
    float x[LEN];
    float y[LEN];
};
// functions for inner array outer struct
void initialInnerArray(InnerArray *ip,  int size)
{
    for (int i = 0; i < size; i++)
    {
        ip->x[i] = (float)( rand() & 0xFF ) / 100.0f;
        ip->y[i] = (float)( rand() & 0xFF ) / 100.0f;
    }
    return;
}
void testInnerArrayHost(InnerArray *A, InnerArray *C, const int n)
{
    for (int idx = 0; idx < n; idx++)
    {
        C->x[idx] = A->x[idx] + 10.f;
        C->y[idx] = A->y[idx] + 20.f;
    }
    return;
}
void printfHostResult(InnerArray *C, const int n)
{
    for (int idx = 0; idx < n; idx++)
    {
        printf("printout idx %d:  x %f y %f\n", idx, C->x[idx], C->y[idx]);
    }
    return;
}
void checkInnerArray(InnerArray *hostRef, InnerArray *gpuRef, const int N)
{
    double epsilon = 1.0E-8;
    bool match = 1;
    for (int i = 0; i < N; i++)
    {
        if (abs(hostRef->x[i] - gpuRef->x[i]) > epsilon)
        {
            match = 0;
            printf("different on x %dth element: host %f gpu %f\n", i,
                   hostRef->x[i], gpuRef->x[i]);
            break;
        }
        if (abs(hostRef->y[i] - gpuRef->y[i]) > epsilon)
        {
            match = 0;
            printf("different on y %dth element: host %f gpu %f\n", i,
                   hostRef->y[i], gpuRef->y[i]);
            break;
        }
    }
    if (!match)  printf("Arrays do not match.\n\n");
}
__global__ void testInnerArray(InnerArray *data, InnerArray * result,
                               const int n)
{
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        float tmpx = data->x[i];
        float tmpy = data->y[i];
        tmpx += 10.f;
        tmpy += 20.f;
        result->x[i] = tmpx;
        result->y[i] = tmpy;
    }
}
__global__ void warmup2(InnerArray *data, InnerArray * result, const int n)
{
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        float tmpx = data->x[i];
        float tmpy = data->y[i];
        tmpx += 10.f;
        tmpy += 20.f;
        result->x[i] = tmpx;
        result->y[i] = tmpy;
    }
}
// test for array of struct
int main(int argc, char **argv)
{
    // set up device
    int dev = 0;
    cudaDeviceProp deviceProp;
    CHECK(cudaGetDeviceProperties(&deviceProp, dev));
    printf("%s test struct of array at ", argv[0]);
    printf("device %d: %s \n", dev, deviceProp.name);
    CHECK(cudaSetDevice(dev));
    // allocate host memory
    int nElem = LEN;
    size_t nBytes = sizeof(InnerArray);
    InnerArray     *h_A = (InnerArray *)malloc(nBytes);
    InnerArray *hostRef = (InnerArray *)malloc(nBytes);
    InnerArray *gpuRef  = (InnerArray *)malloc(nBytes);
    // initialize host array
    initialInnerArray(h_A, nElem);
    testInnerArrayHost(h_A, hostRef, nElem);
    // allocate device memory
    InnerArray *d_A, *d_C;
    CHECK(cudaMalloc((InnerArray**)&d_A, nBytes));
    CHECK(cudaMalloc((InnerArray**)&d_C, nBytes));
    // copy data from host to device
    CHECK(cudaMemcpy(d_A, h_A, nBytes, cudaMemcpyHostToDevice));
    // set up offset for summary
    int blocksize = 128;
    if (argc > 1) blocksize = atoi(argv[1]);
    // execution configuration
    dim3 block (blocksize, 1);
    dim3 grid  ((nElem + block.x - 1) / block.x, 1);
    // kernel 1:
    double iStart = seconds();
    warmup2<<<grid, block>>>(d_A, d_C, nElem);
    CHECK(cudaDeviceSynchronize());
    double iElaps = seconds() - iStart;
    printf("warmup2      <<< %3d, %3d >>> elapsed %f sec\n", grid.x, block.x,
           iElaps);
    CHECK(cudaMemcpy(gpuRef, d_C, nBytes, cudaMemcpyDeviceToHost));
    checkInnerArray(hostRef, gpuRef, nElem);
    CHECK(cudaGetLastError());
    iStart = seconds();
    testInnerArray<<<grid, block>>>(d_A, d_C, nElem);
    CHECK(cudaDeviceSynchronize());
    iElaps = seconds() - iStart;
    printf("innerarray   <<< %3d, %3d >>> elapsed %f sec\n", grid.x, block.x,
           iElaps);
    CHECK(cudaMemcpy(gpuRef, d_C, nBytes, cudaMemcpyDeviceToHost));
    checkInnerArray(hostRef, gpuRef, nElem);
    CHECK(cudaGetLastError());
    CHECK(cudaFree(d_A));
    CHECK(cudaFree(d_C));
    free(h_A);
    free(hostRef);
    free(gpuRef);
    // reset device
    CHECK(cudaDeviceReset());
    return EXIT_SUCCESS;
}
```
AoS
```cpp
#include "../common/common.h"
#include <cuda_runtime.h>
#include <stdio.h>
/*
 * A simple example of using an array of structures to store data on the device.
 * This example is used to study the impact on performance of data layout on the
 * GPU.
 *
 * AoS: one contiguous 64-bit read to get x and y (up to 300 cycles)
 */
#define LEN 1<<22
struct innerStruct
{
    float x;
    float y;
};
struct innerArray
{
    float x[LEN];
    float y[LEN];
};
void initialInnerStruct(innerStruct *ip,  int size)
{
    for (int i = 0; i < size; i++)
    {
        ip[i].x = (float)(rand() & 0xFF) / 100.0f;
        ip[i].y = (float)(rand() & 0xFF) / 100.0f;
    }
    return;
}
void testInnerStructHost(innerStruct *A, innerStruct *C, const int n)
{
    for (int idx = 0; idx < n; idx++)
    {
        C[idx].x = A[idx].x + 10.f;
        C[idx].y = A[idx].y + 20.f;
    }
    return;
}
void checkInnerStruct(innerStruct *hostRef, innerStruct *gpuRef, const int N)
{
    double epsilon = 1.0E-8;
    bool match = 1;
    for (int i = 0; i < N; i++)
    {
        if (abs(hostRef[i].x - gpuRef[i].x) > epsilon)
        {
            match = 0;
            printf("different on %dth element: host %f gpu %f\n", i,
                    hostRef[i].x, gpuRef[i].x);
            break;
        }
        if (abs(hostRef[i].y - gpuRef[i].y) > epsilon)
        {
            match = 0;
            printf("different on %dth element: host %f gpu %f\n", i,
                    hostRef[i].y, gpuRef[i].y);
            break;
        }
    }
    if (!match)  printf("Arrays do not match.\n\n");
}
__global__ void testInnerStruct(innerStruct *data, innerStruct * result,
                                const int n)
{
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        innerStruct tmp = data[i];
        tmp.x += 10.f;
        tmp.y += 20.f;
        result[i] = tmp;
    }
}
__global__ void warmup(innerStruct *data, innerStruct * result, const int n)
{
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        innerStruct tmp = data[i];
        tmp.x += 10.f;
        tmp.y += 20.f;
        result[i] = tmp;
    }
}
int main(int argc, char **argv)
{
    // set up device
    int dev = 0;
    cudaDeviceProp deviceProp;
    CHECK(cudaGetDeviceProperties(&deviceProp, dev));
    printf("%s test struct of array at ", argv[0]);
    printf("device %d: %s \n", dev, deviceProp.name);
    CHECK(cudaSetDevice(dev));
    // allocate host memory
    int nElem = LEN;
    size_t nBytes = nElem * sizeof(innerStruct);
    innerStruct     *h_A = (innerStruct *)malloc(nBytes);
    innerStruct *hostRef = (innerStruct *)malloc(nBytes);
    innerStruct *gpuRef  = (innerStruct *)malloc(nBytes);
    // initialize host array
    initialInnerStruct(h_A, nElem);
    testInnerStructHost(h_A, hostRef, nElem);
    // allocate device memory
    innerStruct *d_A, *d_C;
    CHECK(cudaMalloc((innerStruct**)&d_A, nBytes));
    CHECK(cudaMalloc((innerStruct**)&d_C, nBytes));
    // copy data from host to device
    CHECK(cudaMemcpy(d_A, h_A, nBytes, cudaMemcpyHostToDevice));
    // set up offset for summaryAU: It is blocksize not offset. Thanks.CZ
    int blocksize = 128;
    if (argc > 1) blocksize = atoi(argv[1]);
    // execution configuration
    dim3 block (blocksize, 1);
    dim3 grid  ((nElem + block.x - 1) / block.x, 1);
    // kernel 1: warmup
    double iStart = seconds();
    warmup<<<grid, block>>>(d_A, d_C, nElem);
    CHECK(cudaDeviceSynchronize());
    double iElaps = seconds() - iStart;
    printf("warmup      <<< %3d, %3d >>> elapsed %f sec\n", grid.x, block.x,
           iElaps);
    CHECK(cudaMemcpy(gpuRef, d_C, nBytes, cudaMemcpyDeviceToHost));
    checkInnerStruct(hostRef, gpuRef, nElem);
    CHECK(cudaGetLastError());
    // kernel 2: testInnerStruct
    iStart = seconds();
    testInnerStruct<<<grid, block>>>(d_A, d_C, nElem);
    CHECK(cudaDeviceSynchronize());
    iElaps = seconds() - iStart;
    printf("innerstruct <<< %3d, %3d >>> elapsed %f sec\n", grid.x, block.x,
           iElaps);
    CHECK(cudaMemcpy(gpuRef, d_C, nBytes, cudaMemcpyDeviceToHost));
    checkInnerStruct(hostRef, gpuRef, nElem);
    CHECK(cudaGetLastError());
    // free memories both host and device
    CHECK(cudaFree(d_A));
    CHECK(cudaFree(d_C));
    free(h_A);
    free(hostRef);
    free(gpuRef);
    // reset device
    CHECK(cudaDeviceReset());
    return EXIT_SUCCESS;
}
```
