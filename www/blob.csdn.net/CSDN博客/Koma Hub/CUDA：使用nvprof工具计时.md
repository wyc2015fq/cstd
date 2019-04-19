# CUDA：使用nvprof工具计时 - Koma Hub - CSDN博客
2017年11月28日 16:24:50[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1810
CUDA在运行程序时加上nvprof会对程序进行性能分析，这种性能分析最重要的就是统计不同函数的运行时间（占比）。
```
-bash-4.1$ nvprof ./a
./a Starting...
==19114== NVPROF is profiling process 19114, command: ./a
Vector size 32
Execution configure <<<1, 32>>>
Arrays match.
==19114== Profiling application: ./a
==19114== Profiling result:
Time(%)      Time     Calls       Avg       Min       Max  Name
 38.04%  3.1040us         3  1.0340us     896ns  1.2800us  [CUDA memcpy HtoD]
 33.73%  2.7520us         1  2.7520us  2.7520us  2.7520us  sumArraysOnGPU(float*, float*, float*, int)
 28.24%  2.3040us         1  2.3040us  2.3040us  2.3040us  [CUDA memcpy DtoH]
==19114== API calls:
Time(%)      Time     Calls       Avg       Min       Max  Name
 63.31%  302.79ms         3  100.93ms  9.2340us  302.77ms  cudaMalloc
 35.96%  171.98ms         1  171.98ms  171.98ms  171.98ms  cudaDeviceReset
  0.53%  2.5342ms       332  7.6330us     327ns  285.83us  cuDeviceGetAttribute
  0.06%  268.30us         4  67.076us  66.332us  67.675us  cuDeviceTotalMem
  0.05%  253.84us         4  63.460us  61.246us  68.736us  cuDeviceGetName
  0.05%  230.18us         3  76.727us  10.958us  201.12us  cudaFree
  0.02%  87.188us         4  21.797us  10.783us  32.713us  cudaMemcpy
  0.01%  42.401us         1  42.401us  42.401us  42.401us  cudaLaunch
  0.00%  20.420us         1  20.420us  20.420us  20.420us  cudaSetDevice
  0.00%  6.4060us         4  1.6010us     515ns  4.4500us  cudaSetupArgument
  0.00%  5.1900us         8     648ns     410ns  1.0300us  cuDeviceGet
  0.00%  4.6600us         2  2.3300us     850ns  3.8100us  cuDeviceGetCount
  0.00%  3.0990us         1  3.0990us  3.0990us  3.0990us  cudaConfigureCall
-bash-4.1$
```
