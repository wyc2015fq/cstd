# CUDA全局变量（__device__）的初始化与使用：cudaMemoryToSymbol、cudaMemoryFromSymbol、cudaGetSymbolAddress - Koma Hub - CSDN博客
2017年11月29日 14:24:38[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：2144
在cuda中在设备（device）中声明一个全局变量用__device__关键字修饰：
```cpp
__device__ float devData;
```
初始化为：
```cpp
float value = 3.14f;
cudaMemcpyToSymbol(devData, &value, sizeof(float));
```
在使用结束后，将其转回host：
```cpp
cudaMemcpyFromSymbol(&value, devData, sizeof(float))
```
给出整体的代码（原代码来源《CUDA C语言编程中文版》[源码网址](http://www.wrox.com/WileyCDA/WroxTitle/Professional-CUDA-C-Programming.productCd-1118739329,descCd-DOWNLOAD.html)）
```cpp
#include "../common/common.h"
#include <cuda_runtime.h>
#include <stdio.h>
/*
 * An example of using a statically declared global variable (devData) to store
 * a floating-point value on the device.
 */
__device__ float devData;
__global__ void checkGlobalVariable()
{
    // display the original value
    printf("Device: the value of the global variable is %f\n", devData);
    // alter the value
    devData += 2.0f;
}
int main(void)
{
    // initialize the global variable
    float value = 3.14f;
    CHECK(cudaMemcpyToSymbol(devData, &value, sizeof(float)));
    printf("Host:   copied %f to the global variable\n", value);
    // invoke the kernel
    checkGlobalVariable<<<1, 1>>>();
    // copy the global variable back to the host
    CHECK(cudaMemcpyFromSymbol(&value, devData, sizeof(float)));
    printf("Host:   the value changed by the kernel to %f\n", value);
    CHECK(cudaDeviceReset());
    return EXIT_SUCCESS;
}
```
编译与运行：
```
-bash-4.1$ nvcc -o a globalVariable.cu
-bash-4.1$ ./a
Host:   copied 3.140000 to the global variable
Device: the value of the global variable is 3.140000
Host:   the value changed by the kernel to 5.140000
-bash-4.1$
```
当然，也可以通过获取与提供设备符号相关的全局内存的物理地址来对全局变量进行使用。在上程序代码中加上如下内容：
```cpp
float *dp = NULL;
    cudaGetSymbolAddress((void**)&dp,devData);
    cudaMemcpy(dp,&value,sizeof(float),cudaMemcpyHostToDevice);
    // invoke the kernel
    checkGlobalVariable<<<1, 1>>>();
    // copy the global variable back to the host
    CHECK(cudaMemcpyFromSymbol(&value, devData, sizeof(float)));
    printf("Host:   the value changed by the kernel to %f\n", value);
```
其输出结果为：
```
-bash-4.1$ nvcc -o a globalVariable.cu
-bash-4.1$ ./a
Host:   copied 3.140000 to the global variable
Device: the value of the global variable is 3.140000
Host:   the value changed by the kernel to 5.140000
Device: the value of the global variable is 5.140000
Host:   the value changed by the kernel to 7.140000
-bash-4.1$
```
存在一种仅需简单指针引用，就可以让主机和设备代码直接访问，这就是**CUDA固定内存**。
