# 必须掌握的C++常用关键字汇总 - yixianfeng41的专栏 - CSDN博客
2017年01月22日 14:27:55[第2梦](https://me.csdn.net/yixianfeng41)阅读数：4612标签：[union																[static																[const																[operator																[malloc](https://so.csdn.net/so/search/s.do?q=malloc&t=blog)](https://so.csdn.net/so/search/s.do?q=operator&t=blog)](https://so.csdn.net/so/search/s.do?q=const&t=blog)](https://so.csdn.net/so/search/s.do?q=static&t=blog)](https://so.csdn.net/so/search/s.do?q=union&t=blog)
个人分类：[c++](https://blog.csdn.net/yixianfeng41/article/category/5618607)
所属专栏：[C++编程](https://blog.csdn.net/column/details/14514.html)
本文将对一些常见c++关键字做一个总结，共包括如下关键字： 
const、extern、operator、sizeof、-static、new、volatile、union
## 一、const
### 1、定义常量
const修饰变量，以下两种定义形式在本质上是一样的。它的含义是：const修饰的类型为TYPE的变量value是不可变的。
```
TYPE const ValueName = value; 
const TYPE ValueName = value;
```
#### 1.1、const与宏的区别
- 宏是简单的机械替换，不会存在类型检查等工作;
- 但是const变量不一样，这个存在类型检查，所以更安全一些。
#### 1.2、const变量存储位置
- const全局变量存储在只读数据段，编译期最初将其保存在符号表中，第一次使用时为其分配内存，在程序结束时释放;  
- const局部变量存储在栈中，代码块结束时释放，例如：val_j。
### 2、指针使用CONST
#### 2.1、指针指向的值不可变
如果const位于星号的左侧，则const就是用来修饰指针所指向的变量，即指针指向为常量。
```
char const * pContent; 
const char * pContent;
```
举个贴切点的例子，如果a是一名仓库管理员的话，他所管理的仓库，里面的货物（*a）是他没有权限更改的，仓库里的东西是什么就一直是什么。因此 ：
```
int  b=100；
 const int*a=&b;
 *a=200;  //错误
```
**怎么改变值：**
- 代打。自己不去改变，但别人可改变，可以让别人去呀，最终值也是改变了；
- 换工作。自己不想干当下得了，换个工作嘛，从新指向一个新的值。
代打是什么意思了？首先回顾下指向常量的指针是什么意思。
所谓的指向常量的指针仅仅要求的是，不能通过该指针改变对象的值，没有规定此对象的值不能通过其他途径更改。自己不干坏事，但是别人你管不了啊。
也可以这样想：所谓指向常量的指针或引用，不过是指针或引用“自以为是”罢了，它们是好人，不做坏事。他们觉得指向了常量，所以自觉地不去改变所指对象的值。
因此想改变*a的值，可以这样：
```cpp
int b=100;
const int *a=&b;
b=200;
cout<<*a<<endl;  //得到200
```
换工作。要想改变*a的值还有一种方式，因为当前情况为指针指向的为常量，也就是说你指向的东西不能改变，但不代表你不能指向其它对象呀。
如果你觉得管理水果这活太累，你可以去管理蔬菜嘛，换一个仓库不就行了。
```cpp
int b=100,c=200;
const int *a=&b;
a=&c;
cout<<*a<<endl; //得到200
```
#### 2.2、指针本身是常量不可变
如果const位于星号得右侧，const就是修饰指针本身，即指针本身是常量。
`char * const pContent; `
指针本身为常量，不能对指针本身进行更改操作。
```
int b = 100,c=200;
int *const a = &b;
a = &c; //错误
cot << *a << endl;
```
举个例子，a是一名仓库管理员，分配他看守一个仓库，那就是终身制得了，他就只能控制这个仓库，要是去管其他仓库就是错误的，因此a++这种操作也是错误的；
但是，对于仓库里的东西，他是可以随便换的，想放水果就放水果，想放蔬菜就放蔬菜，（*a=200这种更改操作是允许的）。
#### 2.3、两者皆不可变
`cosnt char * const pContent;`
指针本身和所指向的内容均为常量。
```cpp
int b = 100,c=200;
const int *const a = &b;
a = &c; //错误
*a = 200;  //错误
cout << *a << endl;
```
a还是仓库管理员，这个时候，他就是地地道道的看门的了，一点权力都没有，他不能换工作，也不能动仓库里的东西。
### 3、函数中使用CONST
### 3.1、const修饰函数参数
1）传递过来的参数在函数内不可以改变
`void function(const int Var);`
如果在函数内试图修改var的值，会出现错误，“表达式必须是可修改的左值”
2）参数指针所指内容为常量不可变
`void function(const char* Var);`
同样，函数内改变指针指向的值，会出现错误，“表达式必须是可修改的左值”
3）参数指针本身为常量不可变
`void function(char* const Var);`
函数内改变指针的值，会出现错误，“表达式必须是可修改的左值”
4）参数为引用，为了增加效率同时防止修改。修饰引用参数时：  
`void function(const Class& Var); //引用参数在函数内不可以改变`
如果A是我们自定义的一个数据类型，也就是非内部数据类型，当我们按照值传递的方式传递参数参数时，效率将非常低下。因为函数将复制一个该参数的临时变量，而临时变量的构造、复制、析构都很消耗时间。
引用传递传的是地址不需要复制参数。 
但是正因为传的是地址，又存在一个隐患，参数的值有可能被我们改变。
为了解决这个问题，我们需要加上const修饰参数，因此函数最终成为void Func(const A&a);这是参数是非内部数据类型的情况，如果参数是内部类型，那就不用采用引用传递了，直接值传递就可以了，因为内部数据类型不存在构造、析构的过程，复制也很快。
#### 3.2、const 修饰函数返回值
1）以“指针传递”方式的函数返回值加const 修饰 
如果给以“指针传递”方式的函数返回值加const修饰，那么函数返回值（即指针）的内容不能被修改，该返回值只能被赋给加const 修饰的同类型指针。 
例如函数：
`const char * GetString(void);`
如下语句将出现编译错误：
`char *str = GetString();`
正确的用法是
`const char *str = GetString();`
2）以“值传递”方式的函数返回值加const修饰 
如果函数返回值采用“值传递方式”，由于函数会把返回值复制到外部临时的存储单元中，加const 修饰没有任何价值。
例如：  
`不要把函数int GetInt(void) 写成const int GetInt(void)。`
同理，  
`不要把函数A GetA(void) 写成const A GetA(void)，其中A 为用户自定义的数据类型。`
如果返回值不是内部数据类型，将函数A GetA(void) 改写为const A & GetA(void)的确能提高效率。但此时千万千万要小心，一定要搞清楚函数究竟是想返回一个对象的“拷贝”还是仅返回“别名”就可以了，否则程序会出错。   
函数返回值采用“引用传递”的场合并不多，这种方式一般只出现在类的赋值函数中，目的是为了实现链式表达。
### 4、类相关CONST
#### 4.1、const修饰成员变量
const修饰类的成员函数，表示成员常量，不能被修改，同时它只能在初始化列表中赋值。
```
class A
{ 
    …
    const int nValue;         //成员常量不能被修改
    …
    A(int x): nValue(x) { } ; //只能在初始化列表中赋值
 }
```
#### 4.2、const修饰成员函数
任何不会修改数据成员的函数都应该声明为const 类型。如果在编写const成员函数时，不慎修改了数据成员，或者调用了其它非const 成员函数，编译器将指出错误，这无疑会提高程序的健壮性。
一般写在函数的最后来修饰。
```
class A
{ 
    …
    void function()const; //常成员函数, 它不改变对象的成员变量.                        
    //也不能调用类中任何非const成员函数。
}
```
关于Const函数的几点规则： 
a. const对象只能访问const成员函数,而非const对象可以访问任意的成员函数,包括const成员函数. 
b. const对象的成员是不可修改的,然而const对象通过指针维护的对象却是可以修改的. 
c. const成员函数不可以修改对象的数据,不管对象是否具有const性质.它在编译时,以是否修改成员数据为依据,进行检查. 
e. 然而加上mutable修饰符的数据成员,对于任何情况下通过任何手段都可修改,自然此时的const成员函数是可以修改它的。  
#### 4.3、const修饰类对象/对象指针/对象引用
const修饰类对象表示该对象为常量对象，其中的任何成员都不能被修改。对于对象指针和对象引用也是一样。 
const修饰的对象，该对象的任何非const成员函数都不能被调用，因为任何非const成员函数会有修改成员变量的企图。  
### 5、将const转化为非const
采用const_cast 进行转换。 
用法：const_cast   (expression) 
## 二、extern
extern可以置于变量或者函数前，以标示变量或者函数的定义在别的文件中，提示编译器遇到此变量和函数时在其他模块中寻找其定义。此外extern也可用来进行链接指定。
### 1、作用:
- 
当它与”C”一起连用时，如: extern “C” void fun(int a, int 
b);则告诉编译器在编译fun这个函数名时按着C的规则去翻译相应的函数名而不是C++的。
- 
当extern不与”C”在一起修饰变量或函数时，如在头文件中: extern int 
g_Int;它的作用就是声明函数或全局变量的作用范围的关键字，其声明的函数和变量可以在本模块或其他模块中使用，记住它是一个声明不是定义!也就是说B模块(编译单元)要是引用模块(编译单元)A中定义的全局变量或函数时，它只要包含A模块的头文件即可,在编译阶段，模块B虽然找不到该函数或变量，但它不会报错，它会在连接时从模块A生成的目标代码中找到此函数。
### 2、注意
#### 2.1、extern “C”
在C++环境下使用C函数的时候，常常会出现编译器无法找到obj模块中的C函数定义，从而导致链接失败的情况，应该如何解决这种情况呢？
答案与分析： 
　　C++语言在编译的时候为了解决函数的多态问题，会将函数名和参数联合起来生成一个中间的函数名称，而C语言则不会，因此会造成链接时找不到对应函数的情况，此时C函数就需要用extern “C”进行链接指定，这告诉编译器，请保持我的名称，不要给我生成用于链接的中间函数名。 
#### 2.2、extern变量
在一个源文件里定义了一个数组：char a[6]; 
在另外一个文件里用下列语句进行了声明：extern char *a；请问，这样可以吗？   
答案与分析： 
不可以，程序运行时会告诉你非法访问。原因在于，指向类型T的指针并不等价于类型T的数组。extern char *a声明的是一个指针变量而不是字符数组，因此与实际的定义不同，从而造成运行时非法访问。应该将声明改为extern char a[ ]。
## 三、operator
重载的运算符是具有特殊名字的函数，他们的名字由关键字operator和其后要定义的运算符共同构成。和其他函数一样，重载的运算符也包含返回类型、参数列表以及函数体。重载有两种方式：成员函数和友元函数。成员函数的形式比较简单，就是在类里面定义了一个与操作符相关的函数。友元函数因为没有this指针，所以形参会多一个。
语法形式：
```
函数类型 operator 运算符（形参表） 
{ 
  函数体； 
}
```
### 1、运算符重载
#### 1.1、关系运算符重载
关系运算符有==,!=,<,>,<=,>=,一般定义为非成员函数。
```
bool operator == (const A&a1, const A&a2); 
bool operator != (const A&a1, const A&a2);
bool operator < (const A&a1, const A&a2);
bool operator <= (const A&a1, const A&a2 );
bool operator > (const A&a1, const A&a2 );
bool operator >= (const A&a1, const A&a2);
```
#### 1.2、单目运算符重载
```
A& operator + (const A&a1, const A&a2);
A& operator - (const A&a1, const A&a2);
A* operator & (const A&a1, const A&a2);
A& operator * (const A&a1, const A&a2);
```
#### 1.3、自增减运算符
注意前置和后置的区分，因为形式都一样，所以后置带个参数作为区分。
```
A& operator ++ ();//前置++
A operator ++ (int);//后置++
A& operator --();//前置--
A operator -- (int);//后置--
```
#### 1.4、位运算符重载
```
A operator | (const A&a1, const A&a2);
A operator & (const A&a1, const A&a2);
A operator ^ (const A&a1, const A&a2 );
A operator << (int i);
A operator >> (int i);
A operator ~ ();
```
#### 1.5、赋值运算符重载
这里的赋值运算符指的是改变对象状态的运算符
```
A& operator += (const A& );
A& operator -= (const A& ); 
A& operator *= (const A& );
A& operator /= (const A& );
A& operator %= (const A& );
A& operator &= (const A& );
A& operator |= (const A& );
A& operator ^= (const A& );
A& operator <<= (int i);
A& operator >>= (int i);
```
#### 1.6、内存运算符
```cpp
void *operator new(size_t size);
void *operator new(size_t size, int i);
void *operator new[](size_t size);
void operator delete(void*p);
void operator delete(void*p, int i, int j);
void operator delete [](void* p);
```
#### 1.7、输入输出运算符
与标准库兼容的输入输出运算符必须是非成员函数，而为了能访问类的私有对象，应申明为友元函数。因为如果不这样的话，他们的左侧运算符对象将是一个类对象，这显然不行。
```cpp
friend inline ostream &operator << (ostream&, A&);//输出流
friend inline istream &operator >> (istream&, A&);//输入流
```
#### 1.8、不可重载的运算符
1） . (点符号) 
2）.* 
3）:: 
4）?: 
5）sizeof
#### 1.9、不应该重载的运算符
有些运算符可以被重载，但是重载了之后对我们的使用就不太方便了。 
1）对象求值顺序有关的运算符：逻辑与（&&）、逻辑或（||）； 
2）特殊含义的运算符：逗号运算符、取地址运算符。
#### 1.10、注意
1）内置类型的运算符无法重载； 
2）只能重载已有运算符，不能发明新的运算符； 
3）运算符定义成成员函数时，它的左侧运算符对象必须是运算符所属类的一个对象。 
例如(string的成员‘+’）； 
4）重载不改变优先级与结合性。
```
string s="asfs";
tring t=s+"sa"; //ok
string u="hi"+s;  //error
```
### 2、重载的运算符是成员还是非成员函数？
定义重载运算符时，我们需要选择一下是将其定义为成员运算符还是非成员运算符，有些运算符时必须定义为成员函数的。同时重载运算符作为成员函数和非成员函数时，它的参数也是不一样的。通常一元运算符有一个参数，二元运算符有两个参数，但是当重载预案算符是成员函数时，this绑定到左侧运算符对象，成员运算符参数减一。 
#### 2.1、抉择：
1）赋值，下标，调用，和成员访问**必须是成员**； 
2）复合赋值运算符**一般来说是成员**； 
3）改变对想状态的运算符或者给指定类型没切相关的运算符，如递增、递减和解引用等 ，**通常应该是成员**； 
4）具有对称性的运算符可能转换为任意一端的运算符对象，例如算数、相等性、关系和位运算符d，应该是**非成员函数**； 
5）输入输出运算符应该是**非成员函数**。
#### 2.2、举例：
(a) %  (b) %=  (c) ++  (d) -> (e) << (f) && (g) == (h) ()  
(a) %通常定义为非成员。
(b) %=通常定义为类成员，因为它会改变对象的状态。
(c) ++通常定义为类成员，因为它会改变对象的状态。
(d) ->必须定义为类成员，否则编译报错
(e) <<通常定义为非成员
(f) && 通常定义为非成员。
(g) ==通常定义为非成员。
(h) ()必须定义为类成员，否则编译会报错。
### 3、实例
通过自定义一个String类，名叫MyString,实现一些常用的运算符重载。
#### 3.1、代码
```cpp
class MyString
{
    friend ostream &operator<<(ostream &os, MyString&mstr);
    friend istream&operator>>(istream &is, MyString&mstr);
    friend MyString &operator+(MyString &str1, MyString&str2);
    friend bool operator==(MyString &str1, MyString&str2);
    friend bool operator!=(MyString &str1, MyString&str2);
public:
    //构造函数  
    MyString():length(0),size(0),mdata(NULL){}
    MyString(const char *str);
    //拷贝构造函数  
    MyString(const MyString &other);
    //赋值运算符  
    MyString &operator=(const MyString &other);
    //析构函数  
    ~MyString();
    char&  operator[](int index);
    MyString &operator+=(const MyString &str);
private:
    int length;
    int size;
    char *mdata;
};
//构造函数  
MyString::MyString(const char *str)
{
    if (str != NULL)
    {
        length= strlen(str);
        size = length + 1;
        mdata = new char[size];
        strcpy_s(mdata,size, str);
    }
}
//拷贝构造函数  
MyString::MyString(const MyString &other)
{
    length = strlen(other.mdata);
    size = length + 1;
    mdata = new char[size];
    strcpy_s(mdata, size, other.mdata);
}
//赋值运算符  
MyString & MyString::operator=(const MyString &other)
{
    //检查自赋值  
    if (this != &other)
    {
        delete[]mdata;
        //分配新的资源，并复制内容  
        length = strlen(other.mdata);
        size = length + 1;
        mdata = new char[size];
        strcpy_s(mdata, size, other.mdata);
    }       
    return *this;
}
//析构函数  
MyString::~MyString()
{
    if (mdata != NULL)
    {
        delete[]mdata;
        mdata = NULL;
    }
}
ostream &operator<<(ostream &os, MyString&mstr)
{
    os << mstr.mdata;
    return os;
}
istream&operator>>(istream &is, MyString&mstr)
{
    char *temp=new char[1024];
    gets_s(temp, strlen(temp));
    if (mstr.mdata != NULL)
        delete[]mstr.mdata;
    mstr.length = strlen(temp);
    mstr.size = mstr.length + 1;
    mstr.mdata = new char[mstr.size];
    strcpy_s(mstr.mdata, mstr.size, temp);
    return is;
}
MyString &MyString::operator+=(const MyString &str)
{
    this->length = this->length + str.length;
    this->size = this->length + 1;
    char* buffer = new char[this->size];
    strcpy_s(buffer, this->size, this->mdata);
    strcat_s(buffer, this->size, str.mdata);
    delete[] this->mdata;
    this->mdata = buffer;
    return *this;
}
MyString &operator+(MyString &str1, MyString&str2)
{
    MyString *temp = new MyString();
    temp->length = str1.length + str2.length;
    temp->size = temp->length + 1;
    temp->mdata = new char[temp->size];
    strcpy_s(temp->mdata, temp->size, str1.mdata);
    strcat_s(temp->mdata, temp->size, str2.mdata);
    return *temp;
}
bool operator==(MyString &str1, MyString&str2)
{
    if (str1.size != str2.size) return false;
    else if (strcmp(str1.mdata, str2.mdata) != 0)
        return false;
    return true;
}
bool operator!=(MyString &str1, MyString&str2)
{
    return !(str1 == str2);
}
char& MyString::operator[](int index)
{
    return this->mdata[index];
}
```
#### 3.2、注意
1)赋值运算符
如果没有赋值运算符，那么以下情况就会出现内存泄露和double free问题，其实这就是深拷贝与浅拷贝的问题：
```bash
MyString str1("test1");
MyString str2("test2");
str1=str2;
```
因为str2的mdata和str1的mdata指向同一块内存，那么销毁对象时就会释放两次，还有str2自己的内存没发释放。因此，必须要有自己定义的赋值运算符。
2)本例子那个输入运算符并不好，需要更优解决方案。
### 4、总结
1、我们要注意哪些运算符可重载，那些不可重载；哪些可被重载，但是一般情况下不应该重载 
2、重载时，我们应该申明为成员函数，还是非成员函数，两者有什么区别。 
3、对于输入输出运算符要注意，判断流是否成功，并且输入时，内存的申请等。
## 四、sizeof
sizeof是一个操作符（operator）。 
其作用是返回一个对象或类型所占的内存字节数。 
其返回值类型为size_t。（size_t在头文件stddef.h中定义，它依赖于编译系统的值，一般定义为 typedef unsigned int size_t;）
### 1、语法
sizeof有三种语法形式： 
1. sizeof (object);  //sizeof (对象) 
1. sizeof object;   //sizeof 对象 
1. sizeof (type_name);  //sizeof (类型)  
对象可以是各种类型的变量，以及表达式（一般sizeof不会对表达式进行计算）。 
sizeof对对象求内存大小，最终都是转换为对对象的数据类型进行求值。 
sizeof (表达式); //值为表达式的最终结果的数据类型的大小  
例子：（32位机器下）
```
int i;  
sizeof(int); //值为4  
sizeof(i); //值为4，等价于sizeof(int)  
sizeof i; //值为4  
sizeof(2); //值为4，等价于sizeof(int)，因为2的类型为int  
sizeof(2 + 3.14); //值为8，等价于sizeof(double)，因为此表达式的结果的类型为double
```
`char ary[sizeof(int) * 10]; //OK，编译无误`
最新的C99标准规定sizeof也可以在运行时刻进行计算。 
如下面的程序在Dev-C++中可以正确执行：
```cpp
int n;  
n = 10; // n动态赋值  
char ary[n]; // C99也支持数组的动态定义  
cout<<sizeof(ary); // ok. 输出10
```
但在没有完全实现C99标准的编译器中就行不通了，上面的代码在VC6中就通不过编译。所以我们最好还是认为sizeof是在编译期执行的，这样不会带来错误，让程序的可移植性强些。
### 2、基本数据类型的sizeof
这里的基本数据类型是指short、int、long、float、double这样的简单内置数据类型。 
由于它们的内存大小是和系统相关的，所以在不同的系统下取值可能不同。
### 3、结构体的sizeof
结构体的sizeof涉及到字节对齐问题。
为什么需要字节对齐？计算机组成原理教导我们这样有助于加快计算机的取数速度，否则就得多花指令周期了。为此，编译器默认会对结构体进行处理（实际上其它地方的数据变量也是如此），让宽度为2的基本数据类型（short等）都位于能被2整除的地址上，让宽度为4的基本数据类型（int等）都位于能被4整除的地址上，依次类推。这样，两个数中间就可能需要加入填充字节，所以整个结构体的sizeof值就增长了。  
字节对齐的细节和编译器的实现相关，但一般而言，满足三个准则： 
1. 结构体变量的首地址能够被其最宽基本类型成员的大小所整除。 
1. 结构体的每个成员相对于结构体首地址的偏移量（offset）都是成员大小的整数倍，如有需要，编译器会在成员之间加上填充字节（internal adding）。 
1. 结构体的总大小为结构体最宽基本类型成员大小的整数倍，如有需要，编译器会在最末一个成员后加上填充字节（trailing padding）。  
注意：空结构体（不含数据成员）的sizeof值为1。试想一个“不占空间“的变量如何被取地址、两个不同的“空结构体”变量又如何得以区分呢，于是，“空结构体”变量也得被存储，这样编译器也就只能为其分配一个字节的空间用于占位了。 
例子：
```
struct S1  
{  
    char a;  
    int b;  
};  
sizeof(S1); //值为8，字节对齐，在char之后会填充3个字节。  
struct S2  
{  
    int b;  
    char a;  
};  
sizeof(S2); //值为8，字节对齐，在char之后会填充3个字节。  
struct S3  
{  
};  
sizeof(S3); //值为1，空结构体也占内存。
```
### 4、联合体的sizeof
结构体在内存组织上市顺序式的，联合体则是重叠式，各成员共享一段内存；所以整个联合体的sizeof也就是每个成员sizeof的最大值。
例子：
```
union u  
{  
    int a;  
    float b;  
    double c;  
    char d;  
};  
sizeof(u); //值为8
```
### 5、数组的sizeof
数组的sizeof值等于数组所占用的内存字节数。 
注意： 
1）当字符数组表示字符串时，其sizeof值将’/0’计算进去。 
2）当数组为形参时，其sizeof值相当于指针的sizeof值。   
例子1：   
```cpp
char a[10];  
char n[] = "abc";   
cout<<"char a[10]                 "<<sizeof(a)<<endl;//数组，值为10  
cout<<"char n[] = /"abc/"           "<<sizeof(n)<<endl;//字符串数组，将'/0'计算进去，值为4
```
例子2：
```
void func(char a[3])  
{  
    int c = sizeof(a); //c = 4，因为这里a不在是数组类型，而是指针，相当于char *a。  
}  
void funcN(char b[])  
{  
    int cN = sizeof(b); //cN = 4，理由同上。  
}
```
### 6、 指针的sizeof
指针是用来记录另一个对象的地址，所以指针的内存大小当然就等于计算机内部地址总线的宽度。 
在32位计算机中，一个指针变量的返回值必定是4。 
指针变量的sizeof值与指针所指的对象没有任何关系。 
例子：
```cpp
char *b = "helloworld";  
char *c[10];  
double *d;  
int **e;  
void (*pf)();    
cout<<"char *b = /"helloworld/"     "<<sizeof(b)<<endl;//指针指向字符串，值为4  
cout<<"char *b                    "<<sizeof(*b)<<endl; //指针指向字符，值为1  
cout<<"double *d                  "<<sizeof(d)<<endl;//指针，值为4  
cout<<"double *d                  "<<sizeof(*d)<<endl;//指针指向浮点数，值为8  
cout<<"int **e                  "<<sizeof(e)<<endl;//指针指向指针，值为4  
cout<<"char *c[10]                "<<sizeof(c)<<endl;//指针数组，值为40  
cout<<"void (*pf)();              "<<sizeof(pf)<<endl;//函数指针，值为4
```
### 7、函数的sizeof
sizeof也可对一个函数调用求值，其结果是函数返回值类型的大小，函数并不会被调用。 
对函数求值的形式：sizeof(函数名(实参表))
注意： 
1）不可以对返回值类型为空的函数求值。 
2）不可以对函数名求值。 
3）对有参数的函数，在用sizeof时，须写上实参表。
例子：
```cpp
#include <iostream>  
using namespace std;  
float FuncP(int a, float b)  
{  
    return a + b;  
}  
int FuncNP()  
{  
    return 3;  
}  
void Func()  
{  
}  
int main()  
{  
cout<<sizeof(FuncP(3, 0.4))<<endl; //OK，值为4，sizeof(FuncP(3,0.4))相当于sizeof(float)  
cout<<sizeof(FuncNP())<<endl; //OK，值为4，sizeof(FuncNP())相当于sizeof(int)  
/*cout<<sizeof(Func())<<endl; //error，sizeof不能对返回值为空类型的函数求值*/  
/*cout<<sizeof(FuncNP)<<endl; //error，sizeof不能对函数名求值*/  
}
```
## 五、static
static的作用可以归结为三条：
#### 1、隐藏
当我们同时编译多个文件时，所有未加static前缀的全局变量和函数都具有全局可见性。加了static后，变量或函数就只在本源文件中可见。  
为理解这句话，我举例来说明。我们要同时编译两个源文件，一个是a.c，另一个是main.c。
下面是a.c的内容
```cpp
char a = 'A'; // global variable
void msg() 
{
    printf("Hello\n"); 
}
```
下面是main.c的内容
```cpp
int main(void)
{    
    extern char a;    // extern variable must be declared before use
    printf("%c ", a);
    (void)msg();
    return 0;
}
```
程序的运行结果是：
`A Hello`
你可能会问：为什么在a.c中定义的全局变量a和函数msg能在main.c中使用？前面说过，所有未加static前缀的全局变量和函数都具有全局可见性，其它的源文件也能访问。此例中，a是全局变量，msg是函数，并且都没有加static前缀，因此对于另外的源文件main.c是可见的。
如果加了static，就会对其它源文件隐藏。例如在a和msg的定义前加上static，main.c就看不到它们了。利用这一特性可以在不同的文件中定义同名函数和同名变量，而不必担心命名冲突。Static可以用作函数和变量的前缀，对于函数来讲，static的作用仅限于隐藏，而对于变量，static还有下面两个作用。
#### 2、保持变量内容的持久。
存储在静态数据区的变量会在程序刚开始运行时就完成初始化，也是唯一的一次初始化。共有两种变量存储在静态存储区：全局变量和static变量，只不过和全局变量比起来，static可以控制变量的可见范围，说到底static还是用来隐藏的。虽然这种用法不常见，但我还是举一个例子。
```
#include <stdio.h>
int fun(void){
    static int count = 10;    
    return count--;
}
int count = 1;
int main(void)
{    
    printf("global\t\tlocal static\n");
    for(; count <= 10; ++count)
        printf("%d\t\t%d\n", count, fun());    
    return 0;
}
```
程序的运行结果是：
global          local static
1               10
2               9
3               8
4               7
5               6
6               5
7               4
8               3
9               2
10              1
#### 3、默认初始化为0
其实全局变量也具备这一属性，因为全局变量也存储在静态数据区。在静态数据区，内存中所有的字节默认值都是0x00，某些时候这一特点可以减少程序员的工作量。比如初始化一个稀疏矩阵，我们可以一个一个地把所有元素都置0，然后把不是0的几个元素赋值。如果定义成静态的，就省去了一开始置0的操作。再比如要把一个字符数组当字符串来用，但又觉得每次在字符数组末尾加’\0’太麻烦。如果把字符串定义成静态的，就省去了这个麻烦，因为那里本来就是’\0’。不妨做个小实验验证一下。
```
#include <stdio.h>
int a;
int main(void)
{
    int i;
    static char str[10];
    printf("integer: %d;  string: (begin)%s(end)", a, str);
    return 0;
}
```
程序的运行结果如下
`integer: 0; string: (begin)(end)`
#### 总结：
首先static的最主要功能是隐藏，其次因为static变量存放在静态存储区，所以它具备持久性和默认值0。
## 六、malloc与new
一般我们常用的申请内存的方法有new与malloc,new与delete配对使用，malloc与free配对使用。
### 1、 malloc
#### 1.1、malloc函数定义
函数malloc 的原型如下：  
```cpp
void * malloc(size_t size);
```
函数free 的原型如下：    
```cpp
void free( void * memblock );
```
为什么free 函数不象malloc 函数那样复杂呢？ 
这是因为指针p 的类型以及它所指的内存的容量事先都是知道的，语句free(p)能正确地释放内存。    
如果p 是NULL 指针，那么free对p 无论操作多少次都不会出问题；如果p 不是NULL 指针，那么free 对p连续操作两次就会导致程序运行错误。
#### 1.2、 malloc例子
```cpp
/* malloc example: random string generator*/
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */
int main ()
{
  int i,n;
  char * buffer;
  printf ("How long do you want the string? ");
  scanf ("%d", &i);
  buffer = (char*) malloc (i+1);
  if (buffer==NULL) exit (1);
  for (n=0; n<i; n++)
    buffer[n]=rand()%26+'a';
  buffer[i]='\0';
  printf ("Random string: %s\n",buffer);
  free (buffer);
  return 0;
}
```
### 2、new
new一般与delete配对使用，new申请内存，delete释放内存。值得注意的是，我们要区分好new、operator new
#### 2.1、new
不能被重载，其行为总是一致的。它先调用operator new分配内 存 ， 然后调用构造 函数初始化那段内存。
new 操作符的执行过程： 
1. 调用operator new分配内存 ； 
2. 调用构造函数生成类对象； 
3. 返回相应指针。
#### 2.2、operator new
要实现不同的内存分配行为，应该重载operator new，而不是new。
operator new就像operator + 一样，是可以重载的。如果类中没有重载operator new，那么调用的就是全局的::operator new来完成堆的分配。
operator delete operator new[] operator delete[]也是可以重载的。
C++98
```cpp
throwing (1)    void* operator new (std::size_t size) throw (std::bad_alloc);
nothrow (2) void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) throw();
placement (3)   void* operator new (std::size_t size, void* ptr) throw();
```
C++11
```cpp
throwing (1)    void* operator new (std::size_t size);
nothrow (2) void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) noexcept;
placement (3)   void* operator new (std::size_t size, void* ptr) noexcept;
```
throwing allocation 
 分配size的内存，如果成功返回一个指向内存首地址的指针，如果失败抛出异常。
nothrow allocation 
其他同上，只是在失败时，返回null指针而不是抛出异常。
placement  
placement new只是operator new重载的一个版本, 它并不分配内存， 只是返回指向 已经分配好的某段内存的一个指针。因此不能删除它，但需要调用对象的析构函数。
placement new也就是允许你在一个已经分配好的内存中（栈或者堆中）构造一个新的对象。原型中void* ptr实际上就是指向一个已经分配好的内存缓冲区的的首地址。
placement 使用步骤： 
1）缓存提前分配 
在堆上（new）：  
`char *buffer=new [100];`
在栈上（数组）：  
`char buffer[100];`
直接使用通过地址 ： 
```cpp
void* buffer = reinterpret_cast<void*> (0xB0EF);
```
2）构造对象 
在已分配的缓存上用placement new来构造对象。
`A *a=new (buffer)A;`
3)使用对象a
4)对象的析构：调用它的析构函数
`a->~A();`
5)释放 
可以反复利用已分配好的内存buffer,如果用完了，就需要释放它
`delete[]buffer;`
#### 2.3、例子
```cpp
// operator new example
#include <iostream>     // std::cout
#include <new>          // ::operator new
struct MyClass {
  int data[100];
  MyClass() {std::cout << "constructed [" << this << "]\n";}
};
int main () {
  std::cout << "1: ";
  MyClass * p1 = new MyClass;
      // allocates memory by calling: operator new (sizeof(MyClass))
      // and then constructs an object at the newly allocated space
  std::cout << "2: ";
  MyClass * p2 = new (std::nothrow) MyClass;
      // allocates memory by calling: operator new (sizeof(MyClass),std::nothrow)
      // and then constructs an object at the newly allocated space
  std::cout << "3: ";
  new (p2) MyClass;
      // does not allocate memory -- calls: operator new (sizeof(MyClass),p2)
      // but constructs an object at p2
  // Notice though that calling this function directly does not construct an object:
  std::cout << "4: ";
  MyClass * p3 = (MyClass*) ::operator new (sizeof(MyClass));
      // allocates memory by calling: operator new (sizeof(MyClass))
      // but does not call MyClass's constructor
  delete p1;
  delete p2;
  delete p3;
  return 0;
}
```
可能结果：
```
1: constructed [0x8f0f70]
2: constructed [0x8f23a8]
3: constructed [0x8f23a8]
4:
```
### 3、区别
1）所属语言 
new是c++特性，malloc是c的。c++一般用的是new，但也可以用malloc，而c用malloc，relloc、calloc。
2）申请释放方式 
new与delete,malloc与free配对使用。new的使用比malloc简单很多，因为它内部已经实现了大小计算，类型转换等工作；而malloc使用时需要计算大小以及进行类型转换。
3）malloc标准库函数，new是C++的运算符。 
new可以被重载，malloc不可以；malloc需要函数库支持，new不需要。
4）构造与析构 
new会自动调用构造函数，delete时会自动调用析构函数；但是malloc不行。由于malloc、free 是库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加malloc、free。
5）申请内存失败 
申请内存失败时，默认new抛出异常，malloc返回NULL
6）重新分配内存 
malloc可利用relloc重新分配内存，new不行
7）类型安全性 
new会检查类型是否对应，如果不对应会保存，但是malloc只关注申请内存的多少，不会替你检查类型
8）类型转换 
malloc 返回值的类型是void *，所以在调用malloc 时要显式地进行类型转换，将void * 转换成所需要的指针类型；new不需要。 
9）数组分配 
new有明确的方式处理数组的分配，即new[],释放时delete[],malloc没有。
10）设置内存分配器 
new可以设置自己的内存分配器；malloc不行。
## 七、volatile
volatile 变量是随时可能发生变化的，与volatile变量有关的运算，不要进行编译优化，以免出错
对于一般变量：为提高存取速度，编译器优化时有时会先把变量读取到一个寄存器中。以后再取变量值时，就直接从寄存器中取值。
一个定义为volatile的变量是说这变量可能会被意想不到地改变，这样，编译器就不会去假设这个变量的值了。精确地说就是，优化器在用到这个变量时必须每次都小心地重新读取这个变量的值，而不是使用保存在寄存器里的备份。
**一个参数既可以是const还可以是volatile吗？解释为什么**
是的。一个例子是只读的状态寄存器。它是volatile因为它可能被意想不到地改变。它是const因为程序不应该试图去修改它。
应用 
1、中断服务程序中修改的供其它程序检测的变量需要加volatile； 
2、多任务环境下各任务间共享的标志应该加volatile； 
3、存储器映射的硬件寄存器通常也要加volatile说明，因为每次对它的读写都可能由不同意义；
## 八、union
“联合”是一种特殊的类，也是一种构造类型的数据结构。在一个“联合”内可以定义多种不同的数据类型， 一个被说明为该“联合”类型的变量中，允许装入该“联合”所定义的任何一种数据，这些数据共享同一段内存，已达到节省空间的目的（还有一个节省空间的类型：位域）。 这是一个非常特殊的地方，也是联合的特征。另外，同struct一样，联合默认访问权限也是公有的，并且，也具有成员函数。
### 1、共用体类型数据的特点
1、使用共用体变量的目的是希望在同一个内存段存放几种不同类型的数据。注意：在每一瞬时只能存放其中一种，而不是同时存放多种。换句话说，每一瞬时只有一个成员起作用，其他成员不起作用。
2、能够访问的是共用体变量中最后一次被赋值的成员，在对一个新的成员赋值后原有的成员就失去作用。因此在引用共用体变量时应注意当前在共用体变量中起作用的是哪个成员。
3、共同体变量的地址和它的各成员的地址相同，为同一个地址。
4、不能对共用体变量名赋值；不能引用变量名来得到一个值；不能在定义共用体变量时对它初始化；不能用共用体变量名作为函数参数。
### 2、联合所占内存多少
联合是按=最大需求分配内存的，也就是内存等于其内部最大对象内存。
### 3、union相关题目：
```cpp
union
{
    int i;
    char x[2];
}a;
int main()
{
    a.x[0] = 10;
    a.x[1] = 1;
    cout << a.i<<endl;
    return 0;
}
```
这道题把握整懵逼了，困惑有下： 
1）把10赋给char是干啥，可以这么干吗？ 
2）你赋给x数组了，这i是多好，还真是有些不好确定。 
虽然我大致知道union共用一块内存（由union中变量字节最大的决定），每次只有一个值有效。可是这道题还是懵逼。
后来看《unix网络编程》，上面也出现了类似代码，被用来测试是大端，还是小端存储。
所谓的大端模式，是指数据的低位保存在内存的高地址中，而数据的高位，保存在内存的低地址中；
所谓的小端模式，是指数据的低位保存在内存的低地址中，而数据的高位保存在内存的高地址中。
对于大小端，可以看看这篇文章，写的不错： [大端小端格式详解](http://blog.csdn.net/zhaoshuzhaoshu/article/details/37600857/)
解释疑问： 
1）0-255数字都代表了相应的字符。 
2）对于union，不管你调用那个变量，它都是去他们共用的那块内存读取。 
3）我们一般用的windows x86 是小端模式的，也就是低字节在低地址处，高字节在高地址处。 
看看运行后，内存中的情况，以及i的值： 
![这里写图片描述](https://img-blog.csdn.net/20170209155412014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWl4aWFuZmVuZzQx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
i=266
看来确实是小端存储，那么i就是010a,也就是 0000 0001 0000 1010，即266.
## 九、总结
不以知识小而不学，日积跬步，必至千里，加油！
