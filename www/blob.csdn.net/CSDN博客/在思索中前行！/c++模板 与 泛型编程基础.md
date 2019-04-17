# c++模板 与 泛型编程基础 - 在思索中前行！ - CSDN博客





2015年03月30日 13:41:12[_Tham](https://me.csdn.net/txl16211)阅读数：984








## C++模板


泛型编程就是以独立于任何特定类型的方式编写代码，而模板是泛型编程的基础。

**（1）定义函数模板（function template）**


函数模板是一个独立于类型的函数，可以产生函数的特定类型版本。



```cpp
// implement strcmp-like generic compare function
template <typename T>
int compare(const T &v1, const T &v2)
{
    if (v1 < v2) return -1;
    if (v2 < v1) return 1;
    return 0;
}
```




模板定义以关键字template开始，后接尖括号括住的模板形参表。





模板形参可以是表示类型的类型形参（type parameter），也可以是表示常量表达式的非类型形参（nontype parameter）。上面程序中的T是类型形参。


```
```cpp
// compiler instantiates int compare(const int&, const int&)
cout << compare(1, 0) << endl;
// compiler instantiates int compare(const string&, const string&)
string s1 = “hi”, s2 = “world”;
cout << compare(s1, s2) << endl;
```
```

使用函数模板时，编译器会将模板实参绑定到模板形参。编译器将确定用什么类型代替每个类型形参，用什么值代替每个非类型形参，然后产生并编译（称为实例化）该版本的函数。



上面的例子中，编译器用int代替T创建第一个版本，用string代替T创建第二个版本。


函数模板也可以声明为inline。


```
// inline specifier follows template parameter list
template <typename T> inline T min(const T&, const T&);
```

**（2）定义类模板（class template）**



在定义的类模板中，使用模板形参作为类型或值的占位符，在使用类时再提供具体的类型或值。



```cpp
template <typename Type> 
class Queue
{
public:
    Queue();
    Type & front();
    const Type & front() const;
    void push(const Type &);
    void pop();
    bool empty() const;
private:
    // …
};
```


与调用函数模板不同，使用类模板时，必须为模板形参显示指定实参。



```
Queue<int> qi; // Queue that holds ints
Queue<string> qs; // Queue that holds strings
```

**（3）模板类型形参**



类型形参由关键字class或typename后接说明符构成。在函数模板形参表中，二者含义相同。typename其实比class更直观，更清楚的指明后面的名字是一个类型名（包括内置类型），而class很容易让人联想到类声明或类定义。


此外，在使用嵌套依赖类型（nested depended name）时，必须用到typename关键字。


在类的内部可以定义类型成员。如果要在函数模板内部使用这样的类型，必须显示告诉编译器这个名字是一个类型，否则编译器无法得知它是一个类型还是一个值。默认情况下，编译器假定这样的名字指定（静态）数据成员，而不是类型。所以下面这段程序，如果去掉typename关键字，将会出现编译错误。


```
template <typename Parm, typename U>
Parm fcn(Parm *array, U value)
{
    typename Parm::size_type * p;
}
```

**（4）非类型模板形参**



模板形参也可以是非类型形参，在使用时非类型形参由常量表达式代替。



```cpp
// initialize elements of an array to zero
template <typename T, size_t N>
void array_init(T (&parm)[N])
{
    for (size_t i = 0; i != N; ++i)
        parm[i] = 0;
}
…
int x[42];
double y[10];
array_init(x); // instantiates array_init(int (&)[42])
array_init(y); // instantiates array_init(double (&)[10])
```


**（5）编写泛型程序**



模板代码需要对使用的类型做一些假设，比如上面的compare()要求类型T重载了“<”操作符。所以函数模板内部完成的操作就限制了可用于实例化该函数的类型。


编写模板代码时，对实参类型的要求应尽可能少。比如compare()函数仅使用了“<”操作符，而没有使用“>”操作符。


【学习资料】 《c++ primer》



## **泛型的基本思想：**

泛型编程（Generic Programming）是一种语言机制，通过它可以实现一个标准的容器库。

像类一样，泛型也是一种抽象数据类型，但是泛型不属于面向对象，它是面向对象的补充和发展。

在面向对象编程中，当算法与数据类型有关时，面向对象在对算法的抽象描述方面存在一些缺陷。

比如对栈的描述：

class stack

{




   push(参数类型)  //入栈算法


   pop(参数类型)   //出栈算法


}

如果把上面的伪代码看作算法描述，没问题，因为算法与参数类型无关。但是如果把它写成可编译的源代码，

就必须指明是什么类型，否则是无法通过编译的。使用重载来解决这个问题，即对N种不同的参数类型写N个

push和pop算法，这样是很麻烦的，代码也无法通用。

若对上面的描述进行改造如下：

首先指定一种通用类型T，不具体指明是哪一种类型。

class stack<参数模板 T>

{


   push(T)  //入栈算法


   pop(T)   //出栈算法


}


这里的参数模板T相当于一个占位符，当我们实例化类stack时，T会被具体的数据类型替换掉。

若定义对象S为statc类型，在实例化S时若我们将T指定int型则：

这时候类S就成为：

class S

{

    push(int)  //入栈算法

    pop(int)   //出栈算法

}

这时我可以称class stack<参数模板 T>是类的类，通过它可以生成具体参数类型不同的类。
**泛型在C++中的应用：**泛型在C++中的主要实现为模板函数和模板类。

通常使用普通的函数实现一个与数据类型有关的算法是很繁琐的，比如两个数的加法，要

考虑很多类型：

int add(int a,int b) { return a+b; }

float add(float a,float b) { return  a+b; }

。。。。

虽然在C++中可以通过函数重载来解决这个问题，但是反复写相同算法的函数是比较辛苦的，

更重要的是函数重载是静态编译，运行时占用过多内存。

在此我们可以用C++的模板函数来表达通用型的函数，如下：

template<typename T> // 模板声明

T add(T a,T b) { return a+b; }  // 注意形参和返回值的类型

这时C++编译器会根据add函数的参数类型来生成一个与之对应的带具体参数类型的函数并

调用。

例如：


```cpp
#include <iostream>
using namespace std;
template <typename T>
T add(T a,T b)  //注意形参和返回类型
{   
 return a+b;
} 
void main()
{
    int num1, num2, sum; 
    cin>>num1>>num2;
    sum=add(num1,num2); //用int匹配模版参数T，若sum,num1,num2类型不一致则无法匹配。
    cout<<sum;
}
```

### **函数模板的性质**

1) 函数模板并不是真正的函数，它只是C++编译生成具体函数的一个模子。

