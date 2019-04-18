# interface——关于虚函数的内存分配 - TaigaComplex求职中 - 博客园







# [interface——关于虚函数的内存分配](https://www.cnblogs.com/TaigaCon/archive/2012/07/14/2591039.html)





之前学C++的时候了解到，其中很多控件是基于com的，今天初学接口，写了一小段测试程序

**接口为IX，IY**

**组件为CA，（继承于IX和IY）**

**客户为main函数**



流程如下：



![](https://pic002.cnblogs.com/images/2012/421096/2012071322460018.jpg)



接口连接组件的功能是由虚函数实现的

一般来说，父类作为接口，子类作为组件，子类实现父类的功能（组件实现接口所要求的功能）

虚函数就是实现这一过程的方法

对于虚函数在内存中的分配，下面是个小测试程序：

```
1 #include<iostream>
 2 using namespace std;
 3 class CA
 4 {
 5 public:
 6     int i;
 7     virtual void __stdcall virtualFun()=0;
 8 };
 9 
10 class CASub:public CA
11 {
12     virtual void __stdcall virtualFun()
13     {
14         return;
15     }
16 };
17 
18 class CB
19 {
20 public:
21     int i;
22 };
23 
24 void main()
25 {
26     CASub *pA = new CASub;
27     CB *pB = new CB;
28     cout<<"pA Address:"<<pA<<endl;
29     cout<<"pA int address"<<(&pA->i)<<endl;
30     cout<<"Size of pA:"<<sizeof(*pA)<<endl<<endl;
31     cout<<"pB Address:"<<pB<<endl;
32     cout<<"pB int Address:"<<&(pB->i)<<endl;
33     cout<<"Size of pB:"<<sizeof(*pB)<<endl;
34 }
```

运行结果如下图

![](https://pic002.cnblogs.com/images/2012/421096/2012071323361316.jpg)

由此可见pA（由CASub产生，CASub又是由CA继承而来）的长度为8个字节，pA中有虚函数和int类型

pB（由CB产生）的长度为4个字节，其中pB中只有int类型，因此知道虚函数占用的是4个字节，

不过虚函数中并没有数据，因此该4个字节应该是虚函数的地址，

另外，程序中虚函数是写在int后的，而到了内存中虚函数却在前面，由此可以判定是编译器故意为之，为了提高虚函数调用的速度。



然后我们来看一下接口地址和组件地址的关系，

组件是CA，接口为IX和IY，

测试代码如下

```
1 #include<iostream>
 2 #include<ObjBase.h>
 3 using namespace std;
 4 void trace(const char* pMsg)
 5 {
 6     cout<<pMsg<<endl;
 7 }
 8 
 9 interface IX
10 {
11     virtual void __stdcall Fx1()=0;
12     virtual void __stdcall Fx2()=0;
13 
14 };
15 
16 interface IY
17 {
18     virtual void __stdcall Fy1()=0;
19     virtual void __stdcall Fy2()=0;
20 };
21 
22 class CA : public IX,
23            public IY
24 {
25 public :
26     virtual void __stdcall Fx1(){cout<<"CA::Fx1"<<endl;}
27     virtual void __stdcall Fx2(){cout<<"CA::Fx2"<<endl;}
28 
29     virtual void __stdcall Fy1(){cout<<"CA::Fy1"<<endl;}
30     virtual void __stdcall Fy2(){cout<<"CA::Fy2"<<endl;}
31 };
32 
33 void main()
34 {
35 
36 
37     trace("Client:Create an instance of the component.");
38     CA* pA =new CA;
39 
40     IX* pIX=pA;
41     trace("Client:Use the IX interface.");
42     pIX->Fx1();
43     pIX->Fx2();
44 
45     IY* pIY=pA;
46     trace("Client:Use the IY interface.");
47     pIY->Fy1();
48     pIY->Fy2();
49 
50     
51     cout<<endl;
52     cout<<"pA Address:"<<pA<<endl;
53     cout<<"pIX Address:"<<pIX<<endl;
54     cout<<"pIY Address:"<<pIY<<endl;
55     cout<<"Size of pA:"<<sizeof(*pA)<<endl;
56     cout<<"Size of pIX:"<<sizeof(*pIX)<<endl;
57     cout<<"Size of pIY:"<<sizeof(*pIY)<<endl;
58     
59     trace("Client :Delete the component");
60     delete pA;
61     return ;
62 }
```

运行结果

![](https://pic002.cnblogs.com/images/2012/421096/2012071400231894.jpg)

CA继承于IX和IY，由运行结果可以看出pA地址跟pIX一致，这也是继承的特性，构造CA的时候会先构造父类IX，然后构造IY；

组件CA在内存中的大小为8字节，接口各4个字节，而接口作为内部有虚函数的类，虚函数地址就占了4个字节，并且每个接口有两个函数，



** 据书上的说法，虚函数的实现是，内存中首个4BYTE包含的是指向虚函数表的指针，然后再在表中包含指向实现该虚函数的实际内存地址，如下图：**

![](https://pic002.cnblogs.com/images/2012/421096/2012071920402686.jpg)

** 注：接口的内存结构对于不同的操作系统可能是不同的，例如在mac上，vtbl指针将指向一个伪指针，而第一个函数指针则位于第二个表项中**



**假设子类不含有虚函数Fx1，那么就会把父类的虚函数的入口地址写到子类虚函数表的Fx1处，如果子类存在虚函数Fx1，那么该处就填子类Fx1的入口地址，**

**对于所有不含有Fx1的子类，而父类有Fx1，那么所有子类将共用该函数的代码段**



在学习过程中，本人发现了类中的静态数据类型（static）必须在外部重新定义一遍

如：

class A{public: static int i;};

手动定义一遍，int A::i=0;这个时候才会进行 i 的内存分配

另外还有一个问题是上面的这条int A::i=0；无法放在main 函数中，原因估计是作为类中静态变量，同时也是全局变量，必须时刻都在内存中

由于水平有限，上面部分为推测，如有错误欢迎留言纠正












