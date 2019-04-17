# C++  类模板基础知识 - 在思索中前行！ - CSDN博客





2015年03月30日 22:35:38[_Tham](https://me.csdn.net/txl16211)阅读数：500标签：[C++ 类模板																[C++ 模板](https://so.csdn.net/so/search/s.do?q=C++ 模板&t=blog)](https://so.csdn.net/so/search/s.do?q=C++ 类模板&t=blog)
个人分类：[C/C++																[数据结构](https://blog.csdn.net/txl16211/article/category/2409063)](https://blog.csdn.net/txl16211/article/category/2284665)






类模板与模板类
为什么要引入类模板：类模板是对一批仅仅成员数据类型不同的类的抽象，程序员只要为这一批类所组成的整个类家族创建一个类模板，给出一套程序代码，就可以用来生成多种具体的类，(这类可以看作是类模板的实例)，从而大大提高编程的效率。

注意：

1、类模板是参数化的类，即用于实现数据类型参数化的类。

2、应用类模板可以使类中的数据成员、成员函数的参数及成员函数的返回值，能根据模板参数匹配情况取任意数据类型

语法：

//类模板的定义



```cpp
template <类型形参表>   
    class 类名   
    {    
     //类说明体  
    };
```
//类外成员函数的定义



```cpp
template <类型形参表>  //不要漏了啊。  
    返回值类型  类模板名<类型名表>::成员函数名(参数表)   
    {   
        //成员函数体   
    }
```

类模板的实例化：



```cpp
类模板名 <实际类型>  对象名(实参表)；
```



具体的使用过程：


当类模板创建对象时，根据用户给出的实际类型，系统将类模板中的模板参数置换为确定的参数类型，生成一个具体的类(模板类)，

有对象之后，和普通类的对象使用方法相同。  


注意：

1、模板类的成员函数必须是函数模板

2、类模板中的成员函数的定义，若放在类模板中，则与普通类的成员函数的定义方法相同；若在类模板之外定义，则成员函数的定义格式如上

书写代码时要注意 类模板和普通类的代码区别：

1、针对类整体而言，在类定义前必须有模板声明

2、针对类外定义的成员函数(每个函数都有)而言，定义时必须要冒(模板声明) + 在函数名前、类后必须加<Type>

3、针对类的对象而言，在建立对象时，必须在类名和对象之间要写实际类型<实际类型>

模板类的形参表：template <类型形参表> 

模板形参表类似函数形参表，由类型、非类型和模板三部分组成。

类型形参：代表所有的数据类型-所有的均可代表(类类型+预定义类型)

格式：由 关键字class或typename + 名字 组成

例如：template <class T,class D>,其中T和D都是一个类型形参，名字可以有自己定义

用途：这三种形参作用一样，均可以模板类型形参可作为类型说明符用在模板中的任何地方，即可以用于指定反回类型，变量声明等。

替代时间：类模板的类型实参必须在实例化类对象的时候指定

说明：这里的class表示一类类型，T是在实例化时确定，它的替代可以是任何类型，即类类型，预定义数据类型，比较通用

非类型形参=内置类型形参：代表具体的一类(C++的标准数据类型的一种，如整型)

格式：由 标准数据类型 + 名字 组成

例如：template <int a, int b>,其中a和b都是非类型形参 或 如template<class T, int a>,T为类型参数，a为非类型参数

参数有三种情况：

1、整型或枚举类型 

2、指针类型（包含普通对象的指针类型、函数指针类型、指向成员的指针类型） 

3、引用类型（指向对象或指向函数的引用都是允许的）

有些常值不能作为有效的非类型实参，包括：

1、空指针常量 

2、浮点型值 

3、字符串

说明：

1、非类型形参在模板定义的内部是常量值，也就是说非类型形参在模板的内部是常量，或者说只能是右值，它们不能被取址，也不能被赋值

2、调用非类型模板形参的实参必须是一个常量表达式，或者是一个数，即它必须能在编译时计算出结果。

举例：

```cpp
template <class T, int size>   
    class A  
    {  
      
    };  
    A<int 10> a; //正确  
    int b;  
    A<int, b> a; //错误，b不是常量  
    const int c;  
    A<int, c> a; //正确，c是常量
```
非类型模板形参还可以使用缺省值

```cpp
template <class T, int Size=100>  
    class A  
    {  
      
    };  
    A<int>a;//使用缺省值，设置长度100  
    A<int,200>a;//不使用缺省值长度设置200
```

说明：

百度说，非类型形参一般不应用于函数模板

非类型模板形参的形参和实参间所允许的转换 <引用黄邦勇帅的总结。>

1、允许从数组到指针，从函数到指针的转换。如：template <int *a> class A{}; int b[1]; A<b> m;即数组到指针的转换

2、const修饰符的转换。如：template<const int *a> class A{}; int b; A<&b> m; 即从int *到const int *的转换。

3、提升转换。如：template<int a> class A{}; const short b=2; A<b> m; 即从short到int的提升转换

4、整值转换。如：template<unsigned int a> class A{}; A<3> m; 即从int到unsigned int的转换。

5、常规转换。

模板形参：一个模板作为另一个模板的参数

格式：

```cpp
template <class A,class B,template<class C> class D>   
    class E  
    {//类模板E的定义}
```

说明：

1、template<class C> class D 是一个模板。

2、定义一个模板E，其中有两个类型参数A和B，一个模板参数D。注意C有时候可以省略不写

模板参数使用说明：

1、首先要定义一个模板：用来实例化模板E的对象。

2、A和B可以是任意类型；但是D可以是程序员自己定义的，也可以是STL中的标准模板库

代码：

```cpp
//首先定义一个普通的模板  
    template<class T>  
    class Array   
    {  
       //模板Array的成员  
    };  
    //定义一个模板Container，参数列表：一个类型为T，另一个模板类型Seq  
    template<class T, template<class> class Seq>//这里Seq前面的<>没有写参数名字  
    class Container   
    {  
        Seq<T> seq;  
        ... ...  
    };  
    //注意：注意Seq模板的声明并没有带模板参数的名字,因为这里我们不会用到这个参数,所以可以省略不写.
```
实例化：

```cpp
Container<int, Array> container;
```
说明：模板第一个参数为int，第二个参数为自己定义的模板(注意这里Array也代表一个类型，用户自定义的数据类型)

```cpp
Container<int, vector<int>> container;
```

说明：模板第一个参数为int，第二个参数为STL中提供的模板(注意要使用实例化后的类型，因为这里参数必须是类型)

不知道直接写vector能不能用，是不是直接写vector时，下面使用seq时，要加<int>：seq<int>，写了就不加<int>:seq.

等以后用到在确定下？？？

用途：这样定义一个泛化的容器，容器里所存储的对象也是泛化

分析：需要两个类型：

一个是代表容器类型，vector，list。

另一个是存放数据的类型，如int，string

这时就要使用模板的模板参数：模板参数本身含是一个模板

举例：

```cpp
template<class TElem,template<class Type  
                        ,class Allocator=allocator<Type>> class TContainer>  
    class MyContainer : public TContainer<TElem>  
    {  
      
    };
```
实例化时就可以：

```cpp
MyContainer<int,vector> myContainer;
```
说明：这里参数也可以使用默认的参数，就像函数中的确省参数是一样的，如class Allocator=allocator<Type>