2) 函数模板本身并不生成函数，实际生成的函数是替换函数模板的那个函数，比如上例中的add(sum1,sum2)，

    这种替换是编译期就绑定的。

3) 函数模板不是只编译一份满足多重需要，而是为每一种替换它的函数编译一份。

4) 函数模板不允许自动类型转换。

5) 函数模板不可以设置默认模板实参。比如template <typename T=0>不可以。
**C++模版函数的语法**template  <typename 模版参数列表…>

函数返回类型 函数名（形参列表…）

上面两行可以合并成一行。

例如：

下面的几种写法是等效的并且class 和typename是可以互换的。

```cpp
template  <typename T1, typename T2>
T1 fun(T1, T2, int )
{  //…..}
template  <typename T1,T2>  T1 fun(T1, T2, int )
{  //…..}
template  <class T1, class T2>
 T1 fun(T1, T2, int )
{  //…..}
template  <class T1,T2>  T1 fun(T1, T2, int )
{  //…..}
```
**C++模版类的语法**
```cpp
template  <class 模版参数列表…>
class 类名
{ //类体}
成员的实现…
例如：
//类声明部分，有两个模板参数T1，T2
template  <class T1, class T2 >  
class A {
   private:
   int a;
  T1 b;  //成员变量也可以用模板参数
  public: 
  int fun1(T1 x, int y );
 T2 fun2(T1 x, T2 y);
}
//类实现部分
template  <class T1, class T2 >
int A<T1>:: fun1(T1 x, int y ){//实现…… }
 template  <class T1, class T2 >
T2 A<T1,T2>:: fun2(T1 x, T2 y) {//实现…… }
 //使用类A
 int main( ) {
 //定义对象a,并用int替换T1, float替换T2
   A<int, float>  a;
   //实例化a,调用a的属性和方法……
}
```


