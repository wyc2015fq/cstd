
# uCOS内存管理 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:57:42[seven-soft](https://me.csdn.net/softn)阅读数：268


ANSI C中，可以使用malloc()和free()两个函数来动态分配内存，在嵌入式系统中，一般不这么做。
uCOS中的内存管理是基于固定大小的存储块的分配和回收，相同大小的存储块链接在一起，组成若干组，比如8个字节的存储块为一组，16个字节的是另一组，当我们需要8个字节时，就从8个字节的组里Get拿出一个，用完后再Put回去，因为每次操作的都是固定大小的存储块，所以没有碎片的产生。
基本的使用方法
1.创建
INT8U Mem[BlockNum][BlockSize];//要创建内存分区的大小
定义了一个内存分区，他包含BlockNum个内存块，每个内存块的长度为BlockSize.用来存储INT8U类型的数据
上面的定义只是在内存中划分出了分区即内存块区域，并没有动态分配内存区。
OS_MEM * Mem_Ptr = OSMemCreate(Mem, BlockNum, BlockSize, &err);/*创建内存分区*/
2.从分区获取内存块
new_block_p = (block_t *)OSMemGet(CommRAMA_Ptr, &err);
获得内存块后，可以转换成你所要的存储类型
3.将内存块放回分区
OSMemPut(Mem_Ptr,
 new_block_p);
4.查询分区的状态
INT8U OSMemQuery(OS_MEM *pmem, OS_MEM_DATA *pdata)
该函数返回OS_MEM结构包含的信息，但使用了一个新的OS_MEM_DATA的数据结构，OS_MEM_DATA数据结构还包含了正被使用的内存块数目的域

