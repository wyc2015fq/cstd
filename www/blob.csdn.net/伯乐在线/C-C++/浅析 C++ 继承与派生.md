# 浅析 C++ 继承与派生 - 文章 - 伯乐在线
原文出处： [Fireplusplus](http://blog.csdn.net/qq_33724710/article/details/51635331)
> 
测试环境：
Target: x86_64-linux-gnu
gcc version 5.3.1 20160413 (Ubuntu 5.3.1-14ubuntu2.1)
### 定义
要分析继承，首先当然要知道什么是继承：继承是面向对象程旭设计中使代码可以复用的最重要的手段，它允许程序员在原有类特性的基础上进行扩展，增加功能。这样产生的新类，就叫做派生类（子类）。继承呈现了面向对象程序设计的层次结构,体现了由简单到复杂的认知过程。
### 继承的格式
class 子类名 ：继承权限 基类名
比如下面分别定义了两个类：


```
class A  
{  
public:  
    int pub;  
protected:  
    int pro;  
private:  
    int pri;  
};  
  
class B: public A  
{  
};
```
如上我们就说类B继承了类A，类B叫做类A的派生类或者子类，A类叫做B类的基类或者父类。
### 继承关系&访问限定符
之前学习类的成员访问限定符的时候都知道public, protected, private 这三种访问限定符的作用，public修饰的类成员可以在类外被访问，而protected与private则不可以。这三种访问权限又对应这三种继承关系：
![](http://jbcdn2.b0.upaiyun.com/2016/12/d8a1c09b255c37f4e22aa5e2ebdaffee.png)
继承关系可以影响子类中继承自父类的成员变量的访问权限，还是在上个栗子的基础上，我们定义一个B类对象进行如下操作；


```
int main()  
{  
    B b1;  
    b1.pub;  
    b1.pro;  
    b1.pri;  
  
  
    return 0;  
}
```
编译则会报错：
![](http://jbcdn2.b0.upaiyun.com/2016/12/c4c7b1b9d574c2df2834da4fcf651149.png)
会提示pro与pri变量访问权限分别为protected和private，我们不能在类外使用它们。类似的，在B中定义这样一个成员函数：


```
class B: public A  
{  
    void fun()  
    {  
        cout<<pub;  
        cout<<pro;  
        cout<<pri;  
    }  
};
```
会报这样的错：
![](http://jbcdn2.b0.upaiyun.com/2016/12/7adef3e6158a58ed844063920430e5a3.png)
即基类中的私有成员在子类中是不可见的。关于三种继承方式的成员访问权限总结如下表：
![](http://jbcdn2.b0.upaiyun.com/2016/12/0233f65b019dfb5ae0046702ae0f90dd.png)
总结:
1. 基类的 private 成员在派生类中是不能被访问的,如果基类成员不想在类外直接被访问,但需要在派生类中能访问,就定义为 protected 。可以看出保护成员限定符是因继承才出现的。
2. public继承是一个接口继承,保持is-a原则,每个父类可用的成员对子类也可用,因为每个子类对象也都是一个父类对象。
3. protetced/private继承是一个实现继承,基类的部分成员并非完全成为子类接口的一部分,是 has-a 的关系原则,所以非特殊情况下不会使用这两种继承关系,在绝大多数的场景下使用的都是公有继承。
4. 不管是哪种继承方式,在派生类内部都可以访问基类的公有成员和保护成员,基类的私有成员存在但是在子类中不可见(不能访问)。
5. 使用关键字class时默认的继承方式是private,使用struct时默认的继承方式是public,不过最好显示的写出继承方式。
6. 在实际运用中一般使用都是public继承,极少场景下才会使用protetced/private继承
### 继承关系中构造/析构函数调用顺序
在现有类的基础上添加如下的构造与析构函数：


```
class A  
{  
public:   
    A()  
    {  
        cout<<"A()"<<endl;  
    }  
  
    ~A()  
    {  
        cout<<"~A()"<<endl;  
    }  
  
public:  
    int pub;      
protected:  
    int pro;  
private:  
    int pri;  
};  
  
class B: public A  
{  
public:  
      
    B()  
    {  
        cout<<"B()"<<endl;  
    }  
  
    ~B()  
    {  
        cout<<"~B()"<<endl;  
    }  
};
```
然后，在main函数中定义一个类B的对象：B b; 编译运行，看看输出语句的顺序：
![](http://jbcdn2.b0.upaiyun.com/2016/12/765f0c9994de8a79491d57a64a2b27be.png)
先基类构造，后子类构造；析构的时候先析构子类，后析构基类。依旧和以前一样，先构造的后析构（因为在栈上）。
让我们走进几行代码的反汇编世界：
![](http://jbcdn2.b0.upaiyun.com/2016/12/4145adeae6ef411d55cb335ab6bb256e.png)
这是程序现在运行到了b的定义语句。=> 所指，是当前运行的汇编语句。可以看到，第三条汇编语句调用了B类的构造函数。咦？怎么跟我们刚刚看到的顺序不太一样！不急，先往下看。直接 ni 运行到第三条汇编，然后用 si 命令跟进去：
![](http://jbcdn2.b0.upaiyun.com/2016/12/0d2e584d81e1fe8bdb168af752a57fde.png)
可以看到，程序在正式进入B类的构造函数之前，先调用了A类的构造函数，照这么来看，可以推测出是编译器自动的在B类的构造数的初始化列表位置调用了A类的构造函数。还是让我们把程序看完：
![](http://jbcdn2.b0.upaiyun.com/2016/12/6ea8a9aca6056fb35707d5d00330e46e.png)
果然，又进入了类A的构造函数。
![](http://jbcdn2.b0.upaiyun.com/2016/12/3708a63d78196b4c86ee5f851e52ce8a.png)
从A类构造函数出来后，才正式进入类B构造函数。
![](http://jbcdn2.b0.upaiyun.com/2016/12/77946efb6a7031b27322cda12b9f2dfe.png)
出main函数作用域时，先调用了B类的构造函数
![](http://jbcdn2.b0.upaiyun.com/2016/12/9aaac411e6afbc2ed21f14fdcb8fc5a6.png)
在B类构造函数的末尾调用了A类构造函数。整个过程与我们看到的输出信息一致。
如果类B中还有一个成员变量是一个类对象，那么构造与析构调用顺序又是哪样？


```
class T  
{  
  
public:  
    T(int i = 1)   
    {  
        cout<<"T()"<<endl;  
    }  
  
    ~T()  
    {  
        cout<<"~T()"<<endl;  
    }  
};  
  
class A  
{  
public:   
    A()  
    {  
        cout<<"A()"<<endl;  
    }  
  
    ~A()  
    {  
        cout<<"~A()"<<endl;  
    }  
  
public:  
    int pub;      
protected:  
    int pro;  
private:  
    int pri;  
};  
  
class B: public A  
{  
public:  
      
    B()  
    {  
        cout<<"B()"<<endl;  
    }  
  
    ~B()  
    {  
        cout<<"~B()"<<endl;  
    }  
public:  
    T t;  
};
```
还是刚刚的main函数，在运行一下程序：
![](http://jbcdn2.b0.upaiyun.com/2016/12/61c874fcfd85da4bb9233a672e387b92.png)
很明显，先调用基类构造，然后是成员对象的构造函数，最后是该类自身的构造函数，析构函数顺序则相反。具体的汇编代码就不演示了。总结一下：
![](http://jbcdn2.b0.upaiyun.com/2016/12/e1e6101224e9251356c7e2ebeec449b1.png)
![](http://jbcdn2.b0.upaiyun.com/2016/12/1c4e8da641e27c31f569f14502fa27b7.png)
【说明】
1、基类没有缺省构造函数,派生类必须要在初始化列表中显式给出基类名和参数列表。
2、基类没有定义构造函数,则派生类也可以不用定义,全部使用缺省构造函数。
3、基类定义了带有形参表构造函数,派生类就一定要定义构造函数。
### 继承体系中的作用域
- 基类和派生类是不同的作用域
- 同名隐藏：子类和父类中有同名成员时,子类成员将屏蔽父类对成员的直接访问。(在子类成员函数中,可以使用 基类::基类成员 访问父类成员)
- 在实际中在继承体系里面最好不要定义同名的成员


```
class A  
{  
public:  
    int pub;  
};  
  
class B: public A  
{  
public:  
    int pub;  
};  
  
int main()  
{  
    B b;  
    b.pub = 1;    //访问的是派生类的成员变量，基类同名被隐藏  
    b.A::pub = 2;    //指明作用域，访问基类成员变量  
  
    return 0;  
}
```
### 继承与转换–赋值兼容规则–（前提：public继承）
- 子类对象可以赋值给父类对象
- 父类对象不能赋值给子类对象
- 父类的指针/引用可以指向子类对象
- 子类的指针/引用不能指向父类对象(但可以通过强制类型转换完成)
### 友元与继承
友元关系不能继承,也就是说基类友元不能访问子类私有和保护成员。


```
class Person  
{  
    friend void Display(Person &p , Student&s);  
protected :  
    string _name ;  
};  
  
class Student: public Person  
{  
protected :  
    int _stuNum ;  
};  
  
void Display(Person &p , Student &s)  
{  
    cout<<p._name<<endl;  
    cout<<s._name<<endl;  
    cout<<s._stuNum<<endl; //error  
}  
int main()  
{  
    Person p;  
    Student s;  
    Display (p, s);   
    return 0;  
}
```
### 继承与静态成员
基类定义了static成员,则整个继承体系里面只有一个这样的成员。无论派生出多少个子类,都只有一个static成员实例。如下：


```
class A  
{  
public:  
    static int i;   //注意这里只是声明  
};  
  
int A::i = 0;  
  
class B : public A  
{};  
  
int main()  
{  
    A a;  
    B b;  
      
    cout<<"a.i="<<a.i<<" "<<"b.i="<<b.i<<endl;  
    a.i++;  
    b.i++;  
    cout<<"a.i="<<a.i<<" "<<"b.i="<<b.i<<endl;  
  
    return 0;  
}
```
输出：
![](http://jbcdn2.b0.upaiyun.com/2016/12/2e9bead730c82ef3ca5db869877c09a6.png)
### 单继承&多继承&菱形继承
#### 【单继承】
一个子类只有一个直接父类时称这个继承关系为单继承。
![](http://jbcdn2.b0.upaiyun.com/2016/12/8bcb8171faf678d3621d17302f12500c.png)
#### 【多继承】
一个子类有两个或以上直接父类时称这个继承关系为多继承。
![](http://jbcdn2.b0.upaiyun.com/2016/12/9282a006e93094af878dee0d9c1c5962.png)
#### 【菱形继承】
![](http://jbcdn2.b0.upaiyun.com/2016/12/d1c2055f1cb7804b5d7805d265f524ea.png)
![](http://jbcdn2.b0.upaiyun.com/2016/12/0f8c02ea15b5561a7fbf6577105c0428.png)
例：


```
class Person  
{  
public :  
    string _name ; // 姓名  
};  
class Student : public Person  
{  
    protected :  
    int _num ; //学号  
};  
class Teacher : public Person  
{  
protected :  
    int _id ; // 职工编号  
};  
class Assistant : public Student, public Teacher  
{  
protected :  
    string _majorCourse ; // 主修课程  
};  
void Test ()  
{  
    // 显示指定访问哪个父类的成员  
    Assistant a ;  
    a.Student ::_name = "xxx";  
    a.Teacher ::_name = "yyy";  
}
```
看一下菱形继承的构造与析构函数调用顺序：（main函数中创建了一个D类对象）
B类和C类继承A类，D类继承B和C类：


```
class A  
{  
public:   
    A(){cout<<"A()"<<endl;}  
  
    ~A(){cout<<"~A()"<<endl;}  
};  
  
class B: public A  
{  
public:  
      
    B(){cout<<"B()"<<endl;}  
  
    ~B(){cout<<"~B()"<<endl;}  
};  
  
class C : public A  
{  
public:  
    C(){cout<<"C()"<<endl;}  
  
    ~C(){cout<<"~C()"<<endl;}  
};  
  
class D : public B, public C  
{  
public:  
    D(){cout<<"D()"<<endl;}  
      
    ~D(){cout<<"~D()"<<endl;}  
};
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/14ba35fb6768b6fed838f83f4f7c4f85.png)
对照对象模型来看会很清楚。
#### 虚继承–解决菱形继承的二义性和数据冗余的问题
1. 虚继承解决了在菱形继承体系里面子类对象包含多份父类对象的数据冗余&浪费空间的问题。
2. 虚继承体系看起来好复杂,在实际应用我们通常不会定义如此复杂的继承体系。一般不到万不得已都不要定义菱形结构的虚继承体系结构,因为使用虚继承解决数据冗余问题也带来了性能上的损耗。
![](http://jbcdn2.b0.upaiyun.com/2016/12/9ebebec85e04e1e92cc0aeb8a24b5ded.png)
实际存放情况：
两个Address处存放的是地址，这个地址所代表的空间存放了由当前Address这个位置到_name的偏移量。具体情况略显繁琐，不便演示，可以查看反汇编。
再看一下上面的类中虚继承的情况下构造与析构函数调用顺序：B类和C类虚继承A类


```
class A  
{  
public:   
    A(){cout<<"A()"<<endl;}  
  
    ~A(){cout<<"~A()"<<endl;}  
};  
  
class B: virtual  public A  
{  
public:  
      
    B(){cout<<"B()"<<endl;}  
  
    ~B(){cout<<"~B()"<<endl;}  
};  
  
class C : virtual public A  
{  
public:  
    C(){cout<<"C()"<<endl;}  
  
    ~C(){cout<<"~C()"<<endl;}  
};  
  
class D : public B, public C  
{  
public:  
    D(){cout<<"D()"<<endl;}  
      
    ~D(){cout<<"~D()"<<endl;}  
};
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/3792661f1a4b0f79546ea790024ad171.png)
对照着对象模型看，只需要调用一次B类构造函数即可。
