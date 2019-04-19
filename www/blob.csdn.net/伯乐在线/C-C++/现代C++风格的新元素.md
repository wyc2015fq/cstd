# 现代C++风格的新元素 - 文章 - 伯乐在线
> 
**“C++11就像一门新的语言。” – Bjarne Stroustrup**
C++11标准推出了很多有用的新特性，本文特别关注那些相比C++98更像是一门新语言的特性，理由是：
- 这些特性改变了编写C++程序使用的代码风格和习语【译注 1】，通常也包括你设计C++函数库的方式。例如，你会看到更多参数和返回值类型为智能指针***(smart pointer)***，同时也会看到函数通过值传递返回大型对象。你将会发现在大多数的代码示例中充斥着新特性的身影。例如，几乎每5行现代C++代码示例都会使用到**auto**。
- C++11的其他特性也很棒。但是请先熟悉下面这些，正是由于这些特性的广泛使用使得C++11代码如同其他现代主流语言一样整洁、安全和高效，与此同时保持了C++传统的性能优势。
提示：
- 与Strunk & White【译注 2】一样，本文只做概要总结而不是详尽基本原理和优缺点分析。详细分析请参见其他文章。
- 本文会不断更新，主要变更及内容增加请参见文末变更历史。
译注：
1. Programming idiom：编程习语，在一种或多种编程语言中重复出现的表达形式，用来表示没有在编程语言中内置的简单的任务或者算法，也可以用来表示在编程语言中内置的不常用或者不典型的某个特性。编程习语也可以在更广泛的范围内使用，比如代指复杂的算法或者设计模式。 
2. Strunk & White：代指William Strunk Jr.和E. B. White出版的”The elements of style”，中文版《风格的要素》一书，该书介绍了英语写作要遵循的基本风格。
**auto**
基于以下两个原因，尽可能使用**auto**：首先，使用**auto**会避免重复声明编译器已经知道的类型。

C++
```
// C++98
map<int,string>::iterator i = m.begin();
// C++11
auto i = begin(m);
```
其次，当使用未知类型或者类型名称不易理解时使用**auto**会更加便利，例如大多数的lambda函数【译注 3】——你甚至不能简单的拼写出类型的名字。

C++
```
// C++98
binder2nd< greater<int> > x = bind2nd( greater<int>(), 42 );
// C++11
auto x = [](int i) { return i > 42; };
```
需要注意，使用**auto**并不改变代码的含义。代码仍然是静态类型【译注 4】，每个表达式的类型都是清晰和明确的；C++11只是不需要我们重复声明类型名。一些人刚开始可能会害怕在这里使用**auto**，因为感觉好像没有（重复）声明我们需要的类型就意味着可能会碰巧得到一个不同的类型。如果你想要明确地进行一次***强制类型转换***，没有问题，声明目标类型就好了。然而大多数情况下，只要使用**auto**就可以了；几乎不会出现错误地拿到一个不同类型的情况，即便出现错误，C++的强静态类型系统也会由编译器让你知道这个错误，因为你正试图访问一个变量没有的成员函数或是错误地调用了该函数。
译注：
3. lambda function (λ函数)：编程语言支持λ函数/λ表达式可以使得代码更易于理解，同时也可以使得代码变得更简洁，关于λ函数的技术解释可以参考，维基百科labmda calculus，也可以从[《从.NET中委托写法的演变谈开去（中）：Lambda表达式及其优势》](http://www.cnblogs.com/JeffreyZhao/archive/2009/08/07/from-delegate-to-others-2.html)博客中得到直观的解释。
4. 动态类型语言（dynamic typing language）是指类型检查发生在运行期间（run-time）的语言。静态类型语言（static typing language）是类型检查发生在编译期间（compile-time）的语言。
**智能指针：无须delete**
请始终使用标准智能指针以及非占有原始指针***(non-owning raw pointer)***。绝不要使用占有原生指针***(owning raw pointer)***和**delete**操作，除非你是在实现自己的底层数据结构这种少见的情况下（即使在此时也需要在**class**范围内保持完好的封装）。如果只能够知道你是另一个对象唯一的所有者，请使用**unique_ptr**来表示唯一所有权。一个**“new T”**表达式会马上初始化另一个引用它的对象，通常是一个**unique_ptr**。

C++
```
// C++11 Pimpl Idiom
class widget {
widget();
~widget();
private:
class impl;
unique_ptr<impl> pimpl;
};
// in .cpp file
class impl {
:::
};
widget::widget()
: pimpl( new impl() )
{
}
widget::~widget() = default;
```
使用**shared_ptr**来表示共享所有权。推荐使用**make_shared**来有效地创建共享对象。

C++
```
// C++98
widget* pw = new widget();
:::
delete pw;
// C++11
auto pw = make_shared<widget>();
```
使用**weak_ptr**来退出循环并且表示可选项（例如，实现一个对象缓存）

C++
```
// C++11
class gadget;
class widget {
private:
shared_ptr<gadget> g; // if shared ownership
};
class gadget {
private:
weak_ptr<widget> w;
};
```
如果你知道另一个对象存在时间会更长久并且希望跟踪它，使用一个非占有 ***（non-owning)***原始指针。


