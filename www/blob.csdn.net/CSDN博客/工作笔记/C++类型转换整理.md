# C++类型转换整理 - 工作笔记 - CSDN博客





2012年04月23日 09:30:37[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5398标签：[c++																[reference																[class																[存储																[编译器																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[基础编程](https://blog.csdn.net/App_12062011/article/category/952065)





C++ 提供四种新式转型(常常被称为new-style 或C++-style casts) :

const_cast<T>( e}(expression )

dynamic_cast<T>( expression)

reinterpret_cast<T>( expression)

static_cast<T>(ωψression )

各有不同的目的:

• const_cast通常被用来将对象的常量性转除(cast away the constness) 。它也是唯一有此能力的C++-style 转型操作符.

• dynamic_cast 主要用来执行"安全向下转型" ( safe downcasting) ，也就是用来决定某对象是否归属继承体系中的某个类型。它是唯-无法由旧式语法执行的动作，也是唯一可能耗费重大运行成本的转型动作.有条件转换，动态类型转换，运行时类型安全检查(转换失败返回NULL)：

1. 安全的基类和子类之间转换。

2. 必须要有虚函数。

3. 相同基类不同子类之间的交叉转换。但结果是NULL。

• reinterpret_cast 意固执行低级转型，实际动作(及结果)可能取决于编译器，这也就表示它不可移植。例如将一个pointer to int 转型为一个int。这一类转型在低级代码以外很少见。仅仅重新解释类型，但没有进行二进制的转换：

1. 转换的类型必须是一个指针、引用、算术类型、函数指针或者成员指针。

2. 在比特位级别上进行转换。它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。但不能将非32bit的实例转成指针。

3. 最普通的用途就是在函数指针类型之间进行转换。

4. 很难保证移植性。

• static cast 用来强迫隐式转换(implicit conversions) ，例如将非const转化问const，int转换为double等，也可以执行反向转换。但无法将const转换为非const

**static cast**似于C风格的强制转换。无条件转换，静态类型转换。用于：

1. 基类和子类之间转换：其中子类指针转换成父类指针是安全的；但父类指针转换成子类指针是不安全的。(基类和子类之间的动态类型转换建议用dynamic_cast)

2. 基本数据类型转换。enum, struct, int, char, float等。static_cast不能进行无关类型（如非基类和子类）指针之间的转换。

3. 把空指针转换成目标类型的空指针。

4. 把任何类型的表达式转换成void类型。

5. static_cast不能去掉类型的const、volitale属性(用const_cast)。

**总结**
去const属性用const_cast。

基本类型转换用static_cast。

多态类之间的类型转换用daynamic_cast。

不同类型的指针类型转换用reinterpreter_cast。

通常你想在一个你认定为derived class 对象身上执行derived class 操作函数，但你的手上却只有-个"指向base" 的pointer 或reference ，你只能靠它们来处理对象。有两个一般性做法可以避免这个问题。

1 使用容器并在其中存储直接指向derived class 对象的指针(通常是智能指针)，然后，通过STL迭代器，处理该对象。但这种做法，无法在容器中，存储指针指向所有基类的派生类。若需要处理多个派生类，则需要多个容器。

2 基类提供虚函数，使用容器，存储基类对象指针，然后用迭代器调用。

绝对必须避免的一件事是所谓的"连串(cascading) dynamic_casts"](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




