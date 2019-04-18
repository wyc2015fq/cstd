# C++对内存的使用管理 - sinat_32043495的博客 - CSDN博客





2017年12月05日 11:15:45[LZXandTM](https://me.csdn.net/sinat_32043495)阅读数：61








## 1.内存分区

C++编译器将计算机内存分为代码区和数据区，很显然，代码区就是存放程序代码，而数据区则是存放程序编译和执行过程出现的变量和常量。数据区又分为静态数据区、动态数据区以及常量区，动态数据区包括堆区和栈区。

 以下是各个区的作用：

 （1）代码区：存放程序代码；

 （2）数据区

        a.静态数据区: 在编译器进行编译的时候就为该变量分配的内存,即全局变量和静态变量(用static声明的变量），存放在这个区的数据程序全部执行结束后系统自动释放，声明周期贯穿于整个程序执行过程。

       b 堆区：这部分存储空间完全由程序员自己负责管理，它的分配和释放都由程序员自己负责。这个区是唯一一个可以由程序员自己决定变量生存期的区间。可以用malloc,new申请对内存，并通过free和delete释放空间。如果程序员自己在堆区申请了空间，又忘记将这片内存释放掉，就会造成内存泄露的问题，导致后面一直无法访问这片存储区域。

       c 栈区：存放函数的形式参数和局部变量，由编译器分配和自动释放，函数执行完后，局部变量和形参占用的空间会自动被释放。效率比较高，但是分配的容量很有限。

       d 常量区： 存放常量的区间，如字符串常量等，注意在常量区存放的数据一旦经初始化后就不能被修改。

在弄懂内存分配的问题过后，来看看函数调用的过程：



执行某个函数时，如果有参数，则在栈上为形式参数分配空间（如果是引用类型的参数则类外），继续进入到函数体内部，如果遇到变量，则按情况为变量在不同的存储区域分配空间（如果是static类型的变量，则是在进行编译的过程中已经就分配了空间），函数内的语句执行完后，如果函数没有返回值，则直接返回调用该函数的地方（即执行远点），如果存在返回值，则先将返回值进行拷贝传回，再返回执行远点，函数全部执行完毕后，进行退栈操作，将刚才函数内部在栈上申请的内存空间释放掉。






## **2.例子理解**

下面通过几个例子来谈谈内存分配和函数返回值的问题：

内存分配的问题：

int a=1;               a在栈区

char s[]="123";    s在栈区，“123”在栈区，其值可以被修改

char *s="123";     s在栈区，“123”在常量区，其值不能被修改

int *p=new int;     p在栈区，申请的空间在堆区（p指向的区域）

int *p=(int *)malloc(sizeof(int)); p在栈区，p指向的空间在堆区

static int b=0;      b在静态区

**example1:**

#include<iostream>

using namespace std;

void getMory(char*str)

{
str = (char*)malloc(100);/*传递过来的实参s的值进行复制拷贝，
 传指针方式，并且通过指针变量对指针所指的地址中的内容进行改变时
 会同时改变主函数中的实参所对应的数据, 
 但如果改的只是指针变量本身的值,则没有影响！！ 
 为形参str在栈上分配空间后，讲s的值赋给str
 因此用mallo在堆上申请空间后，str是指向了堆上的一个存储区域，但是s的值依

                                                  然为NULL*/
}

void test(void)

{
char *s = NULL;
getMory(s);
strcpy(s, "hello Word");
cout << s << endl;

}

int main(void)

{
test();
system("pause");
return 0;

}




error C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. 


在运行过程中会报错，即程序发生崩溃，出现的原因就是函数用指针进行传递时，其实传递的也是一个值，也是将实参进行拷贝赋给形参，因此对形参的改变并不影响实参的值。




**example2:**


#include<iostream>

using namespace std;

char* test(void)

{
char str[] = "hello Word";
return str;

}

int main(void)

{
char *p;
p = test();
cout << p << endl;
system("pause");
return 0;

}




结果打印结果可能是helloworld，也可能是乱码。 

出现这种情况的原因在于：在test函数内部声明的str数组以及它的值“hello world”是在栈上保存的，当用return将str的值返回时，将str的值拷贝一份传回，当test函数执行结束后，会自动释放栈上的空间，即存放hello world的单元可能被重新写入数据，因此虽然main函数中的指针p是指向存放hello world的单元，但是无法保证test函数执行完后该存储单元里面存放的还是hello world，所以打印出的结果有时候是hello
 world，有时候是乱码，就是这种原因。




**example3:**


#include<iostream>

using namespace std;

int test(void)

{
int a = 1;
return a;

}

int main(void)

{
int b;
b = test();
cout << b << endl;
system("pause");
return 0;

}


运行结果是 1

有人会问为什么这里传回来的值可以正确打印出来，不是栈会被刷新内容么？是的，确实，在test函数执行完后，存放a值的单元是可能会被重写，但是在函数执行return时，会创建一个int型的临时变量，将a的值复制拷贝给该临时变量，因此返回后能够得到正确的值，即使存放a值的单元被重写数据，但是不会受到影响。



**example4:**

#include<iostream>

using namespace std;

char* test(void)

{
char *str = "hello Word";
return str;

}

int main(void)

{
char *p;
p = test();
cout << p << endl;
system("pause");
return 0;

}


执行结果是 helloworld

同样返回的是指针，为什么这里会正确地打印出hello world？这是因为char *p="hello world"，指针p是存放在栈上的，但是“helloworld”是一个常量字符串，因此存放在常量区，而常量区的变量的生存期与整个程序执行的生命期是一样的，因此在test函数执行完后，str指向存放“hello world”的单元，并且该单元里的内容在程序没有执行完是不会被修改的，因此可以正确输出结果。




**example5:**


#include<iostream>

using namespace std;

char* test(void)

{
char *p = (char*)malloc(100);
strcpy(p, "hello word");
return p;

}

int main(void)

{
char *str;
str = test();
cout << str << endl;
system("pause");
return 0;

}


运行结果 helloworld 

这种情况下同样可以输出正确的结果，是因为是用malloc在堆上申请的空间，这部分空间是由程序员自己管理的，如果程序员没有手动释放堆区的空间，那么存储单元里的内容是不会被重写的，因此可以正确输出结果。




**example6:**


#include<iostream>

using namespace std;

void test(void)

{
char *p = (char*)malloc(100);
strcpy(p, "hello word");
free(p);
if (p == NULL)
{
strcpy(p, "hello world");
cout << p << endl;
}

}

int main(void)

{
test();
system("pause");
return 0;

}


没有输出

在这里注意了，free()释放的是指针指向的内存！注意！释放的是内存，不是指针！这点非常非常重 要！指针是一个变量，只有程序结束时才被销毁。释放了内存空间后，原来指向这块空间的指针还是存在！只不过现在指针指向的内容的垃圾，是未定义的，所以说是垃圾。因此，释放内存后应把把指针指向NULL，防止指针在后面不小心又被使用，造成无法估计的后果。


## 3.常见内存错误及对策:

![](https://img-blog.csdn.net/20171205111429450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171205111435206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171205111439653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171205111444254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




