# C++的虚函数表 - xcw0754 - 博客园
# [C++的虚函数表](https://www.cnblogs.com/xcw0754/p/4973532.html)
这里的例子全部来自陈皓的[C++ 虚函数表解析](http://blog.csdn.net/haoel/article/details/1948051)，经过修改的。
编译器：g++ (Ubuntu 4.9.2-10ubuntu13) 4.9.2
环境：ubuntu 15.04  64位系统（地址占8字节）
**例子1：**
```
1 #define LL long long
 2 
 3 class Base {
 4 public:
 5     virtual void f() { cout << "Base::f" << endl; }
 6     virtual void g() { cout << "Base::g" << endl; }
 7     virtual void h() { cout << "Base::h" << endl; }
 8     LL a;    
 9 };
10 
11 int main(void)
12 {
13     typedef void (*Fun)(void);
14     Base b;
15     b.a=10086;
16 
17     Fun pFun = NULL;
18 
19     cout << "虚函数表地址：" << (int*)(&b) << endl;
20     cout << "虚函数表 — 第一个函数地址：" << (int*)*(int*)(&b) << endl;
21 
22     LL *p=(LL*)*(LL*)&b;
23     for(int i=0; i<3; i++)
24     {
25         pFun = (Fun)*(p+i);
26         pFun();
27     }
28     cout<<"a的地址： "<<(&b)+1<<endl;
29     cout<<"a的值： "<<*((LL*)(&b)+1)<<endl;
30     cout<<"对象b的大小："<<sizeof(b)<<endl;    
31 
32     return 0;
33 }
```
输出：
![](https://images2015.cnblogs.com/blog/641737/201511/641737-20151118093435671-1295559298.png)
　　解释：通过强转对象b的地址，将地址逐个取出来运行看看是什么。
　　总结：对象b中存储了2个东西，第一个是虚函数表指针，第二个是变量a，所以共计8+8=16字节。在虚函数表指针所指的地址中，有3个指针，分别是3个虚函数的指针，每个占8字节，共计3*8=24字节。陈皓说虚表最后加个标志，这点不知道怎么验证～
　　验证了如下这副图：
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_vtable1.jpg)
**例子2：**
　　一般继承，无函数覆盖的，全部函数都声明为virtual：　　
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_Drawing3.jpg)
```
1 class Base {
 2 public:
 3     virtual void f() { cout << "Base::f" << endl; }
 4     virtual void g() { cout << "Base::g" << endl; }
 5     virtual void h() { cout << "Base::h" << endl; }    
 6 };
 7 
 8 class Derive:public Base{
 9 public:
10     virtual void f1() { cout << "Derive::f1" << endl; }
11     virtual void g1() { cout << "Derive::g1" << endl; }
12     virtual void h1() { cout << "Derive::h1" << endl; }    
13 };
14 
15 int main(void)
16 {
17     typedef void (*Fun)(void);
18     Derive d;
19     Fun pFun = NULL;
20 
21     cout << "虚函数表地址：" << (LL*)(&d) << endl;
22     cout << "虚函数表 — 第一个函数地址：" << (LL*)*(LL*)(&d) << endl;
23 
24     LL *p=(LL*)*(LL*)&d;
25     for(int i=0; i<6; i++)
26     {
27         pFun = (Fun)*(p+i);
28         pFun();
29     }
30     cout<<"对象d的大小："<<sizeof(d)<<endl;    
31 
32     return 0;
33 }
```
输出：
![](https://images2015.cnblogs.com/blog/641737/201511/641737-20151118095102874-1538460952.png)
　　解释：仅仅只有2个类，且Derive继承Base类，一共有6个虚函数。
　　总结：对象d中一共占用8个字节，也就是只保存了虚函数表的地址。虚函数表中一共有6个函数指针，分别是Base的3个+Derive的3个。
　　验证了如下这副图：
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_vtable2.JPG)
**例子3：**
　　一般继承，只有1个函数是覆盖的，全部函数都声明为virtual：
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_Drawing4.jpg)
```
1 class Base {
 2 public:
 3     virtual void f() { cout << "Base::f" << endl; }
 4     virtual void g() { cout << "Base::g" << endl; }
 5     virtual void h() { cout << "Base::h" << endl; }    
 6 };
 7 
 8 class Derive:public Base{
 9 public:
10     virtual void f() { cout << "Derive::f" << endl; }
11     virtual void g1() { cout << "Derive::g1" << endl; }
12     virtual void h1() { cout << "Derive::h1" << endl; }    
13 };
14 
15 int main(void)
16 {
17     typedef void (*Fun)(void);
18     Derive d;
19     Fun pFun = NULL;
20 
21     cout << "虚函数表地址：" << (LL*)(&d) << endl;
22     cout << "虚函数表 — 第一个函数地址：" << (LL*)*(LL*)(&d) << endl;
23 
24     LL *p=(LL*)*(LL*)&d;
25     for(int i=0; i<5; i++)
26     {
27         pFun = (Fun)*(p+i);
28         pFun();
29     }
30 
31     cout<<"对象d的大小："<<sizeof(d)<<endl;    
32      
33     return 0;
34 }
```
输出：
![](https://images2015.cnblogs.com/blog/641737/201511/641737-20151118095842171-465175463.png)
　　解释：这个模型跟上面的差不多，而这次有1个函数是覆盖的。
　　总结：虚函数表中一共有5个函数指针，Base中占2个，Derive中占3个。流程是先将基类的3个函数摆在虚函数表前面，接着摆派生类的，由于Derive的函数f覆盖掉基类Base的函数f，所以直接代替基类的函数f的位置。多态是：用基类的指针指向派生类对象，调用的是有覆盖的派生类中的函数。这里就可以实现多态～
　　验证了如下这副图：
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_vtable3.JPG)
**例子4：**
　　多重继承，无虚函数覆盖，但全部函数都声明为虚函数。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_Drawing1.jpg)
```
1 class Base1 {
 2 public:
 3     virtual void f() { cout << "Base1::f" << endl; }
 4     virtual void g() { cout << "Base1::g" << endl; }
 5     virtual void h() { cout << "Base1::h" << endl; }    
 6 };
 7 class Base2 {
 8 public:
 9     virtual void f() { cout << "Base2::f" << endl; }
10     virtual void g() { cout << "Base2::g" << endl; }
11     virtual void h() { cout << "Base2::h" << endl; }    
12 };
13 class Base3 {
14 public:
15     virtual void f() { cout << "Base3::f" << endl; }
16     virtual void g() { cout << "Base3::g" << endl; }
17     virtual void h() { cout << "Base3::h" << endl; }    
18 };
19 
20 class Derive:public Base1,public Base2,public Base3{
21 public:
22     virtual void f1() { cout << "Derive::f1" << endl; }
23     virtual void g1() { cout << "Derive::g1" << endl; }
24 };
25  
26 int main(void)
27 {
28     typedef void (*Fun)(void);
29     Derive d;
30     Fun pFun = NULL;
31 
32     cout << "虚函数表地址：" << (LL*)(&d) << endl;
33     cout << "虚函数表 — 第一个函数地址：" << (LL*)*(LL*)(&d) << endl;
34 
35     LL *p=(LL*)*(LL*)&d;
36     for(int i=0; i<5; i++)
37     {
38         pFun = (Fun)*(p+i);
39         pFun();
40     }
41     for(int i=0; i<3; i++)
42     {
43         pFun = (Fun)*(p+i);
44         pFun();
45     }
46     for(int i=0; i<3; i++)
47     {
48         pFun = (Fun)*(p+i);
49         pFun();
50     }
51 
52     cout<<"对象d的大小："<<sizeof(d)<<endl;    
53      
54     return 0;
55 }
```
输出：
![](https://images2015.cnblogs.com/blog/641737/201511/641737-20151118101116686-855745135.png)
　　解释：Derive类一共有3个基类，按1,2,3的顺序继承。由于有3个基类，所以对象d中有且仅有3个指针，分别指向其基类的虚函数表。
　　总结：虚函数表1中有5个函数，虚函数表2和3中有3个函数。也就是说，基类Derive中的虚函数是借放在第一个基类的虚函数表中的尾部。
　　验证了如下这副图：
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_vtable4.JPG)
例子5：
　　多重继承，有1个虚函数覆盖，全部函数声明为虚函数。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_Drawing2.jpg)
```
1 class Base1 {
 2 public:
 3     virtual void f() { cout << "Base1::f" << endl; }
 4     virtual void g() { cout << "Base1::g" << endl; }
 5     virtual void h() { cout << "Base1::h" << endl; }    
 6 };
 7 class Base2 {
 8 public:
 9     virtual void f() { cout << "Base2::f" << endl; }
10     virtual void g() { cout << "Base2::g" << endl; }
11     virtual void h() { cout << "Base2::h" << endl; }    
12 };
13 class Base3 {
14 public:
15     virtual void f() { cout << "Base3::f" << endl; }
16     virtual void g() { cout << "Base3::g" << endl; }
17     virtual void h() { cout << "Base3::h" << endl; }    
18 };
19 
20 class Derive:public Base1,public Base2,public Base3{
21 public:
22     virtual void f() { cout << "Derive::f" << endl; }
23     virtual void g1() { cout << "Derive::g1" << endl; }
24 };
25 
26 
27  
28 int main(void)
29 {
30     typedef void (*Fun)(void);
31     Derive d;
32     Fun pFun = NULL;
33 
34     cout << "虚函数表地址：" << (LL*)(&d) << endl;
35     cout << "虚函数表 — 第一个函数地址：" << (LL*)*(LL*)(&d) << endl;
36 
37     LL *p=(LL*)*(LL*)&d;
38     for(int i=0; i<4; i++)
39     {
40         pFun = (Fun)*(p+i);
41         pFun();
42     }
43     for(int i=0; i<3; i++)
44     {
45         pFun = (Fun)*(p+i);
46         pFun();
47     }
48     for(int i=0; i<3; i++)
49     {
50         pFun = (Fun)*(p+i);
51         pFun();
52     }
53 
54     cout<<"对象d的大小："<<sizeof(d)<<endl;    
55      
56     return 0;
57 }
```
输出：
![](https://images2015.cnblogs.com/blog/641737/201511/641737-20151118101920186-1912761136.png)
　　解释：例子基本和上一个例子一样。基类Derive中的虚函数f 覆盖掉3个基类中的同名虚函数f。
　　总结：依然是3个函数表，只是每个函数表中的同名函数f 都被替换成了基类Derive中的函数f 。所以虚函数表1中有4个指针，表2和3都分别有3个。

