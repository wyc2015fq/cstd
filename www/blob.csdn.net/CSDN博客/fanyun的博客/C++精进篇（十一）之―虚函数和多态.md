# C++精进篇（十一）之―虚函数和多态 - fanyun的博客 - CSDN博客
2016年04月05日 09:38:24[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：883标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**C++虚函数与多态:**
**1.C++中的虚函数**
C++中的虚函数的作用主要是实现了多态的机制。关于多态，简而言之就是用父类型别的指针指向其子类的实例，然后通过父类的指针调用实际子类的成员函数。这种技术可以让父类的指针有“多种形态”，这是一种泛型技术。所谓泛型技术，说白了就是试图使用不变的代码来实现可变的算法。比如：模板技术，RTTI技术，虚函数技术，要么是试图做到在编译时决议，要么试图做到运行时决议。
对C++ 了解的人都应该知道虚函数（Virtual Function）是通过一张虚函数表（Virtual Table）和一个指向虚函数表的指针（vptr）来实现的。虚函数表，简称为vtbl，虚函数表表对实现多态起着至关重要的作用。在这个表中，主要保存了一个类中的虚函数的地址，这张表解决了继承、覆盖的问题，保证其内容能真实反应实际的函数。每一个包含虚函数的类的实例都包含一个cptr指针，指向虚函数表的首地址。我们可以通过这个指针找到要访问的虚函数的，完成虚函数的调用主要包括：找到虚函数表的首地址（vptr），通过cptr找到要使用虚函数地址，调用虚函数。那么使用虚函数大家总要考虑效率的问题，实际上为了提高效率，C++的编译器是保证虚函数表的指针存在于对象实例中最前面的位置，这是为了保证取到虚函数表的有最高的性能，这意味着我们通过对象实例的地址得到这张虚函数表，然后通过遍历表就可以找到其中的虚函数的地址，然后调用相应的函数。不妨看看下面的代码：
1 #include <iostream>
 2
 3 using namespacestd;
 4
 5 class Base
 6 {
 7 public:
 8     virtual void f() { cout << "Base::f"<< endl; }
 9     virtual void g() { cout << "Base::g"<< endl; }
10     virtual voidh() { cout << "Base::h" << endl; }
11 };
12
13 typedef void(*Fun)(void);
14
15
16 int main()
17 {
18     Base b;
19     Fun pFun =NULL;
20     cout<< "虚函数表地址：" << (int*)(&b)<< endl;
21     cout <<"虚函数表 —第一个函数地址："<< (int*)*(int*)(&b) << endl;
22     pFun =(Fun)*((int*)*(int*)(&b));
23     pFun();
24     return 0;
25 }
通过上例可以看到，通过强行把&b转成int *，取得虚函数表的地址（vptr），然后，再次取址就可以得到第一个虚函数的地址了，也就是Base::f()，这在上面的程序中得到了验证（把int* 强制转成了函数指针）。通过这个示例，我们就可以知道如果要调用Base::g()和Base::h()，其代码如下：
1 (Fun)*((int*)*(int*)(&b)+0); // Base::f()
2. (Fun)*((int*)*(int*)(&b)+1); // Base::g()
3. (Fun)*((int*)*(int*)(&b)+2); // Base::h()
可以看看虚函数表的图是怎么画的：
大家都知道，多态是通过继承实现的，那么我们要说说虚函数继承的问题。继承就涉及到了虚函数的覆盖了，实际上不被覆盖的虚函数和多态又有什么联系呢？这里我们讨论有覆盖的虚函数表是什么样的，假设存在下面的继承关系：
看看虚函数表示什么样的：
可以发现，Base：：f（）被覆盖了，这样若把Derive的实例赋值给一个基类Base指针pBase，通过pBase->f()；则访问的是子类中的f（）也就是完成了多态。那么虚函数表中的内容到底是怎么样的呢？可以看看下面的四句话就会明白！
1.虚函数按照其声明顺序放于表中。
2.父类的虚函数在子类的虚函数前面。
3.覆盖的f()函数被放到了虚表中原来父类虚函数的位置。
4.没有被覆盖的函数依旧。
**2.用虚函数实现多态**
看看下面的多态的代码：
1 #include <iostream>
 2
 3 using namespacestd;
 4
 5 class Base
 6 {
 7 public:
 8     virtual void Print()
 9     {
10        cout<<"Base::Print()"<<endl;
11     }
12 };
13 class Derive : public Base
14 {
15 public:
16     virtual voidPrint()
17     {
18        cout<<"Derive::Print()"<<endl;
19     }
20 };
21 int main()
22 {
23     Derivederive;
24     Base *pBase= &derive;
25    pBase->Print();
26     return 0;
27 }
28 //多态代码
