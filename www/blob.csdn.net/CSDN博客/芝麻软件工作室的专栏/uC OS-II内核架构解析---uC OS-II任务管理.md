
# uC-OS-II内核架构解析---uC-OS-II任务管理 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:08:27[seven-soft](https://me.csdn.net/softn)阅读数：285



## 1. C可执行代码结构
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115131239.jpg)
（1）代码段.text：存放CPU执行的机器指令，通常.text是可共享且只读的。
（2）数据段.data：.rodata(常量数据)、.rwdata（已初始化全局变量、静态变量）。
（3）未初始化数据段.bss：未初始化的全局变量、静态变量。
（4）栈.stack：存放函数参数、局部变量及任务切换时的上下文。
（5）堆.heap：用于动态内存分配。
## 2. 任务结构
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115144455.jpg)
在uC/OS-II中，任务是事件运行和管理的基本单元。一个uC/OS-II任务至少包含程序代码、栈和TCB，还可选择性使用相当于堆的动态内存空间。程序运行时，uC/OS-II中的任务相当于可执行代码（可单独运行的单元）。
uC/OS-II任务的各部分如何管理???
## 3. 任务栈
任务栈数据类型：typedef unsigned short OS_STK
任务栈增长方向：\#define OS_STK_GROWTH 0/1
任务栈基本功能：???
## 4. 任务控制块
### (1) TCB描述
主要用来存储任务的当前属性。（问：任务第一次被调度时，该如何运行？）
### (2) TCB主要成员
任务栈空间位置：OSTCBStkPtr、OSTCBStkBottom、OSTCBStkSize；
任务通信与同步：OSTCBEventPtr、OSTCBMsg；
任务事件组标志：OSTCBFlagNode、OSTCBFlagsRdy；
任务等待/阻塞：OSTCBDly、OSTCBPendTO；
任务当前状态：OSTCBStat；
任务优先级：OSTCBPrio；
（3）TCB全局变量
OS_EXT OS_TCB OSTCBTbl[OS_MAX_TASKS + OS_N_SYS_TASKS];
OS_EXT OS_TCB OSTCBPrioTbl[OS_LOWEST_PRIO + 1];
OS_EXT OS_TCB *OSTCBFreeList;
OS_EXT OS_TCB *OSTCBList;
OS_EXT OS_TCB *OSTCBHighRdy;
OS_EXT OS_TCB *OSTCBCur;
## 5. 任务状态切换
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115181675.jpg)
## 6. 任务管理函数
OS_TASK.C：11个函数
OSTaskCreate();
OSTaskCreateExt();
OSTaskDel();
OSTaskDelReq();
OSTaskChangePrio();
OSTaskSuspend();
OSTaskResume();
OSTaskNameGet();
OSTaskNameSet();
OSTaskStkChk();
OS_TaskStkClr();
OSTaskQuery();

