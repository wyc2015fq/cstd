# WinCE OAL中的RAM定制函数 - xqhrs232的专栏 - CSDN博客
2013年08月17日 23:20:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：633
原文地址::[http://blog.csdn.net/nanjianhui/article/details/4067295](http://blog.csdn.net/nanjianhui/article/details/4067295)
相关网帖
1、[WinCE OAL中的Memory函数介绍](http://blog.csdn.net/nanjianhui/article/details/4038734)----[http://blog.csdn.net/nanjianhui/article/details/4038734](http://blog.csdn.net/nanjianhui/article/details/4038734)
2、[OEMAddressTable介绍](http://blog.csdn.net/nanjianhui/article/details/2027539)----[http://blog.csdn.net/nanjianhui/article/details/2027539](http://blog.csdn.net/nanjianhui/article/details/2027539)
作者：ARM-WinCE
在WinCE系统中，Kernel是如何使用内存的，如何知道内存的大小？是通过config.bib文件的描述，我们会在config.bib文件中MEMORY段定义WinCE内核所占用RAM的起始地址和大小，比如：
**MEMORY     NK      80001000  01E00000  RAMIMAGE**
BIB文件的格式以前介绍过，这里不重复了，在config.bib中定义的是一个物理上连续内存，在OAL中我们可以改变这段被kernel使用的连续物理内存的大小，同时也可以通过函数来枚举其他的可用的物理内存，通常这些内存在物理地址上是不连续的。
**1. MainMemoryEndAddress:**用于改变config.bib中定义的内核所使用的连续物理内存的大小。在启动过程中，内核设置该值为ulRAMEnd，其中ulRAMEnd的值等于config.bib中定义的RAM起始地址+RAM的size，WinCE内核通过MainMemoryEndAddress来得知可用的连续物理内存的大小。该变量可以在OEMInit函数中重新被设置来调整内存的大小。
**2. BOOL OEMGetExtensionDRAM(LPDWORD lpMemStart, LPDWORD lpMemLen)**该函数用于向WinCE内核枚举一块不连续的物理内存。当有另一块物理内存存在，可以通过该函数来枚举这块内存的起始地址和大小，这样WinCE内核就可以使用这块内存了。
**lpMemStart**：内存的起始地址
**lpMemLen**：内存的大小
如果没有额外的内存，该函数直接返回FALSE就可以了。
**3. pNKEnumExtensionDRAM**
如果系统中有多余1个以上的不连续物理内存，就需要实现OEMEnumExtensionDRAM函数，并将它赋给pNKEnumExtensionDRAM函数指针。一般在OEMInit中实现，如下：
**pNKEnumExtensionDRAM = OEMEnumExtensionDRAM;**
然后需要实现OEMEnumExtensionDRAM函数，这个函数比较简单，举个例子就明白了，如下：
DWORD OEMEnumExtensionDRAM(PMEMORY_SECTION pMemSections, DWORD cMemSections)
{
　DWORD dwExtMem=0;
　pMemSections[0].dwFlags=0;
　pMemSections[0].dwStart=(SDRAM_VIRTUAL_MEMORY + SDRAM_MAIN_BLOCK_SIZE);
　pMemSections[0].dwLen=SDRAM_MAIN_BLOCK_SIZE;
　pMemSections[1].dwFlags=0;
　pMemSections[1].dwStart=(SDRAM_VIRTUAL_MEMORY + 2*SDRAM_MAIN_BLOCK_SIZE);
　pMemSections[1].dwLen=SDRAM_MAIN_BLOCK_SIZE;
　pMemSections[2].dwFlags=0;
　pMemSections[2].dwStart=(SDRAM_VIRTUAL_MEMORY + 3*SDRAM_MAIN_BLOCK_SIZE);
　pMemSections[2].dwLen=SDRAM_MAIN_BLOCK_SIZE;
　dwExtMem=3;
　return dwExtMem;
}
其中pMemSections是一个内存的结构，如下：
**typedef struct _MEMORY_SECTION {  DWORD dwFlags; **//预留，必须设置为0
**  DWORD dwStart;** //内存的起始地址
**  DWORD dwLen;**   //内存的大小
**} MEMORY_SECTION, *PMEMORY_SECTION;**
在上面的OEMEnumExtensionDRAM函数中，枚举了3块连续的物理内存，并且返回枚举的内存的个数。
在WinCE引导过程中，会首先检查pNKEnumExtensionDRAM是否为NULL，如果不为NULL则调用它所指向的函数并且不再调用OEMGetExtensionDRAM，如果pNKEnumExtensionDRAM为NULL，则调用OEMGetExtensionDRAM函数。
