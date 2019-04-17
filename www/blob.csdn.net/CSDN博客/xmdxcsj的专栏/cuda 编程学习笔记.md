# cuda 编程学习笔记 - xmdxcsj的专栏 - CSDN博客





2016年11月13日 20:42:56[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1166








## programming model

### kernels

类似于c函数，函数定义使用**global**声明，使用<<<…>>>形式的execution configuration决定kernal执行的线程数，使用threadIdx变量可以获得每一个线程对应的id。

```cpp
// Kernel definition
__global__ void VecAdd(float* A, float* B, float* C)
{
    int i = threadIdx.x;
    C[i] = A[i] + B[i];
}
int main()
{
    ...
    // Kernel invocation with N threads
    VecAdd<<<1, N>>>(A, B, C);
    ...
}
```

execution configuration定义形式为

```
<<< Dg, Db, Ns, S >>>
Dg: 表示grid的维度和大小，对应的block个数为Dg.x * Dg.y * Dg.z
Db: 表示block的维度和大小，对用的thread个数为Db.x * Db.y * Db.z
Ns: 表示shared memory的大小，可选参数
S : 表示cudaStream_t的类型，可选参数
```

### thread hierarchy

thread： 

block: 多个thread，个数类型可以是int/dim3，一般最多1024个thread 

grid: 多个block，个数类型可以是int/dim3 

block之间独立执行，block内的thread可以通过shared memory共享数据，通过调用__syncthreads()函数实现同步。 

- threadIdx: block内thread的id 

- blockIdx: grid内的block的id 

- blockDim: 一个block的thread的个数
### memory hierarchy

每个thread有local memory 

每个block有shared memory，block内的thread都可以访问 

所有的thread都可以访问global memory/const memory/texture memory，其中const memory和texture memory只读。
### heterogeneous programming

host和device维护DRAM中不同的内存空间，分别称为host memory和device memory。

### compute capability

表示GPU硬件的特性，也称为“SM version”，由两部分组成”X.Y”，同一个X的版本号对应的内核架构一致，其中5表示基于Maxwell架构，3表示Kepler架构，2表示Fermi架构，1表示Tesla架构。从CUDA7.0开始不再支持Tesla架构。

## programming interface

### NVCC编译

编译分为offline compilation或者just-in-time compilation 

1. offline compilation 

cuda的代码可以是PTX或者c，都需要通过nvcc进行编译，编译流程 

- 分离host code和device code 

- 编译device code为PTX汇编格式或者cubin二进制格式 

- 使用编译好的kernel替换host code中的kernel函数（以<<<…>>>语法标记的） 

- 修改后的host code使用其他工具编译 

- nvcc编译后的程序可以链接到编译好的host code或者直接通过CUDA driver API进行加载执行- just-in-time compilation 

运行时加载的PTX程序，使用device driver进一步编译成二进制程序，称为just-in-time compilation. 

这种编译方式增加了加载时间，但是可以使用新的device driver带来的特性。
-code指定二进制程序运行的目标设备的架构，比如-code=sm_35产生的二进制程序运行在compute capability 3.5 

-arch指定的架构针对c代码到PTX汇编的编译过程 

支持部分c/c++的语法格式 

64-bit模式编译的device code只能被64-bit模式编译的host code支持；32-bit的同理。 

32-bit的nvcc可以使用-m64选项编译64-bit的device code；64-bit的nvcc可以使用-m32选项编译32-bit的device code。
### cuda c runtime

通过cudart库文件实现: libcudart.a或者libcudart.so。

#### initialization

第一次调用runtime函数的时候执行初始化，为device创建cuda context，所有的host线程共享。 

cudaDeviceReset()函数销毁context。

#### device memory

分为linear memory和CUDA arrays两种。 

linear memory是device上的40-bit的地址空间。 

- cudaMalloc/cudaFree/cudaMemcpy 

- cudaMallocPitch/cudaMalloc3D/cudaMemcpy2D/cudaMemcpy3D: 分配或者复制2D/3D数组，满足对齐要求/高效，尽量使用
#### shared memory

使用__shared__标示，shared memory比global memory快，尽量使用shared memory。

#### page-locked host memory

优点： 

- page-locked host memory和device memory之间内存拷贝可以和kernel执行同时进行 

- 可以map到device的地址空间 

- 具有front-side bus的系统，page-locked host memory和device memory有更高的带宽
#### 异步并行

以下操作相互独立，可以同步执行： 

- host计算 

- device计算 

- host和device回见的内存转移 

- device内部和device之间的内存转移
#### 多设备系统

cudaGetDeviceCount: 获取设备数 

cudaSetDevice: 设置使用设备

#### 统一虚拟地址空间

对于64-bit的进程，host和compute capability 2.0及更高版本的device使用一个地址空间

#### 进程间通信

使用Inter Process Communication API完成

### version

两个版本需要关注：compute capability有关硬件设备的特性；CUDA driver api有关driver API和runtime。 

driver API的版本使用CUDA_VERSION定义，支持向后兼容，也就是低版本的程序可以在高版本的driver上面运行。

### compute modes

可以使用nvidia-smi设置为一下几种模式： 

- default: 多个host线程可以同时使用device 

- exclusive-process: 只能在device创建一个CUDA context，创建context的进程中的多个线程可以使用 

- exclusive-process-and-thread: 只允许创建一个context，而且context一次只能被一个线程使用 

- prohibit: 禁止创建CUDA context
## c语言扩展

### 函数类型：
- __device__: 在device上执行，只能被device调用
- __global__: 标示kernel函数，在device上执行，可以被host/device调用
- __host__: 在host执行，只能被host调用
- __noinline__: 函数不inline
- __forceinline__: 函数inline

### 变量类型：
- __device__: device上的变量
- __constant__: constant memory space上的变量
- __shared__: shared memory，所有以这种形式声明的指针具有相同的起点
- __managed__: host和device都可以读写的地址
- __restrict__: 避免aliasing问题

### 内置vector

```cpp
make_<type name>
```

vector是结构体，可以通过x/y/z/w来获取第1/2/3/4个元素。 

dim3用来表示维度，等同于uint3。

### 内置变量

gridDim: grid维度 

blockIdx: grid中的block index 

blockDim: block维度 

threadIdx: block中thread的index 

warpSize: 线程中的warp size
## 性能优化

### 并行

并行库，比如cuBLAS/cuFFT 

并行编译器，比如使用progma 

并行代码
### debug

[CUDA_GDB](https://developer.nvidia.com/cuda-gdb)

### timing
- 
CPU Timer 

注意CUDA API函数很多是异步的，在使用计时函数前需要调用cudaDeviceSynchronize()函数以同步CPU线程和GPU。

- 
GPU Timer 

使用event计时


```cpp
cudaEvent_t start, stop;
float time;
cudaEventCreate(&start);
cudaEventCreate(&stop);
cudaEventRecord( start, 0 );
kernel<<<grid,threads>>> ( d_odata, d_idata, size_x, size_y,
NUM_REPS);
cudaEventRecord( stop, 0 );
cudaEventSynchronize( stop );
cudaEventElapsedTime( &time, start, stop );
cudaEventDestroy( start );
cudaEventDestroy( stop );
```

## 参考

[cuda-c-programming-guide](http://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#axzz4PsFGyztM)
[cuda-c-best-practices-guide](http://docs.nvidia.com/cuda/cuda-c-best-practices-guide/index.html#axzz4PsFGyztM)














