# C/C++中枚举类型(enum) - 逍遥剑客 - CSDN博客
2006年12月01日 00:46:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2238
如果一个变量你需要几种可能存在的值，那么就可以被定义成为枚举类型。之所以叫枚举就是说将变量或者叫对象可能存在的情况也可以说是可能的值一一例举出来。 
举个例子来说明一吧，为了让大家更明白一点，比如一个铅笔合中有一支笔，但在没有打开之前你并不知道它是什么笔，可能是铅笔也可能是钢笔，这里有两种可能，那么你就可以定义一个枚举类型来表示它！ 
enum box{pencil,pen};//这里你就定义了一个枚举类型的变量叫box，这个枚举变量内含有两个元素也称枚举元素在这里是pencil和pen，分别表示铅笔和钢笔。 
在这里要说一下，如果你想定义两个具有同样特性枚举类型的变量那么你可以用如下的两种方式进行定义！ 
enum box{pencil,pen}; 
enum box box2;//或者简写成box box2; 
再有一种就是在声明的时候同时定义。 
enum {pencil,pen}box,box2; //在声明的同时进行定义！ 
枚举变量中的枚举元素系统是按照常量来处理的，故叫枚举常量，他们是不能进行普通的算术赋值的，(pencil=1;)这样的写发是错误的，但是你可以在声明的时候进行赋值操作！ 
enum box{pencil=1,pen=2}; 
但是这里要特别注意的一点是，如果你不进行元素赋值操作那么元素将会被系统自动从0开始自动递增的进行赋值操作，说到自动赋值，如果你只定义了第一个那么系统将对下一个元素进行前一个元素的值加1操作，例如 
enum box{pencil=3,pen};//这里pen就是4系统将自动进行pen=4的定义赋值操作！ 
C++ 中的枚举类型继承于 C 语言。就像其他从 C 语言继承过来的很多特性一样，C++ 枚举也有缺点，这其中最显著的莫过于作用域问题——在枚举类型中定义的常量，属于定义枚举的作用域，而不属于这个枚举类型。例如下面的示例：
enum FileAccess {
    Read = 0x1,
    Write = 0x2,
};
FileAccess access = ::Read; // 正确
FileAccess access = FileAccess::Read; // 错误
C++枚举的这个特点对于习惯面向对象和作用域概念的人来说是不可接受的。首先，FileAccess::Read 显然更加符合程序员的直觉，因为上面的枚举定义理应等价于如下的定义（实际上，.NET 中的枚举类型便是如此实现的）：
class FileAccess {
    static const int Read = 0x1;
    static const int Write = 0x2;
};
其次，这导致我们无法在同一个作用域中定义两个同样名称的枚举值。也就是说，以下的代码是编译错误：
enum FileAccess {
    Read = 0x1,
    Write = 0x2,
};
enum FileShare {
    Read = 0x1, // 重定义
    Write = 0x2, // 重定义
};
如果这一点没有让你恼怒过的话，你可能还没写过多少 C++ 代码 :-)。实际上，在最新的 C++0x 标准草案中有关于枚举作用域问题的提案，但最终的解决方案会是怎样的就无法未卜先知了，毕竟对于象 C++ 这样使用广泛的语言来说，任何特性的增删和修改都必须十分小心谨慎。
当然，我们可以使用一些迂回的方法来解决这个问题（C++ 总是能给我们很多惊喜和意外）。例如，我们可以把枚举值放在一个结构里，并使用运算符重载来逼近枚举的特性：
struct FileAccess {
    enum __Enum {
        Read = 0x1,
        Write = 0x2
    };
    __Enum _value; // 枚举值
    FileAccess(int value = 0) : _value((__Enum)value) {}
    FileAccess& operator=(int value) {
        this->_value = (__Enum)value;
        return *this;
    }
    operator int() const {
        return this->_value;
    }
};
我们现在可以按照希望的方式使用这个枚举类型：
FileAccess access = FileAccess::Read;
并且，因为我们提供了到 int 类型的转换运算符，因此在需要 int 的地方都可以使用它，例如 switch 语句：
switch (access) {
    case FileAccess::Read:
        break;
    case FileAccess::Write:
        break;
}
当然我们不愿意每次都手工编写这样的结构。通过使用宏，我们可以很容易做到这一点：
#define DECLARE_ENUM(E) /
struct E /
{ /
public: /
    E(int value = 0) : _value((__Enum)value) { /
    } /
    E& operator=(int value) { /
        this->_value = (__Enum)value; /
        return *this; /
    } /
    operator int() const { /
        return this->_value; /
    } /
/
    enum __Enum {
#define END_ENUM() /
    }; /
/
private: /
    __Enum _value; /
};
我们现在可以按如下的方式定义前面的枚举，并且不比直接写 enum 复杂多少。
DECLARE_ENUM(FileAccess)
    Read = 0x1,
    Write = 0x2,
END_ENUM()
DECLARE_ENUM(FileShare)
    Read = 0x1,
    Write = 0x2,
END_ENUM()
