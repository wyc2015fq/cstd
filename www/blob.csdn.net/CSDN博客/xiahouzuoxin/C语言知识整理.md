# C语言知识整理 - xiahouzuoxin - CSDN博客





2013年09月10日 09:05:51[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2812
个人分类：[ComputerLanuage](https://blog.csdn.net/xiahouzuoxin/article/category/1172320)









** 转载出处：[http://blog.csdn.net/mbh_1991/article/details/10149805](http://blog.csdn.net/mbh_1991/article/details/10149805)**

** 原文是一个系列文章，我将他们揉在了一起，并将原标题“C语言深度解剖读书笔记”改名为“C语言知识整理”，并将持续在原作者的基础上进行修改（修订）及增加。整理后文章如下 **





        开始本节学习笔记之前，先说几句题外话。其实对于C语言深度解剖这本书来说，看完了有一段时间了，一直没有时间来写这篇博客。正巧还刚刚看完了国嵌唐老师的C语言视频，觉得两者是异曲同工，所以就把两者一起记录下来。等更新完这七章的学习笔记，再打算粗略的看看剩下的一些C语言的书籍。






# 第一节 关键字使用






**1.c语言中一共有32个关键字**

分别是：auto、int、double、long、char、short、float、unsigned、signed、sizeof、extern、static、goto、if、else、struct、typedef、union、enum、switch、case、break、default、do、while、const、register、volatile、return、void、for、continue。注意：define、include这些带#号的都不是关键字，是预处理指令。



**2.定义与声明：**

定义   是创建一个对象并为止分配内存。  如：int   a;

声明   是告诉编译器在程序中有这么一个对象，并没有分配内存。   如： extern   int    a;




**3.对于register这个关键字定义的变量**，不能进行取地址运算(&)，因为对于x86架构来说，地址都是在内存中的，不是在寄存器中的，所以对寄存器进行取地址是没有意义的。并且应该注意的是给register定义的变量，应该赋一个比寄存器大小 要小的值。注意：register只是请求寄存器变量，但是不一定申请成功。




4.**关键字static**

   对于static有两种用法：

   a.修饰变量：对于静态全局变量和静态局部变量，都有一个特点就是不能被作用域外面，或外文件调用(即使是使用了extern也没用)。原因就是它是存储在静态存储区中的。对于函数中的静态局部变量还有一个问题，就是它是存在静态存储区的，即使函数结束栈区收回，这个变量的值也不改变。static int i=0;
  这是一条初始化语句  而不是一条赋值语句  所以跟i=0不一样的。

   b.修饰函数 ：是定义为静态函数，使函数只能在文件内部使用，这样不同文件中的函数名就不怕重名了。原因也是相同的，就是static修饰的一切都是在静态存储区中的。

![](https://img-blog.csdn.net/20130821162403781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)-

static代码如下：




**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- #include <stdio.h>
- #include <stdlib.h>
- 
- int main(void)   
- {  
- staticint j=0;  
- int k;  
- void fun1()  
-     {  
-         j=0;  
-         j++;  
-         printf("fun1 %d\n",j);  
-     }  
- void fun2()  
-     {  
- 
- staticint i=0;  
- //i=0;
-         printf("fun2 %d\n",i);  
-         i++;  
-     }  
- for(k=0;k<10;k++)  
-     {  
-             fun1();  
-             fun2();  
-     }   
- return 1;    
- }  




**5.****关键字sizeof**

怎么说明sizeof是关键字不是函数，这里有两个例子：

a. int i;    printf("%d\n",sizeof i); 可见 sizeof是关键字

b. sizeof(fun());  不调用fun函数 因为sizeof是在预编译期间完成的  说明是关键字

sizeof的代码：




**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- #include <stdio.h>
- #include <stdlib.h>
- 
- void fun(int b[100])  
- {  
-     printf("sizeof(b) is %d\n",sizeof(b));  
- }  
- 
- int main(void)   
- {  
- int *p=NULL;  
- int a[100];  
- int b[100];  
-     printf("sizeof(p) is %d\n",sizeof(p));  
-     printf("sizeof(*p) is %d\n",sizeof(*p));  
-     printf("sizeof(a[100]) is %d\n",sizeof(a[100]));  
-     printf("sizeof(a) is %d\n",sizeof(a));  
-     printf("sizeof(&a) is %d\n",sizeof(&a));  
-     printf("sizeof(&a[0] is %d\n",sizeof(&a[0]));  
- 
-     fun(b);  
- return 1;  
- }  


6.**关键字if**a.对于bool类型的比较：FLASE都是0  TRUE不一定是1   所以应该用if(bool_num);    if(!bool_num);

对于浮点型与0比较要是否注意：不能直接比较，要定义精度，其实浮点型与浮点型比较也要注意这个问题，就是不能直接比较，要设定精度，如图：

![](https://img-blog.csdn.net/20130821200143781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原因跟浮点型的存储格式有关，因为float的有效位是6位，超出6位就未知了，所以不能直接进行比较。同样的原因，也不能用一个很大的浮点数去加一个很小的浮点数。这个加法可能体现不出来。

b.对于if后面的分号问题 ，一定要注意， 会被解析成if后面有一个空语句， 所以使用空语句的时候最好使用NULL;

c.在使用if else的时候，应该if里面先处理正常情况(出现概率大的情况)，else里面处理异常情况，这是一个好习惯看着代码舒服。




**7.关键字switch、case**

注意case后面应该是整型或者字符型的常量及常量表达式，case后面最好是应该安装字母或数字顺序排列，先处理正常情况，后处理异常情况。




**8.关键字void**

void *的一般用途是， 接收任何类型的指针 ，如当传入函数的指针类型不确定的时候，一般用 void*接收任何类型的指针。

void* 指针作为右值赋值给其他指针的时候一定要强制类型转换，因为void* 指针类型不定。

GNU中void *p p++跟char *p p++是一样的 。

**注意：strcpy跟memcpy的区别 就是 strcpy是char *   memcpy是void ***  。**所以说strcpy是给字符串赋值，memset是给整块内存赋值。**




**9.****关键字extern：**

 extern就有两种用法：一种是声明外部定义的变量或函数、另一种是extern c告诉编译器以标准c语言方式编译




**10.****关键字return：**

使用return的时候，要注意不能返回栈内指针，因为在函数体结束后，栈是会被收回的，其实是不能期望返回一个指针，来返回一块内存。因为返回一个指针或者地址没有问题，因为return是copy然后返回的，但是那个指针指向的内存如果是在函数栈中的话，就很有可能在函数结束后被收回了！！！

return  ; 一般返回的值是1，根据编译器而定。




**11.****关键字const：**

a. const是用来定义只读变量的，切忌它定义的是变量，不是常量，真的常量是#define的和enum。

b. 在陈正冲老师的这本书中的第35页，***有说编译器不为普通const只读变量分配内存空间，而是将它们保存在符号表中，这使得它成为一个编译期间的值，没有了存储与读内存的操作，使得它的效率也很高，节省空间。***具体的没怎么看懂，本次学习也不打算看懂了(因为它说const修饰的全局只读变量是在静态区的，我太认同)~~~嘿嘿

c. 其实const就是修饰变量，然后这个变量就不能当作左值了，当作左值，编译器就报错！！！

d. 其实const中最不好区分的知识点是，如图：

![](https://img-blog.csdn.net/20130821203242859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其实对于这四个情况的记忆很简单，就是看const跟谁近，是const *p   ，还是  * const  p，还是const  *  const  p，这样就很容易看出来const是修饰谁的了吧。

e. 但是const修饰的变量可以通过，指针将其改变。

f. const修饰函数参数表示在函数体内不希望改变参数的值，比如说在strcmp等函数中，用的都是const  char*

g. const修饰函数返回值表示返回值不可以改变，多用于返回指针的情况：



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- cosnt int* func()  
- {  
- staticint  count  =  0;  
-       count++;  
- return &count;  
- }  



h. 在看const修饰谁，谁不变的问题上，可以把类型去掉再看，代码如下：



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- struct student  
- {  
- 
- }*str;  
- const str stu3;  
- str const stu4;  



str是一个类型 ，所以在去掉类型的时候，应该都变成const stu3和const stu4了，所以说应该是stu4和stu3这个指针不能被赋值。


**12.****关键字volatile：**volatile搞嵌入式的，一定都特别属性这个关键字，记得第一使用这个关键字的时候是在韦东山老师的，Arm裸机视频的时候。volatile是告诉编译不要对这个变量进行任何优化，直接在内存中进行取值。一般用在对寄存器进行赋值的时候，或修饰可能被多个线程访问的变量。

注意：const  volatile  int  i；  应该是定义了一个只读寄存器。




**13.****关键字struct：**

a.对于空结构体的大小问题 ，vc和gcc的输出是不一样的，vc是1 、gcc是0 ，而且vc对于结构体的定义也和gcc不一样 ，vc中有c++的标准扩展了struct的作用，而gcc中是纯c的标准，就是按照标准c语言来的。

b.struct这里还有一个很有用的东西，就是柔性数组，这个东西很有意思，我已经在数据结构的静态链表中进行了阐述，这里就仅仅记录一下，不详细说明了。



**14.****关键字union：**

union有一个作用就是判断，pc是大端存储还是小端存储的，x86是小端存储的，这个东西是有cpu决定的。arm(由存储器控制器决定)和x86一样都是小端的。


下面的是一个大端小端的一个例子，代码如下：




**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- #include <stdio.h>
- #include <stdlib.h>
- int main(void)   
- {  
- int a[5]={1,2,3,4,5};  
- int *p=(int *)(&a+1);  //数组指针 加一  进行正常的指针运算 走到数
- 
- 组尾   
- int *d=(int *)((int)a+1);//地址加一  不是指针运算
- //printf("%x\n",*((char *)((int)a+1)-1));
- 
- /*因为是小端存储  高地址  0x00  0x00  0x00  0x02  0x00  0x00  0x00  0x01 低地址*/
- /*变成了 0x02  0x00  0x00  0x00 */
-     printf("%x,%x",p[-1],*d);  /*  第二个值就是这么存储的0x02  0x00  0x00  0x00  低地址处  所以就是2000000*/
- int a=0x11223344;  
- char *p=(char *)((int)&a);  
-     printf("%x\n%x\n",*(p+0),p+0);   
-     printf("%x\n%x\n",*(p+1),p+1);  
- return 0;  
- }  


下面是一个利用union判断PC是大端小端的例子，代码如下：




**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- #include <stdio.h>
- #include <stdlib.h>
- union
- {  
- int i;  
- char a[2];  
- }*p,u;  
- 
- int main(void)   
- {  
-     p=&u;  
-     p->i=0x3839;  
-     printf("%x\n",p->i);  
-     printf("a0p=%x,a1p=%x\n",&(p->a[0]),&(p->a[1]));  
-     printf("a0=%x,a1=%x\n",p->a[0],p->a[1]);    
- return 0;  
- }  

**15.****enum关键字：**

枚举enum其实就是int类型，用来保存枚举常量的。enum枚举类型，这个才是真正的常量，定义常量一般用enum 。#define是宏定义是在预编译期间单纯的替换。#define宏定义无法调试，枚举常量是可以调试的。#define宏定义是无类型信息的，枚举类型是有类型信息的常量，是int型的。




**16.****typedef关键字：**

a.typedef用于给一个已经存在的数据类型重新命名。

b.typedef并没有产生新的数据类型

c.typedef重定义的类型不能进行unsigned和signed进行扩展

原因在于typedef 定义新类型的时候 应该定义全了，unsigned int是一个类型  不能拆开的。




**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- typedef  unsigned  int   int32;  



d.typedef 和 #define的区别：typedef是给已有的类型取别名，而#define只是简单的字符替换。区别如下图：

![](https://img-blog.csdn.net/20130822004435734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#define PCHAR char*             PCHAR p3，p4;  //p3是char*型 p4是char型

typedef char* PCHAR;             PCHAR p1,p2;    //p1和p2都是 char*型

e.有一个知识点忘记了，嘿嘿，程序如下：


**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- typedefstruct student  
- {  
- }str，*str1;  




str1 abc;  就是定义一个struct student *类型

str abc;   就是定义一个struct student 类型
f. 对于const和typedef还有两个问题遗漏了，在[< c++学习笔记(1.c到c++的升级)>](http://blog.csdn.net/mbh_1991/article/details/10431519)这篇文章中的最后
 (8.补充) 中进行了阐述。




**17.****关键字for**：

a.长循环应该在最内层，这样可以减少各个层直接的切换

b.看看如下两段代码有什么区别：



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- 程序一：  
- for(i=0; i<m; i++)  
- {  
- for(j=0; j<n; j++)  
-     {  
- for(k=0; k<p; k++)  
-         {  
-             c[i][j] = a[i][k] * b[k][j];  
-         }  
-     }  
- }  
- 
- 程序二：  
- for(i=0; i<m; i++)  
- {  
- for(k=0; k<p; k++)  
-     {  
- for(j=0; j<n; j++)  
-         {  
-             c[i][j] = a[i][k] * b[k][j];  
-         }  
-     }  
- }  


从程序来看，两者实现了同样的功能，区别只是第二层和第三层循环交换了位置。但是他们的差距却是巨大的 ，这个需要从CPU的cache来说了， cpu每次访问内存的时候都会先从内存将数据读入cache ，然后以后都从cache取数据。但是cache的大小是有限的 ，因此只会有部分进入cache。我们来看这个程序 c[i][j] = a[i][k] * b[k][j];  我们都知道C中二维数组是在内存中一维排列的，如果我们把k循环放在第三层 ，那么cache基本没有用了，
 每次都需要重新到内存取数据，交换后每次取到cache的数据都可以复用多次 。所以说第二种写法效率高。



**18. ****关键字char(本节最重要的知识点char越界的问题)：**

对于char有两种类型，分别是：unsigned  char(范围是0~255)和  signed  char(范围是-128~127)  一个是有符号的，一个是没有符号的。

在计算机中数据都是以数据的补码形式进行存储的，所以如图：

![](https://img-blog.csdn.net/20130822103006812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于无符号类型(unsigned  int)：就是不考虑最高位的问题，都是原码与补码相等的情况。

     然后我们说说越界的问题，对于一个unsigned  char  i;  我们给 i = 256；这很明显越界了，i是0到255的，那256的补码是什么再在它补码中取低八位就是i的值了。256的补码是1  0000  0000，所以printf ("%d\n",i);的值会是0。如果i = -1；-1的补码是1111 1111 所以会打印出255。

     对于一个char类型的越界又是什么样的呢？

     char  i; 我们给 i  =  129； 129是一个正数，它的补码就是原码：是1000 0001，但是它是char型，在char型中1000  0001是什么，如图是-127。所以printf("%d\n",i);  得到的是-127。如果i  =  -129，它的补码是0111  1111，所以它打印出来的是127。如果是i  =  259，我们就把它的补码取低八位来看。259的补码是1 
 0000  0011  所以说打印出来的是3。最后一个例子，如果i  =  385，它的补码是1   1000    0001  ，取低八位是1000   0001，所以打印的应该是-127。

     其实不管是有符号的还是没符号的，原则就一个，把数据转换成为补码，取低八位，然后在上面的图中去比较，就ok了。

给一个练习，代码如下：




**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10149805#)[copy](http://blog.csdn.net/mbh_1991/article/details/10149805#)



- #include <stdio.h>
- #include <stdlib.h>
- #include <string.h>
- int main()  
- {  
- char a[1000];  
- int i;  
- for(i=0; i<1000; i++)  
-     {  
-         a[i] = (-1-i);  
-     }     
- while(a[i])  
-     {  
-         printf("%d\n",a[i]);  
-         i++;  
-     }  
-     printf("%d\n",strlen(a));  
- return 0;  
- }  




打印结构是什么：答案是255   分析步骤跟上面是一样的，自己算算吧！！！

其实int的越界原理跟char是一样的。

19.一个关于tab键的问题：

不同编辑器的tab键的字符数是不一样的，一般是4个字符，也有两个字节的，要注意一下，为了代码格式的整齐，建议设置一下tab或者使用空格。



***本节遗留问题***

1.printf的实现问题，其实就是可变参数的问题，看linux源码，还有一个问题就是转移字符的问题，char p = '\'' 这样一个问题。

2.浮点型的存储格式，为什么有效位是6位，小数是怎么保存的。





# 第二节 字符技巧





**1.注释问题：**


    注释不能把关键字弄断，如：in/*注释*/t


    注释不是简单的剔除，而是使用空格替换


    编译器认为双引号括起来的内容都是字符串，双斜杠也不例外。如：char *p = "heh//jfeafe"   //不起注释作用




**2.接续符：**


    接续符\  ，常用于宏定义中 



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10184395#)[copy](http://blog.csdn.net/mbh_1991/article/details/10184395#)



- #define SWAP(a,b) \
- {                 \  
- int temp = a; \  
-     a = b;        \  
-     b = temp;     \  
- }  



    反斜杠同时有接续符和转义符两个用途，当接续符使用的时候，可以直接在程序中出现。当转义符使用的时候，必须是出现在字符串中。


    接续符，也用与接续一个关键字，代码如下，  注意： 直接连接\两边不能有空格。



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10184395#)[copy](http://blog.csdn.net/mbh_1991/article/details/10184395#)



- #include <stdio.h>
- #include <stdlib.h>
- 
- int main()  
- {  
-     cha\  
- r a = 12;  
- return 0;  
- }  


**3. 逻辑运算符**：短路规则


**4. 最容易忘记规则的两个运算符**


    三目运算符：(a?b:c)   当a的值为真的时候   返回b的值，否则返回c的值


    逗号表达式：a，b    表达式的值为b的值




**5. 位运算**


    对于左移和右移<<  >>问题 ：无符号的，和有符号左移，都是补0 ，对于有符号的在右移动的时候，正数补零，负数补什么跟编译器有关系。并且左移和右移的大小不能大于数据的长度，也不能小于0。


    交换两个数，有一种不借助中间变量的方法，就是异或，代码如下：



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10184395#)[copy](http://blog.csdn.net/mbh_1991/article/details/10184395#)



- #include <stdio.h>
- 
- #define SWAP1(a,b) \
- {                  \  
- int temp = a;  \  
-     a = b;         \  
-     b = temp;      \  
- }  
- 
- #define SWAP2(a,b) \
- {                  \  
-     a = a + b;     \  
-     b = a - b;     \  
-     a = a - b;     \  
- }  
- 
- #define SWAP3(a,b) \
- {                  \  
-     a = a ^ b;     \  
-     b = a ^ b;     \  
-     a = a ^ b;     \  
- }  
- 
- int main()  
- {  
- int a = 1;  
- int b = 2;  
- 
-     SWAP1(a,b);  
-     SWAP2(a,b);  
-     SWAP3(a,b);  
- 
- return 0;  
- }  


**6. ++与--**


        顺序点就是什么时候开始加，什么时候开始减。


```cpp
i = 0;
printf("%i", i++);        /* i=0; printf(i); i=i+1; */
printf("%i", ++i):        /* i=i+1; printf(i) */
```





**7. 贪心法**


        每一个符号应该尽可能多的包含字符


**8. 符号运算优先级问题**


        个人觉得优先级不用记，好好的写括号吧~~~


         给一个易错优先级表，如图：

![](https://img-blog.csdn.net/20130822182303859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**9.****c语言中的类型转换**


    c语言中有两种转换类型，分别是：隐式转换和显示转换(强制类型转换)


    隐式转换的规则：


    a.算术运算中，低类型转换为高类型


    b.赋值运算中，表达式的类型转换为左边变量的类型


    c.函数调用时，实参转换成形参的类型


    d.函数返回值，return表达式转换为返回值的类型

![](https://img-blog.csdn.net/20130822183501296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


隐式转换的例子，代码如下：



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10184395#)[copy](http://blog.csdn.net/mbh_1991/article/details/10184395#)



- #include <stdio.h>
- 
- int main()  
- {  
- int i = -2;  
-     unsigned int j = 1;  
- 
- if( (i + j) >= 0 )  
-     {  
-         printf("i+j>=0\n");  
-     }  
- else
-     {  
-         printf("i+j<0\n");  
-     }  
- 
-     printf("i+j=%d\n", i + j);  
- 
- return 0;  
- }  


注意：在使用C语言的时候，应该特别注意数据的类型是否相同，尽量避免隐式转换带来的不必要的麻烦~~~



# 第二节 预编译处理




**1. 编译过程的简介**


C和C++文件的编译是以*.c或*.cpp为单位进行编译的！

![](https://img-blog.csdn.net/20130823152815265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 预编译：


a.处理所有的注释，以空格代替。


b.将所以#define删除，并展开所有的宏定义，字符串替换。


c.处理条件编译指令#if，#ifdef，#elif，#else，#endif


d.处理#include，并展开被包含的文件，把头文件中的声明，全部拷贝到文件中。


e.保留编译器需要使用的#pragma指令、


怎么样观察这些变化呢？最好的方法就是在GCC中，输入预处理指令，可以看看不同文件经过预处理后变成什么样了，预处理指令：gcc -E file.c -o file.i   注意：-C -E一起使用是预编译的时候保留注释。

编译：


a.对预处理文件进行一系列词法分析，语法分析和语义分析


                词法分析：主要分析关键字，标示符，立即数等是否合法


                语法分析：主要分析表达式是否遵循语法规则


                语义分析：在语法分析的基础上进一步分析表达式是否合法


b.分析结束后进行代码优化生成相应的汇编代码文件               编译指令：gcc -S  file.c  -o  file.s

汇编：


汇编器将汇编代码转变为机器可以执行的指令，每个汇编语句几乎都对应一条机器指令，其实机器指令就是机器码，就是2进制码。汇编指令：gcc  -c  file.c  -o file.o  注意：-c是编译汇编不连接。

   链接：


再把产生的.o文件，进行链接就可以生成可执行文件。连接指令：gcc  file.o  file1.o  -o  file  这句指令是链接file.o和file1.o两个编译并汇编的文件，并生成可执行文件file。


链接分两种：**静态链接和动态链接**，静态链接是在编译器完成的，动态链接是在运行期完成的。静态链接的指令是：gcc -static file.c -o file对于一些没有动态库的嵌入式系统，这是常用的。


一般要想通过一条指令生成可执行文件的指令是：   gcc file.c  -o  file



**2. c语言中的预处理指令**


#define、#undef(撤销已定义过的宏名)、#include、#if、#else、#elif、#endif、#ifdef、#ifndef、#line、#error、#pragma。还有一些ANSI标准C定义的宏：__LINE__、__FILE__、__DATA__、__TIME__、__STDC__。这样使用printf("%s\n",__TIME__);     printf(__DATE__);


一个#undef的例子：



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10222377#)[copy](http://blog.csdn.net/mbh_1991/article/details/10222377#)



- #include <stdio.h>
- #include <stdlib.h>
- #include <string.h>
- 
- 
- #define X 2
- #define Y X*2
- #undef X
- #define X 3
- 
- 
- int main()  
- {  
-     printf("%d\n",Y);  
- return 0;  
- }  



这个输出的是6，说明了#undef的作用





**3. 宏定义字符串的时候**


应该是 #define HELLO "hello world"  记住是双引号。**还有就是一切宏都是不能有分号的，这个一定要切忌！！！**



**4. 宏与函数的比较**


   a.宏表达式在预编译期被处理，编译器不知道有宏表达式存在


   b.宏表达式没有任何的"调用"开销

** c.宏表达式中不能出现递归定义**



**5. 为什么不在头文件中定义全局变量**


如果一个全局变量，想要在两个文件中，同时使用，那这两个文件中都应该#include这个头文件，这样的话就会出现重复定义的问题。其实是重名的问题，因为#include是分别在两个文件中展开的，试想一下，如果在两个文件中的开始部分，都写上int  a = 10;  是不是也会报错。可能你会说那个#ifndef不是防止重复定义吗？是的 ，那是防止在同一个文件中，同时出现两次这个头文件。现在是两个文件中，所以都要展开的。全局变量就重名了！！！所以 对于全局变量，最好是定义在.c文件中，不要定义在头文件中。




**6.#pargma pack 设置字符对齐**


详细参考我的另一篇博文[小侃#pragma](http://blog.csdn.net/xiahouzuoxin/article/details/11096693)




**7.****#运算符**（转换成字符串）：


    假如你希望在字符串中包含宏参数，那我们就用#号，它把语言符号转换成字符串。


    #define SQR(x) printf("the "#x"lait %d\n",((x)*(x)));

    SQR(8)

    输出结果是：the 8 lait 64   **这个#号必须使用在带参宏中**

有个小例子：



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10222377#)[copy](http://blog.csdn.net/mbh_1991/article/details/10222377#)



- #include <stdio.h>
- #include <stdlib.h>
- #include <string.h>
- /*在字符串中  加入宏参用的*/
- #define SCAN(N,String) scanf("%"#N"s",String);  //N是截取的个数  String是存储的字符串 
- int main()  
- {  
- char dd[256];  
-     SCAN(3,dd) //记得没有分号哈  自定义 任意格式输入的scanf  截取输入的前三个 
-     printf("%s\n",dd);  
- return 1;  
- }  


**8. ****##运算符**（粘合剂）


    一般用于粘贴两个东西，一般是用作在给变量或函数命名的时候使用。如#define XNAME(n) x##n


    XNAME(8)为8n   **这个##号可以使用在带参宏或无参宏中**


下面是一个##运算符的小例子，代码如下：



**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10222377#)[copy](http://blog.csdn.net/mbh_1991/article/details/10222377#)



- #include <stdio.h>
- #include <stdlib.h>
- #include <string.h>
- 
- #define BL1 bb##ll##1
- 
- #define BL(N) bbll##N
- int main()  
- {  
- int BL1=10;  
- 
- int BL(4)=15;  
-     printf("%d\n",bbll1);  
- 
-     printf("%d\n",bbll4);  
- return 1;  
- }  


**注意：#号和##号都必须只能在宏定义中使用，不能使用在其他地方**
**9. 不常用到的预编译指令**


用到的时候查查就好。比如说#line、#error、#warning等。



# 第三节 结构体内存对齐






**1. 结构体为什么要内存对齐(也叫字节对齐)**


其实我们都知道，结构体只是一些数据的集合，它本身什么都没有。我们所谓的结构体地址，其实就是结构体第一个元素的地址。这样，如果结构体各个元素之间不存在内存对齐问题，他们都挨着排放的。对于32位机，32位编译器(这是目前常见的环境，其他环境也会有内存对齐问题)，就很可能操作一个问题，就是当你想要去访问结构体中的一个数据的时候，需要你操作两次数据总线，因为这个数据卡在中间，如图：

![](https://img-blog.csdn.net/20130823220251093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在上图中，对于第2个short数据进行访问的时候，在32位机器上就要操作两次数据总线。这样会非常影响数据读写的效率，所以就引入了内存对齐的问题。


另外一层不太重要的原因是：某些硬件平台只能从规定的地址处取某些特定类型的数据，否则会抛出硬件异常。




**2. 内存对齐的规则**


    a.第一个成员起始于0偏移处


    b.每个成员按其类型大小和指定对齐参数n中较小的一个进行对齐


    c.结构体总长度必须为所有对齐参数的整数倍


    d.对于数组，可以拆开看做n个数组元素




**3. 来几个小例子，画画图，有助于理解**


第一个例子，代码如下：





**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10241785#)[copy](http://blog.csdn.net/mbh_1991/article/details/10241785#)



- #include <stdio.h>
- struct _tag_str1  
- {  
- char a;  
- int b;  
- short c;  
- }str1;  
- 
- struct _tag_str2  
- {  
- char a;  
- short c;  
- int b;  
- }str2;  
- 
- int main()  
- {  
-     printf("sizeof str1 %d\n",sizeof(str1));  
-     printf("sizeof str2 %d\n",sizeof(str2));  
- return 0;  
- }   

输出的结果分别是：str1为12    str2为8，分析的过程如下图：


![](https://img-blog.csdn.net/20130823224227609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


看图很自然就知道了str1为12个字节，str2为8个字节。


第二个例子，上面的那个例子有好多问题还没有考虑到，比如说上面的那个例子在8字节对齐，和4字节对齐的情况都是一样的。结构体中嵌套结构体的内存对齐怎么算，所以就有了这个例子，代码如下：





**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10241785#)[copy](http://blog.csdn.net/mbh_1991/article/details/10241785#)



- #include <stdio.h>
- 
- #pragma pack(8)
- //#pragma pack(4)
- struct S1  
- {  
- short a;  
- long b;  
- };  
- 
- struct S2  
- {  
- char c;  
- struct S1 d;  
- double e;  
- };  
- 
- #pragma pack()
- 
- int main()  
- {  
- struct S2 s2;  
- 
-     printf("%d\n", sizeof(struct S1));  
-     printf("%d\n", sizeof(struct S2));  
-     printf("%d\n", (int)&(s2.d) - (int)&(s2.c));  
- 
- return 0;  
- }  

在Dev c++中，默认的是8字节对齐。我们分析下在4字节对齐的情况下输出的是，S2是20，S1是8，分析如图：


![](https://img-blog.csdn.net/20130823234737218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在4字节对齐的情况中，有一个问题值得注意：就是图中画1的地方。这里面本应short是可以上去的。但是对于结构体中的结构体一定要十分警惕，S1是一体的，short已经由于long进行了内存对齐，后面还空了两个字节的内存，其实此时的short已经变成了4个字节了！！！即结构体不可拆，不管是多少字节对齐，他们都是一体的。所有的圈都变成了叉。所以说结构体只能往前篡位置，不能改变整体。


我们在分析一些8字节对齐的情况，如图：

![](https://img-blog.csdn.net/20130824001429218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

同样，到这里又有一个字节对齐的原则要好好重申一下：就是以什么为对齐参数，首先我们要知道编译器或者自己定义的是多少字节对齐的，这个数为n。然后我们要看这个结构体中的各个数据类型，找到所占字节数最大的类型，为m。如果n大于m，就以m为对齐参数，比如说一个4字节对齐的结构体中都是short，那这个结构体以什么为对齐参数，当然是2了，如果m大于n，就以n为对齐参数，比如说在4字节对齐的情况下的double类型。


 以上就是我对内存对齐的小总结，最最想要说明的就是两大段红色的部分。    






# 第四节 堆栈问题




**1.栈的知识**

    对于栈的问题，首先我们通过几个不同的角度来看(因为思维有些小乱所以我们通过分总的形式进行阐述)：

**a.** sp堆栈指针，相信学过51单片机，学过arm裸机的人都知道这个堆栈指针。我们现在从51单片机的角度来看这个堆栈指针寄存器。这个堆栈指针的目的是什么？是用来保护现场(子函数的调用)和保护断点(中断的处理)的，所以在处理中断前，调用子函数前，都应该把现场和返回地址压入栈中。而且堆栈还会用于一些临时数据的存放。51中的sp指针再单片机复位的时候初值为0x07。**常常我们会把这个sp指针指向0x30处**，因为**0x30~0x7f**是用户RAM区(专门为堆栈准备的存储区)。然后要引入一个**栈顶**和**栈底**的概念。栈操作的一段叫栈顶(这里是sp指针移动的那个位置，**sp也叫栈顶指针**)，sp指针被赋初值的那个地址叫栈底(这里是0x30是栈底，因为栈顶永远会只在0x30栈底的一侧进行移动，不会在两层移动)。而且51单片机的sp是向上增长的，叫**做向上增长型堆栈(栈顶指针sp向高地址处进行增长)**。因为PUSH压栈操作，是sp指针先加1(指向的地址就增大一个)，再压入一个字节，POP弹出操作，先弹出一个字节，sp再减1(指向的地址就减少一个)。看PUSH和POP的过程，可见是一个**满堆栈**(满堆栈的介绍在后面)。小结一下：**51的堆栈是一个向上增长型的满堆栈**。

**b. **
对于arm来说，大量的分析过程都与上面相同。只是堆栈不再仅仅局限于处理中断了，而是处理异常。**arm的堆栈有四种增长方式(具体见d)**。**注意：在arm写裸机的时候，那个ldr sp, =8*1024   其实是在初始化栈底。sp是栈顶指针，当没有使用堆栈的时候，栈顶指针是指向栈底的。当数据来的时候，每次都是从栈顶进入的(因为栈的操作入口在栈顶)，然后sp栈顶指针指向栈顶，慢慢远离栈底。说这些是想好好理解下什么是栈顶，什么是栈底。**

**c. **对于8086来说，它的栈的生长方向也是从高地址到低地址，每次栈操作都是以字(两个字节)为单位的。压栈的时候，sp先减2，出栈的时候，sp再加2。可见**8086的堆栈是一个向下增长型的满堆栈**。

** d.** 总结下：

                    (1).当堆栈指针sp指向，最后一个压入堆栈的数据的时候，叫满堆栈。

                    (2).当堆栈指针sp指向，下一个要放入数据的空位置的时候，叫空堆栈。如下图：

![](https://img-blog.csdn.net/20130829162844640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

                  (3).当堆栈由低地址向高地址生长的时候，叫向上生长型堆栈即递增堆栈。

                  (4).当堆栈由高地址向低地址生长的时候，叫向下生长型堆栈即递减堆栈。如图：

![](https://img-blog.csdn.net/20130829234105703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

                 (5). 所以说arm堆栈支持四种增长方式：满递减栈(**常用的ARM，Thumb c/c++编译器都使用这个方式，也就是说如果你的程序中不是纯汇编写的，有c语言就得使用这种堆栈形式**)、满递增栈、空递减栈、空递增栈。这四种方式分别有各自的压栈指令，出栈指针，如下图：
  

![](https://img-blog.csdn.net/20130829235840984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**  e. **对于裸机驱动程序(51、ARM)没有操作系统的，编译器(keil、arm-linux-gcc等)，会给sp指针寄存器一个地址。然后一切的函数调用，中断处理，这些需要的现场保护啊，数据啊都压入这个sp指向的栈空间。(arm的.s文件是自己写的，sp是自己指定的，编译器会根据这个sp寄存器的值进行压栈和出栈，但是压栈和出栈的规则是满递减栈的规则，因为arm-linux-gcc是这个方式的，所以在汇编调用c函数的时候，汇编代码必须使用满递减栈的那套压栈出栈指令)。这种没有操作系统的裸机驱动程序，只有一个栈空间，就是sp指针指向的那个栈空间。

**f. **对于在操作系统上面的程序，里面涉及内存管理、虚拟内存、编译原理的问题。首先说不管是linux还是windows的进程的内存空间都是独立的，linux是前3G，windows是4G，这都是虚拟内存的功劳。那编译器给程序分配的栈空间，在程序运行时也是独立的。每一个进程中的栈空间，应该都是在使用sp指针(但是在进程切换的过程中，sp指针是怎么切换的我就不清楚了，这个应该去看看操作系统原理类的书)Ps：对于x86的32位机来说不再是sp和bp指针了，而是esp和ebp两个指针。有人说程序中的栈是怎么生长的，是由编译器决定的，有人说是由操作系统决定的！！！我觉得都不对，应该是由**硬件决定的**，因为cpu已经决定了sp指针的压栈出栈方式。只要你操作系统在进程运行的过程中，使用的这个栈是sp栈指针指向的(即使用了sp指针)，而不是自己定义的一块内存(与sp指针无关的话)
  Ps：实际中进程使用的是esp和ebp两个指针，这里仅仅用sp是想说明那个意思而已！  操作系统使用的栈空间就必须符合sp指针的压栈和出栈方式，也就是遵循了cpu决定的栈的生长方式。编译器要想编译出能在这个操作系统平台上使用的程序，也必须要遵守这个规则，所以来看这个栈的生长方式是由cpu决定的。这也是为什么我用那么长的篇幅来解释sp指针是怎么工作的原因！

**g. **要记住，由于操作系统有虚拟内存这个东东，所以不要再纠结编译器分配的空间在操作系统中，进程执行的时候空间是怎么用的了。编译器分配的是什么地址，进程中使用这个变量的虚拟地址就是什么！是对应的。当然有的时候，**编译器也会耍些小聪明**。不同编译器对栈空间上的变量分配的地址可能不一样，但方向一定是一样的(**因为这个方向是cpu决定，编译器是无权决定的，是sp指针压栈的方向**)，如图：

![](https://img-blog.csdn.net/20130830131234171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图1和图2的共同点是：都是从高地址处到低地址处，因为sp指针把A、B、C变量压入栈的方向就是从高到低地址的。这个是什么编译器都不会变的。

图1和图2的不同点是：图2进行了编译器的小聪明，它在给A，B，C开辟空间的时候，不是连续开辟的空间，有空闲(其实依然进行了压栈操作只是压入的是0或者是ff)，这样变量直接有间隙就避免了，数组越界，内存越界造成的问题。切记在获取A、B、C变量的时候，不是通过sp指针，而是通过变量的地址获得的啊，sp只负责把他们压入栈中，即给他们分配内存。

**h. **说了那么多栈的原理，现在我们说说栈在函数中究竟起到什么作用：**保存活动记录**！！！如图：

![](https://img-blog.csdn.net/20130830132544656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注意：活动记录是什么上面的这个图已经说的很清楚了，如果再调用函数，这个活动记录会变成什么样呢？会在这个活动记录后面继续添加活动记录(这个活动记录是子函数的活动记录)，增加栈空间，当子函数结束后，子函数的活动记录清除，栈空间继续回到上图状态！

Ps：活动记录如下：

![](https://img-blog.csdn.net/20130830135150937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**i. **函数的调用行为。函数的调用行为中有一个很重要的东西，叫做调用约定。调用约定包含两个约定。

第一个是：参数的传递顺序(这个不是固定的，是在编译器中约定好的)，从左到右依次入栈：__stdcall、__cdecl、__thiscall   (这些指令，直接写在函数名的前面就可以，但是跟编译器有点关系，可能会有的编译器不支持会报错)，从右到左依次入栈：__pascal、__fastcall

第二个是：堆栈的清理(这段代码也是编译器自己添加上的)：调用者清理 或者 被调用函数返回时清理

注意：一般我们都在同一个编译器下编译不会出这个问题。 但是如果是调用动态链接库，恰巧编译动态链接库的编译器跟你的编译器的默认约定不一样，那就惨了！！！或者说如果动态链接库的编写语言跟你的语言都不一样呢？ 

**j.** 这里要声明一个问题：就是栈的增长方向是固定的，是cpu决定的。**但是不代表说你定义的局部变量也一定是先定义的在高地址，后定义的在低地址，局部变量之间都是连续的(这个在上面已经说过了是编译器决定的)，还有就是栈的增长方向也决定不了参数的传递顺序(这个是调用约定，通过编译器的手处理的)**。下面让我们探索下再dev
 c++中，局部变量的地址问题。




**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10531795#)[copy](http://blog.csdn.net/mbh_1991/article/details/10531795#)



- #include <stdio.h>
- 
- void fun()  
- {  
- int a;  
- int b;  
- int c;  
-     printf("funa  %p\n",&a);  
-     printf("funb  %p\n",&b);  
-     printf("func  %p\n",&c);  
- }  
- void main()  
- {  
- int a;  
- int b;  
- int c;  
- int d;  
- int e;  
- int f;  
- int p[100];  
- 
-     printf("a  %p\n",&a);  
-     printf("b  %p\n",&b);  
-     printf("c  %p\n",&c);  
-     printf("d  %p\n",&d);  
-     printf("e  %p\n",&e);  
-     printf("f  %p\n",&f);  
-     printf("p0    %p\n",&p[0]);  
-     printf("p1    %p\n",&p[1]);  
-     printf("p2    %p\n",&p[2]);  
-     printf("p3    %p\n",&p[3]);  
-     printf("p4    %p\n",&p[4]);  
- 
-     printf("p10    %p\n",&p[10]);  
-     printf("p20    %p\n",&p[20]);  
-     printf("p30    %p\n",&p[30]);  
-     printf("p80    %p\n",&p[80]);  
-     printf("p90    %p\n",&p[90]);  
-     printf("p100    %p\n",&p[100]);  
- 
- 
-     fun();  
- 
- }  


运行结果如下(不同编译器的运行结果是不一样的)：

![](https://img-blog.csdn.net/20130830150042781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWJoXzE5OTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

通过上面的运行结果，可以分析得出：在同一个函数中，先定义的变量在高地址处，后定义的变量在低地址处，且他们的地址是相连的中间没有空隙。**定义的数组是下标大的在高地址处，下标小的在低地址处(由此可以推断出malloc开辟出的推空间，也应该是下标大的在高地址处，下标小的在低地址处)**。子函数中的变量，跟父函数中的变量的地址之间有很大的一块空间，这块空间应该是两个函数的其他活动记录，且父函数中变量在高地址处，子函数中的变量在低地址处。
**  k. **下面来一个栈空间数组越界的问题，让大家理解一下，越界的危害，代码如下(猜猜输出结构)：




**[cpp]**[view
 plain](http://blog.csdn.net/mbh_1991/article/details/10531795#)[copy](http://blog.csdn.net/mbh_1991/article/details/10531795#)



- #include<stdio.h>
- /*这是一个死循环*/
- /*这里面有数组越界的问题*/
- /*有栈空间分配的问题*/
- int main()  
- {  
- 
- int i;  
- //  int c;
- int a[5];  
- int c;  
-     printf("i %p,a[5] %p\n",&i,&a[5]); //观察栈空间是怎么分配的  这跟编译器有关系的
-     printf("c %p,a[0] %p\n",&c,&a[0]);  
- for(i=0;i<=5;i++)  
-     {  
-         a[i]=-i;  
-         printf("%d,%d",a[i],i);  
-     }  
- return 1;  
- }  



注意：不同编译器可能结果不一样，比如说vs2008就不会死循环，那是因为vs2008耍了我上面说的那个小聪明(就是局部变量和数组直接有间隙不是相连的，就避开了越界问题，但是如果越界多了也不行)，建议在vc6和dev c++中编译看结果。
**  l. **最后说说数据结构中的栈，其实数据结构中的栈就是一个线性表，且这个线性表只有一个入口和出口叫做栈顶，还是LIFO(后进先出的)结构而已。

**  对栈的总结：**之前就说过了那么多种栈的细节，现在在宏观的角度来看，其实栈就是一种线性的后进先出的结构，只是不同场合用处不同而已！




**2. 堆空间**：堆空间弥补了栈空间在函数返回后，内存就不能使用的缺陷。是需要程序员自行跟操作系统申请的。




**3. 静态存储区**

    程序在编译期，静态存储区的大小就确定了





**4. 使用内存的好习惯**

    a.定义指针变量的时候，最好是初始化为NULL，用完指针后，最好也赋值为NULL。

    b.在函数中使用指针尽可能的，去检测指针的有效性

    c.malloc分配的时候，注意判断是否分配内存成功。

    d.malloc后记得free，防止内存泄漏！

    e.free(p)后应该p=NULL

    f.不要进行多次free

    g.不要使用free后的指针

    h.牢记数组的长度，防止数组越界




**5. 内存常见的六个问题**

    a.野指针问题 ：一个指针没有指向一个合法的地址

    b.为指针分配的内存太小

    c.内存分配成功，但忘记初始化，memset的妙用

    e.内存越界

    f.内存泄漏

    g.内存已经被释放 还仍然在使用(栈返回值问题)


**6. 有关内存布局**的内容请移步google搜索“内存布局”





# 第五节 函数的问题



**1. 参数可变的函数**

参考本博客内博文[可变参数函数的实现](http://blog.csdn.net/xiahouzuoxin/article/details/7685518)




**2. 待续。。。**






















