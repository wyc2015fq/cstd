# C++风格的强制转换 - 逍遥剑客 - CSDN博客
2007年10月14日 14:48:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2777
看起来，使用C风格的强制转换可以把想要的任何东西转换成合乎心意的类型。那为什么还需要一个新的C++类型的强制转换呢？新类型的强制转换可以提供更好的控制强制转换过程，允许控制各种不同种类的强制转换。
C++里有4种不同类型的强制转换操作，取决于要强制转换的东西：static_cast、const_cast、reinterpret_cast和dynamic_cast。
C++强制转换操作与传统的强制转换相比，有一点语法上的细微差别。稍微有点冗长，而且在代码里有点突出。但是不要因为多敲几下键盘就烦了，它也比C风格的cast更能体现强制转换，新风格的cast是下面的格式：
static_cast<type>(expression)
下面的代码使用了C++风格的cast：
//C++ -style cast
float f2 = static_cast<float>(n) / 100;     //cast to a float
C++风格的强制转换其他的好处是，它们能更清晰的表明它们要干什么。程序员只要扫一眼这样的代码，就能立即知道一个强制转换的目的。是的，需要多敲几下键盘，但是确实值得。
**1．static_cast**
static_cast操作是比C风格的转换的更严格版本。它会通知编译器，尝试着在两种数据类型之间进行转换。和C风格的转换一样，它能在内置的数据类型间互相转换，甚至在有可能有精度损失的情况下也能转换。然而和C风格的转换不一样的是，static_cast只能在有联系的指针类型间进行转换。可以在继承体系中把指针转换来、转换去，但是不能转换成继承体系外的一种类型。
class A
{
};
class B : public A
{
};
//Unrelated to A and B
class C
{
};
A * a = new A;
// OK, B is a child of A
B *b = static_cast<B* > (a);
// Compile error. C is unrelated to A
C * c = static_cast<C*>(a);
// The old C cast would work just fine (but what would
// the program do?)
C * c = (C*)(a);
其余的不同在于，static_cast不能实行对常量的改变。和C风格的强制转换一样，如果不能把一个类型强制转换成另外一个，那转换就会失败。
**2．const_cast**
const_cast操作不能在不同的种类间转换。相反，它仅仅把一个它作用的表达式转换成常量。它可以使一个本来不是const类型的数据转换成const类型的，或者把const属性去掉。通常，没有必要把一个非const类型的数据转换成const类型的。这种转换会自动进行，因为这是一个不太严格的转换。换句话说，从const到非const，只能通过强制转换来进行。被迫使用const_cast表明有东西没有正确地符合程序的设计。大多数使用const_cast的情况是由于调用老式的用const修饰的函数。如果发现调用自己的函数，竟然使用了const_cast，那就赶紧打住，重新考虑一下设计吧。
**3．reinterpret_cast**
reinterpret_cast操作符有着和C风格的强制转换同样的能力。它可以转化任何内置的数据类型为其他任何的数据类型，也可以转化任何指针类型为其他的类型。它甚至可以转化内置的数据类型为指针，无须考虑类型安全或者常量的情形。reinterpret_cast的结果就是实现的依赖性，它依靠每一个要转换对象的特定内存分布。使用reinterpret_cast 的场合不多，仅在非常必要的情形下，其他类型的强制转换不能满足要求时才使用。
**4．dynamic_cast**
在第2章里讨论了多继承的问题，曾简单地提到dynamic_cast。所有其他的强制转换操作都是编译器在编译期处理的。至于转换的结果要么正常实现，要么导致一个编译错误。无论哪种情况，都不涉及到运行期开销。但是dynamic_cast有着显著的不同。dynamic_cast 仅能应用于指针或者引用，不支持内置数据类型。然而关键的不同在于，在运行期，会检查这个转换是否可能。它不仅仅像static_cast那样，检查转换前后的两个指针是否属于同一个继承树，它还要检查被指针引用的对象的实际类型，确定转换是否可行。如果可以，它返回一个新指针，甚至计算出为处理多继承的需要的必要的偏移量。如果这两个指针间不能转换，转换就会失败，此时返回空指针（NULL）。很明显，为了让dynamic_cast能正常工作，必须让编译器支持运行期类型信息（RTTI）。如果不希望编译器支持RTTI，则必须找到dynamic_cast的替代方法。
