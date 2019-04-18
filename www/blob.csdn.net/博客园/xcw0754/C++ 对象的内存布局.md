# C++ 对象的内存布局 - xcw0754 - 博客园
# [C++ 对象的内存布局](https://www.cnblogs.com/xcw0754/p/4973978.html)
这里的例子全部来自陈皓的  [C++ 对象的内存布局（上）](http://blog.csdn.net/haoel/article/details/3081328)，经过修改的。而[C++ 对象的内存布局（下）](http://blog.csdn.net/haoel/article/details/3081385)看了没有什么问题的样子，没有时间测试了。
编译器：g++ (Ubuntu 4.9.2-10ubuntu13) 4.9.2
环境：ubuntu 15.04  64位系统（地址占8字节）
**例子1：**
　　单一的一般继承，所有函数均是virtual，每个类中均有1个long long变量：
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/EntryImages/20081015/dd01.jpg)
```
1 class Parent {
 2 public:
 3     LL iparent;
 4     Parent ():iparent (10) {}
 5     virtual void f() { cout << " 1 " << endl; }
 6     virtual void g() { cout << " 2 " << endl; }
 7     virtual void h() { cout << " 3 " << endl; }
 8  
 9 };
10  
11 class Child : public Parent {
12 public:
13     LL ichild;
14     Child():ichild(100) {}
15     virtual void f() { cout << " 11 " << endl; }
16     virtual void g_child() { cout << " 4 " << endl; }
17     virtual void h_child() { cout << " 5 " << endl; }
18 };
19  
20 class GrandChild : public Child{
21 public:
22     LL igrandchild;
23     GrandChild():igrandchild(1000) {}
24     virtual void f() { cout << " 111 " << endl; }
25     virtual void g_child() { cout << " 44 " << endl; }
26     virtual void h_grandchild() { cout << " 6 " << endl; }
27 };
28 int main(void)
29 {
30     typedef void(*Fun)(void);
31     GrandChild gc;
32  
33     LL** pVtab = (LL**)&gc;
34     Fun pFun=NULL;
35 
36     cout << "[0] GrandChild::_vptr->" << endl;
37     for (int i=0; i<6; i++){
38         pFun = (Fun)pVtab[0][i];
39         cout << "    ["<<i<<"] ";
40         pFun();
41     }
42     LL *p=(LL*)&gc;
43     cout << "[1] Parent.iparent = " <<*(p+1)<< endl;
44     cout << "[2] Child.ichild = " << *(p+2)<< endl;
45     cout << "[3] GrandChild.igrandchild = " << *(p+3)<< endl;
46     
47     cout<<"对象gc的大小："<<sizeof(gc)<<endl;    
48  
49     return 0;
50 }
```
输出：
![](https://images2015.cnblogs.com/blog/641737/201511/641737-20151118112115280-362876160.png)、
　　解释：继承关系为 Parent<--Child<--GrandChild，其中Child是中间类，覆盖了Parent中一个函数f。GrandChild是最底派生类，覆盖了Parent和Child的函数f，再覆盖Child中的函数g_child。
　　总结：GrandChild的对象gc一共占4*8=32个字节。其中首项是虚函数表指针，剩下的3项是3个类中的成员变量，按继承的声明顺序排列。虚函数表中有6个函数，其中来自GrandChild的有3个，Child的1个，Parent的2个。GrandChild的3个必定不会少，而且前两个函数f和g_child就覆盖掉所有基类中同名的函数。Parent中剩下2个没有被覆盖的，而f不幸被覆盖。Child中只剩下1个了，前两个都被覆盖了。
　　其实可以先看两层的Parent<--Child，而GrandChild先不看。那么派生类覆盖掉基类1个函数，剩下的就是2+3=5个函数。来了新的基类GrandChild后，也新来3个函数，但是其中2个已经存在，先覆盖，剩下一个直接加进去，即5+1=6个函数。
　　验证了如下这副图：
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/EntryImages/20081015/dd02.jpg)
**例子2：**
　　多重继承，基类与派生类中有同名虚函数，每个类中有自己的1个变量。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/EntryImages/20081015/dd03.jpg)
```
1 class Base1 {
 2 public:
 3     LL ibase1;
 4     Base1():ibase1(10) {}
 5     virtual void f() { cout << "Base1::f()" << endl; }
 6     virtual void g() { cout << "Base1::g()" << endl; }
 7     virtual void h() { cout << "Base1::h()" << endl; }
 8  
 9 };
10  
11 class Base2 {
12 public:
13     LL ibase2;
14     Base2():ibase2(20) {}
15     virtual void f() { cout << "Base2::f()" << endl; }
16     virtual void g() { cout << "Base2::g()" << endl; }
17     virtual void h() { cout << "Base2::h()" << endl; }
18 };
19  
20 class Base3 {
21 public:
22     LL ibase3;
23     Base3():ibase3(30) {}
24     virtual void f() { cout << "Base3::f()" << endl; }
25     virtual void g() { cout << "Base3::g()" << endl; }
26     virtual void h() { cout << "Base3::h()" << endl; }
27 };
28  
29 class Derive : public Base1, public Base2, public Base3 {
30 public:
31     LL iderive;
32     Derive():iderive(100) {}
33     virtual void f() { cout << "Derive::f()" << endl; }
34     virtual void g1() { cout << "Derive::g1()" << endl; }
35 };
36  
37 int main(void)
38 {
39     typedef void(*Fun)(void);
40     Derive d;
41     Fun pFun=NULL;
42     LL** pVtab = (LL**)&d;
43 
44     cout << "[0] Base1::_vptr->" << endl;
45     for(int i=0; i<4; i++)
46     {
47         pFun = (Fun)pVtab[0][i];
48         cout << "     ["<<i<<"] ";
49         pFun();
50     }
51     pFun = (Fun)pVtab[0][4];
52     cout << "     [4] "<<pFun<<endl;
53     cout << "[1] Base1.ibase1 = " << (LL)pVtab[1] << endl;
54 
55 
56     cout << "[4] Base3::_vptr->" << endl;
57     for(int i=0; i<3; i++)
58     {
59         pFun = (Fun)pVtab[4][i];
60         cout << "     ["<<i<<"] ";
61         pFun();
62     }
63     pFun = (Fun)pVtab[4][4];
64     cout << "     [4] "<<pFun<<endl;
65     cout << "[5] Base3.ibase3 = " << (LL)pVtab[5] << endl;
66 
67     return 0;
68 }
```
输出：
![](https://images2015.cnblogs.com/blog/641737/201511/641737-20151118125746327-78629604.png)
　　解释：代码中只测试了派生类对象d的4*8=32个字节的内容（不连续的）。其他的推测一下就行了。**对于[0][4]和[2][4]其实是一个标志，1表示后面没有指针了，是编译器做的手脚（上一篇中讲了无法验证，在这验证了）。**
　　总结：对象d中分4个部分。
　　第一部分是基类Base1的，共2*8=16个字节，前8个字节是虚函数表的指针，后8个字节是变量ibase1的值。
　　第二部分是基类Base2的，共2*8=16个字节，前8个字节是虚函数表的指针，后8个字节是变量ibase2的值。
　　第三部分是基类Base3的，共2*8=16个字节，前8个字节是虚函数表的指针，后8个字节是变量ibase3的值。
　　第四部分是派生类Derive的，共1*8=8个字节，这8个字节保存着变量iderive的值。
　　对象d合计3*16+8=56个字节。
　　同预计一样，派生类Derive中的虚函数依然保存在和基类Base1在一起，紧跟在其后面。而派生类Derive中的虚函数f 分别覆盖掉3个基类中的虚函数f ，并且存在于他们各自的虚函数表中。
　　验证了如下这副图（NULL就是那个标志1）：
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/EntryImages/20081015/dd04.jpg)

