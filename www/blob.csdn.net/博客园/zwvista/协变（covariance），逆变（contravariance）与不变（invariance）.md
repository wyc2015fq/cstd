# 协变（covariance），逆变（contravariance）与不变（invariance） - zwvista - 博客园

## [协变（covariance），逆变（contravariance）与不变（invariance）](https://www.cnblogs.com/zwvista/p/7774232.html)

### 协变，逆变与不变

能在使用父类型的场景中改用子类型的被称为协变。

能在使用子类型的场景中改用父类型的被称为逆变。

不能做到以上两点的被称为不变。

以上的场景通常包括数组，继承和泛型。

### 协变逆变与泛型（C#，Java）

在C#中，泛型参数的类型缺省是不变的，但是我们可以在定义泛型接口或委托时通过给参数类型加上out或in来标注该参数类型是协变还是逆变。
- 协变意味着你能把 IEnumerable<string> 用在需要 IEnumerable<object> 的地方。

这里 IEnumerable<out T> 是协变，使用 out 标注。使用 out 标注的原因是协变参数一般处于输出（output）者的地位，只读不写（read，get）。
- 逆变意味着你能把 IComparable<object> 用在需要 IComparable<string> 的地方。

这里 IComparable<in T> 是逆变，使用 in 标注。使用 in 标注的原因是逆变一般处于输入（input）者的地位，只写不读（write，put）。
- [Covariance and contravariance real world example](https://stackoverflow.com/questions/2662369/covariance-and-contravariance-real-world-example)

```
public interface IEnumerator<out T>
{
  T Current { get; } // 输出（output）者，只读不写
  bool MoveNext();
  void Reset();
}
IEnumerator<string> strEnum = new Enumerator<string>();
IEnumerator<object> objEnum = strEnum;

public interface IComparer<in T>
{
  int Compare(T a, T b); // 输入（input）者，只写不读
}
IComparer<object> objComp = new Comparer<object>();
IComparer<string> strComp = objComp;
```

在Java中，泛型参数的类型是不变的。但是我们可以在使用泛型类或接口时在参数类型的位置上使用通配符加上extends或super来指定该参数类型是协变还是逆变。
- List<? extends T> 是协变（参数类型只能使用T及其子类型，T是上限），用于生产者（指函数的入口参数），只读不写。
- List<? super T> 是逆变（参数类型只能使用T及其父类型，T是下限），用于消费者（指函数的出口参数或返回值），只写不读。
- [Difference between <? super T> and <? extends T> in Java](https://stackoverflow.com/questions/4343202/difference-between-super-t-and-extends-t-in-java)

### 泛型中协变逆变的类型安全性
- 协变泛型参数是生产者，处于输出者地位，只读不写。

协变泛型参数使用子类型代替父类型是类型安全的，这是因为在读取时子类对象可以被看做父类对象。
- 逆变泛型参数是消费者，处于输入者地位，只写不读。

逆变泛型参数使用父类型代替子类型是类型安全的。这是因为在写入时子类对象可以被安全的转换成父类对象。
|协变泛型参数类型||逆变泛型参数类型|
|----|----|----|
|入口|函数|出口|
|输出者||输入者|
|子类 => 父类|======>|子类 <= 父类|

```
public class Collections { 
  public static <T> void copy(List<? super T> dest, List<? extends T> src) {
      for (int i = 0; i < src.size(); i++) 
        dest.set(i, src.get(i)); 
  } 
}
```

### 协变与数组（C#，Java）

在C#和Java中，数组都是协变的。子类数组是父类数组的子类型，因而在调用参数是父类数组的函数时能够传入子类数组。
- 在C#中，string[] 是 object[] 的子类型。
- 在Java中，String[] 是 Object[] 的子类型。
- 协变数组类型是类型不安全的，处理不当会抛出异常。
- 协变数组类型虽然类型不安全，但是数组类型能够协变是一个合理的选择。

这是因为早期C#和Java都缺乏泛型，如果数组不允许协变，将无法使用多态。
- [Why are arrays covariant but generics are invariant?](https://stackoverflow.com/questions/18666710/why-are-arrays-covariant-but-generics-are-invariant)

### 协变逆变与继承

C++和Java支持协变返回值类型（covariant return type），也就是允许基类虚函数（方法）在子类中被覆盖（重写）时，子类中相应虚函数（方法）的返回值类型不同于基类虚函数（方法）的返回值类型，条件是前者是后者的子类型。C#不支持这一特性。

```
class VehicleFactory {
  public:
    virtual Vehicle * create() const { return new Vehicle(); }
    virtual ~VehicleFactory() {}
};

class CarFactory : public VehicleFactory {
public:
    virtual Car * create() const override { return new Car(); } // 协变返回值类型
    // 子类虚函数create的返回值类型Car *是基类虚函数create的返回值类型Vehicle *的子类型。
};
```

某些语言支持逆变（协变）参数类型（contravariant/covariant argument type），也就是允许基类虚函数（方法）在子类中被覆盖（重写）时，子类中相应虚函数（方法）的参数类型不同于基类虚函数（方法）的参数类型，条件是前者是后者的父（子）类型。C++，C#以及Java均不支持逆变或协变参数类型。

### 继承中协变逆变的类型安全性
- 在子类中使用子类型返回值类型代替父类中的父类型返回值类型（协变返回值类型）是类型安全的。
- 在子类中使用父类型参数类型代替父类中的子类型参数类型（逆变参数类型）是类型安全的。
- 在子类中使用子类型参数类型代替父类中的父类型参数类型（协变参数类型）是类型不安全的。
|逆变参数类型||协变返回值类型|
|----|----|----|
|参数|函数|返回值|
|父类 => 子类|======>|父类 <= 子类|

### 协变逆变，父子类型关系以及类型转换（转型）

协变意味着能在使用父类型的场景中改用子类型，也就是在新场景中使用子类型后仍然得到子类型。

逆变意味着能在使用子类型的场景中改用父类型，也就是在新场景中使用父类型后却能得到子类型。

下面改用符号语言：

S是T的子类型（S能够被安全的转型成T），记作 S <: T。

协变：在新场景 F 中，父子类型的关系被维持，即 F(S) <: F(T)。

逆变：在新场景 F 中，父子类型的关系被逆转，即 F(T) <: F(S)。

不变：在新场景 F 中，父子类型的关系不存在，即 F(S) 与 F(T) 两者不相关。

### 协变逆变与函数类型

函数 f 的参数类型为A，返回值类型为B，记作 A -> B。

协变：B <: B' => A -> B <: A -> B'（协变返回值类型）

逆变：A <: A' => A' -> B <: A -> B（逆变参数类型）

函数型语言中函数类型通常满足协变返回值类型和逆变参数类型。
[Can parameters be contra- or covariant in Python?](https://cs.stackexchange.com/questions/19598/can-parameters-be-contra-or-covariant-in-python)

### 协变逆变与C++

在C++中，指针和引用都是协变的，也就是在使用父类指针和引用的场景中能够安全的使用子类指针和引用。

这是因为子类指针和引用是父类指针和引用的子类型，语法上前者能够被安全地隐式地转换成后者，

在C++中，模板的参数类型是不变的。但是标准库的某些类型支持协变和逆变。
- shared_ptr类型支持协变，即 S <: T => shared_ptr<S> <: shared_ptr<T>。
- unique_ptr类型支持协变，即 S <: T => unique_ptr<S> <: unique_ptr<T>。
- 以上两者是协变的原因是以上两者都是智能指针，需要模仿原生指针的特性。
- function的类型参数中的返回值类型支持协变，即 B <: B' => function<B(A)> <: function<B'(A)>。
- function的类型参数中的参数类型支持逆变，即 A <: A' => function<B(A')> <: function<B(A)>。

```
#include <iostream>
#include <memory>
#include <functional>
using namespace std;

struct Base {};
struct Derived : Base {};

int main()
{
    shared_ptr<Derived> p = nullptr;
    shared_ptr<Base> p2 = p; // shared_ptr类型支持协变
    function<Derived*(Base*)> f = nullptr;
    function<Base*(Derived*)> f2 = f; // function的函数参数类型支持逆变，function的函数返回值类型支持协变
}
```

[Covariance and Contravariance in C++ Standard Library](http://cpptruths.blogspot.jp/2015/11/covariance-and-contravariance-in-c.html)


