# 特定于类的内存管理（C++必知必会，代码解析） - 长歌行 - CSDN博客





2013年01月29日 10:59:12[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1104
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)









C++ 必知必会里面有这么一节，说明如何给对特定类的内存进行管理，当然，和普通的内存池有一定的差别。

最基础的内存池，只是提供一个内存分配的容器，需要多少内存，直接从内存池中获取，返回特定的指针，并不只是适用于特定的类。




当然都是重载了operator new和operator delete

ps.由于没有进入构造函数，所以构造函数里面使用Init（）函数来初始化对象。不要直接初始化对象。




放上VS2008编译通过的代码：



```cpp
#include "stdafx.h"
#include <Windows.h>
#include <assert.h>

class Handle
{
public:
	virtual ~Handle()
	{

	}
	void*	operator new(size_t);
	void	operator delete(void*);
protected:
private:
};

struct SRep
{
	enum{ max = 1000};
	static SRep			*free;			// head of freeList
	static int			num_used;		// num of used slot
	union
	{
		char store[sizeof(Handle)];
		SRep *pNext;
	};
};

static	SRep s_Mem[SRep::max];
SRep*	SRep::free		= NULL;
int		SRep::num_used	= 0;

void* Handle::operator new(size_t)
{
	if (SRep::free)
	{
		SRep *pTmp = SRep::free;
		SRep::free = SRep::free->pNext;
		return pTmp;
	}
	else if (SRep::num_used < SRep::max)
	{
		return &s_Mem[SRep::num_used++];
	}
	else
	{
		assert(0);
		return NULL;
	}
}


void Handle::operator delete(void *p)
{
	static_cast<SRep *>(p)->pNext = SRep::free;
	SRep::free = static_cast<SRep*>(p);
}


int _tmain(int argc, _TCHAR* argv[])
{
	Handle *p = new Handle;
	delete p;
	return 0;
}
```


简单说明一下吧

s_Mem创建之后，系统就为它分配了一段内存，给了个内存地址，假设是0x00000001;

Handle假设占用内存为4

s_Mem[1]应该就是0x00000001 + 4;

当new 了p1，p2两个handle指针时，内存地址分别为0x00000001,0x00000005

当deletep2,p2的next指向了free指针，也就是内存分配的头结点，此时是0x00000000，空指针。然后头指针指向p2，就是说free的内存地址是0x00000005

再new p3时，就不像p1p2直接从s_Mem[]中按顺序取数据了。因为有之前用过的空间啊，所以要使用之前用过的内存地址0x00000005

free指向的头是原来p2的地址0x00000005，这个地址就给p3了。然后头指针就要找上上个释放了的指针，然后用它来做头，当然，上上个释放的指针不存在，所以地址是0x00000000




所以，使用该方法删除类的指针时，会建立一个删除指针的地址的链表，之后会根据这个链表的顺序逐一分配内存地址，如果没有删除指针的链表，就直接从数组中按顺序拿地址了。




ps.16进制的一个数字可以表示二进制的4个数字2^4 = 16，所以8个16进制的数字，表示32个二进制数字，也就是32位。



