# c++复习要点 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





   自增和自减运算符有前缀和后缀两种形式，都会改变对象，所以不能对常量对象操作。 前缀形式返回改变后的对象，返回*this。 后缀形式返回改变之前的值，所以必须创建一个代表这个值的独立对象并返回它,是通过传值方式返回的。 如何区分前缀和后缀形式呢？ 后缀形式的自增和自减比前缀形式多一个int参数，这个参数在函数中并不使用，只是作为重载函数的标记来区分前缀和后缀运算。例如，对Byte类对象b，编译器看到++b，会调用Byte::opeartor++()， 而b++会调用Byte::operator++(int)。 如果要重载自增和自减运算符，一般应同时定义自增、自减运算符的前缀式和后缀式。重载运算符的行为应尽量与内置运算符保持一致。

  赋值运算符operator= 只能用成员函数重载。

```
#include <iostream>
using namespace std;
class MinInt {
    char b;
public:
    MinInt(char ch = 0) : b(ch) {};
    MinInt operator-( )const{  //一元运算"-"
        cout<<"MinInt::operator-"<<endl;
        return MinInt(-b);
    }
    MinInt operator+(const MinInt& rv)const {//二元运算 +
        cout << "MinInt::operator+" << endl;
        return MinInt(b + rv.b);          //返回值优化
    }
    MinInt&  operator+=(const MinInt& rv){//复合赋值运算 +=  这里不能加const,const表示的是不变的
        cout << "MinInt::operator+=" << endl;
        b += rv.b;
        return *this;
    }
}; //end of class MinInt
```

_

```
#include <iostream>
#include <cassert>
using namespace std;
class MinInt{
    char m;
public:
    MinInt(int val = 0){            //int类型转换为MinInt
        assert(val>=0 && val<=100); //要求取值范围在0～100之间
        m = static_cast<char>(val); //这不是将int强制转换成char么
    }
    operator int(){                //MinInt对象转换为int 类型
        return static_cast<int>(m);
    }
};
int main(){
    MinInt mi(10), num;
    num = mi + 20;                  /*首先将mi转换为int类型，再执行加法运算；
                                    再将int类型的计算结果30 转换为赋值左边的MinInt 类型 */
    int val = num;                  //将num自动转换为int，并赋值给val
    cout<< mi << '\t' << num << '\t' << val;
                                    //num 和mi 转换为int输出
}
```



_



派生类的构造函数应该包含它的**直接基类**的构造函数。

定义格式如下：

<派生类构造函数名> (<总参数表>):<基类构造函数名> (<参数表>)，<其他初始化项>

{ <派生类自身数据成员初始化> }

_

① 派生类构造函数的执行顺序如下： 先执行基类构造函数； 再执行子对象的构造函数（如有子对象的话）； 最后执行派生类构造函数的函数体。

② 派生类构造函数的成员初始化列表中应该显式地包含基类中带参数的构造函数，或者隐含地包含基类中的默认构造函数。

_

当一个类型至少包含了另一个类型的所有行为，则称该类型是另一个类型的子类型。 例如，在公有继承下，派生类是基类的子类型。 如果类型B是类型A的子类型，则称类型B适应于类型A，这时用类型A对象的操作也可以用于类型B的对象。因此，可以说类型B的对象就是类型A的对象。 子类型的关系是不可逆的。

类型适应是指两种类型之间的关系 B类型适应A类型是指B类型的对象能够用于A类型的对象所能使用的场合。 子类型与类型适应是一致的。

_

多继承

```
1 #include <iostream>
 2 using namespace std;
 3 class A {
 4 public:
 5     A(int i) {
 6         a=i;
 7         cout<<"Constructor called.A\n";
 8     }
 9     ~A()
10     {  cout<<"Destructor called.A\n";  }
11     void Print( )
12     {  cout<<a<<endl;  }
13 private:
14     int a;
15 };
16 class B {
17 public:
18     B(int i) {
19         b=i;
20         cout<<"Constructor called.B\n";
21     }
22     ~B()
23     {  cout<<"Destructor called.B\n";  }
24     void Print()
25     {  cout<<b<<endl;  }
26 private:
27     int b;
28 };
29 class C {
30 public:
31     C(int i) {
32         c=i;
33         cout<<"Constructor called.C\n";
34     }
35     ~C()
36     {  cout<<"Destructor called.C\n";  }
37     int Getc( ) {  return c;  }
38 private:
39     int c;
40 };
41 class D :public A,public B {
42 public:
43     D(int i,int j,int k,int l):B(i),A(j),c(l) {
44         d=k;
45         cout<<"Constructor called.D\n";
46     }
47     ~D()
48     {  cout<<"Destructor called.D\n";  }
49     void Print()
50     {
51         A::Print();
52         B::Print();
53         cout<<d<<','<<c.Getc()<<endl;
54     }
55 private:
56     int d;
57     C c;
58 };
59 
60 int main( )
61 {
62     D d(5,6,7,8);
63     d.Print();
64     B b(2);
65     b=d;     //不同类之间竟然能相互赋值？
66     b.Print();
67     return 0;
68 }
```

