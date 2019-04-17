# #include "*.c"文件的妙用 - xiahouzuoxin - CSDN博客





2013年09月04日 20:58:30[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：5829








在看uCOS II V2.91版本源代码时，在ucos_ii.c源文件中发现下面的代码：



```cpp
#include <os_core.c>
#include <os_flag.c>
#include <os_mbox.c>
#include <os_mem.c>
#include <os_mutex.c>
#include <os_q.c>
#include <os_sem.c>
#include <os_task.c>
#include <os_time.c>
#include <os_tmr.c>
```






在之前，我将ucos_ii.c与上面的os_core.c等都添加到工程编译，出现许多“重复定义”的错误。

之后，将os_core.c等文件都从工程中移除，只保留ucos_ii.c，则编译通过！




**我们知道，通过#include "*.h"可以引用其它源文件中导出的符号，**

**而通过使用#include "*.c"文件可以控制要将哪些源文件编译到工程中。**




比如，这里如果不需要消息邮箱功能和互斥信号功能，则将对应的.c文件注释即可，



```cpp
#include <os_core.c>
#include <os_flag.c>
//#include <os_mbox.c>
#include <os_mem.c>
//#include <os_mutex.c>
#include <os_q.c>
#include <os_sem.c>
#include <os_task.c>
#include <os_time.c>
#include <os_tmr.c>
```


因此，在大型的项目中，如果我们要测试不同的功能，可以为每个功能建立一个.c类型的配置文件，在该配置文件中将需要编译的功能.c文件#include进来就OK，而不用麻烦地在每次测试不同功能时都重复的把旧的功能文件删除，再添加新的功能文件。




**通过控制要编译的文件，可以减少最终生成代码的代码段长度，节省存储空间。**

许多程序设计人员没有丝毫的节省存储空间的概念，经常把与工程无关的文件包含到工程中。











