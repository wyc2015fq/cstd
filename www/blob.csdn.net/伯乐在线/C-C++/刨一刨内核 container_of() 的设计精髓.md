# 刨一刨内核 container_of() 的设计精髓 - 文章 - 伯乐在线
原文出处： [wjlkoorey258](http://blog.chinaunix.net/uid-23069658-id-4745433.html)
新年第一帖，总得拿出点干货才行，虽然这篇水分还是有点大，大家可以晒干了温水冲服。这段时间一直在整理内核学习的基础知识点，期间又碰到了container_of()这个宏，当然还包括一个叫做offsetof()的家伙。在这两个宏定义里都出现将“零”地址强转成目标结构体类型，然后再访问其成员属性的情形。如果有童鞋看过我之前的博文《Segmentation fault到底是何方妖孽》的话，估计此时心里会犯嘀咕：不是说0地址不可以访问么，那container_of()和offsetof()宏定义里用0时怎么没报错呢？到底该TM如何理解“零”地址？结构体被编译时有没有什么猫腻呢？程序到底是如何访问结构体里的每个成员属性的？本篇，我们就来聊聊这几个问题。
先从内核宏定义container_of()入手：


```
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:    the type of the container struct this is embedded in.
 * @member:    the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({            
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    
    (type *)( (char *)__mptr - offsetof(type,member) );})
```
这个宏定义我们已经不止一次遇到过，相信大家对其作用和用法已经了解了(啥玩意？不了解，那就猛击这里)。今天我们主要探究的是container_of()的实现原理相关层面的技术细节。要说清container_of()还是得先过了offsetof()这关才行：


```
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
```
关于这行代码你要是到网上去搜，百分之99%的答案都是：将零地址强制转换成目标结构体类型TYPE，然后访问其成员属性MEMBER，就得到了该成员在其宿主结构体里的偏移量(按字节计算)。当然，人家这个回答也无可厚非，也不能说人家错，可为什么0地址能被这样用呢？编译器就不报错？OK，先让我们看一个简单的例子：


```
#include <stdio.h>
#pragma pack(1)
typedef struct student{
        unsigned char sex;
        unsigned int age;
        unsigned char name[32];
}Student;
int main(int argc,char** argv)
{
        Student stu;
        printf("size_of_stu = %d\n",sizeof(stu));
        printf("add_of_stu = %p\n",&stu);
        printf("add_of_sex = %p\n",&stu.sex);
        printf("add_of_age = %p\n",&stu.age);
        printf("add_of_name = %p\n",&stu.name);
        return 0;
}
```
其中第三行代码是取消编译默认的结构体对齐优化，这样一来Student结构体所占内存空间大小为37字节。运行结果如下：
![](http://blog.chinaunix.net/attachment/201501/2/23069658_1420129108ebI0.jpg)
我们可以看到，Student结构体对象stu里的三个成员属性的地址，按照我们的预期进行排列的(-_-|| 这TM不废话么，难道还倒着排不成)。此时我们知道stu对象的地址是个随机值，每次运行的时候都会变，但是无论怎么变stu.sex的地址永远和stu的地址是一致：
![](http://blog.chinaunix.net/attachment/201501/2/23069658_1420129341lSJY.jpg)
我们来反汇编一下可执行程序test：
![](http://blog.chinaunix.net/attachment/201501/2/23069658_142013080133v5.jpg)
如果你对AT&T的汇编语言不是很熟悉，建议先看一下我的另外一篇博文《深入理解C语言的函数调用过程 》。上面的反汇编代码已经和C源代码关联起来了，注意看第20行反汇编代码“lea 0x1b(%esp),%edx”，用lea指令将esp向高地址偏移27字节的地址，也就是栈空间上stu的地址装载到edx寄存器里，lea指令的全称是load effective address，所以该指令是将要操作的地址装载到目标寄存器里。另外，我们看到，在打印stu.age地址时，第26行也装载的是 0x1b(%esp)地址；打印stu.age时，注意第32、33行代码，因为栈是向高地址增长的，所以age的地址比stu.sex的地址值要大，这里在编译阶段编译器就已经完成了地址偏移的计算过程；同样地，stu.name的地址，观察第39、40行代码，是在0x1b(%esp)的基础上，增加了stu.sex和stu.age的偏移，即5个字节后找到了stu.name的地址。
也就是说，编译器在编译阶段就已经知道结构体里每个成员属性的相对偏移量，我们源代码里的所有对结构体成员的访问，最终都会被编译器转化成对其相对地址的访问，代码在运行时根本没有变量名、成员属性一说，有的也只有地址。OK，那就简单了，我们再看一下下面的程序：
点击(此处)折叠或打开


```
#include <stdio.h>
#pragma pack(1)
typedef struct student{
        unsigned char sex;
        unsigned int age;
        unsigned char name[32];
}Student;
int main(int argc,char** argv)
{
        Student *stu = (Student*)0;
        printf("size_of_stu = %d\n",sizeof(*stu));
        printf("add_of_stu = 0x%08x\n",stu);
        printf("add_of_sex = 0x%08x\n",&stu->sex);
        printf("add_of_age = 0x%08x\n",&stu->age);
        printf("add_of_name = 0x%08x\n",&stu->name);
        return 0;
}
```
运行结果：
![](http://blog.chinaunix.net/attachment/201501/2/23069658_14201319038UT3.jpg)
反汇编：
![](http://blog.chinaunix.net/attachment/201501/2/23069658_14201321649rfr.jpg)
第8行“movl $0x0,0x1c(%esp)” 为指针stu赋值，为了打印stu指针所指向的地址值，第18、19行准备将0x1c(%esp)的值压栈，为调用printf()做准备；准备打印stu->sex时，参见第23、25两行所做的事情，与第18、19行相同；当准备打印stu->age时，参见第29、30行，eax里已经保存了stu所指向的地址0，是从栈上0x1c(%esp)里取来的，然后lea指令将eax所指向地址向“后”偏1字节的地址值装载到edx里，和上面第一个实例代码一样。因为eax的值是0，所以0x1(%eax)的值肯定就是1，即此时在stu=NULL的前提下，找到了stu->age的地址。到这里，我们的问题也就差不多明朗了：
第一：对于任何一个变量，任何时候我们都可以访问该变量的地址，但是却不一定能访问该地址里的值，因为在保护模式下对地址里的值的访问是受限的；
第二，结构体在编译期间就已经确定了每个成员的大小，进而明确了每个成员相对于结构体头部的偏移的地址，源代码里所有对结构体成员的访问，在编译期间都已经静态地转化成了对相对地址的访问。
换句话说，源代码里你可以写类似于int *ptr = 0x12345；这样的语句代码，对ptr执行加、减，甚至强制类型转换都没有任何问题，但是如果你想访问ptr地址里的内容，那么很不幸，你可能会收到一个“Segmentation Fault”的错误提示，因为你访问了非法的内存地址。
最后，让我们回到开篇的那个问题：


```
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
```
相信大家现在对offsetof()定义里那个奇怪的0应该不再会感到奇怪了吧。其实container_of()里还有一个名叫typeof的东东，是用于取一个变量的类型，这是GCC编译器的一个扩展功能，也就是说typeof是编译器相关的。既不是C语言规范的所要求，也不是某个神马标准的一部分，仅仅是GCC编译器的一个扩展特性而已，Windows下的VC编译器就不带这个技能。让我们继续刨一刨container_of()的代码：


```
#define container_of(ptr, type, member) ({            
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    
    (type *)( (char *)__mptr - offsetof(type,member) );})
```
第二句代码意思是用typeof()获取结构体里member成员属性的类型，然后定义一个该类型的临时指针变量__mptr，并将ptr所指向的member的地址赋给__mptr；第三句代码意思就更简单了，__mptr减去它自身在结构体type里的偏移量就找到了结构体的入口地址，最后将该地址强转成目标结构体的地址类型就OK了。如果我们将使用了container_of()的代码进行宏展开后，看得会比较清楚一点：


```
#include <stdio.h>
#pragma pack(1)
typedef struct student{
        unsigned char sex;
        unsigned int age;
        unsigned char name[32];
}Student;
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({ 
        const typeof( ((type *)0)->member ) *__mptr = (ptr);
        (type *)( (char *)__mptr - offsetof(type,member) );})
int main(int argc,char** argv)
{
        Student stu;
        Student *sptr = NULL;
        sptr = container_of(&stu.name,Student,name);
        printf("sptr=%p\n",sptr);
        sptr = container_of(&stu.age,Student,age);
        printf("sptr=%p\n",sptr);
        return 0;
}
```
运行结果：
![](http://blog.chinaunix.net/attachment/201501/2/23069658_1420134296GO2B.jpg)
宏展开后的代码如下：


```
int main(int argc,char** argv)
{
 Student stu;
 Student *sptr = ((void *)0);
 sptr = ({ const typeof(((Student *)0)->name ) *__mptr = (&stu.name); (Student *)( (char *)__mptr - ((size_t) &((Student *)0)->name) );});
 printf("sptr=%p\n",sptr);
 sptr = ({ const typeof(((Student *)0)->age ) *__mptr = (&stu.age); (Student *)( (char *)__mptr - ((size_t) &((Student *)0)->age) );});
 printf("sptr=%p\n",sptr);
 return 0;
}
```
GCC在接下来的编译过程中会将typeof()进行替换处理，我们可以认为此时上述的代码和下面的代码是等价的：


```
int main(int argc,char** argv)
{
 Student stu;
 Student *sptr = ((void *)0);
 sptr = ({ const unsigned char  *__mptr = (&stu.name); (Student *)( (char *)__mptr - ((size_t) &((Student *)0)->name) );});
 printf("sptr=%p\n",sptr);
 sptr = ({ const unsigned int *__mptr = (&stu.age); (Student *)( (char *)__mptr - ((size_t) &((Student *)0)->age) );});
 printf("sptr=%p\n",sptr);
 return 0;
}
```
最后向伟大的程序猿、攻城狮们致敬！！
向“自由、开源”精神致敬！！
