
# Effective C++ (6): Inheritance and Oject-Oritent Design - Wesley's Blog - CSDN博客


2018年09月03日 17:09:08[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：61标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[effective c++																](https://so.csdn.net/so/search/s.do?q=effective c++&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=c++&t=blog)个人分类：[计算机基础																](https://blog.csdn.net/yinanmo5569/article/category/7923781)



## Introduction
本章主要围绕继承展开讨论. 如何确定是 public 继承还是 private 或 protected 继承, 继承中virtual的确定等等. 信息量较大.
## Rule 32: Make sure public inheritance models “is-a”
并不是满足 is-a 的所有关系都可以使用 public 继承, 例如 square is-a rectangle, 但是并不是每一个适用于 rectangle 的函数都适用于 square, 这种时候请更改 rectangle 使之更加泛化或者不让 square 继承 rectangle
Remeber:
**“public 继承”意味着”is-a”关系, 适用于base classes身上的每一件事都适用于derived classes上**
## Rule 33: Avoid hiding inherited names
virtual 函数意味着 “接口被继承”, non-virtual 函数意味着 “接口和实现都必须被继承”
`void Derived::mf4(){
    ...
    mf2();
    ...
}`编译器查找函数名的规则是: 先查找local作用域, 再查找外围作用域(Derived Class), 再查找Base Class, 再到 namespace, 最后到全局作用域.
但是注意对于重载函数的继承, 有些特殊.
例如:
`class Base{
public:
    virtual void mf1() = 0;
    virtual void mf1(int);
    virtual void mf2();
    void mf3();
    void mf3(double);
    ...
};
class Derived: public Base{
public:
    virtual void mf1();
    void mf3();
    ...
};`上面这段代码, 在Derived class实现的 mf3 和 mf1 将会遮挡重载函数, 产生编译错误.
`Derived d;
d.mf1(1);   // 错误
d.mf3(1.0); // 错误`所以对于重载函数的继承, 需要在子类中使用 using, 或者 forwarding :
`class Derived: public Base{
public:
    using Base::mf1;
    using Base::mf3;
    virtual void mf1();
    void mf3();
    ...
};
// or
class Derived public Base{
public:
    virtual void mf1(){
        Base::mf1();
    }
    ...
};`这样就可以达到目的了.
Remeber:
Derived classes内的名称会遮掩base classes内的名称. 所以最好避免使用相同的名字.
为了继承重载函数, 可使用 using 声明式或转交函数(forwarding function)
## Rule 34: Differentiate between inheritance of interface and inheritance of implementation
为了避免子类继承不想要的缺省实现可以通过声明为 pure virtual 函数, 并定义一个 non-virtual 的 defaultImplement实现:
`class Airplane{
public:
    virtual void fly(const Airport& dest) = 0;
    ...
protected:
    void defaultFly(const Airport& dest);
};
class ModelA: public Airplane{
public:
    virtual void fly(const Airport& dest){
        defaultFly(dest);
    }
    ...
};`这样可以避免使用者在无意识的情况下继承缺省实现.
Remeber:
接口继承与实现继承不同. 在public继承之下, derived class总是继承了base class的所有接口, 但是实现却可以覆盖
pure virtual 函数只是具体指定接口继承
impure virtual 函数具体指定接口继承及缺省实现
non-virtual 函数具体指定接口继承及强制性实现继承
## Rule 35: Consider alternatives to virtual functions
有些时候我们可以考虑一些非 virtual function 来替代 virtual function, 会有意向不到的效果.
就比如说通过函数指针实现 Strategy 模式, 下面以一个计算血量的代码为例:
`class GameCharacter;
int defaultHealthCalc(const GameCharacter& gc);
class GameCharacter{
public:
    typedef int (*HealthCalcFunc)(const GameCharacter&);
    explicit GameCharacter(HealthCalcFunc hcf = defaultHealthCalc): healthFunc(hcf)
    {}
    int healthValue() const
    { return healthFunc(*this); }
private:
    HealthCalcFunc healthFunc;  // function pointer
};`这样通过外部嵌入函数指针, 有点类似于依赖注入(IoC)和控制反转(DI). 其实也能够更方便测试(可以很好地利用 GMock 等工具)
对于上面这段代码, 还可以使用`tr1::function`来替换,**tr1::function 是一个函数指针class, 可以兼容任何可调用物(callable entity, 也就是函数指针, 函数对象, 或成员函数指针).**
只需要把上面的
`typedef int (*HealthCalcFunc)(const GameCharacter&);`改为:
`typedef std::tr1::function<int (const GameCharacter&)> HealthCalcFunc;`就可以了.
Remeber:
virtual 函数替代方案包括 NVI 手法及 Strategy 设计模式的多种模式
NVI ( non-virtual interface ) 手法是 Template Method(与c++ tempate)无关, 以 public non-virtual 成员函数包裹较低访问性( private 或 protected) 的virtual 函数, 可以做一些事前事后处理
tr1::function 对象的行为就像一般函数指针. 这样的对象可以接纳”与给定目标签名式兼容”的所有可调用物( callable entities)
## Rule 36: Never redefine an inherited non-virtual function
绝不要重新定义继承而来的 non-virtual 函数.
因为对于同样的 non-virtual 函数, 按道理行为是相同的, 否则容易引起歧义. 如果行为是不相同的, 请改为 virtual
Remeber:
**绝不要重新定义继承而来的 non-virtual 函数**
## Rule 37: Never redefine a function’s inherited default parameter value
这是一条很重要的规则, 缺省参数值都是静态绑定的, 也就是说缺省参数值根据调用者的类型而确定:
比如下面这段代码:
`class Shape{
public:
    virtual void print(int num = 1) const = 0;
};
class Rect: public Shape{
public:
    virtual void print(int num = 2) const{
        printf("%d\n", num);
    }
};
int main(){
    Shape *p = new Rect;
    p->print();
}`实际上输出会是 1, 也就是说默认参数是采取的 Shape 里面的 num = 1, 而不是 Rect 中的 num = 2, 所以说是通过 virtual 动态调用函数, 但是却是静态绑定的参数. 各出一半力完成的这个任务.
所以最好的办法是保持默认参数的一致, 默认参数是不会继承的, 所以需要手动保持一致, 一个修改统一修改. 还一个解决问题的方法就是通过之前提到的 NVI(non-virtual interface) 的手法实现. 在public interface中设定默认参数, 在这里不赘述.
Remeber:
**绝不要重新定义一个继承而来的缺省参数值, 因为缺省参数值都是静态绑定的, 而 virtual 函数却是动态绑定**
## Rule 38: Model “has-a” or “is-implemented-in-terms-of” through composition
Remeber:
**通过复合来实现 has-a 或者 “根据某物实现出”**
## Rule 39: Use private inheritance judiciously
Private继承相对于 public 继承主要有两点不同:
编译器不会自动将一个 derived class 对象转换为一个 base class 对象
private base class 继承来的所有成员在对象中都会变成 private 属性
Remeber:
Private继承意味着 is-implemented-in-terms of(根据某物实现出). 与复合(composition)有点类似, 绝大多数情况使用 composition, 当涉及到 virtual, protected 成员牵扯进来的时候才使用 private 继承
## Rule 40: Use multiple inheritance judiciously
对于多重继承, 首先是函数名容易引起歧义, 当继承的两个父类中有相同的函数名的时候, 必须指明使用的哪个 base class 的函数:
`class BaseA{
public:
    void check();
};
class BaseB{
public:
    void check() const;
};
class Derived: public BaseA, public BaseB
{ ... };
Derived temp;
temp.check();           // 歧义
temp.BaseA::check();    // 正确`当出现”钻石型多重继承”的时候, 就是说多重继承的base classes之上还共同继承了同一个 base class, 需要使用**virtual 继承**来解决:
`class File { ... };
class InputFile: virtual public File { ... };
class OutputFile: virtual public File { ... };
class IOFile: public InputFile, OutputFile { ... };`所以说:
非必要不要使用 virtual bases, 平常使用 non-virtual 继承
如果非要使用, 尽量避免在其中放置数据.
Remeber:
多重继承容易导致歧义, 如果出现”钻石型多重继承”的话则需要使用 virtual 继承
virtual继承会增加 大小, 速度, 初始化(及赋值)复杂度等成本.
多重继承的确有正确用途. 其中一个情节涉及” public继承某个Interface class” 和 “private继承某个协助实现的class” 的两相结合
## 系列文章
[Effective C++ (1): Accustoming Yourself to C++](https://blog.csdn.net/yinanmo5569/article/details/82289290)
[Effective C++ (2): Constructors, Destructors, and Assignment Operators](https://blog.csdn.net/yinanmo5569/article/details/82290194)
[Effective C++ (3): Resource Management](https://blog.csdn.net/yinanmo5569/article/details/82317019)
[Effective C++ (4): Designs and Declaration](https://blog.csdn.net/yinanmo5569/article/details/82317034)
[Effective C++ (5): Implementation](https://blog.csdn.net/yinanmo5569/article/details/82346893)
[Effective C++ (6): Inheritance and Oject-Oritent Design](https://blog.csdn.net/yinanmo5569/article/details/82351493)
[Effective C++ (7): Templates and Generic Programming](https://blog.csdn.net/yinanmo5569/article/details/82420021)
[Effective C++ (8): Customizing new and delete](https://blog.csdn.net/yinanmo5569/article/details/82419808)
[Effective C++ (9): Miscellany](https://blog.csdn.net/yinanmo5569/article/details/82419858)

