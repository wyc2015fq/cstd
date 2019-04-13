
# uC-OS-II内核架构解析---uC-OS-II内存管理 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:10:37[seven-soft](https://me.csdn.net/softn)阅读数：485



## 1. memPart基本原理
uC/OS-II根据需要将内存空间分成多个内存分区，每个内存分区由具有相同大小的内存块（Block）组成。主要包含在C源文件OS_MEM.C中。
OS_EXT OS_MEM OSMemTbl[OS_MAX_MEM_PART];
OS_EXT OS_MEM *OSMemFreeList;
memPart控制块结构体：
typedef struct os_men{
void *OSMemAddr;                 //首地址
void *OSMemFreeList;            //分区中空闲空间的起始地址
INT32U OSMemBlkSize;         //分区中块成员空间大小
INT32U OSMemNBlks;           //块结构数量
INT32U OSMemNFree;           //剩余空闲块个数
INT8U OSMemName[];
}OS_MEM;
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115481719.jpg)
## 2. memPart管理函数
OS_MEM *OSMemCreate(void *addr, INT32U nblks,
INT32U blksize, INT8U *err);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116491645.jpg)
void *OSMemGet(OS_MEM *pmem, INT8U *err); //申请内存分区块
INT8U OSMemPut(OS_MEM *pmem, void *pblk); //释放内存分区块
INT8U OSMemQuery(OS_MEM *pmem, OS_MEM_DATA*);
INT8U OSMemNameGet(OS_MEM *pmem, INT8U *pname, INT8U *err);
INT8U OSMemNameSet(OS_MEM *pmem, INT8U *pname, INT8U *err);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116472069.jpg)
## 3. memPart几个问题
（1）能不能一次申请1B的堆空间？
（2）uC/OS-II在申请栈空间和堆空间时有什么不同？

