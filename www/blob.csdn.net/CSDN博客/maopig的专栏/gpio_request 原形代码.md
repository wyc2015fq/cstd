# gpio_request 原形代码 - maopig的专栏 - CSDN博客
2012年04月05日 14:52:52[maopig](https://me.csdn.net/maopig)阅读数：22781
### [](http://blog.csdn.net/heanyu/article/details/6709571)  其原型为**int gpio_request(unsigned gpio, const char *label) **先说说其参数，gpio则为你要申请的哪一个管脚，label则是为其取一个名字。其具体实现如下：
**[cpp]**[view plain](http://blog.csdn.net/heanyu/article/details/6709571#)[copy](http://blog.csdn.net/heanyu/article/details/6709571#)[print](http://blog.csdn.net/heanyu/article/details/6709571#)[?](http://blog.csdn.net/heanyu/article/details/6709571#)
- int gpio_request(unsigned gpio, constchar *label)   
- {   
- struct gpio_desc *desc;//这个自己看源码 
- struct gpio_chip *chip;//这个自己看源码 
- int   status = -EINVAL;   
-         unsigned long  flags;  
-         spin_lock_irqsave(&gpio_lock, flags);//屏蔽中断
- if (!gpio_is_valid(gpio))//判断是否有效，也就是参数的取值范围判断 
- goto done;   
-         desc = &gpio_desc[gpio];   
- //这个是关键gpio_desc为定义的一个全局的数组变量，这个函数的实值也就是，
- //用gpio_desc里面的一个变量来表示数组中的这个元素已经被申请了，而这个变量就是下面会看到的desc->flags。 
- chip = desc->chip;//按理说这个这个全局的gpio_desc如果没有初始化的话，这个chip就为空了，随后就直接返回-EINVAL了。 
- if (chip == NULL)如果不为空继续往下走   
- goto done;  
- if (!try_module_get(chip->owner))   
- goto done;  
- /* NOTE:  gpio_request() can be called in early boot, 
-   * before IRQs are enabled, for non-sleeping (SOC) GPIOs. 
-   */
- if (test_and_set_bit(FLAG_REQUESTED, &desc->flags) == 0) {  
- //这里测试并设置flags的第FLAG_REQUESTED位，如果没有被申请就返回该位的原值0,分析到这儿，也差不多满足了我的个人要求。 
-   desc_set_label(desc, label ? : "?");   
-   status = 0;   
- } else {   
-   status = -EBUSY;   
-   module_put(chip->owner);   
- goto done;   
- }  
- if (chip->request) {   
- /* chip->request may sleep */
-   spin_unlock_irqrestore(&gpio_lock, flags);   
-   status = chip->request(chip, gpio - chip->base);   
-   spin_lock_irqsave(&gpio_lock, flags);  
- if (status < 0) {   
-    desc_set_label(desc, NULL);   
-    module_put(chip->owner);   
-    clear_bit(FLAG_REQUESTED, &desc->flags);   
-   }   
- }  
- done:   
- if (status)   
-   pr_debug("gpio_request: gpio-%d (%s) status %d\n",   
-    gpio, label ? : "?", status);   
- spin_unlock_irqrestore(&gpio_lock, flags);   
- return status;   
- }  
int gpio_request(unsigned gpio, const char *label) 
{ 
        struct gpio_desc *desc;//这个自己看源码 
        struct gpio_chip *chip;//这个自己看源码 
        int   status = -EINVAL; 
        unsigned long  flags;
        spin_lock_irqsave(&gpio_lock, flags);//屏蔽中断
        if (!gpio_is_valid(gpio))//判断是否有效，也就是参数的取值范围判断 
             goto done; 
        desc = &gpio_desc[gpio]; 
//这个是关键gpio_desc为定义的一个全局的数组变量，这个函数的实值也就是，
//用gpio_desc里面的一个变量来表示数组中的这个元素已经被申请了，而这个变量就是下面会看到的desc->flags。 
chip = desc->chip;//按理说这个这个全局的gpio_desc如果没有初始化的话，这个chip就为空了，随后就直接返回-EINVAL了。 
if (chip == NULL)如果不为空继续往下走 
  goto done;
if (!try_module_get(chip->owner)) 
  goto done;
/* NOTE:  gpio_request() can be called in early boot, 
  * before IRQs are enabled, for non-sleeping (SOC) GPIOs. 
  */
if (test_and_set_bit(FLAG_REQUESTED, &desc->flags) == 0) {
//这里测试并设置flags的第FLAG_REQUESTED位，如果没有被申请就返回该位的原值0,分析到这儿，也差不多满足了我的个人要求。 
  desc_set_label(desc, label ? : "?"); 
  status = 0; 
} else { 
  status = -EBUSY; 
  module_put(chip->owner); 
  goto done; 
}
if (chip->request) { 
  /* chip->request may sleep */ 
  spin_unlock_irqrestore(&gpio_lock, flags); 
  status = chip->request(chip, gpio - chip->base); 
  spin_lock_irqsave(&gpio_lock, flags);
  if (status < 0) { 
   desc_set_label(desc, NULL); 
   module_put(chip->owner); 
   clear_bit(FLAG_REQUESTED, &desc->flags); 
  } 
}
done: 
if (status) 
  pr_debug("gpio_request: gpio-%d (%s) status %d\n", 
   gpio, label ? : "?", status); 
spin_unlock_irqrestore(&gpio_lock, flags); 
return status; 
}
davinci 平台： 
**[cpp]**[view plain](http://blog.csdn.net/heanyu/article/details/6709571#)[copy](http://blog.csdn.net/heanyu/article/details/6709571#)[print](http://blog.csdn.net/heanyu/article/details/6709571#)[?](http://blog.csdn.net/heanyu/article/details/6709571#)
- /* 
- * TI DaVinci GPIO Support 
- * 
- * Copyright (c) 2006 David Brownell 
- * Copyright (c) 2007, MontaVista Software, Inc. <source@mvista.com> 
- * 
- * This program is free software; you can redistribute it and/or modify 
- * it under the terms of the GNU General Public License as published by 
- * the Free Software Foundation; either version 2 of the License, or 
- * (at your option) any later version. 
- */
- 
- #include <linux/errno.h> 
- #include <linux/kernel.h>  
- #include <linux/list.h>  
- #include <linux/module.h>  
- #include <linux/err.h>  
- #include <linux/bitops.h>  
- 
- #include <asm/irq.h>  
- #include <asm/io.h>  
- #include <asm/hardware/clock.h>  
- 
- #include <asm/arch/irqs.h>  
- #include <asm/arch/hardware.h> 
- #include <asm/arch/gpio.h>  
- #include <asm/arch/cpu.h> 
- 
- #include <asm/mach/irq.h> 
- 
- /*  
- 该文件实现了gpio的各种应用功能和向内核注册gpio的中断例程等功能。 
- 用户的驱动程序可调用gpio_request和gpio_free使用或释放该gpio， 
- 可以调用gpio_direction_input和gpio_direction_output函数设置gpio输入输出方向， 
- 调用gpio_get_value和gpio_set_value获取设置值。 
- */
- 
- static DEFINE_SPINLOCK(gpio_lock);   
- 
- /* 总共有DAVINCI_N_GPIO（71）个gpio引脚，故使用相应多的bit来记录这些引脚的使用状态 */
- static DECLARE_BITMAP(gpio_in_use, DAVINCI_N_GPIO);   
- 
- /* 
- 申请一个gpio，其实就是检查该gpio是否空闲，如果空闲就可以使用并将该gpio相应的bit置位 
- （在gpio_in_use中）。 
- */
- int gpio_request(unsigned gpio, constchar *tag)   
- {   
- if (gpio >= DAVINCI_N_GPIO)   
- return -EINVAL;   
- if (test_and_set_bit(gpio, gpio_in_use))   
- return -EBUSY;   
- return 0;   
- }   
- EXPORT_SYMBOL(gpio_request);   
- 
- /* 
- 释放一个gpio，其实就是清除gpio相应的控制bit位（在gpio_in_use中）。 
- */
- void gpio_free(unsigned gpio)   
- {   
- if (gpio >= DAVINCI_N_GPIO)   
- return;   
-     clear_bit(gpio, gpio_in_use);   
- }   
- EXPORT_SYMBOL(gpio_free);   
- 
- /* 获得gpio_controller结构体指针，gpio_controller结构体是gpio的核心控制单元，里面包含 
- gpio的设置和数据寄存器。该结构体和__gpio_to_controller函数在/include/asm-arm/ 
- arch-davinci/gpio.h中定义，具体如下： 
- struct gpio_controller { 
-     u32    dir; 
-     u32    out_data; 
-     u32    set_data; 
-     u32    clr_data; 
-     u32    in_data; 
-     u32    set_rising; 
-     u32    clr_rising; 
-     u32    set_falling; 
-     u32    clr_falling; 
-     u32    intstat; 
- }; 
- 
- static inline struct gpio_controller *__iomem 
- __gpio_to_controller(unsigned gpio) 
- { 
-     void *__iomem ptr; 
-     if (gpio >= DAVINCI_N_GPIO) 
-         return NULL; 
- 
-     if (gpio < 32) 
-         ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x10); 
-     else if (gpio < 64) 
-         ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x38); 
-     else if (gpio < 96) 
-         ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x60); 
-     else 
-         ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x88); 
-     return ptr; 
- } 
- 由上面的定义和ti的SPRUE25.pdf手册可以看出，__gpio_to_controller函数返回的是 
- gpio_controller结构体到第一个成员dir的虚拟地址。获取了这个结构体指针后， 
- 便可以控制相应的gpio了。dm644x共有71个gpio， 
- 所以使用三个gpio_controller结构体控制，关于这个后面会由更详细的分析， 
- */
- /* create a non-inlined version */
- staticstruct gpio_controller *__iomem gpio2controller(unsigned gpio)   
- {   
- return __gpio_to_controller(gpio);   
- }   
- 
- /*  
- 向某个gpio设置值，0或1。如果向gpio写1，则向set_data寄存器相应的位置1，如果写0， 
- 则向clr_data寄存器相应的位置1.__gpio_mask函数在gpio.h中定义，定义如下， 
- static inline u32 __gpio_mask(unsigned gpio) 
- { 
-     return 1 << (gpio % 32); 
- } 
- 因为71个引脚由3个结构体控制，第一个控制前32个gpio，第二个控制次32个gpio， 
- 最后一个控制剩余的7个gpio，故__gpio_mask函数的作用是找到在其相应控制结构体里的偏移数， 
- 比如gpio34,那么其由第二个结构体控制，在这个机构体里的偏移是3（从0开始算，就是第二位）。 
- 使用这个函数之前，必须确认该gpio设置成输出模式。 
- */
- /* 
- * Assuming the pin is muxed as a gpio output, set its output value. 
- */
- void __gpio_set(unsigned gpio, int value)   
- {   
- struct gpio_controller *__iomem g = gpio2controller(gpio);   
- // 设置gpio的值  
-     __raw_writel(__gpio_mask(gpio), value ? &g->set_data : &g->clr_data);    
- }   
- EXPORT_SYMBOL(__gpio_set);   
- 
- /*  
- 通过读取in_data寄存器相应该gpio的位来读取gpio的值。 
- 使用这个函数之前，必须确认该gpio设置成输入模式，否则获得到值不可预料。  
- */
- /* 
- * Read the pin's value (works even if it's set up as output); 
- * returns zero/nonzero. 
- * 
- * Note that changes are synched to the GPIO clock, so reading values back 
- * right after you've set them may give old values. 
- */
- int __gpio_get(unsigned gpio)   
- {   
- struct gpio_controller *__iomem g = gpio2controller(gpio);   
- /* 读取gpio的值，!!的目的是使得返回的值为0或1.*/
- return !!(__gpio_mask(gpio) & __raw_readl(&g->in_data));       
- }                                                                                                                  }   
- EXPORT_SYMBOL(__gpio_get);   
- 
- /*  
- 通过dir寄存器相应该gpio的位来设置gpio输入输出方向，为0，则设置成输出，为1，则设置出输入。 
- 该函数是设置成输入，故设置dir寄存器为1. 
-  正如应为所说的，必须确认该引脚是作为gpio功能，而不是某个模块到功能，比如spi。通过PINMUX0 
- 和PINMUX1两个寄存器来设置。 
- */
- /*--------------------------------------------------------------------------*/
- 
- /* 
- * board setup code *MUST* set PINMUX0 and PINMUX1 as 
- * needed, and enable the GPIO clock. 
- */
- int gpio_direction_input(unsigned gpio)   
- {   
- struct gpio_controller *__iomem g = gpio2controller(gpio);   
-     u32 temp;   
-     u32 mask;   
- 
- if (!g)   
- return -EINVAL;   
-     spin_lock(&gpio_lock);   
-     mask = __gpio_mask(gpio);   
-     temp = __raw_readl(&g->dir);   
-     temp |= mask;    // 设置成1  
-     __raw_writel(temp, &g->dir);    // 设置该gpio为输入 
-     spin_unlock(&gpio_lock);   
- return 0;   
- }   
- EXPORT_SYMBOL(gpio_direction_input);   
- 
- /* 
- 通过dir寄存器相应该gpio的位来设置gpio输入输出方向，为0，则设置成输出，为1，则设置出输入。 
- 该函数是设置成输出，故设置dir寄存器为0. 
- value参数用于选择gpio设置成输出后该gpio输出的值。 
- */
- int gpio_direction_output(unsigned gpio, int value)   
- {   
- struct gpio_controller *__iomem g = gpio2controller(gpio);   
-     u32 temp;   
-     u32 mask;   
- if (!g)   
- return -EINVAL;   
- 
-     spin_lock(&gpio_lock);   
-     mask = __gpio_mask(gpio);   
-     temp = __raw_readl(&g->dir);   
-     temp &= ~mask;    // 设置成0   
- //设置该gpio输出值  
-     __raw_writel(mask, value ? &g->set_data : &g->clr_data);   
-     __raw_writel(temp, &g->dir);    // 设置gpio为输出 
-     spin_unlock(&gpio_lock);   
- return 0;   
- }   
- EXPORT_SYMBOL(gpio_direction_output);   
- 
- /* 
- 向gpio设置值，0或1。 
- */
- void gpio_set_value(unsigned gpio, int value)   
- {   
- if (__builtin_constant_p(value)) {   
- struct gpio_controller *__iomem g;   
-         u32 mask;   
- 
- if (gpio >= DAVINCI_N_GPIO)   
-             __error_inval_gpio();   
- 
-         g = __gpio_to_controller(gpio);   
-         mask = __gpio_mask(gpio);   
- if (value)   
-             __raw_writel(mask, &g->set_data);    // 该gpio输出高 
- 
- else
-             __raw_writel(mask, &g->clr_data);    // 该gpio输出低 
- 
- return;   
-     }   
- 
-     __gpio_set(gpio, value);   
- }   
- EXPORT_SYMBOL(gpio_set_value);   
- 
- /* 
- 读取gpio的值，0或1. 
- */
- int gpio_get_value(unsigned gpio)   
- {   
- struct gpio_controller *__iomem g;   
- 
- if (!__builtin_constant_p(gpio))/* 判断该gpio值是否为编译时常数，如果是常数， 
-                                      函数返回 1，否则返回 0 */
- return __gpio_get(gpio);   
- 
- if (gpio >= DAVINCI_N_GPIO)   
- return __error_inval_gpio();   
- 
-     g = __gpio_to_controller(gpio);   
- 
- // 读取该gpio的值  
- 
- return !!(__gpio_mask(gpio) & __raw_readl(&g->in_data));   
- }   
- EXPORT_SYMBOL(gpio_get_value);   
- 
- /* 
- * We expect irqs will normally be set up as input pins, but they can also be 
- * used as output pins ... which is convenient for testing. 
- * 
- * NOTE: GPIO0..GPIO7 also have direct INTC hookups, which work in addition 
- * to their GPIOBNK0 irq (but with a bit less overhead). But we don't have 
- * a good way to hook those up ... 
- * 
- * All those INTC hookups (GPIO0..GPIO7 plus five IRQ banks) can also 
- * serve as EDMA event triggers. 
- */
- 
- /* 
- 禁止相应该irq的gpio的中断。每个gpio都可以作为中断的来源，其中gpio0-gpio7是独立的中断来源， 
- 也就是分配独立的中断号，其他gpio则共用5个GPIOBNK中断线。其优先级可以在board-evm.c 
- 中设置（已经介绍过）。在dm644x平台上，中断是电平边缘触发的，禁止中断其实就是既不设置 
- 上升沿触发，也不设置下降沿触发。 
- */
- staticvoid gpio_irq_disable(unsigned irq)   
- {   
- struct gpio_controller *__iomem g = get_irq_chipdata(irq);   
-     u32 mask = __gpio_mask(irq_to_gpio(irq));   
- 
-     __raw_writel(mask, &g->clr_falling);    // 清除下降沿触发 
- 
-     __raw_writel(mask, &g->clr_rising);        // 清除上升沿触发 
- 
- }   
- 
- /* 
- 中断使能。 
- 在dm644x平台上，中断是电平边缘触发的，其实就是设置为上升沿或下降沿中断。 
- */
- staticvoid gpio_irq_enable(unsigned irq)   
- {   
- struct gpio_controller *__iomem g = get_irq_chipdata(irq);   
-     u32 mask = __gpio_mask(irq_to_gpio(irq));   
- 
- // 如果先前为下降沿中断，则使能为下降沿中断  
- 
- if (irq_desc[irq].status & IRQT_FALLING)   
-         __raw_writel(mask, &g->set_falling);   
- 
- // 如果先前为上升沿中断，则使能为上升沿中断  
- 
- if (irq_desc[irq].status & IRQT_RISING)       
-         __raw_writel(mask, &g->set_rising);   
- }   
- 
- /* 
- 设置中断类型。 
- 在dm644x平台上，中断有上升沿和下降沿两种触发方式。 
- */
- staticint gpio_irq_type(unsigned irq, unsigned trigger)   
- {   
- struct gpio_controller *__iomem g = get_irq_chipdata(irq);   
-     u32 mask = __gpio_mask(irq_to_gpio(irq));   
- 
- if (trigger & ~(IRQT_FALLING | IRQT_RISING))   
- return -EINVAL;   
- 
-     irq_desc[irq].status &= ~IRQT_BOTHEDGE;   
-     irq_desc[irq].status |= trigger;   
- 
-     __raw_writel(mask, (trigger & IRQT_FALLING)   
-          ? &g->set_falling : &g->clr_falling);     // 设置为下降沿触发 
- 
-     __raw_writel(mask, (trigger & IRQT_RISING)   
-          ? &g->set_rising : &g->clr_rising);    // 设置为上升沿触发 
- 
- return 0;   
- }   
- 
- /*  
- 该结构体用于注册到所有irq的中断描述结构体中（struct irqdesc）， 
- 而所有中断描述结构体定义成一个全局数组irq_desc 。 
- */
- staticstruct irqchip gpio_irqchip = {   
-     .unmask        = gpio_irq_enable, /* 用于使能中断， 
-                                      在enable_irq()等内核函数中会用到。*/
-     .mask        = gpio_irq_disable,/* 用于禁止中断， 
-                                      在disable_irq()等内核函数中会用到。*/
-     .type        = gpio_irq_type, /* 用于设置中断类型， 
-                                      在set_irq_type()内核函数中会用到。*/
- };   
- 
- /* 
- 该函数将在下面的davinci_gpio_irq_setup中使用，将被注册到五个gpio bank中断的 
- irq_desc结构中，目的是处理所有级联的gpio中断。所谓级联的中断, 就是指有n个中断 
- 共用同一个中断线。 
- 在dm644x平台中，除了gpio0-gpio7外，其他63个gpio都共用五个gpiobank中断线，在这里， 
- gpio0-gpio7也被注册到gpiobank中断线，但实际上并不会使用，因为它们拥有自己的 
- 中断线。其中，gpio0-gpio15共用IRQ_GPIOBNK0（56）中断线，gpio16-gpio31共用 
- IRQ_GPIOBNK1（57）中断线，gpio32-gpio47共用IRQ_GPIOBNK2（58）中断线， 
- gpio48-gpio63共用IRQ_GPIOBNK4（59）中断线，gpio64-gpio70共用 
- IRQ_GPIOBNK5（60）中断线， 
- 因为寄存器是32位的，所以实际上只有三组寄存器，第一组包含bank0和bank1， 
- 也就是gpio0-gpio31，第二组包含bank2和bank3，也就是gpio32-gpio63， 
- 第三组包含bank4和bank5，也就是gpio64-gpio70，剩余了25个位没有使用。 
- */
- staticvoid
- gpio_irq_handler(unsigned irq, struct irqdesc *desc, struct pt_regs *regs)   
- {   
- struct gpio_controller *__iomem g = get_irq_chipdata(irq);   
-     u32 mask = 0xffff;   
- 
- /* we only care about one bank */
- // 如果bank中断线是寄数，则说明该中断的中断状态位在INTSTATn寄存器的高16位 
- 
- if (irq & 1)   
-         mask <<= 16;   
- 
- /* temporarily mask (level sensitive) parent IRQ */
-     desc->chip->ack(irq);// 该ack函数会在arch/arm/mach-davinci/irq.c中注册。 
- 
- while (1) {   
-         u32        status;   
- struct irqdesc    *gpio;   
- int        n;   
- int        res;   
- 
- /* ack any irqs */
- /*gpio中断发生后，硬件会在INTSTATn寄存器中置位相应位， 
-          以备程序查询，确定是哪个gpio*/
-         status = __raw_readl(&g->intstat) & mask;    
- if (!status)   
- break;   
-         __raw_writel(status, &g->intstat);    // 向该位写1清除 
- 
- if (irq & 1)   
-             status >>= 16;   
- 
- /* now demux them to the right lowlevel handler */
- // 从下面的davinci_gpio_irq_setup函数可以看出来以下程序的运作。 
- 
-         n = (int)get_irq_data(irq);    // 获取该bank对应的第一个gpio号 
- 
-         gpio = &irq_desc[n];    // 获取该bank第一个gpio号对应的中断描述符 
- 
- while (status) {    // 该bank可能有多个gpio发生了中断 
- 
-             res = ffs(status);    // 获取第一个发生了中断的位（1-32） 
- 
-             n += res;    /* 获得该gpio的中断线（系统实际上只有64（0-63）个中断线， 
-                         但那些共用的gpio的中断也有自己的断描述符和中断线（从64开始）， 
-                         仅仅是为了管理，不能通过request_irq()函数来申请。*/
-             gpio += res;    //     获得该gpio的中断描述符 
- 
- 
- /* 调用下面注册的do_simple_IRQ例程 
-              其又会调用用户通过request_irq() 
-              注册的中断例程 
-             */
-             desc_handle_irq(n - 1, gpio - 1, regs);       
-             status >>= res;           
-         }   
-     }   
-     desc->chip->unmask(irq);    // 打开该irq中断线 
- 
- /* now it may re-trigger */
- }   
- 
- /* 
- * NOTE: for suspend/resume, probably best to make a sysdev (and class) 
- * with its suspend/resume calls hooking into the results of the set_wake() 
- * calls ... so if no gpios are wakeup events the clock can be disabled, 
- * with outputs left at previously set levels, and so that VDD3P3V.IOPWDN0 
- * can be set appropriately for GPIOV33 pins. 
- */
- /* 
- 注册gpio中断例程到内核中，并初始化了一些寄存器。 
- 该函数将会被board_evm.c（其浅析已经发表）中的evm_init()函数调用。具体调用过程如下： 
- start_kernel()-->setup_arch()-->init_machine = mdesc->init_machine 
- (init_machine是个全局函数指针变量，其指向的就是已经注册到机器描述符里evm_init())； 
- 调用函数指针init_machine()的例程是customize_machine()，其定义为 
- arch_initcall(customize_machine)，所以，接下来的调用过程是： 
- start_kernel()-->do_basic_setup()-->do_initcalls()-->customize_machine()--> 
- init_machine()（也就是evm_init()）-->davinci_gpio_irq_setup。 
- 从上可以看出经历了两个过程，才调用davinci_gpio_irq_setup例程来初始化gpio中断。 
- */
- int __init davinci_gpio_irq_setup(void)   
- {   
-     unsigned    gpio, irq, bank, banks;   
- struct clk    *clk;   
- 
-     clk = clk_get(NULL, "gpio");    // 获取时钟 
- 
- if (IS_ERR(clk)) {   
-         printk(KERN_ERR "Error %ld getting gpio clock?\n",   
-          PTR_ERR(clk));   
- return 0;   
-     }   
- 
-     clk_enable(clk);    // 使能gpio时钟并打开该模块电源 
- 
- 
- for (gpio = 0, irq = gpio_to_irq(0), bank = (cpu_is_davinci_dm355() ?   
-      IRQ_DM355_GPIOBNK0 : (cpu_is_davinci_dm6467() ?   
-      IRQ_DM646X_GPIOBNK0 : IRQ_GPIOBNK0));    // dm644x的IRQ_GPIOBNK0(56) 
- 
-      gpio < DAVINCI_N_GPIO; bank++) {    // dm644x的DAVINCI_N_GPIO(71) 
- 
- struct gpio_controller    *__iomem g = gpio2controller(gpio);   
-         unsigned        i;   
- 
- // 关该bank所有gpio的中断  
- 
-         __raw_writel(~0, &g->clr_falling);   
-         __raw_writel(~0, &g->clr_rising);   
- 
- /* set up all irqs in this bank */
- // 同一个bank的所有gpio共用一个中断例程gpio_irq_handler 
- 
-         set_irq_chained_handler(bank, gpio_irq_handler);   
-         set_irq_chipdata(bank, g);   
-         set_irq_data(bank, (void *)irq);   
- 
- for (i = 0; i < 16 && gpio < DAVINCI_N_GPIO;   
-          i++, irq++, gpio++) {   
-             set_irq_chip(irq, &gpio_irqchip);    /* 注册用于gpio中断禁止、设能 
-                                                  和类型选择的回调例程 */
-             set_irq_chipdata(irq, g);            // 保存控制结构体（寄存器）的地址 
- 
-             set_irq_handler(irq, do_simple_IRQ);/* 为每个gpio中断设置同一个中 
-                                                     断例程do_simple_IRQ*/
-             set_irq_flags(irq, IRQF_VALID);        // fiq中断有效 
- 
-         }   
-     }   
- /*     
- 一个共用bank中断线的gpio中断发生后的大致的流程是： 
- --> gpio_irq_handler --> do_simple_IRQ --> __do_irq -->  
- action->handler（用户使用request_irq()注册的中断例程） 
- */
- /* BINTEN -- per-bank interrupt enable. genirq would also let these 
-      * bits be set/cleared dynamically. 
-      */
- if (cpu_is_davinci_dm355())   
-         banks = 0x3f;   
- else
-         banks = 0x1f;   
- 
- // 向BINTEN寄存器写入0x1f（共5个位，每个位控制1个bank），打开所有的bank中断 
- 
-     __raw_writel(banks, (void *__iomem)   
-          IO_ADDRESS(DAVINCI_GPIO_BASE + 0x08));   
- 
-     printk(KERN_INFO "DaVinci: %d gpio irqs\n", irq - gpio_to_irq(0));   
- 
- return 0;   
- }   
- 
- 
- 
- 
- gpio.h   
- 
- 
- /* 
- * TI DaVinci GPIO Support 
- * 
- * Copyright (c) 2006 David Brownell 
- * Copyright (c) 2007, MontaVista Software, Inc. <source@mvista.com> 
- * 
- * This program is free software; you can redistribute it and/or modify 
- * it under the terms of the GNU General Public License as published by 
- * the Free Software Foundation; either version 2 of the License, or 
- * (at your option) any later version. 
- */
- 
- #ifndef    __DAVINCI_GPIO_H  
- #define    __DAVINCI_GPIO_H 
- 
- /* 
- * basic gpio routines 
- * 
- * board-specific init should be done by arch/.../.../board-XXX.c (maybe 
- * initializing banks together) rather than boot loaders; kexec() won't 
- * go through boot loaders. 
- * 
- * the gpio clock will be turned on when gpios are used, and you may also 
- * need to pay attention to PINMUX0 and PINMUX1 to be sure those pins are 
- * used as gpios, not with other peripherals. 
- * 
- * GPIOs are numbered 0..(DAVINCI_N_GPIO-1). For documentation, and maybe 
- * for later updates, code should write GPIO(N) or: 
- * - GPIOV18(N) for 1.8V pins, N in 0..53; same as GPIO(0)..GPIO(53) 
- * - GPIOV33(N) for 3.3V pins, N in 0..17; same as GPIO(54)..GPIO(70) 
- * 
- * For GPIO IRQs use gpio_to_irq(GPIO(N)) or gpio_to_irq(GPIOV33(N)) etc 
- * for now, that's != GPIO(N) 
- */
- #define    GPIO(X)        (X)        /* 0 <= X <= 70 */ 
- #define    GPIOV18(X)    (X)        /* 1.8V i/o; 0 <= X <= 53 */ 
- #define    GPIOV33(X)    ((X)+54)    /* 3.3V i/o; 0 <= X <= 17 */ 
- 
- /*  
- 寄存器都是32位到，每位对应一个gpio。 
- */
- struct gpio_controller {   
-     u32    dir;            // gpio方向设置寄存器 
- 
-     u32    out_data;        // gpio设置为输出时，表示输出状态(0或1) 
- 
-     u32    set_data;        // gpio设置为输出时，用于输出高电平 
- 
-     u32    clr_data;        // gpio设置为输出时，用于输出低电平 
- 
-     u32    in_data;        // gpio设置为输入时，用于读取输入值 
- 
-     u32    set_rising;        // gpio中断上升沿触发设置 
- 
-     u32    clr_rising;        // gpio中断上升沿触发清除 
- 
-     u32    set_falling;    // gpio中断下降沿触发设置 
- 
-     u32    clr_falling;    // gpio中断下降沿触发清除 
- 
-     u32    intstat;        // gpio中断状态位，由硬件设置，可读取，写1时清除。 
- 
- };   
- 
- /* The __gpio_to_controller() and __gpio_mask() functions inline to constants 
- * with constant parameters; or in outlined code they execute at runtime. 
- * 
- * You'd access the controller directly when reading or writing more than 
- * one gpio value at a time, and to support wired logic where the value 
- * being driven by the cpu need not match the value read back. 
- * 
- * These are NOT part of the cross-platform GPIO interface 
- */
- staticinlinestruct gpio_controller *__iomem   
- __gpio_to_controller(unsigned gpio)   
- {   
- void *__iomem ptr;   
- 
- if (gpio >= DAVINCI_N_GPIO)   
- return NULL;   
- 
- if (gpio < 32)   
-         ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x10);   
- elseif (gpio < 64)   
-         ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x38);   
- elseif (gpio < 96)   
-         ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x60);   
- else
-         ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x88);   
- 
- return ptr;   
- }   
- 
- staticinline u32 __gpio_mask(unsigned gpio)   
- {   
- return 1 << (gpio % 32);   
- }   
- 
- /* The get/set/clear functions will inline when called with constant 
- * parameters, for low-overhead bitbanging. Illegal constant parameters 
- * cause link-time errors. 
- * 
- * Otherwise, calls with variable parameters use outlined functions. 
- */
- externint __error_inval_gpio(void);   
- 
- externvoid __gpio_set(unsigned gpio, int value);   
- externint __gpio_get(unsigned gpio);   
- 
- /* Returns zero or nonzero; works for gpios configured as inputs OR 
- * as outputs. 
- * 
- * NOTE: changes in reported values are synchronized to the GPIO clock. 
- * This is most easily seen after calling gpio_set_value() and then immediatly 
- * gpio_get_value(), where the gpio_get_value() would return the old value 
- * until the GPIO clock ticks and the new value gets latched. 
- */
- externint gpio_get_value(unsigned gpio);   
- externvoid gpio_set_value(unsigned gpio, int value);   
- 
- 
- /* powerup default direction is IN */
- externint gpio_direction_input(unsigned gpio);   
- externint gpio_direction_output(unsigned gpio, int value);   
- 
- #include <asm-generic/gpio.h>    /* cansleep wrappers */ 
- 
- externint gpio_request(unsigned gpio, constchar *tag);   
- externvoid gpio_free(unsigned gpio);   
- 
- staticinlineint gpio_to_irq(unsigned gpio)   
- {   
- return DAVINCI_N_AINTC_IRQ + gpio;   
- }   
- 
- staticinlineint irq_to_gpio(unsigned irq)   
- {   
- return irq - DAVINCI_N_AINTC_IRQ;   
- }   
- 
- #endif                /* __DAVINCI_GPIO_H */
- 
/* 
* TI DaVinci GPIO Support 
* 
* Copyright (c) 2006 David Brownell 
* Copyright (c) 2007, MontaVista Software, Inc. <source@mvista.com> 
* 
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 2 of the License, or 
* (at your option) any later version. 
*/ 
#include <linux/errno.h> 
#include <linux/kernel.h> 
#include <linux/list.h> 
#include <linux/module.h> 
#include <linux/err.h> 
#include <linux/bitops.h> 
#include <asm/irq.h> 
#include <asm/io.h> 
#include <asm/hardware/clock.h> 
#include <asm/arch/irqs.h> 
#include <asm/arch/hardware.h> 
#include <asm/arch/gpio.h> 
#include <asm/arch/cpu.h> 
#include <asm/mach/irq.h> 
/*  
该文件实现了gpio的各种应用功能和向内核注册gpio的中断例程等功能。 
用户的驱动程序可调用gpio_request和gpio_free使用或释放该gpio， 
可以调用gpio_direction_input和gpio_direction_output函数设置gpio输入输出方向， 
调用gpio_get_value和gpio_set_value获取设置值。 
*/ 
static DEFINE_SPINLOCK(gpio_lock); 
/* 总共有DAVINCI_N_GPIO（71）个gpio引脚，故使用相应多的bit来记录这些引脚的使用状态 */ 
static DECLARE_BITMAP(gpio_in_use, DAVINCI_N_GPIO); 
/* 
申请一个gpio，其实就是检查该gpio是否空闲，如果空闲就可以使用并将该gpio相应的bit置位 
（在gpio_in_use中）。 
*/ 
int gpio_request(unsigned gpio, const char *tag) 
{ 
    if (gpio >= DAVINCI_N_GPIO) 
        return -EINVAL; 
    if (test_and_set_bit(gpio, gpio_in_use)) 
        return -EBUSY; 
    return 0; 
} 
EXPORT_SYMBOL(gpio_request); 
/* 
释放一个gpio，其实就是清除gpio相应的控制bit位（在gpio_in_use中）。 
*/ 
void gpio_free(unsigned gpio) 
{ 
    if (gpio >= DAVINCI_N_GPIO) 
        return; 
    clear_bit(gpio, gpio_in_use); 
} 
EXPORT_SYMBOL(gpio_free); 
/* 获得gpio_controller结构体指针，gpio_controller结构体是gpio的核心控制单元，里面包含 
gpio的设置和数据寄存器。该结构体和__gpio_to_controller函数在/include/asm-arm/ 
arch-davinci/gpio.h中定义，具体如下： 
struct gpio_controller { 
    u32    dir; 
    u32    out_data; 
    u32    set_data; 
    u32    clr_data; 
    u32    in_data; 
    u32    set_rising; 
    u32    clr_rising; 
    u32    set_falling; 
    u32    clr_falling; 
    u32    intstat; 
}; 
static inline struct gpio_controller *__iomem 
__gpio_to_controller(unsigned gpio) 
{ 
    void *__iomem ptr; 
    if (gpio >= DAVINCI_N_GPIO) 
        return NULL; 
    if (gpio < 32) 
        ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x10); 
    else if (gpio < 64) 
        ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x38); 
    else if (gpio < 96) 
        ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x60); 
    else 
        ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x88); 
    return ptr; 
} 
由上面的定义和ti的SPRUE25.pdf手册可以看出，__gpio_to_controller函数返回的是 
gpio_controller结构体到第一个成员dir的虚拟地址。获取了这个结构体指针后， 
便可以控制相应的gpio了。dm644x共有71个gpio， 
所以使用三个gpio_controller结构体控制，关于这个后面会由更详细的分析， 
*/ 
/* create a non-inlined version */ 
static struct gpio_controller *__iomem gpio2controller(unsigned gpio) 
{ 
    return __gpio_to_controller(gpio); 
} 
/*  
向某个gpio设置值，0或1。如果向gpio写1，则向set_data寄存器相应的位置1，如果写0， 
则向clr_data寄存器相应的位置1.__gpio_mask函数在gpio.h中定义，定义如下， 
static inline u32 __gpio_mask(unsigned gpio) 
{ 
    return 1 << (gpio % 32); 
} 
因为71个引脚由3个结构体控制，第一个控制前32个gpio，第二个控制次32个gpio， 
最后一个控制剩余的7个gpio，故__gpio_mask函数的作用是找到在其相应控制结构体里的偏移数， 
比如gpio34,那么其由第二个结构体控制，在这个机构体里的偏移是3（从0开始算，就是第二位）。 
使用这个函数之前，必须确认该gpio设置成输出模式。 
*/ 
/* 
* Assuming the pin is muxed as a gpio output, set its output value. 
*/ 
void __gpio_set(unsigned gpio, int value) 
{ 
    struct gpio_controller *__iomem g = gpio2controller(gpio); 
    // 设置gpio的值 
    __raw_writel(__gpio_mask(gpio), value ? &g->set_data : &g->clr_data);  
} 
EXPORT_SYMBOL(__gpio_set); 
/*  
通过读取in_data寄存器相应该gpio的位来读取gpio的值。 
使用这个函数之前，必须确认该gpio设置成输入模式，否则获得到值不可预料。  
*/ 
/* 
* Read the pin's value (works even if it's set up as output); 
* returns zero/nonzero. 
* 
* Note that changes are synched to the GPIO clock, so reading values back 
* right after you've set them may give old values. 
*/ 
int __gpio_get(unsigned gpio) 
{ 
    struct gpio_controller *__iomem g = gpio2controller(gpio); 
    /* 读取gpio的值，!!的目的是使得返回的值为0或1.*/ 
    return !!(__gpio_mask(gpio) & __raw_readl(&g->in_data));     
}                                                                                                                  } 
EXPORT_SYMBOL(__gpio_get); 
/*  
通过dir寄存器相应该gpio的位来设置gpio输入输出方向，为0，则设置成输出，为1，则设置出输入。 
该函数是设置成输入，故设置dir寄存器为1. 
 正如应为所说的，必须确认该引脚是作为gpio功能，而不是某个模块到功能，比如spi。通过PINMUX0 
和PINMUX1两个寄存器来设置。 
*/ 
/*--------------------------------------------------------------------------*/ 
/* 
* board setup code *MUST* set PINMUX0 and PINMUX1 as 
* needed, and enable the GPIO clock. 
*/ 
int gpio_direction_input(unsigned gpio) 
{ 
    struct gpio_controller *__iomem g = gpio2controller(gpio); 
    u32 temp; 
    u32 mask; 
    if (!g) 
        return -EINVAL; 
    spin_lock(&gpio_lock); 
    mask = __gpio_mask(gpio); 
    temp = __raw_readl(&g->dir); 
    temp |= mask;    // 设置成1 
    __raw_writel(temp, &g->dir);    // 设置该gpio为输入 
    spin_unlock(&gpio_lock); 
    return 0; 
} 
EXPORT_SYMBOL(gpio_direction_input); 
/* 
通过dir寄存器相应该gpio的位来设置gpio输入输出方向，为0，则设置成输出，为1，则设置出输入。 
该函数是设置成输出，故设置dir寄存器为0. 
value参数用于选择gpio设置成输出后该gpio输出的值。 
*/ 
int gpio_direction_output(unsigned gpio, int value) 
{ 
    struct gpio_controller *__iomem g = gpio2controller(gpio); 
    u32 temp; 
    u32 mask; 
    if (!g) 
        return -EINVAL; 
    spin_lock(&gpio_lock); 
    mask = __gpio_mask(gpio); 
    temp = __raw_readl(&g->dir); 
    temp &= ~mask;    // 设置成0  
    //设置该gpio输出值 
    __raw_writel(mask, value ? &g->set_data : &g->clr_data); 
    __raw_writel(temp, &g->dir);    // 设置gpio为输出 
    spin_unlock(&gpio_lock); 
    return 0; 
} 
EXPORT_SYMBOL(gpio_direction_output); 
/* 
向gpio设置值，0或1。 
*/ 
void gpio_set_value(unsigned gpio, int value) 
{ 
    if (__builtin_constant_p(value)) { 
        struct gpio_controller *__iomem g; 
        u32 mask; 
        if (gpio >= DAVINCI_N_GPIO) 
            __error_inval_gpio(); 
        g = __gpio_to_controller(gpio); 
        mask = __gpio_mask(gpio); 
        if (value) 
            __raw_writel(mask, &g->set_data);    // 该gpio输出高 
        else 
            __raw_writel(mask, &g->clr_data);    // 该gpio输出低 
        return; 
    } 
    __gpio_set(gpio, value); 
} 
EXPORT_SYMBOL(gpio_set_value); 
/* 
读取gpio的值，0或1. 
*/ 
int gpio_get_value(unsigned gpio) 
{ 
    struct gpio_controller *__iomem g; 
    if (!__builtin_constant_p(gpio))/* 判断该gpio值是否为编译时常数，如果是常数， 
                                     函数返回 1，否则返回 0 */ 
        return __gpio_get(gpio); 
    if (gpio >= DAVINCI_N_GPIO) 
        return __error_inval_gpio(); 
    g = __gpio_to_controller(gpio); 
     
    // 读取该gpio的值 
    return !!(__gpio_mask(gpio) & __raw_readl(&g->in_data)); 
} 
EXPORT_SYMBOL(gpio_get_value); 
/* 
* We expect irqs will normally be set up as input pins, but they can also be 
* used as output pins ... which is convenient for testing. 
* 
* NOTE: GPIO0..GPIO7 also have direct INTC hookups, which work in addition 
* to their GPIOBNK0 irq (but with a bit less overhead). But we don't have 
* a good way to hook those up ... 
* 
* All those INTC hookups (GPIO0..GPIO7 plus five IRQ banks) can also 
* serve as EDMA event triggers. 
*/ 
/* 
禁止相应该irq的gpio的中断。每个gpio都可以作为中断的来源，其中gpio0-gpio7是独立的中断来源， 
也就是分配独立的中断号，其他gpio则共用5个GPIOBNK中断线。其优先级可以在board-evm.c 
中设置（已经介绍过）。在dm644x平台上，中断是电平边缘触发的，禁止中断其实就是既不设置 
上升沿触发，也不设置下降沿触发。 
*/ 
static void gpio_irq_disable(unsigned irq) 
{ 
    struct gpio_controller *__iomem g = get_irq_chipdata(irq); 
    u32 mask = __gpio_mask(irq_to_gpio(irq)); 
    __raw_writel(mask, &g->clr_falling);    // 清除下降沿触发 
    __raw_writel(mask, &g->clr_rising);        // 清除上升沿触发 
} 
/* 
中断使能。 
在dm644x平台上，中断是电平边缘触发的，其实就是设置为上升沿或下降沿中断。 
*/ 
static void gpio_irq_enable(unsigned irq) 
{ 
    struct gpio_controller *__iomem g = get_irq_chipdata(irq); 
    u32 mask = __gpio_mask(irq_to_gpio(irq)); 
    // 如果先前为下降沿中断，则使能为下降沿中断 
    if (irq_desc[irq].status & IRQT_FALLING) 
        __raw_writel(mask, &g->set_falling); 
     
    // 如果先前为上升沿中断，则使能为上升沿中断 
    if (irq_desc[irq].status & IRQT_RISING)     
        __raw_writel(mask, &g->set_rising); 
} 
/* 
设置中断类型。 
在dm644x平台上，中断有上升沿和下降沿两种触发方式。 
*/ 
static int gpio_irq_type(unsigned irq, unsigned trigger) 
{ 
    struct gpio_controller *__iomem g = get_irq_chipdata(irq); 
    u32 mask = __gpio_mask(irq_to_gpio(irq)); 
    if (trigger & ~(IRQT_FALLING | IRQT_RISING)) 
        return -EINVAL; 
    irq_desc[irq].status &= ~IRQT_BOTHEDGE; 
    irq_desc[irq].status |= trigger; 
    __raw_writel(mask, (trigger & IRQT_FALLING) 
         ? &g->set_falling : &g->clr_falling);     // 设置为下降沿触发 
    __raw_writel(mask, (trigger & IRQT_RISING) 
         ? &g->set_rising : &g->clr_rising);    // 设置为上升沿触发 
    return 0; 
} 
/*  
该结构体用于注册到所有irq的中断描述结构体中（struct irqdesc）， 
而所有中断描述结构体定义成一个全局数组irq_desc 。 
*/ 
static struct irqchip gpio_irqchip = { 
    .unmask        = gpio_irq_enable, /* 用于使能中断， 
                                     在enable_irq()等内核函数中会用到。*/     
    .mask        = gpio_irq_disable,/* 用于禁止中断， 
                                     在disable_irq()等内核函数中会用到。*/ 
    .type        = gpio_irq_type, /* 用于设置中断类型， 
                                     在set_irq_type()内核函数中会用到。*/ 
}; 
/* 
该函数将在下面的davinci_gpio_irq_setup中使用，将被注册到五个gpio bank中断的 
irq_desc结构中，目的是处理所有级联的gpio中断。所谓级联的中断, 就是指有n个中断 
共用同一个中断线。 
在dm644x平台中，除了gpio0-gpio7外，其他63个gpio都共用五个gpiobank中断线，在这里， 
gpio0-gpio7也被注册到gpiobank中断线，但实际上并不会使用，因为它们拥有自己的 
中断线。其中，gpio0-gpio15共用IRQ_GPIOBNK0（56）中断线，gpio16-gpio31共用 
IRQ_GPIOBNK1（57）中断线，gpio32-gpio47共用IRQ_GPIOBNK2（58）中断线， 
gpio48-gpio63共用IRQ_GPIOBNK4（59）中断线，gpio64-gpio70共用 
IRQ_GPIOBNK5（60）中断线， 
因为寄存器是32位的，所以实际上只有三组寄存器，第一组包含bank0和bank1， 
也就是gpio0-gpio31，第二组包含bank2和bank3，也就是gpio32-gpio63， 
第三组包含bank4和bank5，也就是gpio64-gpio70，剩余了25个位没有使用。 
*/ 
static void 
gpio_irq_handler(unsigned irq, struct irqdesc *desc, struct pt_regs *regs) 
{ 
    struct gpio_controller *__iomem g = get_irq_chipdata(irq); 
    u32 mask = 0xffff; 
    /* we only care about one bank */ 
    // 如果bank中断线是寄数，则说明该中断的中断状态位在INTSTATn寄存器的高16位 
    if (irq & 1) 
        mask <<= 16; 
    /* temporarily mask (level sensitive) parent IRQ */ 
    desc->chip->ack(irq);// 该ack函数会在arch/arm/mach-davinci/irq.c中注册。 
    while (1) { 
        u32        status; 
        struct irqdesc    *gpio; 
        int        n; 
        int        res; 
        /* ack any irqs */ 
        /*gpio中断发生后，硬件会在INTSTATn寄存器中置位相应位， 
         以备程序查询，确定是哪个gpio*/ 
        status = __raw_readl(&g->intstat) & mask;  
        if (!status) 
            break; 
        __raw_writel(status, &g->intstat);    // 向该位写1清除 
        if (irq & 1) 
            status >>= 16; 
        /* now demux them to the right lowlevel handler */ 
        // 从下面的davinci_gpio_irq_setup函数可以看出来以下程序的运作。 
        n = (int)get_irq_data(irq);    // 获取该bank对应的第一个gpio号 
        gpio = &irq_desc[n];    // 获取该bank第一个gpio号对应的中断描述符 
        while (status) {    // 该bank可能有多个gpio发生了中断 
            res = ffs(status);    // 获取第一个发生了中断的位（1-32） 
            n += res;    /* 获得该gpio的中断线（系统实际上只有64（0-63）个中断线， 
                        但那些共用的gpio的中断也有自己的断描述符和中断线（从64开始）， 
                        仅仅是为了管理，不能通过request_irq()函数来申请。*/ 
            gpio += res;    //     获得该gpio的中断描述符 
             
            /* 调用下面注册的do_simple_IRQ例程 
             其又会调用用户通过request_irq() 
             注册的中断例程 
            */ 
            desc_handle_irq(n - 1, gpio - 1, regs);     
            status >>= res;         
        } 
    } 
    desc->chip->unmask(irq);    // 打开该irq中断线 
    /* now it may re-trigger */ 
} 
/* 
* NOTE: for suspend/resume, probably best to make a sysdev (and class) 
* with its suspend/resume calls hooking into the results of the set_wake() 
* calls ... so if no gpios are wakeup events the clock can be disabled, 
* with outputs left at previously set levels, and so that VDD3P3V.IOPWDN0 
* can be set appropriately for GPIOV33 pins. 
*/ 
/* 
注册gpio中断例程到内核中，并初始化了一些寄存器。 
该函数将会被board_evm.c（其浅析已经发表）中的evm_init()函数调用。具体调用过程如下： 
start_kernel()-->setup_arch()-->init_machine = mdesc->init_machine 
(init_machine是个全局函数指针变量，其指向的就是已经注册到机器描述符里evm_init())； 
调用函数指针init_machine()的例程是customize_machine()，其定义为 
arch_initcall(customize_machine)，所以，接下来的调用过程是： 
start_kernel()-->do_basic_setup()-->do_initcalls()-->customize_machine()--> 
init_machine()（也就是evm_init()）-->davinci_gpio_irq_setup。 
从上可以看出经历了两个过程，才调用davinci_gpio_irq_setup例程来初始化gpio中断。 
*/ 
int __init davinci_gpio_irq_setup(void) 
{ 
    unsigned    gpio, irq, bank, banks; 
    struct clk    *clk; 
    clk = clk_get(NULL, "gpio");    // 获取时钟 
    if (IS_ERR(clk)) { 
        printk(KERN_ERR "Error %ld getting gpio clock?\n", 
         PTR_ERR(clk)); 
        return 0; 
    } 
    clk_enable(clk);    // 使能gpio时钟并打开该模块电源 
    for (gpio = 0, irq = gpio_to_irq(0), bank = (cpu_is_davinci_dm355() ? 
     IRQ_DM355_GPIOBNK0 : (cpu_is_davinci_dm6467() ? 
     IRQ_DM646X_GPIOBNK0 : IRQ_GPIOBNK0));    // dm644x的IRQ_GPIOBNK0(56) 
     gpio < DAVINCI_N_GPIO; bank++) {    // dm644x的DAVINCI_N_GPIO(71) 
        struct gpio_controller    *__iomem g = gpio2controller(gpio); 
        unsigned        i; 
        // 关该bank所有gpio的中断 
        __raw_writel(~0, &g->clr_falling); 
        __raw_writel(~0, &g->clr_rising); 
        /* set up all irqs in this bank */ 
        // 同一个bank的所有gpio共用一个中断例程gpio_irq_handler 
        set_irq_chained_handler(bank, gpio_irq_handler); 
        set_irq_chipdata(bank, g); 
        set_irq_data(bank, (void *)irq); 
        for (i = 0; i < 16 && gpio < DAVINCI_N_GPIO; 
         i++, irq++, gpio++) { 
            set_irq_chip(irq, &gpio_irqchip);    /* 注册用于gpio中断禁止、设能 
                                                 和类型选择的回调例程 */ 
            set_irq_chipdata(irq, g);            // 保存控制结构体（寄存器）的地址 
            set_irq_handler(irq, do_simple_IRQ);/* 为每个gpio中断设置同一个中 
                                                    断例程do_simple_IRQ*/ 
            set_irq_flags(irq, IRQF_VALID);        // fiq中断有效 
        } 
    } 
/*     
一个共用bank中断线的gpio中断发生后的大致的流程是： 
--> gpio_irq_handler --> do_simple_IRQ --> __do_irq -->  
action->handler（用户使用request_irq()注册的中断例程） 
*/ 
    /* BINTEN -- per-bank interrupt enable. genirq would also let these 
     * bits be set/cleared dynamically. 
     */ 
    if (cpu_is_davinci_dm355()) 
        banks = 0x3f; 
    else 
        banks = 0x1f; 
     
    // 向BINTEN寄存器写入0x1f（共5个位，每个位控制1个bank），打开所有的bank中断 
    __raw_writel(banks, (void *__iomem) 
         IO_ADDRESS(DAVINCI_GPIO_BASE + 0x08)); 
    printk(KERN_INFO "DaVinci: %d gpio irqs\n", irq - gpio_to_irq(0)); 
    return 0; 
} 
 
