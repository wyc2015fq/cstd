# 编写Linux中断驱动时，由于头文件缺失引起的编译错误 - TaigaComplex求职中 - 博客园







# [编写Linux中断驱动时，由于头文件缺失引起的编译错误](https://www.cnblogs.com/TaigaCon/archive/2012/11/03/2752614.html)





内核版本：2.6.32.2

错误：

　　error:implicit declaration of function 'request_irq'

　　error:implicit declaration of function 'free_irq'

解决方法：

　　#include <linux/interrupt.h>



错误：

　　error: 'IRQT_BOTHEDGE' undeclared (first use in this function)

解决方法：

　　#define __IRQT_FALEDGE IRQ_TYPE_EDGE_FALLING
　　#define __IRQT_RISEDGE IRQ_TYPE_EDGE_RISING
　　#define __IRQT_LOWLVL IRQ_TYPE_LEVEL_LOW
　　#define __IRQT_HIGHLVL IRQ_TYPE_LEVEL_HIGH
　　#define IRQT_NOEDGE (0)
　　#define IRQT_RISING (__IRQT_RISEDGE)
　　#define IRQT_FALLING (__IRQT_FALEDGE)
　　#define IRQT_BOTHEDGE (__IRQT_RISEDGE|__IRQT_FALEDGE)
　　#define IRQT_LOW (__IRQT_LOWLVL)
　　#define IRQT_HIGH (__IRQT_HIGHLVL)
　　#define IRQT_PROBE IRQ_TYPE_PROBE



错误：

 　　error: 'TASK_INTERRUPTIBLE' undeclared (first use in this function)
　　error: implicit declaration of function 'signal_pending'
　　error: implicit declaration of function 'schedule'

解决方法：

　　#include <linux/sched.h>



错误：

　　error:implicit declaration of function 's3c2410_gpio_getpin'

解决方法：

　　#include <linux/gpio.h>



另外还有其他的一些问题解决方法可以参考：

asm/arch/regs-gpio.h: No such file or directory：

[http://www.armbbs.net/forum.php?mod=viewthread&tid=11269](http://www.armbbs.net/forum.php?mod=viewthread&tid=11269)

class_create报错：

[http://hi.baidu.com/pleasehyj/item/cf1ad5334c3ac0b8623affcd](http://hi.baidu.com/pleasehyj/item/cf1ad5334c3ac0b8623affcd)