由上例可以看出, 类模板参数T1,T2对类的成员变量和成员函数均有效。

在C++编程中，当你要实现的一个类的某些成员函数和成员变量的算法

数据类型有关，可以考虑用类模板。C++版的数据结构算法大都用类模板实现。

### **类模板的性质**

1) 类模板不是真正的类，它只是C++编译器生成具体类的一个模子。

2) 类模板可以设置默认模板实参。
**C++ STL简介**　　STL（Standard Template Library，标准模板库)是C++对泛型编程思想的实现，最早是惠普实验室开发的。

在被引入C++之前该技术就已经存在了很长的一段时间。后来STL成为ANSI/ISO C++标准的一部分。各个

C++厂商也有各自相应的模板库，这些库效率可能很高，但可移植性不一定好。

　　STL广义上分为三类：algorithm（算法）、container（容器）和iterator（迭代器），几乎所有的代码都采

用了模板类和模板函数的方式，这相比于传统的由函数和类组成的库来说提供了更好的代码重用机会。

　　在C++标准中，STL被组织为下面的13个头文件：<algorithm>、<deque>、<functional>、<iterator>、<vector>、

<list>、<map>、<memory>、<numeric>、<queue>、<set>、<stack> 和<utility>。

1) 算法（algorithm）

　　STL提供了大约100个实现算法的模版函数，算法部分主要由头文件<algorithm>，<numeric> 和<functional>组成。

　　<algorithm>是所有STL头文件中最大的一个，它是由一大堆模板函数组成的，其中常用到的功能范围涉及到比较、

交换、查找、遍历操作、复制、修改、移除、反转、排序、合并等等。

　　<numeric>体积很小，只包括一些简单数学运算的模板函数。

　　<functional>中则定义了一些模板类，用以声明函数对象。

2)  容器（container）（又称集合collection）

　　在实际的开发过程中，数据结构本身的重要性不会逊于操作于数据结构的算法的重要性，当程序中存在着对时间要

求很高的部分时，数据结构的选择就显得更加重要。

　　通过设置一些模版类，STL容器对最常用的数据结构提供了支持，这些模板的参数允许指定容器中元素的数据类

型，可以将许多重复而乏味的工作简化。

如下表：




|数据结构||实现头文件|
|----|----|----|
|向量(vector)|顺序性容器|<vector>|
|列表(list)|顺序性容器|<list>|
|双队列(deque)|顺序性容器|<deque>|
|集合(set)|关联容器|<set>|
|多重集合(multiset)|关联容器|<set>|
|栈(stack)|容器适配器|<stack>|
|队列(queue)|容器适配器|<queue>|
|优先队列(priority_queue)|容器适配器|<queue>|
|映射(map)|关联容器|<map>|
|多重映射(multimap)|关联容器|<map>|





3)迭代器（iterator）

　　迭代器是一种允许程序员检查容器内元素，并实现元素遍历的数据类型。C++标准库为每一种标准容器定义了一种迭代器类型。迭代器类型提供了比下标操作更一般化的方法：所有的标准库容器都定义了相应的迭代器类型，而只有少数的容器（比如数组）支持下标操作。因为迭代器对所有的容器都适用，现代C++程序更倾向于使用迭代器而不是下标操作访问容器元素。

　　迭代器从作用上来说是STL最基本的部分，迭代器在STL中用来将算法和容器联系起来，起着一种黏和剂的作用。几乎STL提供的所有算法都是通过迭代器存取元素序列进行工作的，每一个容器都定义了其本身所专有的迭代器，用以存取容器中的元素。

　　迭代器部分主要由头文件<utility>,<iterator> 和<memory>组成。<utility>是一个很小的头文件，它包括了贯穿使用在STL中的几个模板的声明，<iterator>中提供了迭代器使用的许多方法， <memory>为容器中的元素分配存储空间，同时也为某些算法执行期间产生的临时对象提供机制,<memory>中的主要部分是模板类allocator，它负责产生所有容器中的默认分配器。





