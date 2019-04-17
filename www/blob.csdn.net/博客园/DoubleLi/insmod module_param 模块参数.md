# insmod module_param 模块参数 - DoubleLi - 博客园







**模块参数**

引导模块时，可以向它传递参数。要使用模块参数加载模块，这样写：

|insmod module.ko [param1=value param2=value ...]|
|----|

为了使用这些参数的值，要在模块中声明变量来保存它们，并在所有函数之外的某个地方使用宏MODULE_PARM(variable, type) 和 MODULE_PARM_DESC(variable, description) 来接收它们。type参数应该是一个格式为 [min[-max]]{b,h,i,l,s} 字符串，其中 min 和 max 是数组的长度限度。如果两者都忽略了，则默认为 1。最后一个字符是类型说明符：

|b byteh short i int l long s string|
|----|










在用户态下编程可以通过main()的来传递命令行参数，而编写一个内核模块则通过module_param ()
module_param宏是Linux 2.6内核中新增的，该宏被定义在include/linux/moduleparam.h文件中，具体定义如下：
#define module_param(name, type, perm)
module_param_named(name, name, type, perm)

其中使用了 3 个参数:要传递的参数变量名, 变量的数据类型, 以及访问参数的权限。
module_param (name,type,perm);
module_param 使用了 3 个参数: 变量名, 它的类型, 以及一个权限掩码用来做一个辅助的 sysfs 入口(啥意思). 这个宏定义应当放在任何函数之外, 典型地是出现在源文件的前面.定义如：
static char *whom = "world";
static int howmany = 1;
module_param (howmany, int, S_IRUGO);
module_param (whom, charp, S_IRUGO);
模块参数支持许多类型:
bool
invbool
一个布尔型( true 或者 false)值(相关的变量应当是 int 类型). invbool 类型颠倒了值, 所以真值变成 false, 反之亦然.
charp
一个字符指针值. 内存为用户提供的字串分配, 指针因此设置.
int
long
short
uint
ulong
ushort
基本的变长整型值. 以 u 开头的是无符号值.

数组参数, 用逗号间隔的列表提供的值, 模块加载者也支持. 声明一个数组参数, 使用:

module_param _array(name,type,num,perm);
这里 name 是你的数组的名子(也是参数名),
type 是数组元素的类型,
num 是一个整型变量,
perm 是通常的权限值.
如果数组参数在加载时设置, num 被设置成提供的数的个数. 模块加载者拒绝比数组能放下的多的值.

perm参数的作用是什么？
最后的 module_param 字段是一个权限值; 你应当使用 中定义的值. 这个值控制谁可以存取这些模块参数在 sysfs 中的表示.如果 perm 被设为 0, 就根本没有 sysfs 项. 否则, 它出现在 /sys/module下面, 带有给定的权限. 使用 S_IRUGO 作为参数可以被所有人读取, 但是不能改变; S_IRUGO|S_IWUSR 允许 root 来改变参数. 注意, 如果一个参数被 sysfs 修改, 你的模块看到的参数值也改变了, 但是你的模块没有任何其他的通知. 你应当不要使模块参数可写, 除非你准备好检测这个改变并且因而作出反应.
perm参数的作用是什么？
最后的 module_param 字段是一个权限值,表示此参数在sysfs文件系统中所对应的文件节点的属性。你应当使用 中定义的值. 这个值控制谁可以存取这些模块参数在 sysfs 中的表示.当perm为0时，表示此参数不存在 sysfs文件系统下对应的文件节点。 否则, 模块被加载后，在/sys/module/ 目录下将出现以此模块名命名的目录, 带有给定的权限.。
权限在include/linux/stat.h中有定义
比如：
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
使用 S_IRUGO 作为参数可以被所有人读取, 但是不能改变; S_IRUGO|S_IWUSR 允许 root 来改变参数. 注意, 如果一个参数被 sysfs 修改, 你的模块看到的参数值也改变了, 但是你的模块没有任何其他的通知. 你应当不要使模块参数可写, 除非你准备好检测这个改变并且因而作出反应.

