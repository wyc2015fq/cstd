# C++之父：C++ 的五个普遍误解（1） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Sheng Gordon](http://www.jobbole.com/members/shenggordon) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Bjarne Stroustrup](http://isocpp.org/blog/2014/12/myths-1)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
[编注：为了增加您冬天阅读的乐趣，我们很荣幸的奉上Bjarne Stroustrup大神的这个包含3个部分的系列文章。这是第一部分；[第二](http://blog.jobbole.com/82461/)和[第三部分](http://blog.jobbole.com/82628/)将在接下来的两个周一发布，即在圣诞节之前完成这个系列。请欣赏。——Ed]
**1. 简介**
本系列包括 3 篇文章，我将向大家展示并澄清关于C++的五个普遍的误解：
- 1. “要理解C++，你必须先学习C”
- 2. “C++是一门面向对象的语言”
- 3. “为了软件可靠性，你需要垃圾回收”
- 4. “为了效率，你必须编写底层代码”
- 5. “C++只适用于大型、复杂的程序”
如果你深信上述误解中的任何一个，或者有同事深信不疑，那么这篇短文正是为你而写。对某些人，某些任务，在某些时间，其中一些误解曾经只是正确的。然而，在如今的C++，应用广泛使用的最先进的ISO C++ 2011编译器和工具，它们只是误解。
我认为这些误解是“普遍的”，是因为我经常听到。偶尔，它们有原因来支持，但是它们经常地被作为明显的、不需要理由的支持地表达出来。有时，它们成为某些场景下不考虑使用C++的理由。
每一个误解，都需要一大篇文章，甚至一本书来澄清，但是这里我的目标很简单，就是抛出问题，并简明地陈述我的原因。
**2. 误解1：“要理解C++，你必须先学习C”**
不。学习C++基础编程比学习C要容易地多。
C几乎是C++的一个子集，但是它不是最先要学习的最好的子集，因为C缺少计数支持，类型安全，和易用的标准库，而C++为简单任务提供了这些。考虑一个拼接email地址的简单函数：

C++
```
; html-script: false ]string compose(const string& name, const string& domain)
{
  return name+'@'+domain;
}
```
它可能被这样使用

C++
```
; html-script: false ]string addr = compose("gre","research.att.com");
```
而C语言版本需要显式地字符复制和显式地内存管理：

C++
```
; html-script: false ]char* compose(const char* name, const char* domain)
{
  char* res = malloc(strlen(name)+strlen(domain)+2); // space for strings, '@', and 0
  char* p = strcpy(res,name);
  p += strlen(name);
  *p = '@';
  strcpy(p+1,domain);
  return res;
}
```
它可能被这样使用

C++
```
; html-script: false ]char* addr = compose("gre","research.att.com");
// …
free(addr); // release memory when done
```
哪个版本你更愿意教？哪个版本更容易使用呢？刚才我的C版本代码写对了吗？你确定？为什么？
最后，哪个版本可能更有效率呢？是的，是C++版本。因为它不需要数参数中字符的个数，也不需要释放参数中短字符串的空间（动态内存）。
**2.1 学习C++**
这不是一个奇怪的孤立例子。我认为它是典型的。那为什么有那么多老师坚持“先学习C”的观点？
• 因为多年来他们一直这么做。
• 因为这是课程所要求的。
• 因为老师们年轻时就是这么学习的。
• 因为C比C++小，就认为C比C++简单。
• 因为学生们迟早要学习C（或者C++的C子集）。
然而，C并不是最先学习C++的最容易或者最常用的子集。更进一步，一旦你知道了C++的合理数量，C子集很容易学习。先学习C，会导致不断忍受错误，以及学习如何避免这些错误，而在C++中很容易避免这些错误。
对于用现代的方法教学C++，看我的书Programming: Principles and Practice Using C++。它甚至在结尾有一章，专门讲如何使用C。它在多个大学中数以万计的初学者中成功应用。为了灵活学习，这本书的第二版中使用了C++11和C++14工具。
在C++11[11-12]中，C++对初学者更友好。例如，标准库中使用元素序列初始化一个vector：

C++
```
; html-script: false ]vector<int> v = {1,2,3,5,8,13};
```
在C++98中，我们只能使用列表来初始化数组。而在C++11中，我们可以定义一个构造函数，接收可以是任意需要类型的{}初始化列表。
我们能够使用range-for循环来遍历vector：

C++
```
; html-script: false ]for(int x : v) test(x);
```
它将对v中的每一个元素，调用一次test()。
range-for循环可以遍历任意序列，因此我们可以直接使用初始化列表来简化上面的例子：

C++
```
; html-script: false ]for (int x : {1,2,3,5,8,13}) test(x);
```
C++11的一个目标就是，让简单的事情简单化。自然地，它在没有增加性能负担的前提下实现了。
**3. 误解2：“C++是一门面向对象的语言”**
不。C++支持OOP和其他编程风格，但它并不局限于狭隘的“面向对象”。它综合地支持了包括面向对象和泛型编程技术。通常，一个问题的最优解决方案，包含不止一种风格（范例）。“最优”，我指的是最短、最易于理解、最有效率和最易于维护等。
“C++是一门面向对象的语言”使人们认为C++不是必要的（当与C做比较时），除非你需要一个巨大的类继承层次以及很多须函数（运行时多态）——对很多人和很多问题，这样应用并不合适。相信这个误区导致C++因为不是纯面向对象而遭到谴责；毕竟，如果你把“好”和“面向对象”等同起来，那么C++明显包含了很多不是面向对象的东西，一定会被认为是“不好”。不管是哪种情形，这个误解为不学习C++提供了一个很好的接口。
考虑一个例子：

C++
```
; html-script: false ]void rotate_and_draw(vector<Shape*>& vs, int r)
{
  for_each(vs.begin(),vs.end(), [](Shape* p) { p->rotate(r); });  // rotate all elements of vs
  for (Shape* p : vs) p->draw();                                  // draw all elements of vs
}
```
它是面向对象的吗？当然是；它关键依赖类的虚函数机制。它是泛型吗？当然是；它关键依赖于一个参数化容器（vector）和泛型函数for_each。它是函数式吗？某种程度上；它使用了lambda表达式（[]构造器）。那么它到底是什么？它是现代C++：C++11。
我使用了range-for和标准库算法for_each，就是为了炫一下特性。在真实的代码中，不管我使用哪种方式，我只会使用一种循环。
**3.1 泛型编程**
你想让这段代码更通用吗？毕竟，它只在指向Shapes的指针向量中使用。如果使用列表和内置数组会怎么样？“智能指针”（资源管理指针）呢，例如shared_ptr和unique_ptr？对于那些你可以调用draw()和rotate()方法，但是类名称不是Shape的对象呢？考虑：

C++
```
; html-script: false ]template<typename Iter>
void rotate_and_draw(Iter first, Iter last, int r)
{
  for_each(first,last,[](auto p) { p->rotate(r); });  // rotate all elements of [first:last)
  for (auto p = first; p!=last; ++p) p->draw();       // draw all elements of [first:last)
}
```
对于任意你需要从first遍历到last的序列，它能够运行。这就是C++标准库算法的风格。我使用了auto，来避免不得不对“类似shape的对象”的接口命名。这是一个C++11特性，即“使用表达式的类型初始化”，因此对于for循环，p的类型由第一次赋值的类型决定。使用auto来表示lambda参数类型是C++14的一个特性，但是已经在使用了。
考虑：

C++
```
; html-script: false ]void user(list<unique_ptr<Shape>>& lus, Container<Blob>& vb)
{
  rotate_and_draw(lst.begin(),lst.end());
  rotate_and_draw(begin(vb),end(vb));
}
```
这里，我假设Blob是一个包含draw()和rotate()操作的图形类型，而Container是某种容器类型。标准库列表(std::list)有成员函数begin()和end()，来帮助用户遍历它序列的元素。这是经典的面向对象编程。但是如果Container不支持C++标准算法库遍历半开序列[b:e]的约定呢？如果不包含begin()和end()成员呢？好了，我还从来没有见过哪些容器类是不能被遍历的，因此我们可以定义适当语义的独立begin()和end()函数。这个标准库支持C样式的数组，因此如果Container是C样式的数组，问题能够解决——并且C样式的数组仍然很常见。
**3.2 适应性**
考虑一个更难的情况：如果容器包含对象指针，而且有着不同的访问和遍历模式呢？例如，假设你需要这样访问一个Container：

C++
```
; html-script: false ]for (auto p = c.first(); p!=nullptr; p=c.next()) { /* do something with *p */}
```
这种风格并非不常见。我们可以把它转化成一个[b:e)序列：

C++
```
template<typename T> struct Iter {
  T* current;
  Container<T>& c;
};
template<typename T> Iter<T> begin(Container<T>& c) { return Iter<T>{c.first(),c}; }
template<typename T> Iter<T> end(Container<T>& c)   { return Iter<T>{nullptr}; }
template<typename T> Iter<T> operator++(Iter<T> p)  { p.current = c.next(); return this; }
template<typename T> T*      operator*(Iter<T> p)   { return p.current; }
```
注意这种修改方式是非侵入式的：我不需要修改Container或者它的继承类，来把Container映射到支持C++标准算法的遍历模型。这是一种形式的适应，而不是重构。
我选择这个例子，是为了展示泛型编程技术并不局限于标准算法库（标准算法中它很常见）。同样，对大多数常见的“面向对象”定义，它们不是面向对象的。
C++代码必须是面向对象的想法（即到处使用类继承和虚方法），可能严重损坏效率。如果你需要一组类型的运行时解决方案，面向对象编程的观点是伟大的。我就经常使用它。然而，它是相对严格的（并不是每个类型都适应类继承），并且虚方法调用抑制了内链函数（它可以在一些简单而重要的场景中降低你50倍的速度）。
**附言**
在我的下一部分，我将讲解“[为了软件可靠性，你需要垃圾回收](http://blog.jobbole.com/82461/)”。
