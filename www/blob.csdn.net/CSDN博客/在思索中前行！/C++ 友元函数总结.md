# C++  友元函数总结 - 在思索中前行！ - CSDN博客





2015年03月30日 21:10:02[_Tham](https://me.csdn.net/txl16211)阅读数：532标签：[C++ 友元函数																[友元函数](https://so.csdn.net/so/search/s.do?q=友元函数&t=blog)](https://so.csdn.net/so/search/s.do?q=C++ 友元函数&t=blog)
个人分类：[C/C++](https://blog.csdn.net/txl16211/article/category/2284665)







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

       1、  要访问非static成员时，需要对象做参数；--常用(友元函数常含有参数)

       2、  要访问static成员或全局变量时，则不需要对象做参数

       3、  如果做参数的对象是全局对象，则不需要对象做参数

友元函数的位置：

       因为友元函数是类外的函数，所以它的声明可以放在类的私有段或公有段且没有区别。

友元函数的调用：

可以直接调用友元函数，不需要通过对象或指针

友元函数的分类：

根据这个函数的来源不同，可以分为三种方法：

### 1、普通函数友元函数：

       a) 目的：使普通函数能够访问类的友元

       b) 语法：声明位置：公有私有均可，常写为公有

                        声明： friend + 普通函数声明

                        实现位置：可以在类外或类中

                        实现代码：与普通函数相同（不加不用friend和类::）

                        调用：类似普通函数，直接调用

       c) 代码：



```cpp
class INTEGER  
    {    
    private:  
        int num;  
    public:  
        friend void Print(const INTEGER& obj);//声明友元函数  
    };  
    void Print(const INTEGER& obj)//不使用friend和类：：  
    {  
        //函数体  
    }  
    void main()  
    {  
        INTEGER obj;  
        Print(obj);//直接调用  
    }
```

2、类Y的所有成员函数都为类X友元函数—友元类



      a）目的：使用单个声明使Y类的所有函数成为类X的友元

            它提供一种类之间合作的一种方式，使类Y的对象可以具有类X和类Y的功能

            具体来说：

前提：A是B的友元（=》A中成员函数可以访问B中有所有成员，包括私有成员和公有成员--老忘）

            则：在A中，借助类B，可以直接使用～B . 私有变量～的形式访问私有变量

      b）语法：声明位置：公有私有均可，常写为私有(把类看成一个变量)

            声明： friend + 类名---不是对象啊

            调用：

      c）代码：



```cpp
class girl;  
      
    class boy  
    {    
    private:  
        char *name;    
        int age;    
    public:    
        boy();  
        void disp(girl &);     
    };    
      
    void boy::disp(girl &x) //函数disp()为类boy的成员函数，也是类girl的友元函数   
    {   
        cout<<"boy's name is:"<<name<<",age:"<<age<<endl;//正常情况，boy的成员函数disp中直接访问boy的私有变量  
        cout<<"girl's name is:"<<x.name<<",age:"<<x.age<<endl;   
        //借助友元，在boy的成员函数disp中，借助girl的对象，直接访问girl的私有变量  
        //正常情况下，只允许在girl的成员函数中访问girl的私有变量  
    }  
      
    class girl  
    {    
    private：  
        char *name;    
        int age;    
        friend boy;   //声明类boy是类girl的友元    
    public:    
        girl();     
    };    
    void main()    
    {     
        boy b;    
        girl g;    
        b.disp(g);  //b调用自己的成员函数，但是以g为参数，友元机制体现在函数disp中  
    }
```

### 3、类Y的一个成员函数为类X的友元函数



      a）目的：使类Y的一个成员函数成为类X的友元

            具体而言：而在类Y的这个成员函数中，借助参数X，可以直接以X。私有变量的形式访问私有变量

      b）语法：声明位置：声明在公有中 （本身为函数）

            声明：friend + 成员函数的声明

            调用：先定义Y的对象y---使用y调用自己的成员函数---自己的成员函数中使用了友元机制

      c）代码： 


```cpp
class girl;   
    class boy  
    {    
    private:  
        char *name;    
        int age;    
    public:    
        boy();  
        void disp(girl &);       
    };     
       
    class girl  
    {  
    private:  
        char *name;    
        int age;    
    public:    
        girl(char *N,int A);    
        friend void boy::disp(girl &); //声明类boy的成员函数disp()为类girl的友元函数    
    };    
       
    void boy::disp(girl &x)    
    {     
        cout<<"boy's name is:"<<name<<",age:"<<age<<endl;  //访问自己(boy)的对象成员,直接访问自己的私有变量    
        cout<<"girl's name is:"<<x.name<<",age:"<<x.age<<endl;    
        //借助友元，在boy的成员函数disp中，借助girl的对象，直接访问girl的私有变量  
        //正常情况下，只允许在girl的成员函数中访问girl的私有变量    
    }    
    void main()    
    {     
        boy b();    
        girl g();    
        b.disp(g);  }
```

### 4、在模板类中使用友元operator<<(对<<运算符的重载)

a)使用方法：

在模板类中声明：


```cpp
friend ostream& operator<< <>(ostream& cout,const MGraph<VexType,ArcType>& G);
```


在模板类中定义：



```cpp
template<class VexType,class ArcType>  
    ostream& operator<<(ostream& cout,const MGraph<VexType,ArcType>& G)  
    {  
        //函数定义  
    }
```

b)注意：

把函数声明非模板函数：


```cpp
friend ostream& operator<< (ostream& cout,const MGraph& G);
```


把函数声明为模板函数：


```cpp
friend ostream& operator<< <>(ostream& cout,const MGraph<VexType,ArcType>& G);
```


或：


```cpp
friend ostream& operator<< <VexType,ArcType>(ostream& cout,const MGraph<VexType,ArcType>& G);
```


说明：

 在函数声明中加入operator<< <>：是将operator<<函数定义为函数模板，将函数模板申明为类模板的友员时,是一对一绑定的

 实际的声明函数：这里模板参数可以省略，但是尖括号不可以省略


```cpp
friend ostream& operator<< <VexType,ArcType>(ostream& cout,const MGraph<VexType,ArcType>& G);
```


5、友元函数和类的成员函数的区别：成员函数有this指针，而友元函数没有this指针。

6、记忆：A是B的友元《=》A是B的朋友《=》借助B的对象，在A中可以直接 通过B。成员变量（可以是公有，也可以为私有变量） 的方式访问B






