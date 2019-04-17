# 编译LDD3的scull驱动问题总结 - xiahouzuoxin - CSDN博客





2013年05月10日 21:41:36[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：3416







**由于Linux内核版本更新的原因，LDD3（v2.6.10）提供的源码无法直接使用，下面是本人编译scull源码时出现的一些问题及解决方法。编译环境：CentOS 6.3（kernel version 2.6.32）**
**编译错误：**
make -C /lib/modules/2.6.32-279.14.1.el6.i686/build M=/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull LDDINC=/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/../include modules
make[1]: Entering directory `/usr/src/kernels/2.6.32-279.14.1.el6.i686'
scripts/Makefile.build:49: *** CFLAGS was changed in "/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/Makefile". Fix it to use EXTRA_CFLAGS.  Stop.
make[1]: *** [_module_/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull] Error 2
make[1]: Leaving directory `/usr/src/kernels/2.6.32-279.14.1.el6.i686'
make: *** [modules] Error 2
**解决方案：**
CFLAGS与Makefile.build中的CFLAGS冲突，错误提示要求将CFLAG换成EXTRA_CFLAGS重新编译即可；


**编译错误：**
error: linux/config.h: No such file or directory
**解决方案：**
从linux-2.6.20起，config.h就已经被移除了.
将#include <linux/config.h>修改成如下——>
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18))
#include <linux/config.h>
#endif

**编译错误：**
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c: In function ‘scull_p_read’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:131: error: ‘TASK_INTERRUPTIBLE’ undeclared (first use in this function)
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:131: error: (Each undeclared identifier is reported only once
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:131: error: for each function it appears in.)
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:131: error: implicit declaration of function ‘signal_pending’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:131: error: implicit declaration of function ‘schedule’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c: In function ‘scull_getwritespace’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:168: error: ‘TASK_INTERRUPTIBLE’ undeclared (first use in this function)
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c: In function ‘scull_p_write’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:219: error: ‘TASK_INTERRUPTIBLE’ undeclared (first use in this function)
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:223: error: ‘SIGIO’ undeclared (first use in this function)
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.c:223: error: ‘POLL_IN’ undeclared (first use in this function)
make[2]: *** [/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/pipe.o] Error 1
make[1]: *** [_module_/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull] Error 2
**解决方案：**
头文件变动原因，在pipe.c中添加
#include <linux/sched.h> 

**编译错误：**
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c: In function ‘scull_u_open’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:106: error: dereferencing pointer to incomplete type
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:107: error: dereferencing pointer to incomplete type
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:114: error: dereferencing pointer to incomplete type
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c: In function ‘scull_w_available’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:165: error: dereferencing pointer to incomplete type
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:166: error: dereferencing pointer to incomplete type
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c: In function ‘scull_w_open’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:179: error: ‘TASK_INTERRUPTIBLE’ undeclared (first use in this function)
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:179: error: (Each undeclared identifier is reported only once
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:179: error: for each function it appears in.)
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:179: error: implicit declaration of function ‘signal_pending’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:179: error: implicit declaration of function ‘schedule’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:184: error: dereferencing pointer to incomplete type
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c: In function ‘scull_w_release’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:205: error: ‘TASK_INTERRUPTIBLE’ undeclared (first use in this function)
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c: In function ‘scull_c_open’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:277: error: dereferencing pointer to incomplete type
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:281: error: dereferencing pointer to incomplete type
make[2]: *** [/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.o] Error 1
make[1]: *** [_module_/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull] Error 2
make[1]: Leaving directory `/usr/src/kernels/2.6.32-279.14.1.el6.i686'
make: *** [modules] Error 2
**解决方案：**
头文件变动原因，在access.c中添加
#include <linux/sched.h>   


**编译错误：**
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c: In function ‘scull_u_open’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:107: error: ‘struct task_struct’ has no member named ‘uid’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:108: error: ‘struct task_struct’ has no member named ‘euid’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:115: error: ‘struct task_struct’ has no member named ‘uid’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c: In function ‘scull_w_available’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:166: error: ‘struct task_struct’ has no member named ‘uid’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:167: error: ‘struct task_struct’ has no member named ‘euid’
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c: In function ‘scull_w_open’:
/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.c:185: error: ‘struct task_struct’ has no member named ‘uid’
make[2]: *** [/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull/access.o] Error 1
make[1]: *** [_module_/mnt/HappyStudy/MyDesigner/Linux/LDD3/examples/scull] Error 2
make[1]: Leaving directory `/usr/src/kernels/2.6.32-279.14.1.el6.i686'
make: *** [modules] Error 2
**解决方案：**
因为内核版本的升级，struct task_struct发生了变动，uid和euid放到了cred域下.
修改，做如下替换
current->uid 替换成current->cred->uid
current->euid替换成current->cred->euid

重新编译，成功！


