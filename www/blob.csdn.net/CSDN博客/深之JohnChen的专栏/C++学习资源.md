# C++学习资源 - 深之JohnChen的专栏 - CSDN博客

2005年11月02日 10:08:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3490标签：[c++																[编译器																[delete																[cache																[存储																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=cache&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C/C++](https://blog.csdn.net/byxdaz/article/category/284144)


**1.****结构大小**


int *pi = new int[12];中的pi纯粹是个指针变量，它就是一个指针，在32位环境下占4个字节；而int ia[]={0, 1, 2};中的ia是个数组，虽然它的值是第一个元素的地址，但是它代表了整个这一个数组，数组中有三个元素，每个int类型的元素占4 个字节，共是12个字节,pi申请完内存后，是整个内存块的首地址，*pi代表的就是第一个元素，每个元素为int类型的，当然为4了，如果为char *pi = new char[12];

cout<<"*pi:"<<sizeof(*pi)//结果应该是1

**2.****指针与引用**


引用是一种没有指针语法的指针.与指针一样,引用提供对对象的间接访问.


传指针时，我们可以通过指针来修改它在外部所指向的内容。但如果要修改外部指针所指向的对象是不可能的。例如传递外部指针到函数内来分配空间，必须传递指针的指针或指针的引用。

int x = 0;

int &a(int &i)

{

i = - 1;

return x ;

}

main()

{

int j=10;

a(j)=100;

//这时候j = -1,x = 100.了

奇怪的宏定义

(1)#define for if(0); else for 

如果加上标题的那句,那么就可以了,就是让i作用域局限在else中.

for(int i=0;i<90;i++)

{

...;

}

for(int i=0;i<90;i++)

{

...;

}//vc 就不会报错



**3. ****引用与指针的比较：**


(１）引用在创建的同时必须初始化，即引用到一个有效的对象；而指针在定义的时候不必初始化，可以在定义后面的任何地方重新赋值．

（２）不存在ＮＵＬＬ引用，引用必须与合法的存储单元关联;而指针则可以是NULL.

（３）引用一旦被初始化为指向一个对象，它就不能被改变不另一个对象的引用；而指针在任何时候都可以改变为指向另一个对象．给引用赋值并不是改变它和原始对象的绑定关系．

（４）引用的创建和销毁并不会调用类的拷贝构造函数

（５）在语言层面，引用的用法和对象一样；在二进制层面，引用一般都是通过指针来实现的，只不过编译器帮我们完成了转换．

总的来说：引用既具有指针的效率，又具有变量使用的方便性和直观性．

１值传递

void Func(int x)

{

x=x+10;//修改的是n在堆栈中的拷贝x

}

int n=0;

Func(n);

cout<<"n="<<n<<endl;//n=0

２指针传递

void Func2(int *x)

{

(*x)=(*x)+10;//修改指针x指向的内存单元的值

}

int n=0;

Func(&n);

cout<<"n="<<n<<endl;//n=10

３．引用传递

void Func3(int &x)

{

x=x+10;//修改的是x引用到的对象n

}

int  n=0;

Func3(n);

cout<<"n="<<n<<endl;//n=10



**4. ****函数指针**


在注册一个回调函数时候,我们常常使用函数指针.c++/c的连接器在连接程序的时候必须把函数的调用语句上,因此函数地址必须在编译时就确定下来.也就是编译器为函数生成代码的时候.

typedef int(* FuncPtr)(const char*) 定义一个函数指针的类型

类的成员函数有4种类型：inline，virtual,static,normal.inline函数在运行会展开，虽然语言允许取其地址，但是没有太大的意义．virtual成员函数的地址指的是其在vtable中的位置;static成员函数的地址和普通全局函数的地址没有任何区别；普通成员函数的地址和一般函数的地址也没有区别，就是函数代码在内存中的真实地址，但是由于它的调用要绑定到一个实在实在在的对象身上，因此无论是其函数指针的声明方式还是其地址的获取方式都比较特别．

#include<iostream.h>
class CTest
{
public:
 void f(void)            //普通成员函数
 {
  cout<<"CTest::f()"<<endl;
 }
 static void g(void)     //静态成员函数
 {
  cout<<"CTest::g()"<<endl;

 }
 virtual void h(void)    //虚拟的成员函数
 {
  cout<<"CTest::h()"<<endl;
 }
private:
};

void main()
{
 typedef void (*GFPtr)(void);//定义一个全局函数指针类型
 GFPtr fp=CTest::g;//取静态成员函数地址的方法和取一个全局函数的地址相似
 fp();//通过函数指针调用类静态成员函数
 typedef void (CTest::*MemFuncPtr)(void);//声明类成员函数指针类型
 MemFuncPtr mfp_1=&CTest::f;//声明成员函数指针变量并初始化
 MemFuncPtr mfp_2=&CTest::h;//注意获得成员函数地址的方法
    CTest theObj;
 (theObj.*mfp_1)();//使用对象和成员函数指针调用成员函数
 (theObj.*mfp_2)();
    CTest *pTest=&theObj;//使用对象指针和成员函数指针调用成员函数
 (pTest->*mfp_1)();
 (pTest->*mfp_2)();

}
实际上，任何成员函数的代码体都是独立于类的对象而存在的，只是非静态成员函数在调用的时候需要与具体的对象建立绑定关系而已（即this指针），c++/c编译器最终把所有的成员函数经过Name-Mangling的处理后转换全局函数，并且增加一个入参this作为第一个参数，供所属类的所有对象共享．因此成员函数的地址实际上就是这些全局函数的地址．



**5. ****内存管理**


内存分配的方式：

（１）从静态存储区域分配．内存在程序的时候就已经分配好了（即已经编址），这些内存在程序的整个运行期间都存在．例如全局变量,static变量度等．

（２）在堆栈上创建．在函数执行期间，函数内局部变量（包括形参）的存储单元都创建在堆栈上，函数结束时这些存储单元自动释放（堆栈清栈）．堆栈内存分配运算内置于处理器的指令集中，效率很高，并且不存在失败的危险，但是分配的内存容量有限．

（３）从堆上分配，亦称动态内存分配．程序在运行期间用malloc 或new 申请任意数量的内存，程序员自己掌握释放内存的恰当时机（使用free或delete).动态内存的生存期由程序员决定，使用非常灵活，但也最容易产生问题．

有了malloc/free为什么还要不得new/delete？

malloc与free是c++/c 语言的标准库函数，new/delete是c++的运算符，它们都是用申请和释放动态内存．

对于非内部的数据类型（如ADT/UDT)的对象而言，光用malloc/free无法满足动态对象的要求：对象在创建的同时要自动调用构造函数，对象在销毁的时候要自动调用析构函数．由于malloc/free是库函数，不在编译器控制范围内．

malloc/free的使用要点：

函数malloc的原型如下：

void *malloc(size_t size);

用malloc申请一块长度为length的整型数组的内存，程序如下：

int *p=(int*)malloc(length);

函数free的原型如下一步:

void free(void* memblock);

如果p==NULL,free对p的操作无论多次都不会出现问题．

new/delete的使用要点：

int *p1=(int*)malloc(sizeof(int)*lengyh);

int* p2=new int[length];

note:如果用new 创建对象数组，那么只能使用对象的默认构造函数，不能使用带参数的构造函数．

在使用不delete释放对象数组时，留意不要丢了符号＂［］＂

对于内部数据类型（int,float,double）的动态数组p而言，delete p与delete[]p是等价的．

全写法Cache（Write through)

每次写入Cache时同时写入内存，使内存和Ｃache始终保持一致．

写回法Cache（Write back）

当每一次写入时，只对Cache写入，如果已经写入过的Cache需要再写入时，就要把它的内容写入内存中










