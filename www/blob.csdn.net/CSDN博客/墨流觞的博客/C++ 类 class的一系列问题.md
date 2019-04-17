# C++ 类 class的一系列问题 - 墨流觞的博客 - CSDN博客





2018年12月12日 22:31:52[墨氲](https://me.csdn.net/dss_dssssd)阅读数：32








### c++类定义

衍生于结构体Struct，

定义了类对象包括什么(数据成员, 属性)和可以对这个对象进行什么操作(操作)。

类定义的一般格式：

```
class 类名{
//数据成员
《《private:》
	成员1;》
《public:
成员2;》
《protected:
成员3;》
//函数操作
《《private:》
 函数返回类型 函数名(参数){函数体};》
《public:
成员2;》
《protected:
成员3;》
}
```

比如下列代码：

```
class Box
{
   public:
      double length;   // 长度
      double breadth;  // 宽度
      double height;   // 高度
      double getVolume()
      {
         return length * breadth * height;
      }
};
```

需要注意：
- {}之内为类体，定义数据成员和函数操作
- public, private, protected 为访问限定符
- 这三种限定符的作用域为：**从该说明符出现开始到下一个说明符之前或类体结束之前**
- 如果类体开始没有限定符修饰，默认为private
- 对于访问，只有public修饰的，可以通过对象访问，而private和protected不可通过类对象访问。还有一部分是在继承处使用


### 数据成员的访问

可以通过**直接成员访问运算符(.)**来访问成员。

```
#include <iostream>
 
using namespace std;
 
class Box
{
   public:
      double length;   // 长度
      double breadth;  // 宽度
      double height;   // 高度
      double getVolume()
      {
         return length * breadth * height;
      }
};


int main( )
{
   Box Box1;        // 声明 Box1，类型为 Box
   Box Box2;        // 声明 Box2，类型为 Box
   double volume = 0.0;     // 用于存储体积
 
   // box 1 详述
   Box1.height = 5.0; 
   Box1.length = 6.0; 
   Box1.breadth = 7.0;
 
 
   // box 1 的体积
   volume = Box1.height * Box1.length * Box1.breadth;
   cout << "Box1 的体积：" << volume <<endl;
 
   // box 1的体积
   volume = box1.getVolume()
   cout << "Box2 的体积：" << volume <<endl;
   return 0;
}
```

### 对象的创建和引用

对象的存储方案有两种，如第一个图为每一个对象分配全套的内存，包括**存放成员数据的数据区和存放成员函数的代码区**，

但是，区别同一个类的中各个对象是由数据成员决定的，不同对象的数据成员内容是不同的；而行为是用函数来描述的，这些代码对于所有对象都是一样的。

假设每个对象的数据内存栈1KB，代码栈50KB，共占51KB，则 100个对象占5100KB，其中重复内存为4950KB，极其耗费内存。

因而采用第二种内存分配内存：
**数据成员和在类内部定义实现的函数存放在数据区，而在类外部定义实现的函数放在公共代码区**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181220200601513.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181220200833769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
- 直接定义类的实例–对象
`Box box1`
`Box box1(参数值)`  涉及到构造函数- 动态创建
- 创建
`指针变量名 =new类型名(初始化方式)`
- 删除
`delete 指针名`

```
Box *pbox1=new Box();
delete pnox1;
```

### c++ 类成员函数

##### 1. 在类体内直接定义函数

##### 2. 在类外定义函数

要使用**范围解析运算符 ::**,比如说在类内声明，类外定义实现函数`getVolume()`:

```
class Box
{
   public:
      double length;   // 长度
      double breadth;  // 宽度
      double height;   // 高度
      double getVolume()；
     
};

double Box:: getVolume() {
         return length * breadth * height;
      }
```

注意：
- 范围解析运算符`::`前面加**类名**，表明该函数属于此类中
- 成员函数与内联 inline

用于消除调用函数是所造成的固定时间的消耗的方法。

在编译阶段将使用函数的定义体来替代函数调用语句[c++内联函数解释](https://www.cnblogs.com/QG-whz/p/4641479.html)
默认在类体内实现的成员函数都是内联的；如果在类体外定义函数，而仅在类体内给出函数声明，如果要实现内联，则在类外函数定义前加关键字`inline`

```
class A
{
    public:
    void Foo(int x, int y);
    
}

inline void A::Foo(int x, int y){} ;
```

但是呢，内联仅仅是对编译器的一个建议，C++标准规定仅当类的成员函数很小并且不包循环等复杂结构时，且这些成员函数的函数体在类内部直接定义或在外部定义为内联是，才使用内联扩展方式实现。但是，**在具体的C++平台上也许根本不理睬内敛建议**

### 构造函数和析构函数
- 构造函数
- 
定义： `类名(参数){}`

- 
类的构造函数是类的一种特殊的成员函数，它会在每次创建类的新对象时执行，并在该对象的生命周期中只会调用一次

- 
构造函数的名称与类的名称是完全相同的

- 
无函数返回类型，构造函数可用于为某些成员变量设置初始值

- 
构造函数可以重载，用不同的参数表区分

- 
若类定义中没有给出任何构造函数，c++编译器会自动生成默认的构造函数： `类名 (){}`，但是，只要程序员定义一个构造函数，系统就不会生成默认的构造函数


```
//演示重载构造函数
#include <iostream>
#include<cstring>
using namespace std;
 
class CGoods
{
   public:
      CGoods();
      CGoods(char [], int, float);
      CGoods(char[], float);
 
   private:
      char Name[20];
      int Amount;
      float Price;
      float Total_value;
};
// 构造函数定义

 CGoods::CGoods(){
 Name[0]='\0';
 Price=0.0;
 Amount=0;
 Total_value=0.0;
 }
 CGoods::CGoods(char name[], int amount, float price){
 strcpy(Name, name);
 Price=price;
 Amount=amount;
 Total_value=0.0;
 }
 CGoods::CGoods(char name[], float price){
 strcpy(Name, name);
 Price=price;
 Amount=0;
 Total_value=0.0;
 }
```

**注意：**
- 
如果想使用默认的构造函数，则在定义对象时不能使用**括号**

比如：
`CGoods Car3;`  Car3为CGoods类的对象，定义时调用不带参数的构造函数，

而，
`CGoods Car4();` 实际上并没有调用默认的构造函数，而是定义一个函数声明。- 
构造函数的另一种初始化方式：


`类名(参数列表): 数据成员1(参数值1), 数据成员2(参数值2)[...]`

```
#include <iostream>
#include <stdlib.h>
using namespace std;

class CMyClass {
public:
	CMyClass(int);
	int getx() { return m_x; };
	int gety() { return m_y; };
private:
	int m_x;
	int m_y;
};

CMyClass::CMyClass(int i) : m_y(i), m_x(m_y)
{
};
/*
CMyClass::CMyClass(int i) {
	m_y = i;
	m_x = m_y;
};
*/

int main()
{
	CMyClass a(1);
	cout << a.getx() << endl << a.gety() << endl;
	system("pause");
	return 0;
}
```

out:

> 
-858993460

1


而正常的在构造函数内部初始化，则不受影响，*注意执行一下代码中注释的部分*

2. 析构函数

在定义一个对象时，C++会自动调用构造函数创建该对象并完成初始化，而当一个对象的生命周期结束时，C++编译器会自动调用一个特殊的成员函数–**析构函数**，进行善后工作- 析构函数的名称与类的名称是完全相同的，只是在前面加了个波浪号（~）作为前缀
- 无函数返回类型，并且无任何参数
- 一个类中有且只能有一个析构函数，可以默认
- 在注销对象是，系统会自动调用析构函数，通常在析构函数中完成**关闭文件，释放内存等**操作

```
#include <iostream>
#include <stdlib.h>
using namespace std;
 
class Line
{
   public:
      void setLength( double len );
      double getLength( void );
      Line();   // 这是构造函数声明
      ~Line();  // 这是析构函数声明
 
   private:
      double length;
};
 
// 成员函数定义，包括构造函数
Line::Line(void)
{
    cout << "Object is being created" << endl;
}
Line::~Line(void)
{
    cout << "Object is being deleted" << endl;
}
 
void Line::setLength( double len )
{
    length = len;
}
 
double Line::getLength( void )
{
    return length;
}
// 程序的主函数
int main( )
{
   Line line;
 
   // 设置长度
   line.setLength(6.0); 
   cout << "Length of line : " << line.getLength() <<endl;
   system("pause");
   return 0;
}
```

### 引用和复制构造函数

##### 引用

函数的参数传递方式为传值，即在函数域中为参数重新分配内存，并把实参的数值传递到新分配的内存中。优点是有效避免了函数的副作用，在函数调用时不会无意中修改实参的值。

有两个问题：
- 想修改实参的值
- 如果实参为一个复杂的对象，重新分配内存会引起执行效率的降低

c++引入新的类型： 引用(reference), 也称为别名(alias)

引用为一种特殊的数据类型，不是定义一个新的变量，而是给一个已定义的变量重新起一个别名，C++系统系统不为引用类型变量分配内存空间。
**引用主要用于函数之间的数据传递。**

定义格式：`类型 & 引用变量名=已定义过的变量名`

例如：
```
double number;
double & newNumber= number;
```

内存模型：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181220195023135.png)

引用的概念可以用用指针来理解。

但是有三个主要不同：- 不存在空引用，引用必须指向一块合法的内存
- 一旦引用被初始化为一个队形，就不能指向另一个对象
- 引用必须在创建时初始化；指针可以在任意时间初始化

引用可以作为函数参数，也可以作为函数返回值

作为函数参数：

```
#include <iostream>
using namespace std;
 
// 函数声明
void swap(int& x, int& y);
 
int main ()
{
   // 局部变量声明
   int a = 100;
   int b = 200;
 
   cout << "交换前，a 的值：" << a << endl;
   cout << "交换前，b 的值：" << b << endl;
 
   /* 调用函数来交换值 */
   swap(a, b);
 
   cout << "交换后，a 的值：" << a << endl;
   cout << "交换后，b 的值：" << b << endl;
 
   return 0;
}
 
// 函数定义
void swap(int& x, int& y)
{
   int temp;
   temp = x; /* 保存地址 x 的值 */
   x = y;    /* 把 y 赋值给 x */
   y = temp; /* 把 x 赋值给 y  */
  
   return;
}
```

作为函数返回值：

```
#include <iostream>
 
using namespace std;
 
double vals[] = {10.1, 12.6, 33.1, 24.1, 50.0};
 
double& setValues( int i )
{
  return vals[i];   // 返回第 i 个元素的引用
}
 
// 要调用上面定义函数的主函数
int main ()
{
 
   cout << "改变前的值" << endl;
   for ( int i = 0; i < 5; i++ )
   {
       cout << "vals[" << i << "] = ";
       cout << vals[i] << endl;
   }
 
   setValues(1) = 20.23; // 改变第 2 个元素
   setValues(3) = 70.8;  // 改变第 4 个元素
 
   cout << "改变后的值" << endl;
   for ( int i = 0; i < 5; i++ )
   {
       cout << "vals[" << i << "] = ";
       cout << vals[i] << endl;
   }
   return 0;
}
```

与指针一样，不能返回函数内局部变量，对于数组，只能引用数组元素，不能引用真个数组，数组名本身为数组第一个元素的地址

##### 复制构造函数（拷贝构造函数）

在创建对象时，用同一类中之前创建的对象来初始化新创建的对象，这个复制过程只复制数据成员，而函数成员是共用的，不需要复制。

**指针类型要重新分配内存**

```
#include <iostream>
#include<stdlib.h>
#include <string>
using namespace std;


class Line
{
public:
	int getLength(void);
	Line(int len);             // 简单的构造函数
	Line(const Line &obj);      // 复制构造函数
	~Line();                     // 析构函数
	void display();


private:
	int *ptr;
	string name;
};

// 成员函数定义，包括构造函数
Line::Line(int len)
{
	cout << "调用构造函数" << endl;
	// 为指针分配内存
	ptr = new int;
	*ptr = len;
	name = "Line";
}

Line::Line(Line &obj)
{
	cout << "调用拷贝构造函数并为指针 ptr 分配内存" << endl;
	ptr = new int;
	*ptr = *obj.ptr; // 拷贝值
	name = obj.name;
}

Line::~Line(void)
{
	cout << "释放内存" << endl;
	delete ptr;
}
int Line::getLength(void)
{
	return *ptr;
}

void Line:: display()
{
	cout << "line 大小 : " << *ptr << endl;
	cout << "line 名字：" << name << endl;
}

// 程序的主函数
int main()
{
	cout << "Line1" << endl;
	Line line1(10);
	line1.display();
	cout << "Line2" << endl;
	Line line2 = line1; // 这里也调用了拷贝构造函数
	line2.display();
	cout << "Line3" << endl;
	Line lin3(line1); // 这里也调用了拷贝构造函数,line1为实参

	lin3.display();
	system("pause");
	return 0;
}
```

out:

> 
Line1

调用构造函数

line 大小 : 10

line 名字：Line

Line2

调用拷贝构造函数并为指针 ptr 分配内存

line 大小 : 10

line 名字：Line

Line3

调用拷贝构造函数并为指针 ptr 分配内存

line 大小 : 10

line 名字：Line

请按任意键继续. . .
注意：
- 
当成员函数的参数为**同一类型的对象或者它的引用**时，在函数内部使用参数对象的私有数据成员是，可以用对象名加成员访问操作符（点号）进行，而不必通过接口（公有函数）。

从逻辑上将，每个对象都有自己的成员函数，访问其他对象的私有数据成员时应该通过该对象的公有函数，但是在物理上只有一个成员函数的副本，它属于同类中的所有对象，所以在成员函数中访问直接访问同类其他对象的私有成员是合理的。

- 
为何复制构造函数的参数为引用？

c++按值传递会在函数中重新分配一块内存，建立与参数同类型的变量或对象，再把参数的数据成员重新赋给新的变量或对象。

在建立新对象时，会自动调用复制构造函数，而如果其参数为真实对象而不是引用，则会引用新一轮调用复制构造函数的过程，出现无穷递归，因而必须采用引用传递方式- 
如果类定义中没有显式的给出构造函数或复制构造函数，则会调用默认的构造函数或默认的复制构造函数


使用复制构造函数的三个方面：
- 使用同类的一个对象初始化该类的另一个对象
- 当函数的形参是类的对象， 这时，在函数内部要创建一个新的局部对象，把实参的值复制给新的对象，此时要调用复制构造函数
- 当函数的返回值为类的对象，也会调用复制构造函数。 理由：要创建一个临时对象作为载体，在返回给调用者。多数情况下返回的是局部对象，而局部对象在离开创建它的函数的时候就消失了，不可能在返回调用函数后继续存在。因而在处理函数返回值时，编译器一律在调用函数的表达式中创建一个无名临时对象，**该对象的生命周期只在函数调用处的表达式中**，返回对象实际上是调用复制构造函数把该对象的值复制到临时对象中。 如果返回的是变量，处理过程类似，只是不再调用复制构造函数

此处讲的复制构造函数实际上不用显示定义，系统会自动提供，即每个类成员会依次被浅复制，也称为默认的按成员语义初始化。因为函数成员对于所有的类对象只有一个，所以物理上，一个类对象向另一个类对象按成员进行复制是通过依次复制每个数据成员实现的。 对于所有的类对象，赋值运算符“=”为默认的**复制赋值操作符**，同类对象之间可以用“=”直接赋值。大多数情况下，默认的已经够用了。但对于某些指针的操作，默认的赋值构造函数不再安全，需要类的设计者提供特殊的复制构造函数和复制赋值操作符。

##### 成员对象与构造函数

类中的成员处理数据成员和函数成员意外，还有对象成员(也称为成员对象)，即其他类的对象作为类的成员。 使用成员对象 的技术成为聚合(aggregation).

对于成员对象，C++要分配内存和初始化操作。而对于成员对象的初始化，必须调用该对象成员的构造函数类实现。 **首先依次调用各成员的构造函数，最后使用类自己的构造函数**

其格式：
`类名::构造函数(参数列表):对象成员1(参数), ... 对象成员n(参数){...}`

例子：

```
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<stdlib.h>
#include <string>
using namespace std;



class StudentID {
	long value;
public:
	StudentID(long id = 0) {
		value = id;
		cout << "分配给学生的学号：" << id << endl;
	}
	~StudentID() {
		cout << "删除学号：" << endl;
	}
};

class Student {
private:
	StudentID id;
	string name;
public:
	Student(string sname = "no name", long sid = 0) :id(sid) {
		name = sname;
		cout << "学生名：" << sname << endl;
	}
	~Student() {
		cout << "删除学生名：" << name << endl;
	}

};

int main()
{
	Student ss("李明", 82020132);
	
	system("pause");
	return 0;

}
```

### this指针

注意到，前面说过，同一个类的多个对象共用一份成员函数，那么在成员函数中使用数据成员时，又如何确定使用同一个对象的呢？

在调用某个对象的成员函数时，**系统会自动的产生一个隐藏的指针， this指针，指向该对象自己。** 与python中的self作用相同，

比如：

```
void CGoods::RegisterGoods(char *name, int amount, float price){
strcpy(this->Name, name);
this->Amount=amount;
this->Price=price;
}
```

在c++中可能的实现方式：
- 系统会默认的在成员函数中增加一个附加参数-this指针

```
void CGoods::RegisterGoods(CGoods *this, char *name, int amount, float price){
strcpy(this->Name, name);
this->Amount=amount;
this->Price=price;
}
```
- 每个类成员函数的调用会在默认增加一个附加的参数 --被调用对象的地址

```
Car1.RegisterGoods(String, number,pr);

//变为：
Car1.RegisterGoods(&Car1, String, number,pr);
```

注意：
- 友元函数没有this指针，因为友元函数不是类的成员函数
- 静态成员函数没有this指针，静态成员函数在逻辑和物理上都只有一份代码，不存在this指针
- 在重载运算符的成员函数中有时需要借助this指针

### 运算符的重载

运算符重载：对常用的运算符，比如+,-,*,/等，重新定义

比如C++中没有复数类型，而想使用复数类型的+,-等，就需要重新定义这些运算符，当C++编译器碰到此类编译器时，可以调用类中定义的运算符

定义格式：
`返回值类型 类名::operator 重载的运算符(参数列表){...}`
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181223190103896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181223190222457.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

注意：
- 
运算符重载还是南湖的函数名必须为关键字operator 加一个合法的运算符。在调用该函数时，将右操作数 作为函数的实参

- 
当用类的成员函数实现运算符的重载时，运算符重载函数的参数为1个(当为双目运算符时)或没有(当为单目运算符时)。 运算符的左操作数一定是对象，因为重载的运算符是该对象的成员函数；而右操作数是该函数的参数，其类型无严格限制。c++不允许重载三目运算符。

- 
单目运算符 "++“和”–"存在前置与后置的问题。
- 前置“++”的格式：`返回类型 类名:: operator++(){...}`
- 后置“++”的格式`返回类型 类名:: operator++(int){...}`

其中，后置“++”中的参数“int”仅用作区分，并无实际意义，可以给一个变量名，也可以不给


注意：在一个运算表达式中如果有个值为const类型，会发生隐式类型转换，即式子最后的值也会const类型，有时自定义的赋值构造函数可能会失效。

```
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<stdlib.h>
#include <string>
using namespace std;


class Complex {
private: 
	double Real, Image;
public:
	//构造函数
	Complex(double r = 0.0, double i = 0.0) { Real = r; Image = i; }
	//赋值构造函数
	Complex(Complex &com) {
		Real = com.Real;
		Image = com.Image;
	}
	Complex 

	//定义重载函数
	Complex operator+( const Complex&);

};
Complex Complex::operator+( const Complex &c) {
	return Complex(Real+c.Real, Image+c.Image);//在这个地方编译器会报错，没有合适的复制构造函数，当在复制构造函数中添加const后，错误消失
	

	
}
```

实例代码：

```
#include <iostream>
#include<stdlib.h>
#include <string>
using namespace std;


class Complex {
private: 
	double Real, Image;
public:
	//构造函数
	Complex(double r = 0.0, double i = 0.0) { Real = r; Image = i; }

	void Print() {
		cout << "Real:" << Real << "  " << "Image:" << Image << endl;
	}
	//定义重载函数
	Complex operator+( const Complex&);
	Complex& operator=(Complex&);
	Complex& operator+= (Complex&);
	Complex& operator++(int);
	Complex& operator+(const double&);

};
Complex Complex::operator+( const Complex &c) {
	return Complex(Real+c.Real, Image+c.Image);
	

	
}
Complex& Complex::operator=(Complex& com) {
	Real = com.Real;
	Image = com.Image;
	return *this;
}

Complex& Complex::operator+=(Complex& com) {
	Real += com.Real;
	Image += com.Image;
	return *this;
}
Complex& Complex::operator++(int) {
	Real += 1;
	Image += 1;
	return *this;
}
Complex& Complex::operator+(const double &d) {
	Real += d;

	return *this;

}
int main()

{
	Complex a, b, c(0.1, 0.1);
	a = b = c;
	double d = 0.5;
	int ic = 1;

	a.Print();
	b.Print();
	c.Print();
	a++;
	a.Print();
	b = b + d;
	b.Print();
	c = c + 0.5;
	c.Print();
	c = c + ic;
	c.Print();
	system("pause");
	return 0;

}
```

在以上代码中需要主要的点：
- 
参数中`const Complex &com`
- `Complex &com`采用引用方式，不用重新分配内存，效率会更高
- 增加Const关键字，表示被引用的实参是不可改变的，如果在函数体中重新赋值被引用的实参，则c++编译器报错。

引用在内部存放的是被引用对象的地址，不可寻址的值是不能引用的；当引用作为形参时，实参不能是不可寻址的值，更不可能进行类型转换（比如实数转换为证书），但是const引用不同，它为只读的，为了保证绝对不会发生访问误改，**编译器使用const引用时，会生成一个临时对象，应用实际上指向该临时对象，但用户不能访问他**， 所以const可以实现不可寻址的值（比如文字常量）的引用

比如:

```
double dval=1024;
  const int &ri=dval
```

是允许的，编译器将其转换为：

```
double dval=1024;
int temp=dval;
const int &ri=temp
```

所以当const引用作为形参时，实参也可以使用不可寻址的值和进行类型转换

比如：示例代码中的

```
b = b + d;
 b.Print();
 c = c + 0.5;
 c.Print();
 c = c + ic;
 c.Print();
```

- 
返回值`Complex&`

要想实现连续操作，比如`a=b=c=d`,`(a++)++`等，必须返回引用类型，否则不成功


[[http://www.runoob.com/cplusplus/cpp-overloading.html](http://www.runoob.com/cplusplus/cpp-overloading.html)]给出了运算符重载的实例

### 友元函数(friend)

当运算符“+”的前面不是Complex的对象时怎么重载，比如`c=d+c`,显然这时候不能用成员函数来重载运算符，c++中采用友元函数来解决此问题。

**某一类的友元函数不属于该类，但可以在类外访问该类中任何成员，拥有与成员函数一样的特权， 使用friend关键字说明**

```
class Complex {
private: 
	double Real, Image;
public:
	//构造函数
	Complex(double r = 0.0, double i = 0.0) { Real = r; Image = i; }

	void Print() {
		cout << "Real:" << Real << "  " << "Image:" << Image << endl;
	}
	//定义重载函数
	Complex operator+( const Complex&);
	Complex& operator=(Complex&);
	Complex& operator+= (Complex&);
	Complex& operator++(int);
	Complex& operator+(const double&);
	friend Complex operator+(double, Complex&);

};
Complex operator+(double d, Complex& com) {
	return Complex(d + com.Real, com.Image);
}
```

注意：
- friend只在类中声明时添加，不用在实现时添加
- 友元函数不是成员函数，在类外定义时不添加`类名::`
- 友元函数不是类的成员函数，在函数体中访问对象的成员是，需添加对象名加运算符"."将对象的成员名。这和一般函数相同，但是友元函数可以访问类中所有的成员(公有，私有，受保护的)，而一般函数只能访问类中的公有函数
- 友元函数不受类中的访问权限限制，可以放在类的公有，私有，受保护部分
- 某类的友元函数的作用域并非该类域。如果该友元函数是另一个类的成员函数，则其作用域为另一类的类域，否则与一般函数相同。
- 友元函数能不用则不用

##### 友元类

整个类是另一个类的友元。友元类的**每个成员函数都是另一个类的友元函数。**

```
class A{
...
friend class B;//声明B为A的友元类
...
};
```

### 静态成员

###### 静态数据成员

用关键字static修饰的数据成员为静态数据成员。

为该类所有对象共享的一块内存区域，在编译时分配，在定义对象时不再为静态成员分配空间。

静态数据实际上为所有类对象公有，更像面向对象过程中的全局变量，是属于整个类的，而不属于类中某一特定对象。因此可以采用以下格式访问：
`类名：静态数据成员名`

用途1：对建立的对象的数量进行计数
```
#include <iostream>
#include<stdlib.h>
#include <string>
using namespace std;
class Ctest {
private:
	static int count;
public:
	Ctest() {
		++count;
		cout << "对象数量=" << count << '\n';
	}
	~Ctest() {
		--count;
		cout << "对象数量=" << count << '\n';
	}
};

int Ctest::count = 0; //A行，对静态数据的定义性说明

int main()

{
	Ctest a[3];
	system("pause");
	return 0;

}
```

注意：
- 代码中A行是对静态数据成员count做定义性说明，**在文件域中必须做一次，且只能做一次说明**，不管其为私有还是公有的， 但还是建议为私有的
- 建议不要在构造函数中为静态数据成员置初值，仅在定义性说明时一次性指定即可
- 由于静态数据成员对所有类对象只有一个，所以在物理上，一个类对象向另一个类对象按成员复制是通过依次复制**非静态数据成员**实现的。


















