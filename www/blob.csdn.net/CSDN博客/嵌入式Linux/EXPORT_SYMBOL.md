
# EXPORT_SYMBOL - 嵌入式Linux - CSDN博客

2014年08月11日 14:34:39[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：541个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)


linux2.6的“/prob/kallsyms”文件对应着内核符号表，记录了符号以及符号所在的内存地址。
模块可以使用如下宏导出符号到内核符号表：
**[c-sharp]**[view
 plain](http://blog.csdn.net/lisan04/article/details/4076013#)[copy](http://blog.csdn.net/lisan04/article/details/4076013#)

EXPORT_SYMBOL(符号名);
EXPORT_SYMBOL_GPL(符号名)

导出的符号可以被其他模块使用，不过使用之前一定要声明一下。EXPORT_SYMBOL_GPL()只适用于包含GPL许可权的模块。
代码演示：
**[c-sharp]**[view
 plain](http://blog.csdn.net/lisan04/article/details/4076013#)[copy](http://blog.csdn.net/lisan04/article/details/4076013#)

//hello.c文件，定义2个函数，用于导出
\#include <linux/init.h>
\#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
intadd_integar(inta,intb)
{
returna + b;
}
intsub_integar(inta,intb)
{
returna - b;
}
EXPORT_SYMBOL(add_integar);
EXPORT_SYMBOL(sub_integar);
//test.c 用于调用hello模块导出的函数
\#include <linux/init.h>
\#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
externintadd_integar(int,int);//声明要调用的函数
externintsub_integar(int,int);//声明要调用的函数
intresult(void)
{
inta,b;
a = add_integar(1,1);
b = sub_integar(1,1);
printk("%d/n",a);
printk("%d/n",b);
return0;
}

make后，先加在hello模块，再加载test模块。
然后cat /proc/kallsyms | grep integer
显示：
**[c-sharp]**[view
 plain](http://blog.csdn.net/lisan04/article/details/4076013#)[copy](http://blog.csdn.net/lisan04/article/details/4076013#)

[root@localhost test]\# cat /proc/kallsyms |grep integar
e053d000 u add_integar  [test]
e053d004 u sub_integar  [test]
e053d02c r __ksymtab_sub_integar        [hello]
e053d03c r __kstrtab_sub_integar        [hello]
e053d034 r __ksymtab_add_integar        [hello]
e053d048 r __kstrtab_add_integar        [hello]
e053d000 T add_integar  [hello]
e053d004 T sub_integar  [hello]


