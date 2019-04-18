# C++中指针和引用的区别 - 心纯净，行致远 - CSDN博客





2018年12月11日 15:22:43[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：54
个人分类：[【C++】](https://blog.csdn.net/zhanshen112/article/category/7639351)









1.指针和引用的定义和性质区别：

（1）指针：指针是一个变量，只不过这个变量存储的是一个地址，指向内存的一个存储单元；而引用跟原来

的变量实质上是同一个东西，只不过是原变量的一个别名而已。如：

int a=1;int *p=&a;

int a=1;int &b=a;

上面定义了一个整形变量和一个指针变量p，该指针变量指向a的存储单元，即p的值是a存储单元的地址。

而下面2句定义了一个整形变量a和这个整形a的引用b，事实上a和b是同一个东西，在内存占有同一个存储单

元。

(2)引用不可以为空，当被创建的时候，必须初始化，而指针可以是空值，可以在任何时候被初始化。

(3)可以有const指针，但是没有const引用；

(4)指针可以有多级，但是引用只能是一级（int **p；合法 而 int &&a是不合法的）

(5)指针的值可以为空，但是引用的值不能为NULL，并且引用在定义的时候必须初始化；

(6)指针的值在初始化后可以改变，即指向其它的存储单元，而引用在进行初始化后就不会再改变了。

(7)”sizeof引用”得到的是所指向的变量(对象)的大小，而”sizeof指针”得到的是指针本身的大小；

(8)指针和引用的自增(++)运算意义不一样；

(9)如果返回动态内存分配的对象或者内存，必须使用指针，引用可能引起内存泄漏；

2.指针和引用作为函数参数进行传递时的区别。

(1)指针作为参数进行传递：

```cpp
#include<iostream>
#include<stdlib.h>
using namespace std;


void swap_int(int *a,int *b)
{
    int *temp=a;
    a=b;
    b=temp;
}

void test(int *p)
{
    int a=1;
    p=&a;
    cout<<p<<" "<<*p<<endl<<endl;;
}

int main(void)
{
    int a=1,b=2;
    int *p=NULL;
    swap_int(&a,&b);
    cout<<a<<" "<<b<<endl<<endl;
    test(p);
    if(p==NULL)
    cout<<"指针p为NULL"<<endl<<endl;
    system("pause");
}
```

运行后的结果如下：

这里写图片描述

![](https://img-blog.csdnimg.cn/20181211152020348.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

结果为2 1；

swap_int函数使用指针传递参数，可以实现对实参进行改变的目的，是因为传递过来的是实参的地址，因此

使用*a实际上是取存储实参的内存单元里的数据，即是对实参进行改变，因此可以达到目的。调用test函数

运行结果为：

0x6afecc 1

指针p为NULL

在main函数中声明了一个指针p，并赋值为NULL，当调用test函数时，事实上传递的也是地址，只不过传递

的是指地址。也就是说将指针作为参数进行传递时，事实上也是值传递，只不过传递的是地址。当把指针作

为参数进行传递时，也是将实参的一个拷贝传递给形参，即上面程序main函数中的p何test函数中使用的p不

是同一个变量，存储2个变量p的单元也不相同（只是2个p指向同一个存储单元），那么在test函数中对p进

行修改，并不会影响到main函数中的p的值。如果要想达到也同时修改的目的的话，就得使用引用了。

（2）将引用作为函数的参数进行传递。

在讲引用作为函数参数进行传递时，实质上传递的是实参本身，即传递进来的不是实参的一个拷贝，因此对

形参的修改其实是对实参的修改，所以在用引用进行参数传递时，不仅节约时间，而且可以节约空间。

```cpp
#include<iostream>
#include<stdlib.h>
using namespace std;

void test(int &a)
{
    cout<<&a<<" "<<a<<endl<<endl;
}

int main(void)
{
    int a=1;
    cout<<&a<<" "<<a<<endl<<endl;
    test(a);
    system("pause");
}
```

运行后的结果如下：

这里写图片描述

![](https://img-blog.csdnimg.cn/20181211152104208.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

所以在引用进行参数传递时，事实上传递的是实参本身而不是拷贝副本。所以在上述要达到同时修改指针的

目的的话，就得使用引用了。

```cpp
#include<iostream>
#include<stdlib.h>
using namespace std;

void test(int *&p)
{
    int a=1;
    p=&a;
    cout<<p<<" "<<*p<<endl<<endl;
}

int main(void)
{
    int *p=NULL;
    test(p);
    if(p!=NULL)
    cout<<"指针p不为NULL"<<endl<<endl;
    cout<<p<<" "<<*p<<endl<<endl;
    system("pause");
}
```

运行后的结果如下：

这里写图片描述

![](https://img-blog.csdnimg.cn/20181211152143370.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

---------------------

作者：给我一个和尚

来源：CSDN

原文：https://blog.csdn.net/qq_27678917/article/details/70224813

版权声明：本文为博主原创文章，转载请附上博文链接！



