# VC中关于GlobalAlloc，GlobalLock，GlobalUnLock的用法及疑问 - L_Andy的专栏 - CSDN博客

2014年05月12日 17:55:20[卡哥](https://me.csdn.net/L_Andy)阅读数：2395



调用GlobalAlloc函数分配一块内存，该函数会返回分配的内存句柄。 

调用GlobalLock函数锁定内存块，该函数接受一个内存句柄作为参数，然后返回一个指向被锁定的内存块的指针。 您可以用该指针来读写内存。 

调用GlobalUnlock函数来解锁先前被锁定的内存，该函数使得指向内存块的指针无效。 

调用GlobalFree函数来释放内存块。您必须传给该函数一个内存句柄。

**GlobalAlloc **说明 

分配一个全局内存块 

返回值 

Long，返回全局内存句柄。零表示失败。会设置GetLastError 

参数表 

参数 类型及说明 

wFlags Long，对分配的内存类型进行定义的常数标志，如下所示： 

             GMEM_FIXED 分配一个固定内存块 

             GMEM_MOVEABLE 分配一个可移动内存块 

             GMEM_DISCARDABLE 分配一个可丢弃内存块 

             GMEM_NOCOMPACT 堆在这个函数调用期间不进行累积 

             GMEM_NODISCARD 函数调用期间不丢弃任何内存块 

             GMEM_ZEROINIT 新分配的内存块全部初始化成零 

dwBytes Long，要分配的字符数 

注解 

如指定了 GMEM_FIXED，那么返回值就是要使用的实际内存地址即指针（GlobalLock 会返回同样的值）——所以在使用固定内存块的时候不需要执行一个 GlobalLock/GlobalUnlock 操作

由于 Win32 采用了高级的内存管理方案，所以使用可移动的内存块并没有什么好处

用这个函数分配的内存块允许在8位边界以内

【附】关于GlobalAlloc的问题

--------------------------------------------------------------------------------

问：在使用 GlobalAlloc 分配一个全局内存块时，使用GMEM_FIXED分配一个固定内存块与使用GMEM_MOVEABLE分 配一个可移动内存块到底有什么不同？(请 具 体 点)

其效率上是否也存在差异？

为什么在有些源码中，再使用GMEM_MOVEABLE标志分配内存时，将使用GlobalFree对其返回的内存句柄进行释放操作的语句注释掉，或者干脆就不写？难道是不需要这么做吗？

--------------------------------------------------------------------------------

答：GMEM_MOVEABLE是允许操作系统（或者应用程序）实施对内存堆的管理，在必要时，操作系统可以移动内存块获取更大的块，或者合并一些空闲的内存块，也称“垃圾回收”，它可以提高内存的利用率。一般情况下，内存堆空间是由用户来管理的，windows操作系统不干预。如果存在下列情况，即堆中有10个1K的空闲块，这时如果直接申请一个5K的内存空间，会得到不成功的信息。但如果其它已经被占用的内存块是movable，这时系统就可以移动这些内存块，合并出一个5k的内存块，并成功分配给用户使用。它的空间效率是以运行时的时间效率为代价的。

**GlobalLock**

函数功能描述:锁定一个全局的内存对象，返回指向该对象的第一个字节的指针

函数原型：

LPVOID GlobalLock( HGLOBAL hMem )

参数：

hMem：全局内存对象的句柄。这个句柄是通过GlobalAlloc或GlobalReAlloc来得到的

返回值：

调用成功，返回指向该对象的第一个字节的指针

调用失败，返回NULL，可以用GetLastError来获得出错信息

注意：

调用过GlobalLock锁定一块内存区后，一定要调用GlobalUnlock来解锁。

**GlobalUnlock**函数功能描述:解除被锁定的全局内存对象

函数原型：BOOL GlobalUnlock( HGLOBAL hMem );

参数：hMem：全局内存对象的句柄

返回值：

非零值，指定的内存对象仍处于被锁定状态

0，函数执行出错，可以用GetLastError来获得出错信息，如果返回NO_ERROR，则表示内存对象已经解锁了

注意：    这个函数实际上是将内存对象的锁定计数器减一，如果计数器不为0，则表示执行过多个GlobalLock函数来对这个内存对象加锁，需要对应数目的GlobalUnlock函数来解锁。

    如果通过GetLastError函数返回错误码为ERROR_NOT_LOCKED，则表示未加锁或已经解锁。

示例：

// Malloc memory

hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nSize);

// Lock memory

pMem = (BYTE *) GlobalLock(hMem);

..................

// Unlock memory

GlobalUnlock(hMem);

