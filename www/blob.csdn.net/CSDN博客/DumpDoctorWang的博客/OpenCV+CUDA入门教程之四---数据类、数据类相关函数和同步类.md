# OpenCV+CUDA入门教程之四---数据类、数据类相关函数和同步类 - DumpDoctorWang的博客 - CSDN博客





2018年07月18日 21:00:47[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：407








**目录**

[一、数据类](#%E4%B8%80%E3%80%81%E6%95%B0%E6%8D%AE%E7%B1%BB)

[二、数据相关类及相关函数](#%E4%BA%8C%E3%80%81%E6%95%B0%E6%8D%AE%E7%9B%B8%E5%85%B3%E7%B1%BB%E5%8F%8A%E7%9B%B8%E5%85%B3%E5%87%BD%E6%95%B0)

[三、同步相关类](#%E4%B8%89%E3%80%81%E5%90%8C%E6%AD%A5%E7%9B%B8%E5%85%B3%E7%B1%BB)

# 一、数据类

**1、PtrStepSz**

一个包装设备端内存(GPU内存，也就是显存)的轻量级类。可作为CUDA核函数(CUDA kernel)的参数。可以从主机端调用其成员函数、也可以从设备端调用其成员函数。

```cpp
template<typename T>
struct PtrStepSz {
    int cols;
    int rows;
    T *data;
    size_t step;

    /**
    * 默认构造函数
    */
    PtrStepSz() : cols(0), rows(0), data(0), step(0) {};

    /**
    * 构造函数，初始化一个由参数指定的PtrStepSz实例
    * @param rows 行
    * @param cols 列
    * @param data 数据指针
    * @param step 一行数据的长度(包含填充的字节)，单位为字节
    */
    PtrStepSz(int rows, int cols, T *data, size_t step);

    /**
    * 构造函数，以别的类型的PtrStepSz<U>实例来初始化
    * @tparam U 类型
    * @param d 类型为PtrStepSz<U>的实例
    */
    template<typename U>
    explicit PtrStepSz(const PtrStepSz<U> &d);

    /**
    * 元素大小，字节
    */
    typedef T elem_type;
    enum {
        elem_size = sizeof(elem_type)
    };

    /**
    * @return 返回元素字节数
    */
    __CV_GPU_HOST_DEVICE__ size_t

    elemSize() const;

    /**
    * 返回指向第y行第一个数据的指针
    * @param y 函数
    * @return 指针
    */
    __CV_GPU_HOST_DEVICE__ T
    *

    ptr(int y = 0);

    __CV_GPU_HOST_DEVICE__ const T
    *

    ptr(int y = 0) const;
};

typedef PtrStepSz<unsigned char> PtrStepSzb;
typedef PtrStepSz<float> PtrStepSzf;
typedef PtrStepSz<int> PtrStepSzi;
```

**2、PtrStep**

和PtrStep相似的结构体，只含有数据指针data和每行数据的长度step，为了提高性能而设计。这个结构体应在CUDA核函数(CUDA kernel)中使用。

```cpp
template<typename T> struct PtrStep
{
    T* data;//数据指针
    size_t step;//每行数据的长度

    /**
    * 默认构造函数
    */
    PtrStep();
    /**
    * 构造函数，使用其他同类型的实例来构造
    * @param mem 实例
    */
    PtrStep(const PtrStepSz<T>& mem);


    typedef T elem_type;
    /**
    * 元素的大小
    */
    enum { elem_size = sizeof(elem_type) };

    /**
    * @return 返回元素大小
    */
    __CV_GPU_HOST_DEVICE__ size_t elemSize() const;
    /**
    * 返回指向第y行第一个元素的指针
    * @param y 行
    * @return 类型为T的指针
    */
    __CV_GPU_HOST_DEVICE__ T* ptr(int y = 0);
    __CV_GPU_HOST_DEVICE__ const T* ptr(int y = 0) const;
};

typedef PtrStep<unsigned char> PtrStep;
typedef PtrStep<float> PtrStepf;
typedef PtrStep<int> PtrStepi;
```

**3、CudaMem(OpenCV2)**

CudaMem的接口类似于Mat。和Mat相比，构造函数多一个参数:内存类型(alloc_type)，分别如下：
- ALLOC_PAGE_LOCKED 分配页锁存(page locked memory)，便于更快和异步的上传下载数据。
- ALLOC_ZEROCOPY 如果设备支持的话，该函数的功能是把主机端的内存地址映射到设备端，这样就不需要在主机端和设备端来回拷贝数据，数据的同步由CUDA自动完成。
- ALLOC_WRITE_COMBINED 分配不会占CPU Cache的内存(write combined buffer)，只有GPU会读取该内存。避免占用CPU的Cache，优化主机端程序的速度。

以上的几种内存，不懂的请参考CUDA的文档中关于内存的部分。

```cpp
class CV_EXPORTS CudaMem
{
public:
    /**
    * 枚举变量，列出内存类型
    */
    enum { ALLOC_PAGE_LOCKED = 1, ALLOC_ZEROCOPY = 2,
        ALLOC_WRITE_COMBINED = 4 };

    /**
    * 构造函数
    * @param size 大小
    * @param type 元素类型
    * @param alloc_type 分配的内存类型，默认为页锁存(Page locked memory)
    */
    CudaMem(Size size, int type, int alloc_type = ALLOC_PAGE_LOCKED);

    /**
    * 从m中复制数据并初始化
    * @param m Mat的实例
    * @param alloc_type 分配的内存类型，默认为页锁存(Page locked memory)
    */
    explicit CudaMem(const Mat& m, int alloc_type = ALLOC_PAGE_LOCKED);

    ......

    /**
    * 初始化CudaMem实例
    * @param size 大小
    * @param type 元素类型
    * @param alloc_type 分配的内存类型，默认为页锁存(Page locked memory)
    */
    void create(Size size, int type, int alloc_type = ALLOC_PAGE_LOCKED);

    /**
    * 返回一个Mat头，数据为CudaMemory中的数据。
    * @return Mat的实例
    */
    Mat createMatHeader() const;
    operator Mat() const;

    /**
    * 将主机端内存地址映射到设备端
    * @return 返回一个GpuMat
    */
    GpuMat createGpuMatHeader() const;
    operator GpuMat() const;

    /**
    * 判断主机端内存能否映射到设备端内存
    * @return 能返回true，不能返回false
    */
    static bool canMapHostMemory();

    int alloc_type;
};
```

**4、HostMem(OpenCV3)**

在opencv3中，CudaMem的名称更改为HostMem，多一个reshape成员。

```cpp
class CV_EXPORTS HostMem
{
public:
    /**
    * 内存类型，是CUDA中概念；
    * PAGE_LOCKED，页锁存（可提高主机内存和显存之间复制数据的速度），
    * 和OpenCV2中的ALLOC_PAGE_LOCKED对应。
    * SHARED，映射内存(主机内存地址映射到设备端，设备访问显存的时候，
    * 实际上是在访问主机内存)，和OpenCV2中的ALLOC_ZEROCOPY对应。
    * WRITE_COMBINED(不占用CPU的Cache，主要是作为GPU传输数据用)。
    * 和OpenCV2中的ALLOC_WRITE_COMBINED对应。
    */
    enum AllocType { PAGE_LOCKED = 1, SHARED = 2, WRITE_COMBINED = 4 };

    static MatAllocator* getAllocator(AllocType alloc_type = PAGE_LOCKED);

    //! 默认构造函数
    explicit HostMem(AllocType alloc_type = PAGE_LOCKED);

    //! 构造函数，参数为HostMem的其他实例
    HostMem(const HostMem& m);

    //! 构造函数，新建指定大小和类型的HostMem实例
    HostMem(int rows, int cols, int type, AllocType alloc_type = PAGE_LOCKED);
    HostMem(Size size, int type, AllocType alloc_type = PAGE_LOCKED);

    //! 构造函数，根据arr的类型和大小新建HostMem实例，并从arr中复制数据
    explicit HostMem(InputArray arr, AllocType alloc_type = PAGE_LOCKED);

    ~HostMem();

    HostMem& operator =(const HostMem& m);

    //! 和b交换
    void swap(HostMem& b);

    //! HostMem的深拷贝，数据也拷贝。
    HostMem clone() const;

    //! 完成分配内存等一些列工作，size和type已经指定时，调用此函数无效
    void create(int rows, int cols, int type);
    void create(Size size, int type);

    /**
    * 创建一个通道数为cn，行数为rows的HostMem新实例，不过数据和本实例共享。
    * 任何一个实例修改数据时，都会影响到另外一个实例。
    * @param cn 通道数
    * @param rows 行数
    * @return HostMem的实例
    */
    HostMem reshape(int cn, int rows = 0) const;

    //! 对数据的引用计数减一，当为引用计数为0时释放内存。
    //! (OpenCV中的Mat、GpuMat、HostMem的数据指针都是智能指针，包含引用技术功能)
    void release();

    //! 返回大小、类型和本实例相同，数据指针和本示例相同的Mat头
    Mat createMatHeader() const;

    /**
    * 把主机内存映射到显存。并返回GpuMat的一个实例。当且仅当alloc_type
    * 为SHARED且GPU支持映射这个功能。
    * @return GpuMat的实例
    */
    GpuMat createGpuMatHeader() const;

    // 参见Mat的文档
    bool isContinuous() const;
    size_t elemSize() const;
    size_t elemSize1() const;
    int type() const;
    int depth() const;
    int channels() const;
    size_t step1() const;
    Size size() const;
    bool empty() const;

    // 参见Mat的文档
    int flags;
    int rows, cols;
    size_t step;

    uchar* data;
    int* refcount;

    uchar* datastart;
    const uchar* dataend;

    AllocType alloc_type;
};
```

**4、GpuMat**

GpuMat类似于Mat，不过管理的是GPU内存，也就是显存。GpuMat和Mat相比，存在以下限制：
- 只支持2D，也就是二维数据。
- 没有返回指向数据的指针的函数。
- 不支持模板技术。

请注意，后一种限制可能会引起重载的矩阵运算符导致内存分配（从官方文档中翻译过来的，不知道啥意思）。GpuMat可以转换为PtrStepSz和PtrStep，因此，GpuMat可以直接当做参数传给CUDA的kernel。

注意：与Mat相比，大多数情况下 GpuMat::isContinuous() == false，这意味着每行所占的内存大小(row size)会对齐到依赖于设备的一个大小(对齐内存，存取速度更快)。只有一行的GpuMat总是连续的。接下来给出GpuMat的部分成员函数（其他函数请参考Mat的文档）：

```cpp
class CV_EXPORTS GpuMat
{
public:
    /**
    * 构造函数。参数含义和Mat的相同
    */
    GpuMat();
    GpuMat(int rows, int cols, int type);
    GpuMat(Size size, int type);

    .....

    /**
    * 从m中的数据来初始化一个GpuMat。初始化完成前会阻塞当前线程。
    * @param m Mat类型的实例
    */
    explicit GpuMat (const Mat& m);


    /**
    * 将GpuMat的row、cols、step、data(数据指针)作为参数构造PtrStepSz<T>的实例，
    * 作为CUDA内核的参数时，对实例中的数据进行修改时，会影响到对应的GpuMat中的数据。
    * @tparam T 类型
    * @return 类型为PtrStepSz<T>的实例
    */
    template <class T> operator PtrStepSz<T>() const;
    /**
    * 将GpuMat的step、data(数据指针)作为参数构造PtrStep<T>的实例，作为CUDA内核的
    * 参数时，对实例中的数据进行修改时，会影响到对应的GpuMat中的数据。
    * @tparam T 类型
    * @return 类型为PtrStep<T>的实例
    */
    template <class T> operator PtrStep<T>() const;

    /**
    * 以m中的数据来构建GpuMat，并将Mat的数据上传到GPU内存。上传完成以前将会阻塞当前线程。
    * @param m Mat的实例
    */
    void upload(const cv::Mat& m);
    /**
    * 以m中的数据来构建GpuMat，并将Mat的数据上传到GPU内存。上传任务会放到stream中(类似
    * 于CUDA中的Stream)，是一个异步过程，不会阻塞当前线程，但需要同步。
    * @param m 类型CudaMem的实例
    * @param stream 类型Stream的实例
    */
    void upload(const CudaMem& m, Stream& stream);

    /**
    * 从GPU端把数据下载到m中，下载完成前会阻塞当前线程。
    * @param m 类型Mat的实例
    */
    void download(cv::Mat& m) const;

    /**
    * 从GPU端把数据下载到m中，下载任务会被放到stream中，不会阻塞当前线程，但需要同步。
    * @param m 类型m的实例
    * @param stream stream 类型Stream的实例
    */
    void download(CudaMem& m, Stream& stream) const;
};
```

# 二、数据相关类及相关函数

**1、createContinuous**

在显存中创建连续矩阵(continuous matrix ,每行的结尾没有空白填充字节，行与行之间的数据是连续的)。

```cpp
/**
* 在显存中创建连续矩阵
* @param rows 行数
* @param cols 列数
* @param type 类型
* @param m 创建好的GpuMat实例
*/
void createContinuous(int rows, int cols, int type, GpuMat& m);
GpuMat createContinuous(int rows, int cols, int type);
void createContinuous(Size size, int type, GpuMat& m);
GpuMat createContinuous(Size size, int type);
```

**2、ensureSizeIsEnough**

确保矩阵有足够的内存和是否是恰当的类型。如果都满足的话不会重新分配。

```cpp
/**
* 确保m有足够的内存和恰当的类型，如果都满足的话不会重新分配内存
* @param rows 最小的行数
* @param cols 最小的列数
* @param size 包含rows和cols大小
* @param type 类型
* @param m GpuMat的实例
*/
void ensureSizeIsEnough(int rows, int cols, int type, GpuMat& m);
void ensureSizeIsEnough(Size size, int type, GpuMat& m);
```

**3、registerPageLocked、unregisterPageLocked**

映射内存、取消映射内存。内存映射是指把主机内存中的一段地址空间映射到GPU显存的一段地址空间。

```cpp
/**
* 把m的内存变为页锁存(Paged lock memery)映射(map)到显存
* @param m Mat的实例
*/
CV_EXPORTS void registerPageLocked(Mat& m);

/**
* 把m的内存取消映射(unmap)，并变回普通内存。
* @param m Mat的实例
*/
CV_EXPORTS void unregisterPageLocked(Mat& m);
```

# 三、同步相关类

**1、Stream(OpenCV2)**

Stream类封装了一个异步调用的流(可以理解为一系列有先后顺序的调用组成的队列)。某些函数的重载多了一个Stream类型的参数，这些函数做好初始化工作(分配输出缓存、上传常量等等)之后，启动CUDA Kernel，然后立即返回，不管kernel是否执行完成。你可以通过调用Stream::queryIfComplete()来查询队列中的所有操作是否已经完成。通过Stream，你可以异步的上/下载页锁存(Page-locked buffer)到显存中。参数为CudaMem或者数据指针指向一个CudaMem的数据区域的Mat实例。

注意：当把两次相同的调用(分别有不同的数据)放入队列中时，会遇到一些问题，两次调用都用到了Constant Memory，后一次的调用就有可能覆盖之前调用的常量内存，造成错误。不过，把不同的调用放入到队列中时，是安全的，不会出现上述错误。内存复制、上传、下载和初始化都是安全的，可以放心的放到stream中。另外stream**并不是线程安全的，**在不同的的线程中需要使用不用的Stream实例。

```cpp
class CV_EXPORTS Stream
{
public:
    Stream();
    ~Stream();

    Stream(const Stream&);
    Stream& operator =(const Stream&);

    //! 查询当前stream中调用是否已经完成，完成返回true
    bool queryIfComplete();
    //! 直到当前stream中的任务完成前，阻塞当前线程
    void waitForCompletion();

    //! 异步地把数据从src(数据在显存)下载到dst中
    void enqueueDownload(const GpuMat& src, CudaMem& dst);
    //! 异步地把数据从src(数据在显存)下载到dst中。dst需要指向一个CudaMem的数据区
    void enqueueDownload(const GpuMat& src, Mat& dst);

    //! 异步地把数据从src上传到dst(显存)
    void enqueueUpload(const CudaMem& src, GpuMat& dst);
    //! 异步地把数据从src(内存)上传到dst(显存)。src的数据指针需要指向一个CudaMem的数据区
    void enqueueUpload(const Mat& src, GpuMat& dst);

    //! 异步地复制数据(显存复制到显存)
    void enqueueCopy(const GpuMat& src, GpuMat& dst);

    //! 异步地初始化内存，mask指定要初始化的区域
    void enqueueMemSet(GpuMat& src, Scalar val);
    void enqueueMemSet(GpuMat& src, Scalar val, const GpuMat& mask);

    //! 转换GpuMat的数据类型。dtype是dst的类型
    void enqueueConvert(const GpuMat& src, GpuMat& dst, int dtype, double a = 1, double b = 0);

    typedef void (*StreamCallback)(Stream& stream, int status, void* userData);
    //! 添加一个回调函数。这个函数将在当前队列中的操作调用完成以后被调用。
    //! 注意：回调函数中不能调用任何CUDA API！！！
    void enqueueHostCallback(StreamCallback callback, void* userData);

    //! 返回默认stream
    static Stream& Null();

    //! 如果不是默认stream，返回true
    operator bool() const;
};
```

**2、Stream和Event(OpenCV3)**

在OpenCV3中，Stream和Event类都是为了异步调用的类，Stream的变化比较大

```cpp
class CV_EXPORTS Stream
{
    typedef void (Stream::*bool_type)() const;
    void this_type_does_not_support_comparisons() const {}

public:
    //! 回调函数形式
    typedef void (*StreamCallback)(int status, void* userData);

    //! 构造函数
    Stream();

    //! 以自定义的allocator初始化Stream的实例
    Stream(const Ptr<GpuMat::Allocator>& allocator);

    //! 查询当前stream中调用是否已经完成，完成返回true
    bool queryIfComplete() const;

    //! 直到当前stream中的任务完成前，阻塞当前线程
    void waitForCompletion();

    //! 阻塞直到事件event发生
    void waitEvent(const Event& event);


    /**
     * 添加一个回调函数。这个函数将在当前队列中的操作调用完成以后被调用。
     * @note 回调函数不能调用任何CUDA API。
     * @param callback 回调函数
     * @param userData 用户数据
     */
    void enqueueHostCallback(StreamCallback callback, void* userData);

    //! 返回默认stream
    static Stream& Null();

    //! 如果不是默认stream，返回true
    operator bool_type() const;

    class Impl;

private:
    Ptr<Impl> impl_;
    Stream(const Ptr<Impl>& impl);

    friend struct StreamAccessor;
    friend class BufferPool;
    friend class DefaultDeviceInitializer;
};
```

事件

```cpp
class CV_EXPORTS Event
{
public:
    enum CreateFlags
    {
        DEFAULT        = 0x00,  /**< 默认事件 */
        BLOCKING_SYNC  = 0x01,  /**< 阻塞同步事件 */
        DISABLE_TIMING = 0x02,  /**< 非计时事件 */
        INTERPROCESS   = 0x04   /**< 适合INTERPROCESS的非计时事件 */
    };

    //! 构造函数
    explicit Event(CreateFlags flags = DEFAULT);

    //! 记录事件
    void record(Stream& stream = Stream::Null());

    //! 查询事件是偶完成
    bool queryIfComplete() const;

    //! 阻塞当前线程，直到事件完成
    void waitForCompletion();

    //! 计算两事件的时间
    static float elapsedTime(const Event& start, const Event& end);

    class Impl;

private:
    Ptr<Impl> impl_;
    Event(const Ptr<Impl>& impl);

    friend struct EventAccessor;
};
```

**3、StreamAccessor、EventAccessor**

有时候程序中需要直接写CUDA程序，就要用到cudaStream_t或者cudaEvent_t，就需要这两个类提供的函数来获取。

```cpp
//opencv2
#include <opencv2/gpu/stream_accessor.hpp>

struct StreamAccessor
{
    //! @brief 从cuda::Stream获取cudaStream_t
    CV_EXPORTS static cudaStream_t getStream(const Stream& stream);
    //! @brief 把cudaStream_t封装为Stream
    CV_EXPORTS static Stream wrapStream(cudaStream_t stream);
};
```

```cpp
//opencv3
#include <opencv2/core/cuda_stream_accessor.hpp>

struct StreamAccessor
{
    //! @brief 从cuda::Stream获取cudaStream_t
    CV_EXPORTS static cudaStream_t getStream(const Stream& stream);
    //! @brief 把cudaStream_t封装为Stream
    CV_EXPORTS static Stream wrapStream(cudaStream_t stream);
};


struct EventAccessor
{
    //! @brief 从cuda::Event获取cudaEvent_t
    CV_EXPORTS static cudaEvent_t getEvent(const Event& event);
    //! @brief 把cudaEvent_t封装为cuda::Event
    CV_EXPORTS static Event wrapEvent(cudaEvent_t event);
};
```





