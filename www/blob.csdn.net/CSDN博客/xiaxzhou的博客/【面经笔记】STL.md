# 【面经笔记】STL - xiaxzhou的博客 - CSDN博客





2017年07月16日 21:10:37[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：383








#### **空间配置器：**

为什么不说allocator是内存配置器：**空间不一定是内存**，空间也**可以是磁盘或其他存储介质**，你可以写一个allocator直接向硬盘取空间。

SGI STL的配置器名为alloc，是缺省的空间配置器。 

虽然也定义了一个符合部分标准的allocator配置器，但只是把C++中new和delete简单包装而已，性能不佳。

> 
**SGI  std::alloc**:


new包含了：operator new内存配置 和 对象构造 两个过程。 

delete包含了： 对象析构 和 operator delete内存释放

STL 将两个阶段操作区分开来：
- 
**内存配置**由alloc::allocatr()负责，内存释放由alloc::deallocate()负责；

- 
**对象构造**由construct()负责，对象析构由deallocate()负责。


考虑小型区块造成的内存破碎问题，SGI设计了双层级配置器
- 
第一级直接使用allocate()调用malloc()、deallocate()调用free()，使用类似new_handler机制解决内存不足，配置无法满足的问题。

- 
第二级视情况使用不同的策略，当配置区块大于128bytes时，调用第一级配置器，当配置区块小于128bytes时，采用内存池的整理方式：配置器维护16个（128/8）自由链表，负责16种小型区块的此配置能力。内存池以malloc配置而得，如果内存不足转第一级配置器处理。


##### **内存池**

内存池管理：每次配置一大块内存，并维护对应之自由链表：free_list，下次若还有相同大小的内存需求，**直接从链表中拔出**，如果客户端释还小额区块，就由配置器**回收到free_list中**。为了方便管理，任何小额内存需求均上调至8的倍数。

allocator需要维护一个存储16个空闲块列表表头的数组free_list，数组元素i是一个指向块大小为8*(i+1)字节的空闲块列表的表头，**一个指向内存池起始地址的指针start_free和一个指向结束地址的指针end_free**。空闲块列表节点的结构如下：

```
union obj 
{ 
        union obj *free_list_link; 
        char client_data[1]; 
};
```

这个结构可以看做是从一个内存块中抠出4个字节大小来，当这个内存块空闲时，它存储了下个空闲块，当这个内存块交付给用户时，它存储的时用户的数据

当free_list不够时，从内存池中取新空间为free_list填充新空间。 

当内存池不够时，从堆申请新空间。 

当堆不够时，交由第一级配置器使用类似new_handler机制处理。
#### **iterator**

> 
traits（萃取）技术： 


**利用模板的参数推导机制，获取任意迭代器的特征信息：**

```bash
Template<class T>
Struct  iterator_traits
{ 
Typedef  typename T::value_type  value_type;  
 } 
如果T定义有自己的value_type，通过traits的作用，萃取出来的就是T：：value_type。
```

这种多了一层**间接性**的**好处**是可以拥有原生指针的偏特例化版本：如偏特例化原始指针版本`iterator_traits<T*>和iterator_traits<const T*>`。

原生指针int*虽不是一种类类型，亦可通过iterator_traits取其value_type。

#### **序列容器：vector**

> 
频繁对vector调用push_back()对性能的影响和原因：


当**push_back()**将新元素插入vector尾端时，首先检查是否还有备用空间，如果容量被用完，**并不是在原空间后接续新空间，而是以原大小的两倍重新分配一块空间，将原内容拷贝过来**，并添加上新元素，释放原空间。

**一旦push_back()引起空间重新配置，指向原vector的所有迭代器失效。**

#### **序列容器：list**

> 
双向链表


STL list是一个**环状双向链表**

#### **序列容器：deque 双向队列**

deque没有容量（capacity）的概念，动态的以分断连续空间组合而成，可以随时增加一段新的空间并链接起来。

deque是由一段一段的连续空间构成，由迭代器维护整体连续的假象

**中控器为一个连续数组空间（映射/map），每个元素都是指针，指向一段连续线性空间（缓冲区），缓冲区大小一致**

在deque上排序很慢，可将数据拷贝至vector ，排序后拷贝回deque

#### **容器适配器：stack 栈**

stack底层默认以deque实现，不提供迭代器

```cpp
stack<int,deque<int>>
stack<int,list<int>>
```

#### **容器适配器：queue 队列**

queue底层默认是deque实现，不提供迭代器

```cpp
queue<int,deque<int>>
queue<int,list<int>>
```

heap只有算法，属于幕后工作者，是优先队列的助手

#### **容器适配器：priority queue 优先队列**

优先队列默认是最大堆实现 

最大堆默认是vector实现的完全二叉树：

> 
[http://blog.csdn.net/xiaxzhou/article/details/74943175](http://blog.csdn.net/xiaxzhou/article/details/74943175)


```cpp
priority_queue<int,vector<int>,less<int>>
priority_queue<int,vector<int>,greater<int>>
```




