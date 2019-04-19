# CUDA：一维、二维的grid、block的核函数线程分配 - Koma Hub - CSDN博客
2017年11月28日 17:00:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：704
1.一维grid，一维block
```cpp
int nx = 1 << 14;
    int ny = 1 << 14;
    int dimx = 32;
    dim3 block(dimx, 1);
    dim3 grid((nx + block.x - 1) / block.x, 1);
```
核函数
```cpp
__global__ void kernel_function()
{
    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;
    if (ix < nx )
    {
        do_something();
    }
}
```
2.二维grid，一维block
```cpp
int nx = 1 << 14;
    int ny = 1 << 14;
    int dimx = 32;
    dim3 block(dimx, 1);
    dim3 grid((nx + block.x - 1) / block.x, ny);
```
核函数
```cpp
__global__ void kernel_function()
{
    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int iy = blockIdx.y;
    unsigned int idx = iy * nx + ix;
    if (ix < nx && iy < ny)
    {
        do_something();
    }
}
```
3.二维grid，二维block
```cpp
int nx = 1 << 14;
    int ny = 1 << 14;
    int dimx = 32;
    int dimy = 32;
    dim3 block(dimx, dimy);
    dim3 grid((nx + block.x - 1) / block.x, (ny + block.y - 1) / block.y);
```
核函数
```cpp
__global__ void kernel_function()
{
    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int iy = threadIdx.y + blockIdx.y * blockDim.y;
    unsigned int idx = iy * nx + ix;
    if (ix < nx && iy < ny)
    {
        do_something();
    }
}
```
