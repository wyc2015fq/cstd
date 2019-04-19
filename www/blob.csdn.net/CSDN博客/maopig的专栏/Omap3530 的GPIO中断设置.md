# Omap3530 的GPIO中断设置 - maopig的专栏 - CSDN博客
2012年04月06日 09:09:54[maopig](https://me.csdn.net/maopig)阅读数：2824标签：[null																[input																[byte																[cmd																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[c/C++																[Linux																[DM368](https://blog.csdn.net/maopig/article/category/919673)](https://blog.csdn.net/maopig/article/category/850267)](https://blog.csdn.net/maopig/article/category/847675)
Omap3530 的GPIO中断设置：
1.配置成GPIO，申请GPIO中断
omap_cfg_reg(OMAP3_KBD_GPIO);配置成gpio
if (gpio_request(OMAP3_KBD_GPIO, "kbd7279 IRQ") < 0)
printk(KERN_ERR "Failed to request GPIO%d for kbd IRQ/n");//申请GPIO为中断引脚。
2. 设置中断触发模式
set_irq_type(OMAP_GPIO_IRQ(OMAP3_KBD_GPIO),IRQ_TYPE_EDGE_FALLING);
3.使能中断
enable_irq(gpio_to_irq(OMAP3_KBD_GPIO));
gpio_to_irq()函数将OMAP3_KBD_GPIO映射为中断管脚，enable_irq()函数将其使能，这样该端口就设置为中断了。
4.申请中断
if(result = request_irq(OMAP_GPIO_IRQ(OMAP3_KBD_GPIO), &Kbd7279_ISR,0, "Ds7279", NULL))
{
printk(KERN_INFO"[FALLED: Cannot register Kbd7279_Interrupt!]/n");
return -EBUSY;
}
else
printk("[OK]/n");
下面是完整的设置程序:
void Setup_Kbd7279(void)
{
int i,result;
omap_cfg_reg(OMAP3_KBD_GPIO);
if (gpio_request(OMAP3_KBD_GPIO, "kbd7279 IRQ") < 0)
printk(KERN_ERR "Failed to request GPIO%d for kbd IRQ/n");
gpio_direction_input(OMAP3_KBD_GPIO); 
set_irq_type(OMAP_GPIO_IRQ(OMAP3_KBD_GPIO),IRQ_TYPE_EDGE_FALLING);//IRQ_TYPE_EDGE_RISING
enable_irq(gpio_to_irq(OMAP3_KBD_GPIO));
//set_irq_type(OMAP_GPIO_IRQ(OMAP3_KBD_GPIO),0 );IRQ_TYPE_LEVEL_LOW
if(result = request_irq(OMAP_GPIO_IRQ(OMAP3_KBD_GPIO), &Kbd7279_ISR,0, "Ds7279", NULL))
{
printk(KERN_INFO"[FALLED: Cannot register Kbd7279_Interrupt!]/n");
return -EBUSY;
}
else
printk("[OK]/n");
send_byte(cmd_reset);
setcs1;
printk("HD7279 setup complete! --- > [OK]/n");
for(i=0;i<1000;i++);
write7279(decode1+7,0x1);
write7279(decode1+6,0x2);
}
这里有个地方一定要注意的，一定要确定设为中断的引脚工作在GPIO模式，如果不是，先要设置为GPIO模式。

