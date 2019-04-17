# (转载)简单linux C++内存池 - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年07月17日 11:17:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2303
个人分类：[C/C++](https://blog.csdn.net/fjssharpsword/article/category/6480441)









C++代码





[?](http://my.csdn.net/cyebo/code/detail/37135#)

```
```cpp
在学习内存池的过程中可谓云游太虚。一般都是针对标准内存池再次实现。大部分以链表的形式讨论。诚然最正宗也最准确，但是相对比较晦涩，本文是针对刚刚接触内存池的同学写的。大大减少了对内存池整体认识的难度。
```



```cpp

```



```cpp
内存池：
```



```cpp
如果程序中涉及频繁申请释放内存，并且每次使用的内存并不是很大，这时候应该考虑内存池。
```



```cpp
内存池可以有有效的避免内存碎片的产生。
```



```cpp
内存池的框架：
```



```cpp
class
```

```cpp
MemPool{
```



```cpp
public
```

```cpp
:
```



```cpp
MemPool（）{初始化分配N个小的内存块}
```



```cpp
~MemPool（）{真正删除整个分配了的内存空间}
```



```cpp
void
```

```cpp
* getmem(){获取内存（块）}
```



```cpp
void
```

```cpp
freemem(){释放内存到内存池中，这里并不是真正的释放掉内存，只是回收再利用；}
```



```cpp
private
```

```cpp
:
```



```cpp
struct
```

```cpp
LinkBlock * p;
```

```cpp
//把很多小内存块关联起来的结构，大部分写成链表的形式
```



```cpp
mem_block* m_block;
```

```cpp
//一个小的内存块。
```



```cpp
}；
```



```cpp
工作机制：
```



```cpp
事先分配出一大块内存，再把这一大块分成很多小块。当程序中需要申请内存时就拿出一小块来用。用完了就把这一小块放回大块中（注意：这个小块内存并没释放掉！），只要不是一下用掉了所有一大块内存，无论多少次申请内存都是重复利用这些小的内存块。知道程序结束真正释放掉整片内存。
```



```cpp
所以用内存池可以把大量的内存申请控制在可计算的范围内。内存碎片少。比如：如果用系统的
```

```cpp
new
```

```cpp
或者
```

```cpp
malloc
```

```cpp
申请10000次内存，可能要10000的小的内存空间，但是使用内存池只需申请100块空间，循环利用100次而已。作用显而易见！
```



```cpp

```



```cpp
为了让程序简单明了使用STL标准库的vector来充当小内存块的管理器。
```



```cpp
vector<
```

```cpp
char
```

```cpp
*> vec; 这样每次申请的是vec的一个迭代器的空间。
```



```cpp

```



```cpp
代码非原创，稍稍做了改动而已。
```



```cpp
========================================================================
```



```cpp
MemPool.h
```



```cpp

```



```cpp
#ifndef _MEM_POOL_H
```



```cpp
#define _MEM_POOL_H
```



```cpp
#include <vector>
```



```cpp
#include <iostream>
```



```cpp
using
```

```cpp
namespace
```

```cpp
std;
```



```cpp

```



```cpp
/*
```



```cpp

```

```cpp
在内存池中分配固定大小的内存块
```



```cpp

```

```cpp
目的是加速内存分配速度，并且减少因重复分配相同
```



```cpp
*/
```



```cpp

```



```cpp
class
```

```cpp
CMemPool
```



```cpp
{
```



```cpp
public
```

```cpp
:
```



```cpp

```



```cpp

```

```cpp
//创建大小为blockSize的内存块，内存池数目为预分配的数目preAlloc
```



```cpp

```

```cpp
CMemPool(
```

```cpp
int
```

```cpp
blockSize,
```

```cpp
int
```

```cpp
preAlloc = 0,
```

```cpp
int
```

```cpp
maxAlloc = 0);
```



```cpp

```



```cpp

```

```cpp
~CMemPool();
```



```cpp

```



```cpp

```

```cpp
//获取一个内存块。如果内存池中没有足够的内存块，则会自动分配新的内存块
```



```cpp

```

```cpp
//如果分配的内存块数目达到了最大值，则会返回一个异常
```



```cpp

```

```cpp
void
```

```cpp
* Get();
```



```cpp

```



```cpp

```

```cpp
//释放当前内存块，将其插入内存池
```



```cpp

```

```cpp
void
```

```cpp
Release(
```

```cpp
void
```

```cpp
* ptr);
```



```cpp

```



```cpp

```

```cpp
//返回内存块大小
```



```cpp

```

```cpp
int
```

```cpp
BlockSize()
```

```cpp
const
```

```cpp
;
```



```cpp

```



```cpp

```

```cpp
//返回内存池中内存块数目
```



```cpp

```

```cpp
int
```

```cpp
Allocated()
```

```cpp
const
```

```cpp
;
```



```cpp

```



```cpp

```

```cpp
//返回内存池中可用的内存块数目
```



```cpp

```

```cpp
int
```

```cpp
Available()
```

```cpp
const
```

```cpp
;
```



```cpp

```



```cpp
private
```

```cpp
:
```



```cpp

```

```cpp
CMemPool();
```



```cpp

```

```cpp
CMemPool(
```

```cpp
const
```

```cpp
CMemPool&);
```



```cpp

```

```cpp
CMemPool& operator = (
```

```cpp
const
```

```cpp
CMemPool&);
```



```cpp

```



```cpp

```

```cpp
enum
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
BLOCK_RESERVE = 128
```



```cpp

```

```cpp
};
```



```cpp

```



```cpp

```

```cpp
typedef
```

```cpp
std::vector<
```

```cpp
char
```

```cpp
*> BlockVec;
```



```cpp

```



```cpp

```

```cpp
int
```

```cpp
m_blockSize;
```



```cpp

```

```cpp
int
```

```cpp
m_maxAlloc;
```



```cpp

```

```cpp
int
```

```cpp
m_allocated;
```



```cpp

```

```cpp
BlockVec    m_blocks;
```



```cpp
};
```



```cpp

```



```cpp
inline
```

```cpp
int
```

```cpp
CMemPool::BlockSize()
```

```cpp
const
```



```cpp
{
```



```cpp

```

```cpp
return
```

```cpp
m_blockSize;
```



```cpp
}
```



```cpp

```



```cpp
inline
```

```cpp
int
```

```cpp
CMemPool::Allocated()
```

```cpp
const
```



```cpp
{
```



```cpp

```

```cpp
return
```

```cpp
m_allocated;
```



```cpp
}
```



```cpp

```



```cpp
inline
```

```cpp
int
```

```cpp
CMemPool::Available()
```

```cpp
const
```



```cpp
{
```



```cpp

```

```cpp
return
```

```cpp
(
```

```cpp
int
```

```cpp
) m_blocks.size();
```



```cpp
}
```



```cpp
#endif
```



```cpp

```



```cpp
=========================================================
```



```cpp
MemPool.cpp
```



```cpp

```



```cpp

```



```cpp
#include "MemPool.h"
```



```cpp
CMemPool::CMemPool(
```

```cpp
int
```

```cpp
blockSize,
```

```cpp
int
```

```cpp
preAlloc,
```

```cpp
int
```

```cpp
maxAlloc):
```



```cpp
m_blockSize(blockSize),
```



```cpp
m_maxAlloc(maxAlloc),
```



```cpp
m_allocated(preAlloc)
```



```cpp
{
```



```cpp

```

```cpp
if
```

```cpp
( preAlloc < 0 || maxAlloc == 0 || maxAlloc < preAlloc )
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
cout<<
```

```cpp
"CMemPool::CMemPool parameter error."
```

```cpp
<<endl;
```



```cpp

```

```cpp
}
```



```cpp

```



```cpp

```

```cpp
int
```

```cpp
r = BLOCK_RESERVE;
```



```cpp

```

```cpp
if
```

```cpp
(preAlloc > r)
```



```cpp

```

```cpp
r = preAlloc;
```



```cpp

```

```cpp
if
```

```cpp
(maxAlloc > 0 && maxAlloc < r)
```



```cpp

```

```cpp
r = maxAlloc;
```



```cpp

```

```cpp
m_blocks.reserve(r);
```



```cpp

```

```cpp
for
```

```cpp
(
```

```cpp
int
```

```cpp
i = 0; i < preAlloc; ++i)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
m_blocks.push_back(
```

```cpp
new
```

```cpp
char
```

```cpp
[m_blockSize]);
```



```cpp

```

```cpp
}
```



```cpp
}
```



```cpp
CMemPool::~CMemPool()
```



```cpp
{
```



```cpp

```

```cpp
for
```

```cpp
(BlockVec::iterator it = m_blocks.begin(); it != m_blocks.end(); ++it)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
delete
```

```cpp
[] *it;
```



```cpp

```

```cpp
}
```



```cpp
}
```



```cpp

