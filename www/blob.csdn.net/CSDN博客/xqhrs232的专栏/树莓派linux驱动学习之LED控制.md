# 树莓派linux驱动学习之LED控制 - xqhrs232的专栏 - CSDN博客
2016年04月05日 16:03:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：675
个人分类：[树莓派技术](https://blog.csdn.net/xqhrs232/article/category/6762827)
原文地址::[http://blog.csdn.net/hcx25909/article/details/16860725](http://blog.csdn.net/hcx25909/article/details/16860725)
相关文章
1、为树莓派交叉编译 8192eu 网卡驱动----[http://www.codesec.net/view/197894.html](http://www.codesec.net/view/197894.html)
2、[翻译中] 树莓派内核编译----[http://my.oschina.net/funnky/blog/141249](http://my.oschina.net/funnky/blog/141249)
前面我们编写了hello world的程序，接下来继续研究GPIO功能，通过GPIO来控制LED的亮灭，这在单片机中应该算是十分简单的一个程序了，但是在Linux系统中控制GPIO没有那么简单，难点就在于GPIO地址的获取，也是我一直在纠结的问题。
# 一、GPIO地址
        我看了中嵌的嵌入式开发视频，里面使用三星2440控制LED的亮灭，但是驱动程序中没有写清楚具体的底层是如何实现的，这也是我查找的重点。我首先翻阅了树莓派CPU（bcm2835）的芯片手册，查到了GPIO的物理地址：
![](https://img-blog.csdn.net/20131121143319531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGN4MjU5MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        但是在芯片资料的最开始，有提到芯片内部已经把上图中的物理总线地址抽象到了面对操作系统的物理地址：
![](https://img-blog.csdn.net/20131121143551000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGN4MjU5MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20131121143604828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGN4MjU5MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        所以，我们在编写驱动程序的时候，IO空间的起始地址是0x20000000,加上GPIO的偏移量2000000,所以GPIO的物理地址应该是从0x20200000开始的，然后在这个基础上进行Linux系统的MMU内存虚拟化管理，银蛇到虚拟地址上。
# 二、硬件平台
        我在树莓派的扩展口的GPIO 17上接上了一个LED：
![](https://img-blog.csdn.net/20131121144511890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGN4MjU5MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20131121144816234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGN4MjU5MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 三、编写驱动代码
        一般的设备驱动我们需要设置主设备号和次设备号，在编写应用程序的时候还要生成设备文件，比较麻烦。Linux针对像LED这样的操作，有一种设备叫做混杂设备：是一种特殊的字符设备，杂设备早已经存在，是为了给开发者一个较为简单的操作方式，因为不用再重新申请一个设备号了（misc就是混杂设备的意思）。
        驱动代码：
**[cpp]**[view
 plain](http://blog.csdn.net/hcx25909/article/details/16860725#)[copy](http://blog.csdn.net/hcx25909/article/details/16860725#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75927/fork)
- #include <linux/miscdevice.h>
- #include <linux/delay.h>
- #include <asm/irq.h>
- #include <linux/kernel.h>
- #include <linux/module.h>
- #include <linux/init.h>
- #include <linux/mm.h>
- #include <linux/fs.h>
- #include <linux/types.h>
- #include <linux/delay.h>
- #include <linux/moduleparam.h>
- #include <linux/slab.h>
- #include <linux/errno.h>
- #include <linux/ioctl.h>
- #include <linux/cdev.h>
- #include <linux/string.h>
- #include <linux/list.h>
- #include <linux/pci.h>
- #include <asm/uaccess.h>
- #include <asm/atomic.h>
- #include <asm/unistd.h>
- #include <asm/io.h>
- #include <asm/uaccess.h>
- #include <linux/ioport.h>
- 
- #include "bcm2835.h"
- 
- // Blinks on RPi Plug P1 pin 11 (which is GPIO pin 17)
- #define PIN RPI_GPIO_P1_11
- 
- int open_state = 0;         //文件打开状态
- 
- staticint leds_open(struct inode *inode, struct file *filp)  
- {  
- if(open_state == 0)    
-     {    
-         open_state =  1;    
-         printk("Open file suc!\n");    
- return 0;    
-     }    
- else
-     {    
-         printk("The file has opened!\n");    
- return -1;    
-     }    
- }  
- 
- staticint leds_ioctl(struct file*filp, unsigned int cmd, unsigned long arg)  
- {  
- switch(cmd)    
-     {    
- case 0:    
-             bcm2835_gpio_clr(PIN);  
-             printk("LED OFF!\n");  
- break;    
- case 1:    
-             bcm2835_gpio_set(PIN);  
-             printk("LED ON!\n");  
- break;    
- 
- default:    
- return-EINVAL;    
-     }    
- 
- return 0;  
- }  
- 
- staticint leds_release(struct inode *inode, struct file *filp)  
- {  
- if(open_state == 1)    
-     {    
-         open_state =  0;    
-         printk("close file suc!\n");    
- return 0;    
-     }    
- else
-     {    
-         printk("The file has closed!\n");    
- return -1;    
-     }    
- }  
- 
- staticconststruct file_operations leds_fops = {  
-     .owner = THIS_MODULE,  
-     .open = leds_open,  
-     .unlocked_ioctl = leds_ioctl,  
-     .release = leds_release,  
- };  
- 
- staticstruct miscdevice misc = {  
-     .minor =MISC_DYNAMIC_MINOR,  
-     .name ="my_leds",  
-     .fops =&leds_fops,  
- };  
- 
- 
- staticint __init leds_init(void)  
- {  
- int ret;  
- 
- //注册混杂设备
-     ret =misc_register(&misc);  
- 
- //配置功能选择寄存器为输出
-     bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);  
- 
- //设置输出电平为高电平，LED亮
-     bcm2835_gpio_set(PIN);  
- 
-     printk("ledsinit.\n");  
- return ret;  
- }  
- 
- staticvoid leds_exit(void)  
- {  
- //LED灭
-     bcm2835_gpio_clr(PIN);  
- 
-     misc_deregister(&misc);          
- 
-     printk("leds_exit\n");  
- }  
- 
- module_init(leds_init);  
- module_exit(leds_exit);  
- 
- MODULE_AUTHOR("Hu Chunxu");  
- MODULE_LICENSE("GPL");  
    硬件相关操作：
**[cpp]**[view
 plain](http://blog.csdn.net/hcx25909/article/details/16860725#)[copy](http://blog.csdn.net/hcx25909/article/details/16860725#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75927/fork)
- #include <linux/miscdevice.h>
- #include <linux/delay.h>
- #include <asm/irq.h>
- #include <linux/kernel.h>
- #include <linux/module.h>
- #include <linux/init.h>
- #include <linux/mm.h>
- #include <linux/fs.h>
- #include <linux/types.h>
- #include <linux/delay.h>
- #include <linux/moduleparam.h>
- #include <linux/slab.h>
- #include <linux/errno.h>
- #include <linux/ioctl.h>
- #include <linux/cdev.h>
- #include <linux/string.h>
- #include <linux/list.h>
- #include <linux/pci.h>
- #include <asm/uaccess.h>
- #include <asm/atomic.h>
- #include <asm/unistd.h>
- #include <asm/io.h>
- #include <asm/uaccess.h>
- #include <linux/ioport.h>
- 
- #include "bcm2835.h"
- 
- int bcm2835_gpio_fsel(uint8_t pin, uint8_t mode)  
- {  
- //初始化GPIOB功能选择寄存器的物理地址
- volatile uint32_t * bcm2835_gpio = (volatile uint32_t *)ioremap(BCM2835_GPIO_BASE, 16);  
- volatile uint32_t * bcm2835_gpio_fsel = bcm2835_gpio + BCM2835_GPFSEL0/4 + (pin/10);  
-     uint8_t   shift = (pin % 10) * 3;  
-     uint32_t  value = mode << shift;  
-     *bcm2835_gpio_fsel = *bcm2835_gpio_fsel | value;  
- 
-     printk("fsel address: 0x%lx : %x\n", bcm2835_gpio_fsel, *bcm2835_gpio_fsel);  
- 
- return 0;  
- }  
- 
- int bcm2835_gpio_set(uint8_t pin)  
- {  
- //GPIO输出功能物理地址
- volatile uint32_t * bcm2835_gpio = (volatile uint32_t *)ioremap(BCM2835_GPIO_BASE, 16);  
- volatile uint32_t * bcm2835_gpio_set = bcm2835_gpio + BCM2835_GPSET0/4 + pin/32;  
-     uint8_t   shift = pin % 32;  
-     uint32_t  value = 1 << shift;  
-     *bcm2835_gpio_set = *bcm2835_gpio_set | value;  
- 
-     printk("set address:  0x%lx : %x\n", bcm2835_gpio_set, *bcm2835_gpio_set);  
- 
- return 0;  
- }  
- 
- int bcm2835_gpio_clr(uint8_t pin)  
- {  
- //GPIO清除功能物理地址
- volatile uint32_t * bcm2835_gpio = (volatile uint32_t *)ioremap(BCM2835_GPIO_BASE, 16);  
- volatile uint32_t * bcm2835_gpio_clr = bcm2835_gpio + BCM2835_GPCLR0/4 + pin/32;  
-     uint8_t   shift = pin % 32;  
-     uint32_t  value = 1 << shift;  
-     *bcm2835_gpio_clr = *bcm2835_gpio_clr | value;  
- 
-     printk("clr address:  0x%lx : %x\n", bcm2835_gpio_clr, *bcm2835_gpio_clr);  
- 
- return 0;  
- }  
        应用测试程序：
**[cpp]**[view
 plain](http://blog.csdn.net/hcx25909/article/details/16860725#)[copy](http://blog.csdn.net/hcx25909/article/details/16860725#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75927/fork)
- #include <stdio.h>
- #include <stdlib.h>
- #include <unistd.h>
- #include <sys/ioctl.h>
- 
- int main(int argc, char **argv)  
- {  
- int on;  
- int fd;  
- if (argc != 2 || sscanf(argv[1],"%d", &on) != 1 ||on < 0 || on > 1 ) {  
-         fprintf(stderr, "Usage:%s 0|1\n",argv[0]);  
-         exit(1);  
-     }  
-     fd = open("/dev/my_leds", 0);  
- if (fd < 0) {  
-         perror("open device leds");  
-         exit(1);  
-     }  
- /*通过ioctl来控制灯的亮、灭*/
- if(on){  
-         printf("turn on leds!\n");  
-         ioctl(fd, 1);  
-     }  
- else {  
-         printf("turn off leds!\n");  
-         ioctl(fd, 0);  
-     }  
-     close(fd);  
- return 0;  
- }  
      分别编译，插入模块，然后运行测试程序，可以控制LED的亮灭了。
----------------------------------------------------------------
**欢迎大家转载我的文章。**
**转载请注明：转自古-月**
**[](http://blog.csdn.net/yorhomwang)[http://blog.csdn.net/hcx25909](http://blog.csdn.net/hcx25909)**
**欢迎继续关注我的博客**