```
// C++11
class node {
vector< unique_ptr<node> > children;
node* parent;
public:
:::
};
```
**nullptr**
始终使用**nullptr**表示一个**null**指针值，绝不要使用数字**0**或者**NULL**宏，因为它们也可以代表一个整数或者指针从而产生歧义。

C++
```
// C++98
int* p = 0;
// C++11
int* p = nullptr;
```
**Range for**
基于范围的循环使得按顺序访问其中的每个元素变得非常方便。

C++
```
// C++98
for( vector<double>::iterator i = v.begin(); i != v.end(); ++i ) {
total += *i;
}
// C++11
for( auto d : v ) {
total += d;
}
```
**非成员（nonmember） begin和end**
始终使用非成员**begin**和**end**，因为它是可扩展的并且可以应用在所有的容器类型***(container type)***，不仅仅是遵循了STL风格提供了**.begin()**和**.end()**成员函数的容器，甚至数组都可以使用。
如果你使用了一个非STL风格的**collection**类型，虽然提供了迭代但没有提供STL的**.begin()**和**.end()**，通常可以为这个类型编写自己的非成员begin和end来进行重载。这样你就可以使用STL容器的编程风格来遍历该类型。C++11标准提供了示例数组就是这样一个类型，标准同时为数组提供了**begin**和**end**。

C++
```
vector<int> v;
int a[100];
// C++98
sort( v.begin(), v.end() );
sort( &a[0], &a[0] + sizeof(a)/sizeof(a[0]) );
// C++11
sort( begin(v), end(v) );
sort( begin(a), end(a) );
```
**Lambda函数和算法**
Lambda函数是决定乾坤的因素，它会使你编写的代码变得更优雅、更快速。Lambda使得STL算法的可用性提高了近100倍。新近开发的C++函数库都是基于lambda可以用的前提（例如，PPL）并且有些函数库甚至要求你编写lambda来调用函数库（例如，C++ AMP）
下面是一个快速示例：找到**v**里面大于**x**并且小于**y**的第一个元素。在C++11中，最简单和干净的代码就是调用一个标准函数。

C++
```
// C++98: 直接编写一个循环 （使用std::find_if会非常困难）
vector<int>::iterator i = v.begin(); // 因为我们需要在后边用到i
for( ; i != v.end(); ++i ) {
if( *i > x && *i < y ) break;
}
// C++11: use std::find_if
auto i = find_if( begin(v), end(v), [=](int i) { return i > x && i < y; } );
```
想要使用C++编写一个循环或者类似的新特性？不用着急；只要编写一个模板函数***（template function)***(函数库算法），并且***几乎可以***将lambda当做语言特性来使用，与此同时会更加灵活，因为它不是固定的语言特性而是一个真正的函数库。


C#
```
// C#
lock( mut_x ) {
... use x ...
}
```

C++
```
// 不使用lambda的C++11：已经非常简洁并且更灵活（例如，可以使用超时以及其他选项）
{
lock_guard<mutex> hold( mut_x );
... use x ...
}
// 使用了lambda的C++11可以带一个辅助算法：在C++中使用C#的文法
// 算法：template<typename T, typename F> void lock( T& t, F f ) { lock_guard<T> hold(t); f(); }
lock( mut_x, [&]{
... use x ...
});
```
去熟悉lambda吧。你会不断用到它，不仅仅在C++中——lambda已经广泛应用于很多主流的编程语言。一个开始的好去处请参考我在PDC2010的演讲[《无处不在的lambda》](http://herbsutter.com/2010/10/30/pdc-languages-panel-andshortened-lambdas-talk/)
**Move / &&**
Move被认为是**copy**的最佳优化，尽管它也使得其他事情成为可能比如信息被转发。

C++
```
// C++98：避免copy的替代方法
vector<int>* make_big_vector(); // 选择1: 返回指针: 没有拷贝，但不要忘记delete
:::
vector<int>* result = make_big_vector();
void make_big_vector( vector<int>& out ); // 选择2: 通过引用传递: 没有拷贝，但是调用者需要传入一个有名对象
:::
vector<int> result;
make_big_vector( result );
// C++11: move
vector<int> make_big_vector(); // 通常对于”被调用者(callee)分配的空间“也适用
:::
vector<int> result = make_big_vector();
```
Move语法改变了我们设计API的方式。我们可以更多地设计通过值传递。为你的类型启用**move**语法，使用时会比**copy**更有效。
**更多变化**
还有更多现代C++的特性。并且我计划在未来编写更多深入C++11新特性以及其他特性的短文，我们会知道更多并且喜欢上它。
但目前，这是必须知道的新特性。这些特性组成了现代C++风格的核心，使得C++代码看起来和执行时像他们设计的那样，你将会看到这些特性会出现在几乎每一段你看到或者编写的现代C++代码中。并且它们使得现代C++更加干净、安全且快速，使得C++在未来的若干年仍然是我们产业的依靠。
**主要变更**
2011-10-30: 为Lambda增加C# lock示例. 重新组织智能指针首先介绍**unique_prt**。
【如需转载，请标注并保留原文链接、译文链接和译者等信息，谢谢合作！】