```



```cpp
void
```

```cpp
* CMemPool::Get()
```



```cpp
{
```



```cpp

```



```cpp

```

```cpp
if
```

```cpp
(m_blocks.empty())
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
if
```

```cpp
(m_maxAlloc == 0 || m_allocated < m_maxAlloc)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
++m_allocated;
```



```cpp

```

```cpp
return
```

```cpp
new
```

```cpp
char
```

```cpp
[m_blockSize];
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
else
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
cout<<
```

```cpp
"CMemPool::get CMemPool exhausted."
```

```cpp
<<endl;
```



```cpp

```

```cpp
return
```

```cpp
(
```

```cpp
void
```

```cpp
*)NULL;
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
else
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
char
```

```cpp
* ptr = m_blocks.back();
```



```cpp

```

```cpp
m_blocks.pop_back();
```



```cpp

```

```cpp
return
```

```cpp
ptr;
```



```cpp

```

```cpp
}
```



```cpp
}
```



```cpp

```



```cpp

```



```cpp
void
```

```cpp
CMemPool::Release(
```

```cpp
void
```

```cpp
* ptr)
```



```cpp
{
```



```cpp

```

```cpp
memset
```

```cpp
(ptr,0,
```

```cpp
sizeof
```

```cpp
(
```

```cpp
char
```

```cpp
)*m_blockSize);
```

```cpp
//内存回收回来以后并没销毁，原数据仍在，所以应该清空
```



```cpp

```

```cpp
m_blocks.push_back(
```

```cpp
reinterpret_cast
```

```cpp
<
```

```cpp
char
```

```cpp
*>(ptr));
```



```cpp
}
```



```cpp

