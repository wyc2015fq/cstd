# OpenCV+CUDA入门教程之二---GPU模块简介 - DumpDoctorWang的博客 - CSDN博客





2018年07月18日 20:59:50[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：440








**目录**

[一、概括信息](#%E4%B8%80%E3%80%81%E6%A6%82%E6%8B%AC%E4%BF%A1%E6%81%AF)

[二、针对不同GPU架构的编译选项](#%E4%BA%8C%E3%80%81%E9%92%88%E5%AF%B9%E4%B8%8D%E5%90%8CGPU%E6%9E%B6%E6%9E%84%E7%9A%84%E7%BC%96%E8%AF%91%E9%80%89%E9%A1%B9)

[三、多GPU支持](#%E4%B8%89%E3%80%81%E5%A4%9AGPU%E6%94%AF%E6%8C%81)

## 一、概括信息

OpenCV的GPU模块是一组利用GPU计算功能的类和函数。 它使用NVIDIA公司的CUDA API实现，仅支持NVIDIA GPU。 OpenCV GPU模块包括工具函数，和高级算法。工具函数函数和低级视觉函数为开发利用GPU的快速视觉算法提供了强大的基础，而高级算法包括一些最先进的算法（人脸和人体检测器等） 。

OpenCV GPU模块易于使用，不需要任何CUDA知识，但是CUDA知识对于处理不是一般的算法或实现最高性能算法肯定是有用的。GPU模块是快速实现GPU加速计算机视觉算法的有效工具。但是，如果你的算法涉及许多简单的操作，那么为了获得最佳性能，可能仍需要编写自己的CUDA内核以避免对中间结果进行额外的写入和读取操作。

要启用CUDA支持，在使用cmake编译OpenCV时，应加上WITH_CUDA = ON这个选项。如果不启用CUDA模块，GPU模块仍然构建，但在运行时，模块中的所有函数都抛出异常，并带有CV_GpuNotSupported错误代码，但gpu :: getCudaEnabledDeviceCount()除外。因此，使用gpu :: getCudaEnabledDeviceCount()函数(返回支持CUDA的GPU数)，你可以根据上述函数的返回值选择使用主机端的函数还是GPU模块的函数(使用CPU完成计算还是使用GPU完成计算)。

## 二、针对不同GPU架构的编译选项

CUDA的编译器可以根据参数生成指定GPU架构的二进制代码（cubin和fatbin）和向后兼容的中间代码（PTX）。 二进制代码通常意味着为特定的GPU架构和生成，因此无法保证与其他GPU架构的兼容性。 PTX是中间层代码，GPU载入PTX文件之后，再编译成自己的架构编译成相对应的二进制文件。 新架构的GPU支持低版本的PTX代码，但是一些新的特性不被支持。

在第一次调用GPU模块的函数时，将初始化CUDA环境，并使用JIT编译器将PTX代码编译为特定GPU的二进制代码。 当目标GPU的计算能力(Compute Capability，缩写为CC，是CUDA中的概念)低于PTX代码时，编译会失败。

编译OpenCV时，可通过指定CUDA_ARCH_BIN和CUDA_ARCH_PTX两个参数来控制二进制代码的版本和PTX代码版本。例如-D CUDA_ARCH_BIN =“2.0 3.0 5.0 5.2”  -D CUDA_ARCH_PTX="2.0 3.0 5.0 5.2", 就是启用了CC为2.0、3.0、5.0、5.2的架构支持。[CUDA计算能力查询表](https://blog.csdn.net/allyli0022/article/details/54628987)。附上自己的cmake命令：

```bash
cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local/opencv320 \
    -D OPENCV_EXTRA_MODULES_PATH=/home/creator/Libs/OpenCV/opencv-3.2.0/opencv_contrib-3.2.0/modules \
    -D WITH_CUDA=ON \
    -D CUDA_ARCH_BIN="3.0 5.0 5.2" \
    -D CUDA_ARCH_PTX="3.0 5.0 5.2" \
    -D ENABLE_FAST_MATH=ON \
    -D CUDA_FAST_MATH=ON \
    -D WITH_CUBLAS=1 \
    -D WITH_NVCUVID=on \
    -D CUDA_GENERATION=Auto \
    -D WITH_TBB=ON \
    -D WITH_OPENMP=ON \
    -D WITH_OPENGL=ON ..
```

可以在运行时确定OpenCV GPU构建的二进制文件（或PTX代码）是否与你的GPU兼容。函数gpu :: DeviceInfo :: isCompatible()返回兼容性（true / false）。

## 三、多GPU支持

在当前版本中，每个OpenCV GPU算法只能使用一个GPU。因此，要使用多个GPU，必须手动切换GPU。切换GPU可以使用gpu :: setDevice()函数完成。有关详细信息，请阅读Cuda C编程指南。

在为多个GPU开发算法时，请注意数据传递开销。对于基础函数和小图像，它可能很重要，这可能消除了具有多个GPU的所有优点。但对于高级算法，请考虑使用多GPU加速。