通过宏MODULE_PARM_DESC()对参数进行说明：
static unsigned short size = 1;
module_param(size, ushort, 0644);
MODULE_PARM_DESC(size, “The size in inches of the fishing pole”
“connected to this computer.” );







module_param() 和module_param_array() 的作用就是让那些全局变量对insmod 可见，使模块装载时可重新赋值。

module_param_array() 宏的第三个参数用来记录用户insmod 时提供的给这个数组的元素个数，NULL 表示不关心用户提供的个数

module_param() 和module_param_array() 最后一个参数权限值不能包含让普通用户也有写权限，否则编译报错。这点可参考linux/moduleparam.h 中__module_param_call() 宏的定义。

字符串数组中的字符串似乎不能包含逗号，否则一个字符串会被解析成两个 



**一个测试用例：parm_hello.c**







|#include<linux/module.h>#include<linux/moduleparam.h>#include<linux/kernel.h>#defineMAX_ARRAY 6staticintint_var=0;staticconstchar*str_var="default";static int int_array[6];intnarr;module_param(int_var,int,0644);MODULE_PARM_DESC(int_var,"A integer variable");module_param(str_var,charp,0644);MODULE_PARM_DESC(str_var,"A string variable");module_param_array(int_array,int,&narr,0644);MODULE_PARM_DESC(int_array,"A integer array");staticint__init hello_init(void){       inti;       printk(KERN_ALERT"Hello, world./n");       printk(KERN_ALERT"int_var %d./n",int_var);       printk(KERN_ALERT"str_var %s./n",str_var);       for(i=0;i<narr;i++){               printk("int_array[%d] = %d/n",i,int_array[i]);       }       return0;}staticvoid__exit hello_exit(void){       printk(KERN_ALERT"Bye, world./n");}module_init(hello_init);module_exit(hello_exit);MODULE_LICENSE("GPL");MODULE_AUTHOR("kelly");MODULE_DEION("This module is a example.");|
|----|




Linux中EXPORT_SYMBOL的用法


**EXPORT_SYMBOL标签内定义的函数对全部内核代码公开，不用修改内核代码就可以在您的内核模块中直接调用。**您还可以手工修改内核源代码来导出另外的函数，用于重新编译并加载新内核后的测试。


Linux symbol export method:

[1] If we want export the symbol in a module, just use the **EXPORT_SYMBOL(xxxx)** in the C or H file.
    And compile the module by adding the compile flag **-DEXPORT_SYMTAB**.
    Then we can use the **xxxx** in the other module.


[2] If we want export some symbol in Kernel that is not in a module such as **xxxx** in the **/arch/ppc/fec.c**.
    Firstly, define the **xxxx** in the **fec.c**;
    Secondly, make a new file which contain the **"extern"** define the **xxxx**(for example, extern int **xxxx**);
    Lastly, in the **ppc_ksyms.c** we includes the new file, and add the **EXPORT_SYMBOL**(**xxxx**).
    Then we can use the **xxxx**.























对于如何向模块传递参数，Linux kernel 提供了一个简单的框架。其允许驱动程序声明参数，并且用户在系统启动或模块装载时为参数指定相应值，在驱动程序里，参数的用法如同全局变量。

使用下面的宏时需要包含头文件。



    通过宏module_param()定义一个模块参数：
module_param(name, type, perm);
name既是用户看到的参数名，又是模块内接受参数的变量； 

type表示参数的数据类型，是下列之一：byte, short, ushort, int, uint, long, ulong, charp, bool, invbool；     

perm指定了在sysfs中相应文件的访问权限。访问权限与linux文件爱你访问权限相同的方式管理，如0644，或使用stat.h中的宏如S_IRUGO表示。

0表示完全关闭在sysfs中相对应的项。


这些宏不会声明变量，因此在使用宏之前，必须声明变量，典型地用法如下：
static unsigned int int_var = 0;   
module_param(int_var, uint, S_IRUGO);

