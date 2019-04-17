# cuda 概况和安装 - xmdxcsj的专栏 - CSDN博客





2016年11月13日 20:41:03[xmucas](https://me.csdn.net/xmdxcsj)阅读数：350








## 下载安装

参考[官网](https://developer.nvidia.com/cuda-downloads)

## 概况

cuda是英伟达公司的并行计算平台和编程模型，利用GPU加速计算。 
![这里写图片描述](https://img-blog.csdn.net/20161113203946789)
nvidia产品包括： 

- Tesla: super computing 

- Quadro: professional visualization 

- NVS: multi-display graphics 

- GeForce: entertainment 

- TEGRA: mobile processor
nvidia产品的架构包括： 

- pascal 

- maxwell 

- kepler 

- fermi
cuda相关的库，具体型号参考[官网](https://developer.nvidia.com/cuda-gpus)： 

- cuFFT 

- cuBLAS 

- cuSPARSE 

- cuRAND 

- NPP 

- nvGRAPH 

- Thrust
cuda相关的开发工具 

- NVCC: nvidia cuda compiler 

- Nsight Integrated Development Environments 

- Visual Profiler 

- CUDA-GDB 

- CUDA_MEMCHECK
## 相关命令

linux使用的命令 

nvcc：CUDA编译器驱动程序，类似于gcc 

ptxas：PTX汇编工具，会被nvcc调用 

cuobjdump：CUDA目标文件转存工具 

nvidia-smi：英伟达系统管理接口，可以查看gpu内存等的使用情况 

lspci | grep -i nvidia








