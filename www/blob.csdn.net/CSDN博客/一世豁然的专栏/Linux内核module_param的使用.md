# Linux内核module_param的使用 - 一世豁然的专栏 - CSDN博客





2014年12月22日 20:33:37[一世豁然](https://me.csdn.net/Explorer_day)阅读数：754标签：[Linux驱动](https://so.csdn.net/so/search/s.do?q=Linux驱动&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)









本博客转载于：[http://blog.csdn.net/dysh1985/article/details/7802080](http://blog.csdn.net/dysh1985/article/details/7802080)







1.定义模块参数的方法:

module_param(name, type, perm);

其中,name:表示参数的名字;

     type:表示参数的类型;

     perm:表示参数的访问权限;



2. 数组类型模块参数的定义:

用逗号间隔的列表提供的值;

声明一个数组参数:

module_param_array(name, type, num, perm);

其中,name:表示数组的名字;

     type:表示参数的类型;

     num :表示数组中元素数量;

     perm:表示参数的访问权限;




3.type支持的基本类型有:

bool   :布尔类型

invbool:颠倒了值的bool类型;

charp  :字符指针类型,内存为用户提供的字符串分配;

int    :整型

long   :长整型

short  :短整型

uint   :无符号整型

ulong  :无符号长整型

ushort :无符号短整型



4. perm参数 设定访问权限

modlue_param和module_param_array中的perm用于设定该参数的访问权限;

perm表示该参数在sysfs文件系统中所对应的文件节点的属性;你用该使用<linux/stat.h>中定义的权限值;这个值控制谁可以存取这些模块参数在sysfs文件系统中的表示;当perm为0时,表示此参数不存在sysfs文件系统下对应的文件节点;否则,模块被加载后,在/sys/module/目录下将会出现以此模块名命名的目录,带有给定的权限;

比如:

#define S_IRWXU 00700

#define S_IRUSR 00400

#define S_IWUSR 00200

#define S_IXUSR 00100

#define S_IRWXG 00070

#define S_IRGRP 00040

#define S_IWGRP 00020

#define S_IXGRP 00010

#define S_IRWXO 00007

#define S_IROTH 00004

#define S_IWOTH 00002

#define S_IXOTH 00001

注意:如果一个参数被sysfs修改了,那么你的模块看到的参数值也被修改了,但是你的模块不会收到任何通知;你应当不要使模块参数可写,除非你准备好检测这个改变并因而作出反应;



5. 也可以在insmod（装载模块）的时候为参数指定值，如果没有指定则使用默认值，如下面的例子。



**[cpp]**[view plain](http://blog.csdn.net/dysh1985/article/details/7802080#)[copy](http://blog.csdn.net/dysh1985/article/details/7802080#)

- MODULE_LICENSE("Dual BSD/GPL");  
- staticint num=10;  
- module_param(num,int,S_IRUGO);  
- staticint hello_init(void)  
- {  
-     printk("Hello module init./n");  
-     printk("num=%d/n",num);  
- return 0;  
- }  
- staticvoid   hello_exit(void)  
- {  
-     printk("Goodbye module exit./n");  
- }  
- module_init(hello_init);  
- module_exit(hello_exit);  
- MODULE_DESCRIPTION("a simple module");  
- MODULE_ALIAS("hello");  


上面的num=10,则10是参数num的默认值。

保存为源文件hello.c, 编译生成hello.ko , 当我们使用insmod进行载入时, 使用

insmod hello.ko num=20


则可以 cat /sys/module/hello/parameters/num ，看到为 20。



