# Caffe中SyncedMemory类阅读

2016年01月02日 22:39:14 [鱼蛋蛋哥](https://me.csdn.net/u010327085) 阅读数：1043



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/u010327085/article/details/50449728

SyncedMemory简单的管理分配内存，并且负责同步CPU和GPU。

## 1.成员变量

```
void* cpu_ptr_;\\cpu中指针
void* gpu_ptr_;\\GPU中指针
size_t size_;\\数据大小
SyncedHead head_;\\当前数据状态
bool own_cpu_data_;\\cpu数据释放被本类所拥有，即没有其他指针指向该数据。
bool cpu_malloc_use_cuda_;\\分配cpu内存是否用的cudaMallocHost()分配。
bool own_gpu_data_;\\作用own_cpu_data类似
int gpu_device_;\\GPU序号
```

关于SymceHead，有四种状态，分别是未初始化，数据在 cpu 中，数据在 gpu 中，数据在 cpu 和 gpu 中都有。enum SyncedHead { UNINITIALIZED , HEAD_AT_CPU , HEAD_AT_GPU , SYNCED };

## 2.函数

**inline函数**

```
inline void CaffeMallocHost(void** ptr, size_t size, bool* use_cuda);
inline void CaffeFreeHost(void* ptr, bool use_cuda);
```

分别是分配和释放内存，这里指的是CPU内存。

**私有成员函数**

```
 void to_cpu();
 void to_gpu();
```

分别为将数据导入CPU和GPU。

**共有成员函数** 
首先是构造函数

```
SyncedMemory();
explicit SyncedMemory(size_t size);
```

第一个为简单初始化，第二个只是把 size （大小）设置了，并未申请内存。

然后是析构函数

```
  ~SyncedMemory();1
```

主要就是释放数据。这里说明一点是，own_gpu_data和own_cpu_data。这两个成员变量的作用我是到这里才明白。表示是否拥有该数据，也即在cpu或gpu中是否有其他指针指向该数据。

接下来可能看着比较繁琐无趣了。

```
const void* cpu_data();
const void* gpu_data();
```

分别是获取cpu，gpu中数据的指针，需要说明的一点是，该过程会同步数据。 
有获取，就有设置，下面两个函数就是设置数据了。

```
void set_cpu_data(void* data);
void set_gpu_data(void* data);
```

需要说明的是，这里设置后就不是拥有该数据，即own_cpu_data或own_gpu_data就为false，因为还有data指向该数据。一般来说，只有当同步后才会为true。也即to_cpu()或者to_gpu()后。

```
void* mutable_cpu_data();
void* mutable_gpu_data();
```

这里理解不是很明了，从功能上说，是返回cpu或gpu数据的指针，并设置数据状态为在cpu或gpu上。具体有什么显著的功效还没看出来。

```
 SyncedHead head();
 size_t size()
```

返回数据状态和大小。

最后一个

```
void async_gpu_push(const cudaStream_t& stream);1
```

只有在没定义CPU_ONLY时才编译。感觉是同步用的，将cpu上的数据同步到gpu上。

## 3.关于DISABLE_COPY_AND_ASSIGN

在头文件中有这样一句

```
DISABLE_COPY_AND_ASSIGN(SyncedMemory);1
```

这其实是一个宏，用来把该类的拷贝函数和等号操作符给禁止掉。 
其实就是

```
private:\
SyncedMemory( const SyncedMemory&);\
SyncedMemory& operator =( const SyncedMemory&)
```

如果想让你的类不能使用 copy 构造函数和赋值操作符，只要将该类的 copy 构造函数和赋值操作符函数定义为 private 即可，并且只是声明，不用实现 .