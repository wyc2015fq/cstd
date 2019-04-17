# OpenCV+CUDA入门教程之三---初始化和设备信息 - DumpDoctorWang的博客 - CSDN博客





2018年07月18日 21:00:14[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：704








**目录**

[一、一些概念及说明](#%E4%B8%80%E3%80%81%E4%B8%80%E4%BA%9B%E6%A6%82%E5%BF%B5%E5%8F%8A%E8%AF%B4%E6%98%8E%EF%BC%9A)

[二、设备信息函数](#%E4%BA%8C%E3%80%81%E8%AE%BE%E5%A4%87%E4%BF%A1%E6%81%AF%E5%87%BD%E6%95%B0)

# 一、一些概念及说明

1、主机端(Host端)、设备端(Device端、GPU端)

在CUDA中，有主机端和设备端这两个概念，主机端是指CPU+内存，设备端是指GPU+显存。主机端的代码在CPU上执行，访问主机内存；设备端代码在GPU上执行，访问显存。在使用GPU计算时，需要在主机内存好显存之间来回拷贝数据；当然，一些新技术可以不用拷贝数据，请参考后面的章节或者CUDA文档中关于内存的部分。

2、GPU模块在OpenCV2和OpenCV3中的区别
- 模块变化：在OpenCV2中，GPU模块的头文件主要在opencv2/gpu这个文件夹里。在OpenCV3中，头文件主要在opencv2/core和opencv2/cudev里面，其他文件夹也有。
- 命名空间变化：在OpenCV2中，GPU模块的命名空间为cv::gpu；在opencv3中，命名空间为cv::cuda。接下来介绍的函数和类，在opencv2中，属于cv::gpu这个命名空间；在opencv3中属于命名空间cv::cuda。使用这些函数或者类时，需要在头文件之后指定命名空间(opencv2: using namespace cv::gpu;，opencv3: using namespace cv::cuda;)或者在类或者函数的前面加上命名空间名称(opencv2: cv::gpu::setDevice(0)，opencv3: cv::cuda::setDevice(0))。

3、CUDA 核函数(CUDA kernel)

CUDA核函数是CUDA里面额概念，是指在GPU上执行的函数。它的语法是C++语言的一个子集，然后前面用特殊的标识描述。

4、GPU模块与CUDA模块

同一个模块的不同叫法。

# 二、设备信息函数

1、getCudaEnabledDeviceCount

返回已安装的支持CUDA的设备的数量。

int getCudaEnabledDeviceCount()

在任何其他GPU函数调用之前使用此函数。 如果在没有启用GPU(CUDA)支持的情况下编译OpenCV，则此函数返回0。

2、setDevice

设置设备并为当前线程初始化它。

void  setDevice(int device)

    参数：

         device - 以0开头的GPU设备的系统索引。

如果忽略此函数的调用，则在第一次GPU使用时初始化默认设备。

3、getDevice

返回由gpu :: setDevice()设置或默认初始化的当前设备索引。

int getDevice()

4、resetDevice

明确销毁并清除当前进程中与当前设备关联的所有资源。

void resetDevice()

对此设备的任何后续API调用都将重新初始化设备。

5、FeatureSet

GPU特性枚举。

```
// opencv2 <opencv2/gpu/gpu.hpp>
enum FeatureSet
{
    FEATURE_SET_COMPUTE_10 = 10,
    FEATURE_SET_COMPUTE_11 = 11,
    FEATURE_SET_COMPUTE_12 = 12,
    FEATURE_SET_COMPUTE_13 = 13,
    FEATURE_SET_COMPUTE_20 = 20,
    FEATURE_SET_COMPUTE_21 = 21,
    FEATURE_SET_COMPUTE_30 = 30,
    FEATURE_SET_COMPUTE_35 = 35,

    GLOBAL_ATOMICS = FEATURE_SET_COMPUTE_11,
    SHARED_ATOMICS = FEATURE_SET_COMPUTE_12,
    NATIVE_DOUBLE = FEATURE_SET_COMPUTE_13,
    WARP_SHUFFLE_FUNCTIONS = FEATURE_SET_COMPUTE_30,
    DYNAMIC_PARALLELISM = FEATURE_SET_COMPUTE_35
};
```

opencv3

```cpp
// opencv3 <opencv2/core/cuda.hpp>
enum FeatureSet
{
    FEATURE_SET_COMPUTE_10 = 10,
    FEATURE_SET_COMPUTE_11 = 11,
    FEATURE_SET_COMPUTE_12 = 12,
    FEATURE_SET_COMPUTE_13 = 13,
    FEATURE_SET_COMPUTE_20 = 20,
    FEATURE_SET_COMPUTE_21 = 21,
    FEATURE_SET_COMPUTE_30 = 30,
    FEATURE_SET_COMPUTE_32 = 32,
    FEATURE_SET_COMPUTE_35 = 35,
    FEATURE_SET_COMPUTE_50 = 50,

    GLOBAL_ATOMICS = FEATURE_SET_COMPUTE_11,
    SHARED_ATOMICS = FEATURE_SET_COMPUTE_12,
    NATIVE_DOUBLE = FEATURE_SET_COMPUTE_13,
    WARP_SHUFFLE_FUNCTIONS = FEATURE_SET_COMPUTE_30,
    DYNAMIC_PARALLELISM = FEATURE_SET_COMPUTE_35
};
```

6、类TargetArchs

提供一组静态方法的类，用来检查OpenCV的GPU模块是否支持某些特性。

下面是一组方法，用来检查模块是否包含给定体系结构的中间（PTX）或二进制GPU代码：

```cpp
/**
* 检查是否支持feature_set里面指定的特性
* @param feature_set 特性
* @return 支持返回true，否则返回false
*/
static bool TargetArchs::builtWith(FeatureSet feature_set);
/**
*
* @param major 主计算能力版本.
* @param minor 主计算能力版本.
* @return
*/
static bool TargetArchs::has(int major, int minor);
static bool TargetArchs::hasPtx(int major, int minor);
static bool TargetArchs::hasBin(int major, int minor);
static bool TargetArchs::hasEqualOrLessPtx(int major, int minor);
static bool TargetArchs::hasEqualOrGreater(int major, int minor);
static bool TargetArchs::hasEqualOrGreaterPtx(int major, int minor);
static bool TargetArchs::hasEqualOrGreaterBin(int major, int minor);
```

7、DeviceInfo

提供查询指定GPU属性的功能的类。

```cpp
// opencv2
class CV_EXPORTS DeviceInfo{
        public:
        /**
        * 构造函数，初始化当前设备的设备信息
        */
        DeviceInfo();

        /**
        * 构造函数，初始化指定device_id的设备信息
        * @param device_id GPU的设备索引，第一个设备为0
        */
        DeviceInfo(int device_id);

        /**
        * @return 返回设备名称
        */
        string name() const;

        /**
        * @return 返回主计算能力的版本，如CC为“5.2”，则返回5
        */
        int majorVersion() const;

        /**
        * @return 返回次计算能力的版本，如CC为“5.2”，则返回2
        */
        int minorVersion() const;

        /**
        * @return 返回流处理器个数
        */
        int multiProcessorCount() const;

        /**
        * @return 返回GPU可用内存
        */
        size_t freeMemory() const;

        /**
        * @return 返回GPU总内存
        */
        size_t totalMemory() const;

        /**
        * 查询设备是否支持指定特性
        * @param feature 特性
        * @return 支持返回true，不支持返回false
        */
        bool supports(FeatureSet feature) const;

        /**
        * 查询GPU模块是否支持指定设备
        * @return 支持返回true，不支持返回false
        */
        bool isCompatible() const;

        /**
        * @return 返回设备ID
        */
        int deviceID() const;
};
```

在opencv3中DeviceInfo可以查询的信息更多，头文件里对每个函数都有详细的说明，我就懒得翻译了。另外，CudaMem::canMapHostMemory()移到了DeviceInfo这个类里面。

```cpp
/**
* 打印设备device的详细信息
* @param device int类型的值，从0开始的设备id
*/
CV_EXPORTS void printCudaDeviceInfo(int device);
/**
* 打印设备device的缩略信息
* @param device int类型的值，从0开始的设备id
*/
CV_EXPORTS void printShortCudaDeviceInfo(int device);
```