gpio.h 
/* 
* TI DaVinci GPIO Support 
* 
* Copyright (c) 2006 David Brownell 
* Copyright (c) 2007, MontaVista Software, Inc. <source@mvista.com> 
* 
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 2 of the License, or 
* (at your option) any later version. 
*/ 
#ifndef    __DAVINCI_GPIO_H 
#define    __DAVINCI_GPIO_H 
/* 
* basic gpio routines 
* 
* board-specific init should be done by arch/.../.../board-XXX.c (maybe 
* initializing banks together) rather than boot loaders; kexec() won't 
* go through boot loaders. 
* 
* the gpio clock will be turned on when gpios are used, and you may also 
* need to pay attention to PINMUX0 and PINMUX1 to be sure those pins are 
* used as gpios, not with other peripherals. 
* 
* GPIOs are numbered 0..(DAVINCI_N_GPIO-1). For documentation, and maybe 
* for later updates, code should write GPIO(N) or: 
* - GPIOV18(N) for 1.8V pins, N in 0..53; same as GPIO(0)..GPIO(53) 
* - GPIOV33(N) for 3.3V pins, N in 0..17; same as GPIO(54)..GPIO(70) 
* 
* For GPIO IRQs use gpio_to_irq(GPIO(N)) or gpio_to_irq(GPIOV33(N)) etc 
* for now, that's != GPIO(N) 
*/ 
#define    GPIO(X)        (X)        /* 0 <= X <= 70 */ 
#define    GPIOV18(X)    (X)        /* 1.8V i/o; 0 <= X <= 53 */ 
#define    GPIOV33(X)    ((X)+54)    /* 3.3V i/o; 0 <= X <= 17 */ 
/*  
寄存器都是32位到，每位对应一个gpio。 
*/ 
struct gpio_controller { 
    u32    dir;            // gpio方向设置寄存器 
    u32    out_data;        // gpio设置为输出时，表示输出状态(0或1) 
    u32    set_data;        // gpio设置为输出时，用于输出高电平 
    u32    clr_data;        // gpio设置为输出时，用于输出低电平 
    u32    in_data;        // gpio设置为输入时，用于读取输入值 
    u32    set_rising;        // gpio中断上升沿触发设置 
    u32    clr_rising;        // gpio中断上升沿触发清除 
    u32    set_falling;    // gpio中断下降沿触发设置 
    u32    clr_falling;    // gpio中断下降沿触发清除 
    u32    intstat;        // gpio中断状态位，由硬件设置，可读取，写1时清除。 
}; 
/* The __gpio_to_controller() and __gpio_mask() functions inline to constants 
* with constant parameters; or in outlined code they execute at runtime. 
* 
* You'd access the controller directly when reading or writing more than 
* one gpio value at a time, and to support wired logic where the value 
* being driven by the cpu need not match the value read back. 
* 
* These are NOT part of the cross-platform GPIO interface 
*/ 
static inline struct gpio_controller *__iomem 
__gpio_to_controller(unsigned gpio) 
{ 
    void *__iomem ptr; 
    if (gpio >= DAVINCI_N_GPIO) 
        return NULL; 
    if (gpio < 32) 
        ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x10); 
    else if (gpio < 64) 
        ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x38); 
    else if (gpio < 96) 
        ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x60); 
    else 
        ptr = (void *__iomem)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x88); 
    return ptr; 
} 
static inline u32 __gpio_mask(unsigned gpio) 
{ 
    return 1 << (gpio % 32); 
} 
/* The get/set/clear functions will inline when called with constant 
* parameters, for low-overhead bitbanging. Illegal constant parameters 
* cause link-time errors. 
* 
* Otherwise, calls with variable parameters use outlined functions. 
*/ 
extern int __error_inval_gpio(void); 
extern void __gpio_set(unsigned gpio, int value); 
extern int __gpio_get(unsigned gpio); 
/* Returns zero or nonzero; works for gpios configured as inputs OR 
* as outputs. 
* 
* NOTE: changes in reported values are synchronized to the GPIO clock. 
* This is most easily seen after calling gpio_set_value() and then immediatly 
* gpio_get_value(), where the gpio_get_value() would return the old value 
* until the GPIO clock ticks and the new value gets latched. 
*/ 
extern int gpio_get_value(unsigned gpio); 
extern void gpio_set_value(unsigned gpio, int value); 
/* powerup default direction is IN */ 
extern int gpio_direction_input(unsigned gpio); 
extern int gpio_direction_output(unsigned gpio, int value); 
#include <asm-generic/gpio.h>    /* cansleep wrappers */ 
extern int gpio_request(unsigned gpio, const char *tag); 
extern void gpio_free(unsigned gpio); 
static inline int gpio_to_irq(unsigned gpio) 
{ 
    return DAVINCI_N_AINTC_IRQ + gpio; 
} 
static inline int irq_to_gpio(unsigned irq) 
{ 
    return irq - DAVINCI_N_AINTC_IRQ; 
} 
#endif                /* __DAVINCI_GPIO_H */
 

