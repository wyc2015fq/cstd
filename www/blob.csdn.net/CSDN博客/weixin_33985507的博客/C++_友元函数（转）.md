# C++_友元函数（转） - weixin_33985507的博客 - CSDN博客
2017年06月28日 00:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
1、为什么要引入友元函数：在实现类之间数据共享时，减少系统开销，提高效率
      具体来说：为了使其他类的成员函数直接访问该类的私有变量
      即：允许外面的类或函数去访问类的私有变量和保护变量，从而使两个类共享同一函数
      优点：能够提高效率，表达简单、清晰
      缺点：友元函数破环了封装机制，尽量不使用成员函数，除非不得已的情况下才使用友元函数。
2、什么时候使用友元函数：
      1)运算符重载的某些场合需要使用友元。
      2)两个类要共享数据的时候
3、怎么使用友元函数：
友元函数的参数：
       因为友元函数没有this指针，则参数要有三种情况：
       1、  要访问非static成员时，需要对象做参数；--常用(友元函数常含有参数)
       2、  要访问static成员或全局变量时，则不需要对象做参数
       3、  如果做参数的对象是全局对象，则不需要对象做参数
友元函数的位置：
       因为友元函数是类外的函数，所以它的声明可以放在类的私有段或公有段且没有区别。
友元函数的调用：
       可以直接调用友元函数，不需要通过对象或指针
友元函数的分类：
根据这个函数的来源不同，可以分为三种方法：
1、普通函数友元函数：
       a) 目的：使普通函数能够访问类的友元
       b) 语法：声明位置：公有私有均可，常写为公有
                        声明： friend + 普通函数声明
                        实现位置：可以在类外或类中
                        实现代码：与普通函数相同（不加不用friend和类::）
                        调用：类似普通函数，直接调用
       c) 代码：        
```
1 class INTEGER  
 2 {    
 3 private:  
 4     int num;  
 5 public:  
 6     friend void Print(const INTEGER& obj);//声明友元函数  
 7 };  
 8 void Print(const INTEGER& obj)//不使用friend和类：：  
 9 {  
10     //函数体  
11 }  
12 void main()  
13 {  
14     INTEGER obj;  
15     Print(obj);//直接调用  
16 }
```
2、类Y的所有成员函数都为类X友元函数—友元类
      a）目的：使用单个声明使Y类的所有函数成为类X的友元
                        它提供一种类之间合作的一种方式，使类Y的对象可以具有类X和类Y的功能
                        具体来说：
                                前提：A是B的友元（=》A中成员函数可以访问B中有所有成员，包括私有成员和公有成员--老忘）
                                    则：在A中，借助类B，可以直接使用～B . 私有变量～的形式访问私有变量
      b）语法：声明位置：公有私有均可，常写为私有(把类看成一个变量)
                        声明： friend + 类名---不是对象啊
                        调用：
      c）代码：
```
1 class girl;  
 2   
 3 class boy  
 4 {    
 5 private:  
 6     char *name;    
 7     int age;    
 8 public:    
 9     boy();  
10     void disp(girl &);     
11 };    
12   
13 void boy::disp(girl &x) //函数disp()为类boy的成员函数，也是类girl的友元函数   
14 {   
15     cout<<"boy's name is:"<<name<<",age:"<<age<<endl;//正常情况，boy的成员函数disp中直接访问boy的私有变量  
16     cout<<"girl's name is:"<<x.name<<",age:"<<x.age<<endl;   
17     //借助友元，在boy的成员函数disp中，借助girl的对象，直接访问girl的私有变量  
18     //正常情况下，只允许在girl的成员函数中访问girl的私有变量  
19 }  
20   
21 class girl  
22 {    
23 private：  
24     char *name;    
25     int age;    
26     friend boy;   //声明类boy是类girl的友元    
27 public:    
28     girl();     
29 };    
30 void main()    
31 {     
32     boy b;    
33     girl g;    
34     b.disp(g);  //b调用自己的成员函数，但是以g为参数，友元机制体现在函数disp中  
35 }
```
3、类Y的一个成员函数为类X的友元函数
      a）目的：使类Y的一个成员函数成为类X的友元
             具体而言：而在类Y的这个成员函数中，借助参数X，可以直接以X。私有变量的形式访问私有变量
      b）语法：声明位置：声明在公有中 （本身为函数）
                        声明：friend + 成员函数的声明
                        调用：先定义Y的对象y---使用y调用自己的成员函数---自己的成员函数中使用了友元机制
      c）代码： 
```
1 class girl;   
 2 class boy  
 3 {    
 4 private:  
 5     char *name;    
 6     int age;    
 7 public:    
 8     boy();  
 9     void disp(girl &);       
10 };     
11    
12 class girl  
13 {  
14 private:  
15     char *name;    
16     int age;    
17 public:    
18     girl(char *N,int A);    
19     friend void boy::disp(girl &); //声明类boy的成员函数disp()为类girl的友元函数    
20 };    
21    
22 void boy::disp(girl &x)    
23 {     
24     cout<<"boy's name is:"<<name<<",age:"<<age<<endl;  //访问自己(boy)的对象成员,直接访问自己的私有变量    
25     cout<<"girl's name is:"<<x.name<<",age:"<<x.age<<endl;    
26     //借助友元，在boy的成员函数disp中，借助girl的对象，直接访问girl的私有变量  
27     //正常情况下，只允许在girl的成员函数中访问girl的私有变量    
28 }    
29 void main()    
30 {     
31     boy b();    
32     girl g();    
33     b.disp(g);  }
```
4、在模板类中使用友元operator<<(对<<运算符的重载)
    a)使用方法：
在模板类中声明：
```
friend ostream& operator<< <>(ostream& cout,const MGraph<VexType,ArcType>& G);
```
在模板类中定义：
```
template<class VexType,class ArcType>  
ostream& operator<<(ostream& cout,const MGraph<VexType,ArcType>& G)  
{  
    //函数定义  
}
```
  b)注意：
把函数声明非模板函数：
```
friend ostream& operator<< (ostream& cout,const MGraph& G);
```
把函数声明为模板函数：
```
friend ostream& operator<< <>(ostream& cout,const MGraph<VexType,ArcType>& G);
```
或：
```
friend ostream& operator<< <VexType,ArcType>(ostream& cout,const MGraph<VexType,ArcType>& G);
```
说明：
 在函数声明中加入operator<< <>：是将operator<<函数定义为函数模板，将函数模板申明为类模板的友员时,是一对一绑定的
 实际的声明函数：这里模板参数可以省略，但是尖括号不可以省略
```
friend ostream& operator<< <VexType,ArcType>(ostream& cout,const MGraph<VexType,ArcType>& G);
```
5、友元函数和类的成员函数的区别：成员函数有this指针，而友元函数没有this指针。
6、记忆：A是B的友元《=》A是B的朋友《=》借助B的对象，在A中可以直接 通过B。成员变量（可以是公有，也可以为私有变量） 的方式访问B 
转自：[http://blog.csdn.net/insistgogo/article/details/6608672](http://blog.csdn.net/insistgogo/article/details/6608672)