```



```cpp
=========
```



```cpp
main.h
```



```cpp

```



```cpp
#include "stdafx.h"
```



```cpp
#include "MemPool.h"
```



```cpp
#include <string.h>
```



```cpp
int
```

```cpp
main(
```

```cpp
int
```

```cpp
argc,
```

```cpp
char
```

```cpp
* argv[])
```



```cpp
{
```



```cpp
CMemPool *m_cache =
```

```cpp
new
```

```cpp
CMemPool(50,0,10);
```



```cpp

```



```cpp
char
```

```cpp
* src_date=
```

```cpp
"abcdefg"
```

```cpp
;
```



```cpp
char
```

```cpp
*p1=(
```

```cpp
char
```

```cpp
*)(m_cache->Get());
```



```cpp
char
```

```cpp
*p2=(
```

```cpp
char
```

```cpp
*)(m_cache->Get());
```



```cpp
int
```

```cpp
*p3=(
```

```cpp
int
```

```cpp
*)(m_cache->Get());
```



```cpp
strcpy
```

```cpp
(p1,src_date);
```



```cpp
strcpy
```

```cpp
(p2,src_date);
```



```cpp
p3[0]=9;p3[1]=25;
```



```cpp

```



```cpp
m_cache->Release(p1);
```



```cpp
m_cache->Release(p2);
```



```cpp
m_cache->Release(p3);
```



```cpp

```



```cpp
//把MemPool.cpp中void CMemPool::Release(void* ptr) 的
```



```cpp
//memset(ptr,0,sizeof(char)*m_blockSize);注释掉可以验证每次内存回收以后是重复利用而非销毁
```



```cpp
cout<<*(
```

```cpp
int
```

```cpp
*)(m_cache->Get())<<endl;
```



```cpp
cout<<(
```

```cpp
char
```

```cpp
*)(m_cache->Get())<<endl;
```



```cpp
cout<<(
```

```cpp
char
```

```cpp
*)(m_cache->Get())<<endl;
```



```cpp
delete
```

```cpp
m_cache;
```



```cpp

```

```cpp
return
```

```cpp
0;
```



```cpp
}
```



```cpp

```



```cpp
完毕！
```



```cpp

```



```cpp
当然这只是探究内存池而已，更高级的比如线程安全，内存扩容，模板等等，仍需解决。
```



```cpp
但是相信想初窥内存池门径应该还是有帮助的！
```
```




**描述：**以STL-vector为数据存储单元，实现简单的内存池功能。