这些必须写在模块源文件的开头部分。即int_var是全局的。也可以使模块源文件内部的变量名与外部的参数名有不同的名字，通过module_param_named()定义。module_param_named(name, variable, type, perm);其中name是外部可见的参数名，variable是源文件内部的全局变量名，而module_param通过module_param_named实现，只不过name与variable相同。

例如：
static unsigned int max_test = 9;
module_param_name(maximum_line_test, max_test, int, 0);



如果模块参数是一个字符串时，通常使用charp类型定义这个模块参数。内核复制用户提供的字符串到内存，并且相对应的变量指向这个字符串。

例如：
static char *name;
module_param(name, charp, 0);

另一种方法是通过宏module_param_string()让内核把字符串直接复制到程序中的字符数组内。
module_param_string(name, string, len, perm);

这里，name是外部的参数名，string是内部的变量名，len是以string命名的buffer大小（可以小于buffer的大小，但是没有意义），perm表示sysfs的访问权限（或者perm是零，表示完全关闭相对应的sysfs项）。

例如：
static char species[BUF_LEN]；
module_param_string(specifies, species, BUF_LEN, 0);



如果需要传递多个参数可以通过宏module_param_array()实现。 
module_param_array(name, type, nump, perm);
其中，name既是外部模块的参数名又是程序内部的变量名，type是数据类型，perm是sysfs的访问权限。指针nump指向一个整数，其值表示有多少个参数存放在数组name中。值得注意是name数组必须静态分配。

例如：
static int finsh[MAX_FISH];
static int nr_fish;
module_param_array（fish, int, &nr_fish, 0444）; //最终传递数组元素个数存在nr_fish中

通过宏module_param_array_named()使得内部的数组名与外部的参数名有不同的名字。

例如：
module_param_array_named(name, array, type, nump, perm);



通过宏MODULE_PARM_DESC()对参数进行说明：
static unsigned short size = 1;
module_param(size, ushort, 0644);
MODULE_PARM_DESC(size, “The size in inches of the fishing pole”
“connected to this computer.” );





** 说明：from  **http://blog.csdn.net/iczyh/archive/2008/10/26/3149727.aspx

module_param() 和 module_param_array() 的作用就是让那些全局变量对 insmod 可见，使模块装载时可重新赋值。

module_param_array() 宏的第三个参数用来记录用户 insmod 时提供的给这个数组的元素个数，NULL 表示不关心用户提供的个数

module_param() 和 module_param_array() 最后一个参数权限值不能包含让普通用户也有写权限，否则编译报错。这点可参考 linux/moduleparam.h 中 __module_param_call() 宏的定义。

字符串数组中的字符串似乎不能包含逗号，否则一个字符串会被解析成两个



**一个测试用例：parm_hello.c**



|#include <linux/module.h>#include <linux/moduleparam.h>#include <linux/kernel.h>#define MAX_ARRAY 6static int int_var = 0;static const char *str_var = "default";static int int_array[6];int narr;module_param(int_var, int, 0644);MODULE_PARM_DESC(int_var, "A integer variable");module_param(str_var, charp, 0644);MODULE_PARM_DESC(str_var, "A string variable");module_param_array(int_array, int, &narr, 0644);MODULE_PARM_DESC(int_array, "A integer array");static int __init hello_init(void){       int i;       printk(KERN_ALERT "Hello, my LKM.\n");       printk(KERN_ALERT "int_var %d.\n", int_var);       printk(KERN_ALERT "str_var %s.\n", str_var);       for(i = 0; i < narr; i ++){               printk("int_array[%d] = %d\n", i, int_array[i]);       }       return 0;}static void __exit hello_exit(void){       printk(KERN_ALERT "Bye, my LKM.\n");}module_init(hello_init);module_exit(hello_exit);MODULE_LICENSE("GPL");MODULE_AUTHOR("ydzhang");MODULE_DEION("This module is a example.");|
|----|



测试：insmod parm_hello.ko int_var=100 str_var=hello int_array=100,200









