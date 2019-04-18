# malloc,new,VirtualAlloc,HeapAlloc性能(速度)比较 - ljx0305的专栏 - CSDN博客
2009年06月30日 17:21:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1392标签：[iostream																[测试																[null																[delete																[windows																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=iostream&t=blog)
个人分类：[C++																[VC																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/380566)
 这里比较的VC++编译的C++代码中的性能
我用的是VC6.0测试的
就不介绍这几个的用法了
我写了一段简单的测试代码
测试结果是：
malloc:390
new:391
VirtualAlloc:454
HeapAlloc:47
很明显的是HeapAlloc分配速度最快，malloc次之，new和malloc差不多，VirtualAlloc最慢了（以前小强跟我说这个最快）
我有跟踪了一下
new调用了这段代码
void * __cdecl _nh_malloc ( 
        size_t nSize, 
        int nhFlag 
        ) 
{ 
        return _nh_malloc_dbg(nSize, nhFlag, _NORMAL_BLOCK, NULL, 0); 
}
malloc函数是这样的：
_CRTIMP void * __cdecl malloc ( 
        size_t nSize 
        ) 
{ 
        return _nh_malloc_dbg(nSize, _newmode, _NORMAL_BLOCK, NULL, 0); 
} 
很明显，new和malloc最终调用相同的_nh_malloc_dbg，只是new多了一次函数调用
再继续跟下去，发现最终调用的是return HeapAlloc(_crtheap, 0, size);
基本上真相大白了
VirtualAlloc跟踪不进去，如果说分配的是虚拟内存的话，有可能会慢吧。
回头再认真看看《Windows核心编程》这本书！
欢迎指正！欢迎交流！
测试代码如下：
/****************************************************************** 
* 
* Copyright (c) 2008,  xxxx 
* All rights reserved. 
* 
* 文件名称：main.cpp 
* 摘   要： 测试申请内存的速度 
* 
* 当前版本：1.0 
* 作    者：吴会然 
* 完成日期：2008-11-30 
* 
* 取代版本： 
* 原  作者： 
* 完成日期： 
* 
******************************************************************/ 
#include <iostream> 
#include <windows.h> 
using namespace std; 
int main( int argc, char *argv[] ) 
{ 
    int i = 0; 
    DWORD dw1 = 0, dw2 = 0, dw3 = 0, dw4 = 0; 
    DWORD dwStart = 0; 
    DWORD dwEnd = 0; 
    for( int j = 0; j < 10; j++ ) 
    { 
        dwStart = ::GetTickCount(); 
        for( i = 0; i < 20000; i++ ) 
        { 
            char *pDest1 = (char *)malloc(4096); 
            free( pDest1 ); 
        } 
        dwEnd = ::GetTickCount(); 
        cout << "malloc 10000次4096大小的内存块，耗时" << dwEnd - dwStart << endl; 
        dw1 += dwEnd - dwStart; 
        dwStart = ::GetTickCount(); 
        for( i = 0; i < 20000; i++ ) 
        { 
            char *pDest2 = new char[4096]; 
            delete pDest2; 
        } 
        dwEnd = ::GetTickCount(); 
        cout << "new 10000次4096大小的内存块，耗时" << dwEnd - dwStart << endl; 
        dw2 += dwEnd - dwStart; 
        dwStart = ::GetTickCount(); 
        for( i = 0; i < 20000; i++ ) 
        { 
            void* pMem = ::VirtualAlloc(NULL, 4096,  MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE ); 
            ::VirtualFree(pMem, 0, MEM_RELEASE); 
        } 
        dwEnd = ::GetTickCount(); 
        cout << "VirtualAlloc 10000次4096大小的内存块，耗时" << dwEnd - dwStart << endl; 
        dw3 += dwEnd - dwStart; 
        HANDLE hHeap = ::HeapCreate(HEAP_NO_SERIALIZE, 0, 0); 
        dwStart = ::GetTickCount(); 
        for( i = 0; i < 20000; i++ ) 
        { 
            void* pMem2 = ::HeapAlloc(hHeap, HEAP_NO_SERIALIZE, 4096 ); 
            ::HeapFree(hHeap, HEAP_NO_SERIALIZE, pMem2); 
        } 
        dwEnd = ::GetTickCount(); 
        cout << "HeapAlloc 10000次4096大小的内存块，耗时" << dwEnd - dwStart << endl; 
        dw4 += dwEnd - dwStart; 
    } 
    cout << "malloc:" << dw1 << endl; 
    cout << "new:" << dw2 << endl;    
    cout << "VirtualAlloc:" << dw3 << endl;    
    cout << "HeapAlloc:" << dw4 << endl;    
    return 0; 
}
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wuhuiran/archive/2008/11/30/3414742.aspx](http://blog.csdn.net/wuhuiran/archive/2008/11/30/3414742.aspx)
