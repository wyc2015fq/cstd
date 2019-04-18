# C++ 类成员函数继承（virtual、非virtual） - z69183787的专栏 - CSDN博客
2012年11月15日 16:09:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5793
**类继承**
★ 对于父类函数（virtual、非virtual），如果子类没有同名函数，则正常继承
★ 对于父类函数（virtual、非virtual），如果子类有同名函数，无同型函数，则不能调用父类函数
★ 对于父类函数（virtual、非virtual），如果有同型函数：
----非virtual函数由指针类型决定调用哪个
----virtual函数由指针指向的对象决定调用哪个（运行时决定）
 1![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//如果对于父类函数（virtual/非virtual），如果子类没有同名函数，则正常继承
 2![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 3![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)class Base
 4![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 5![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
 7![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)public:  void func(int i){ cout <<"Base::func(int)"<< endl; }
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
 9![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}; 
10![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
11![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
12![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
13![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)class Derived : public Base
14![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
15![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){ }; 
16![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
17![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
18![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
19![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main()
20![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
21![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
22![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
23![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         Base *pb = new Derived();
24![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
25![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pb->func(1); //Base::func(int)
26![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
27![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         delete pb; 
28![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
29![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
30![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
31![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         Derived *pd = new Derived();
32![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
33![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pd->func(1); //Base::func(int)
34![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
35![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         delete pd;         
36![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
37![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
 1![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//对于父类函数（virtual、非virutal），子类有同名函数，无同型函数，则不能调用父类函数
 2![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 3![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)class Base
 4![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 5![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)public:
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
 9![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)void func(int i){ cout <<"Base::func(int i)"<< endl; }
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
11![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)virtualvoid func2(int i) { cout << "Base::func2(int i)" << endl;}
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
13![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}; 
14![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
15![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
16![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
17![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)class Derived : public Base
18![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
19![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
20![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
21![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)public:      
22![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
23![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)void func(){ cout <<"Derived::func()"<< endl; }
24![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
25![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)void func2(){ cout <<"Derived::func2()"<< endl; }
26![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
27![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}; 
28![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
29![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
30![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
31![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main()
32![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
33![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
34![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
35![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         Base *pb = new Derived();
36![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
37![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pb->func(1); //Base::func(int)
38![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
39![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pb->func2(1); //Base::func2(int i)
40![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
41![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         delete pb; 
42![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
43![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
44![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
45![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         Derived *pd = new Derived();
46![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
47![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pd->func(); //Derived::func()
48![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
49![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pd->func2(); //Derived::func2()
50![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
51![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// pd->func2(1); //不能调用 
52![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
53![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         delete pd;       
54![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
55![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
 1![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//对于父类函数（virtual、非virtual），如果有同型函数：
 2![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 3![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//----非virtual函数由指针类型决定调用哪个
 4![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 5![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//----virtual函数由指针指向的对象决定调用哪个（运行时决定）
 6![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 7![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)class Base
 8![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 9![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){  public:
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
11![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)void func(int i){ cout <<"Base::func(int i)"<< endl; }
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
13![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)void func() {cout << "Base::func() " << endl;}
14![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
15![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)virtualvoid func2(int i) { cout << "Base::func2(int i)" << endl;}
16![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
17![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}; 
18![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
19![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
20![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
21![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)class Derived : public Base
22![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
23![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){  public:      
24![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
25![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)void func(int i){ cout <<"Derived::func()"<< endl; }
26![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
27![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)void func2(int i){ cout <<"Derived::func2(int i)"<< endl; }
28![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
29![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}; 
30![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
31![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
32![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
33![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main()
34![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
35![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
36![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
37![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         Base *pb = new Derived();
38![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
39![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pb->func(1);  //Base::func(int i)
40![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
41![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pb->func();  //Base:func()
42![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
43![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pb->func2(1);  //Derived::func2(int i)
44![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
45![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         delete pb; 
46![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
47![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
48![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
49![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         Derived *pd = new Derived();
50![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
51![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pd->func(1); //Derived::func(int i)
52![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
53![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// pd->func(); //不能调用 
54![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
55![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         pd->func2(1); //Derived::func2(int i)
56![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
57![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         delete pd;
58![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
59![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
