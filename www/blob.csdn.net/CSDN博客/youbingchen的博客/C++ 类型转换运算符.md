# C++ 类型转换运算符 - youbingchen的博客 - CSDN博客





2016年07月01日 09:20:42[youbingchen](https://me.csdn.net/youbingchen)阅读数：825








## C语言存在两种类型转换（隐式转换，显式转换）

> 
隐式转换，是针对不同类型之间的赋值和运算，函数调用传递参数  这些的转换都是由编译器自动完成

```
char ch; 
int i=ch;
```

显式转换 由程序员在变量前面添加型转换
` char *pc=(char *)pb void *ps=(void*)pa`

## C++ 中的类型转换

***通过上述的两种方式，C语言大部分的类型转换都可以顺利进行，C++继承了C中的隐式和显式转换的方式，由于C++本身对象模型的复杂性，C++增加了4种类型转换运算符
- dynamic_cast
- const_cast
- static_cast
- reinterpret_cast

###### 在了解C++四种类型转换之前，一些概念先了解一下一些基本概念

> 
RTTI 是运行阶段类型识别 

  C++ 有3个支持RTTI的元素

- 如果可能的话，dynamic_cast 运算符将使用一个指向基类的指针来生成一个指向派生类的指针，否则该运算返回0（空指针）
- typeid运算符返回一个指向对象类型的值
- typeinfo结构存储了有关特定类型的信息

# 1. stactic_cast 支持基本类型的数据转换（char,int）,及指针之间的转换

### (1) 用于基本的数据类型转换

```

```

```cpp
test_enum type = test_enum_1;
char a ;
int b = static_cast <int>(a);
char c = static_cast<char>(b);
type = static_cast<test_enum>(b);
char*pa = NULL;
int *pb = (int*)pa;
//int *pb = static_cast <int*> pa);        //error
//pa = static_cast<char*>pb)           //error
char *pc = (char*)pb;
//char *pc = static_cast<char*>(pb);    //error
void *p = static_cast<void*>(pa);
pb = static_cast<int*>(p);
pc = static_cast<char*>(p);
```

### (2)类层次中基类和子类之间或引用之间的转换

**  上行转换：子类指针或引用转换成基类表示——安全**
**下行转换：基类指针或引用转换成子类表示——危险（没有动态类型检查）**

```

```

```cpp
class A
{
};
class B:public A
{
};
class C:public A
{
};
class D
{
};

A objA;
B objB;
A*pObjA = new A();
B*pObjB = new B();
C*pObjC = new C();
D*pObjD = new D();

objA = static_cast<A&>(objB);     //转换为基类引用    
objA = static_cast<A>(objB);
objB = static_cast<B>(objA);      //不能进行转换  

pObjA = pObjB;                    //right 基类指针指向子类对象
//objB = objA;                      //error 子类指针指向基类对象
pObjA = static_cast<A*>(pObjB);   //right 基类指针指向子类
pObjB = static_cast<B*>(pObjA);   //可以强制转换基类到子类
//pObjC = static_cast<C*>(pObjB);   //error 继承于统一类的派生指针之间转换 
//pObjD = static_cast<D*>(pObjC);   //error 两个无关联之间转换
```

# 2. dynamic_cast的用法 （除了基类指针指向子类对象，可以没有虚函数外，其它要进行dynamic_cast转换必须具有虚函数才行）

#### (1)继承关系的类指针对象或引用之间转换

```

```

```cpp
class A
{
};
class B:public A
{
};
class C:public A
{
};
class D
{
};

A objA;
B objB;
A*pObjA = new A();
B*pObjB = new B();
C*pObjC = new C();
D*pObjD = new D();
//objA = dynamic_cast<A>(objB);         //error 非引用

objA = dynamic_cast<A&>(objB);
//objB = dynamic_cast<B&>(objA);      //error A 不是多态类型不能转换 若有虚函数则可以进行转换

pObjA = dynamic_cast<A*>(pObjB);
//pObjB = dynamic_cast<B*>(pObjA);    //error A 继承关系 不是多态类型不能转换
//pObjB = dynamic_cast<B*>(pObjC);    //error C 兄弟关系 不是多态类型不能转换
//pObjB = dynamic_cast<B*>(pObjD);    //error D 没有关系 不是多态类型不能转换
```

##### 包含有虚函数之间对象指针的转换

```

```

```cpp
class A
{
Public:
     Virtual ~A(){}
};
class B:public A
{
};
class C:public A
{
};
class D
{
Public:
Virtual ~D(){}
};
pObjB = dynamic_cast<B*>(pObjA);    // worning 继承关系 父类具有虚函数 多态
pObjB = dynamic_cast<B*>(pObjD);    //worning 没有关系 D是多态类型可以转换
//以上结果：pObjB == NULL 此处会发生一个运行时错误
```

# const_cast(去掉类型的const或volatile)

##### > const_cast  (expression) 中的type_name和expression的类型必须是一样的

```
```cpp
High bar
const High *pbar=&bar
....
High *pb=const_cast
```

# 总结
- 对于**从子类到基类的指针转换，**static_cast和dynamic_cast都是成功并且正确的（所谓成功是说转换没有编译错误或者运行异常；所谓正确是指方法的调用和数据的访问输出是期望的结果），这是面向对象多态性的完美体现。
- 而从**基类到子类的转换**，static_cast和dynamic_cast都是成功的，但是正确性方面，我对两者的结果都先进行了是否非空的判别：dynamic_cast的结果显示是空指针，而static_cast则是非空指针。但很显然，static_cast的结果应该算是错误的，子类指针实际所指的是基类的对象，而基类对象并不具有子类的Study()方法(除非妈妈又想去接受个”继续教育”)。
- 对于**没有关系的两个类之间的转换**，输出结果表明，dynamic_cast必须在是有虚函数的前提条件下（不然编译也是不通过）依然是返回一个空指针以表示转换是不成立的；static_cast直接在编译期就拒绝了这种转换。reinterpret_cast成功进行了转换，而且返回的值并不是空指针，但是结果显然是错误的.
- ** 总得说来，static_cast和reinterpret_cast运算符要么直接被编译器拒绝进行转换，要么就一定会得到相应的目标类型的值。 而dynamic_cast却会进行判别，确定源指针所指的内容，是否真的合适被目标指针接受。如果是否定的，那么dynamic_cast则会返回null。这是通过检查”运行期类型信息”（Runtime type information，RTTI）来判定的，它还受到编译器的影响，有些编译器需要设置开启才能让程序正确运行，因此dynamic_cast也就不能用传统的转换方式来实现了。**
- 去const属性用const_cast。基本类型转换用static_cast。多态类之间的类型转换用daynamic_cast。不同类型的指针类型转换用reinterpret_cast。

### 　C++的四种强制转型形式每一种适用于特定的目的：
- ·dynamic_cast 主要用于执行“安全的向下转型（safe downcasting）”，也就是说，要确定一个对象是否是一个继承体系中的一个特定类型。它是唯一不能用旧风格语法执行的强制转型，也是唯一可能有重大运行时代价的强制转型。
- 
·static_cast 可以被用于强制隐型转换（例如，non-const 对象转型为 const 对象，int 转型为 double，等等），它还可以用于很多这样的转换的反向转换（例如，void* 指针转型为有类型指针，基类指针转型为派生类指针），但是它不能将一个 const 对象转型为 non-const 对象（只有 const_cast 能做到），它最接近于C-style的转换。

- 
·const_cast 一般用于强制消除对象的常量性。它是唯一能做到这一点的 C++ 风格的强制转型。 

- ·reinterpret_cast 是特意用于底层的强制转型，导致实现依赖（implementation-dependent）（就是说，不可移植）的结果，例如，将一个指针转型为一个整数。这样的强制转型在底层代码以外应该极为罕见。
|类型|用途|
|----|----|
|dynamic_cast|通常在基类和派生类之间转换时使用|
|static_cast|一般的转换，如果你不知道该用哪个，就用这|
|const_cast|主要针对const和volatile的转换|
|reinterpret_cast|用于进行没有任何关联之间的转换，比如一个字符指针转换为一个整形数|