```
C:\Users\Administrator\CLionProjects\untitled\cmake-build-debug\untitled.exe
Constructor called.A
Constructor called.B
Constructor called.C
Constructor called.D
6
5
7,8
Constructor called.B
5
Destructor called.B
Destructor called.D
Destructor called.C
Destructor called.B
Destructor called.A

Process finished with exit code 0
```

_

 使用虚基类避免产生二义性问题

```
#include <iostream>
using namespace std;
class B{
public: int b;
};

class B1 : virtual public B {
public: int b1;
};

class B2 : virtual public B { 
private: int b2;
};

class C : public B1, public B2{
private: float d;
};
int main()
{
    C c;
    c.b;
    return 0;
}
```

![](https://images2018.cnblogs.com/blog/1196023/201805/1196023-20180522154125810-377495884.png)

——

编译时多态性： 函数重载 运算符重载 模板

运行时多态性： 借助虚函数来获得

——

虚函数的用法

```
#include <iostream>
using namespace std;
class shape{
public:
    virtual float area(){ return -1;}
};

class circle : public shape{
    float radius;
public:
    void circle(float r){ radius = r; }
    float area(){ return 3.14159*radius*radius; }
};

int main() 
{
    shape obj, *ptr;  //定义基类指针，虚函数好像只有在用这种特殊情况下才有用
    circle c(3.6);
    ptr=&obj;
    cout<<ptr->area( )<<endl;
    ptr=&c;
    cout<<ptr->area( )<<endl;
    return 0;
}
```

**只有当访问虚函数是通过基类指针s时才可获得运行时的多态性。**

——

尽管可以用对象名和点算符的方式调用虚函数，即向对象发送消息： tri.area( ) 或者 rect.area( ) 但这时是静态联编方式

——

包含虚函数的类被称为多态类。

——

虚函数与重载函数的比较：

1. 从形式上说，重载函数要求函数有相同的函数名称，并有不同的参数序列；

2. 重载函数可以是成员函数和非成员函数；

3.对重载函数的调用是以所传递参数序列的差别（参数个数或类型的不同）作为调用不同函数的依据；

-1.而虚函数要求这三项(函数名称、返回值和参数序列）完全相同，即具有完全相同的函数原型。

-2.而且虚函数只能是成员函数。

-3.虚拟函数是根据对象的不同去调用不同类的虚拟函数。

虚函数在运行时表现出多态功能，正是C++的精髓之一；而重载函数不具备这一功能。

——

虚函数要点：

为了实现运行时的多态性，调用虚函数应该通过第一次定义该虚函数的基类对象指针 只有通过指针或引用标识对象来操作虚函数时，才对虚函数采取动态联编方式。 如果采用一般类型的标识对象来操作虚函数，则将采用静态联编方式调用虚函数。

——

纯虚函数

通过将虚函数声明为纯虚函数可以强制在派生类中重新定义虚函数。 如果没有在派生类中重新定义，编译器将会报告错误。

抽象类：包含有纯虚函数的类称为抽象类。 不能说明抽象类的对象，但能说明指向抽象类的指针，一个抽象类只能作为基类来派生其他的类。 抽象类的指针用于指向该抽象类的派生类的对象。

```
virtual float area() = 0;
```

用法基本和虚函数差不多。

```
#include <iostream>
using namespace std;

class Shape
{
public:
    virtual double GetArea()const = 0;
};


class Circle:public Shape
{
protected:
    double radius;
public:
    Circle(double r):radius(r){};
    double GetArea() const
    {
        return 3.14*radius*radius;
    }
};


class Rectangle:public Shape
{
protected:
    double length;
    double width;
public:
    Rectangle(double x,double y):length(x),width(y){};
    double GetArea() const
    {
        return length*width;
    }
};


void ShowArea(const Shape &shape) //纯虚函数的作用
{
    cout << shape.GetArea() <<endl;
}

int main()
{
    double a,b,c;
    cin >> a >> b >> c;
    Circle circle(a);
    Rectangle rect(b,c);
    ShowArea(circle);
    ShowArea(rect);
    return 0;
}
```

 ——

拷贝构造本质是构造函数，所以写法可以稍微的参考一下构造函数

```
class point
{
private:
    int x,y;
public:
    point(point &p)
    {
        x = p.x;
        y = p.y;
    }
};
```

——

内联函数：

1，内联函数是一种函数体被替换，而不是被调用的函数。（提高运行时的效率）

2，内联函数体中不要有复杂结构（如循环语句和switch语句）。

用法就是在原本的函数上加上inline

——

静态成员：

1，静态成员解决了数据共享的问题。 在类体内使用关键字static说明的成员称为静态成员，包括： 静态数据成员 静态成员函数

2，静态成员的特点是它不是属于某对象的，而是属于整个类的，即所有对象的。

3，静态成员的访问，可以通过对象引用，也可以通过类名来引用。

静态变量在类中的正确初始化及定义法

```
#include <iostream>
using namespace std;

class point
{
private:
    int x,y;
public:          //不能将s设置为private哦，会访问不了
    static int s;
};

int point::s = 0; //必须要初始化，初始化方式，必须在类外初始化

int main()
{
    cout << point::s << endl; //访问方式
    return 0;
}
```

引用静态成员函数有如下两种方式： <类名>::<静态成员函数名>(<参数表>)

<对象名>.<静态成员函数名>(<参数表>)

在静态成员函数中可以直接引用其静态成员，而**引用非静态成员时需用对象名引用**。

```
#include <iostream>
using namespace std;

class point
{
public:
    point(int xx):x(xx){};
    static void func(point p)
    {
//        cout << point.x << endl;  错误point未实例化
        cout << p.x << endl;    //正确写法
//        cout << x << endl; 错误 静态成员函数本质上属于所有类
    }

private:
    int x;
    static int y;
};

int point::y = 0; //初始化

int main()
{
    point p1(1);
    //两种调用静态成员函数的方法
    p1.func(p1);
    point::func(p1);
    return 0;
}
```

如果逆看的懂上面这段沙雕代码，静态成员就基本掌握了

——

将运算符函数定义为成员函数时，调用成员函数的对象（this指向的对象）被作为运算符的第一个操作数，所以如果是一元运算符，无需提供参数。使用成员函数重载二元运算符时，将当前对象（this指向的对象）作为左操作数，需要提供一个参数作为右操作数。 如果将运算符函数定义为全局函数，则通常要将其声明为类的友元函数。重载一元运算符时需要提供一个类类型的参数，重载二元运算符时需要提供两个参数，分别作为左右操作数，其中至少一个参数必须是类类型的。

——

友元函数是说明在类体内的一般函数，**它不是这个类中的成员函数**，但是它访问该类所有成员。

可能是普通函数或者其他类的成员函数

友元函数基本写法：

```
#include <iostream>
using  namespace std;

class point
{
private:
    int x;
public:
    point(int xx):x(xx){};
    friend void func(point p);
};

void func(point p)  //直接在类外定义，无需加point::,因为不是point的成员函数
{
    cout << p.x << endl;
}
```

友元类：

若一个类为另一个类的友元，则此类的所有成员都能访问对方类的私有成员。

友元类基本操作：

```
#include <iostream>
using  namespace std;

class A
{
    friend class B;  //声明 B 是我的好朋友，我所有的东西他都可以访问
private:
    int x;
};


class B
{
public:
    void func(A aa) //友元函数由于不是类的成员所以一定要自带类的形参     
    {
        cout << aa.x << endl;
    }
};
```
友元函数由于不是类的成员所以一定要自带类的形参  
当一个函数需要访问多个类时，应该把这个函数同时定义为这些类的友元函数。

当友元函数为其他类的成员函数：
```
#include <iostream>
using namespace std;

class B;//B需要提前说明

class A
{
private:
    int a;
public:
    A(int x):a(x){};
    void fun(B &bb);  //fun为A的成员函数
};

class B
{
private:
    int b;
public:
    B(int y):b(y){};
    friend void A::fun(B &bb); //申明其他类的友元函数
};

void A::fun(B &bb) {
    cout << bb.b <<endl;
    cout << a << endl;
}

int main()
{
    A obja(1);
    B objb(2);
    obja.fun(objb);
    return 0;
}
```
——
 指向类成员的指针：

```
#include <iostream>
using  namespace std;

class A
{
public:
    int num;
    A(int x):num(x){};
};

int main()
{
    A a(1);
    A *pa = &a;
    int A::*pc = &A::num;//指向类成员的指针初始化
    cout << a.*pc << endl;//两种调用形式
    cout << pa->*pc << endl;
    return 0;
}
```

还有是指向类函数的指针，由于ppt错了，所以我也不知道....

——

对象指针作为函数参数

　　实现传址调用 使用对象指针作为形参仅将对象的地址值传给形参，提高运行效率。

对象引用作函数参数

　　实现传址调用 比指针更简单、更直接（※）

——

每一个成员函数中都隐含的存在this指针，当你调用类的成员时，其实你实际上是在调用this指针。

```
class A
{
public:
    int num;
    A(int x):num(x){};
    void copy(A &a)
    {
        if(this == &a)  //也称为自赋值检测（很重要）
            return;
        *this = a;
    }
};
```

——

指向数组的指针和指针数组

```
#include <iostream>
using  namespace std;
const int maxn = 100;
int main()
{
    int a[maxn][maxn];//指向数组的指针
    int (*pa)[maxn] = a;

    int *pl[maxn];  //数组指针
    return 0;
}
```

——

引用（&）：int a ;  int &t = a; 引用的作用相当于给变量再加一个别名，在改变别名的时候所有的值都会随之改变。（实际上是用的相同的地址）

常引用：const int &t ,加了const说明值不会被改变，也就是单纯的为了减少内存空间，加快运行速度。

——

int *const p1 = &a; // 地址值为常量的指针

const int *p2 = &a; // 所指向值为常量的指针

——

常成员函数：就后面加个const，表示在此函数中类中的数据成员不能被改变。

常对象：1，常对象的数据成员都是常数据成员，其值不能改变。2， 常对象只能调用常成员函数，不能调用非常成员函数。

常数据成员：常数据成员的值是不能改变的。 常数据成员初始化是通过构造函数的成员初始列表来实现的。

```
class A
{
public:
    const int num;
    A(int x):num(x){};
//    A(int x){     //错误，必须用初始化列表
//        num = x;
//    }
};
```

常静态成员数据：

初始化和静态成员数据差不多

```
class B
{
public:
    static const int b;
};

const int B::b = 0;
```

——

子对象：子对象初始化应放在构造函数的成员初始化列表中。

```
class B
{
public:
    int b;
    B(int x):b(x){};
};

class A
{
public:
    int a;
    B objb;
    A(int y):objb(y){};
};
```

——

堆对象（动态对象）

创建：A * pa; pa = new A(3,5);

A * parray; parray = new A[10];

删除：delete pa;    delete [] pa;

——

在执行算术运算时，低类型自动转换为高类型。

在赋值表达式中，赋值运算符右边表达式的类型自动转换为左边变量的类型。

——

静态成员函数没有隐含的this指针（思考一下就知道，静态成员函数本质上不属于类的一部分，而this代表的就是类本身，所以得到如此结论）

 ——

临时对象语法：返回值优化（自己想是啥）

——

![](https://images2018.cnblogs.com/blog/1196023/201805/1196023-20180521171919945-574922869.png)

——

 默认情况下，结构体是public，类是private。

——

当基类和派生类有同名函数，定义哪一个类就是调用哪一个类的函数。

——

保护继承：可以被本类或派生类的成员函数访问，但不能被外部函数访问。

——

了解：

函数模板定义了参数化的**非成员函数**，使程序能用不同类型的参数调用相同的函数。

由编译器决定该使用哪一种类型，并且从模板中生成相应的代码。

适合对各种数据类型，执行完全相同的操作 根据函数调用中提供的参数，编译器自动实例化不同的对象代码函数

和普通函数一样，函数模板之间也可以重载 而且函数模板也可以与普通函数之间构成重载关系

——

所谓模板的思想就是想做一个通用的东西出来。

比如一个通用的函数，通用的类。

注意：应该是具有近似功能的函数或者类才能进一步抽象为类模板或者函数。

函数模板就是将近似功能的多个函数抽象为一个，从而提高代码的利用率。

注意函数模板实例化出来的函数之间是一个重载的关系。

类模板就是将具有近似功能的多个类进一步抽象为一个类，从而提高代码的利用率。

注意此时的类模板实例化后就是普通的类，对普通的类进行实例化后就是对象。

注意：类模板并不是抽象类。

——

C++流库是用继承方法建立起来的一个输入输出类库。

流是对字节序列从一个对象移动到另一个对象的抽象。

流对象是内存与文件（或字符串）之间数据传输的信道。

标准流与系统预定义的外部设备连接； 串流与串对象或字符数组连接；文件流与用户定义的外部文件连接。

一旦流对象和通信对象关联，程序就可以使用流的操作方式传输数据。

标准流与外设的连接是C++预定义的。

其他流类对象与通信对象的连接使用流类构造函数实现。

——













