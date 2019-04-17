# inline总结 - 工作笔记 - CSDN博客





2012年04月28日 09:29:13[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5401标签：[编译器																[string																[exception																[class																[优化																[algorithm](https://so.csdn.net/so/search/s.do?q=algorithm&t=blog)
个人分类：[基础编程](https://blog.csdn.net/App_12062011/article/category/952065)





                
inline 函数——多么棒的主意啊！它们看起来像函数，它们产生的效果也像函数，它们在各方面都比宏好得太多太多，而你却可以在调用它们时不招致函数调用的成本。你还有什么更多的要求呢？

　　 实际上你得到的可能比你想的更多，因为避免函数调用的成本只是故事的一部分。在典型情况下，编译器的优化是为了一段连续的没有函数调用的代码设计的，所以当你 inline 化一个函数，你可能就使得编译器能够对函数体实行上下文相关的特殊优化。大多数编译器都不会对 "outlined" 函数调用实行这样的优化。


　　 然而，在编程中，就像在生活中，没有免费午餐，而 inline 函数也不例外。一个 inline 函数背后的思想是用函数本体代替每一处对这个函数的调用，而且不必拿着统计表中的 Ph.D. 就可以看出这样可能会增加你的目标代码的大小。在有限内存的机器上，过分热衷于 inline 化会使得程序对于可用空间来说过于庞大。即使使用了虚拟内存，inline 引起的代码膨胀也会导致附加的分页调度，减少指令缓存命中率，以及随之而来的性能损失。


　　 在另一方面，如果一个 inline 函数本体很短，为函数本体生成的代码可能比为一个函数调用生成的代码还要小。如果是这种情况，inline 化这个函数可以实际上导致更小的目标代码和更高的指令缓存命中率！ 记住，inline 是向编译器发出的一个请求，而不是一个命令。这个请求能够以显式的或隐式的方式提出。隐式的方法就是在一个类定义的内部定义一个函数： class Person {


　 public: 

　　 ... 

　　 int age() const { return theAge; } // an implicit inline request: age is 

　　 ... // defined in a class definition 

　 private: 

　　 int theAge; 

}; 

　　 这样的函数通常是成员函数，不过我们知道友元函数也能被定义在类的内部，如果它们在那里，它们也被隐式地声明为 inline。 

　　 显式的声明一个 inline 函数的方法是在它的声明之前加上 inline 关键字。例如，以下就是标准 max 模板（来自 <algorithm>）经常用到的的实现方法： template<typename T> // an explicit inline


inline const T& std::max(const T& a, const T& b) // request: std::max is 

{ return a < b ? b : a; } // preceded by "inline" 

　　 max 是一个模板的事实引出一个观察结论：inline 函数和模板一般都是定义在头文件中的。这就使得一些程序员得出结论断定函数模板必须是 inline。这个结论是非法的而且有潜在的危害，所以它值得我们考察一下。 inline 函数一般必须在头文件内，因为大多数构建环境在编译期间进行 inline 化。为了用被调用函数的函数本体替换一个函数调用，编译器必须知道函数看起来像什么样子。（有一些构建环境可以在连接期间进行 inline 化，还有少数几个——比如，基于
 .NET Common Language Infrastructure (CLI) 的控制环境——居然能在运行时 inline 化。然而，这些环境都是例外，并非规则。inline 化在大多数 C++ 程序中是一个编译时行为。）


　　 模板一般在头文件内，因为编译器需要知道一个模板看起来像什么以便用到它时对它进行实例化。（同样，也不是全部如此。一些构建环境可以在连接期间进行模板实例化。然而，编译期实例化更为普遍。） 模板实例化与 inline 化无关。如果你写了一个模板，而且你认为所有从这个模板实例化出来的函数都应该是 inline 的，那么就声明这个模板为 inline，这就是上面的 std::max 的实现被做的事情。但是如果你为没有理由要 inline 化的函数写了一个模板，就要避免声明这个模板为
 inline（无论显式的还是隐式的）。inline 化是有成本的，而且你不希望在毫无预见的情况下遭遇它们。我们已经说到 inline 化是如何引起代码膨胀的，但是，还有其它的成本，过一会儿我们再讨论。


　　 在做这件事之前，我们先来完成对这个结论的考察：inline 是一个编译器可能忽略的请求。大多数编译器拒绝它们认为太复杂的 inline 函数（例如，那些包含循环或者递归的），而且，除了最细碎的以外的全部虚拟函数的调用都不会被 inline 化。不应该对这后一个结论感到惊讶。虚拟意味着“等待，直到运行时才能断定哪一个函数被调用”，而 inline 意味着“执行之前，用被调用函数取代调用的地方”。如果编译器不知道哪一个函数将被调用，你很难责备它们拒绝 inline 化这个函数本体。


　　 所有这些加在一起，得出：一个被指定的 inline 函数是否能真的被 inline 化，取决于你所使用的构建环境——主要是编译器。幸运的是，大多数编译器都有一个诊断层次，在它们不能 inline 化一个你提出的函数时，会导致一个警告。


　　 有时候，即使当编译器完全心甘情愿地 inline 化一个函数，他们还是会为这个 inline 函数生成函数本体。例如，如果你的程序要持有一个 inline 函数的地址，编译器必须为它生成一个 outlined 函数本体。他们怎么能生成一个指向根本不存在的函数的指针呢？再加上，编译器一般不会对通过函数指针的调用进行 inline 化，这就意味着，对一个 inline 函数的调用可能被也可能不被 inline 化，依赖于这个调用是如何做成的： inline void f() // assume compilers
 are willing to inline calls to f 

void (*pf)() = f; // pf points to f 

... 

f(); // this call will be inlined, because it""s a "normal" call 

pf(); // this call probably won""t be, because it""s through 

// a function pointer 

　　 甚至在你从来没有使用函数指针的时候，未 inline 化的 inline 函数的幽灵也会时不时地拜访你，因为程序员并不必然是函数指针的唯一需求者。有时候编译器会生成构造函数和析构函数的 out-of-line 拷贝，以便它们能得到指向这些函数的指针，在对数组中的对象进行构造和析构时使用。


　　 事实上，构造函数和析构函数对于 inline 化来说经常是一个比你在不经意的检查中所能显示出来的更加糟糕的候选者。例如，考虑下面这个类 Derived 的构造函数： class Base {


　 public: 

　　 ... 

　 private: 

　　 std::string bm1, bm2; // base members 1 and 2 

}; 

class Derived: public Base { 

　 public: 

　　 Derived() {} // Derived""s ctor is empty - or is it? 

　　 ... 

　 private: 

　　 std::string dm1, dm2, dm3; // derived members 1-3 

}; 

　　 这个构造函数看上去像一个 inline 化的极好的候选者，因为它不包含代码。但是视觉会被欺骗。 

　　 C++ 为对象被创建和被销毁时所发生的事情做出了各种保证。例如，当你使用 new 时，你的动态的被创建对象会被它们的构造函数自动初始化，而当你使用 delete。则相应的析构函数会被调用。当你创建一个对象时，这个对象的每一个基类和每一个数据成员都会自动构造，而当一个对象被销毁时，则发生关于析构的反向过程。如果在一个对象构造期间有一个异常被抛出，这个对象已经完成构造的任何部分都被自动销毁。所有这些情节，C++ 只说什么必须发生，但没有说如何发生。那是编译器的实现者的事，但显然这些事情不会自己发生。在你的程序中必须有一些代码使这些事发生，而这些代码——由编译器写出的代码和在编译期间插入你的程序的代码——必须位于某处。有时它们最终就位于构造函数和析构函数中，所以我们可以设想实现为上面那个声称为空的
 Derived 的构造函数生成的代码就相当于下面这样： Derived::Derived() // conceptual implementation of 

{ 

　 // "empty" Derived ctor 

　 Base::Base(); // initialize Base part 

　 try { dm1.std::string::string(); } // try to construct dm1 

　 catch (...) { // if it throws, 

　　 Base::~Base(); // destroy base class part and 

　 throw; // propagate the exception 

} 

try { dm2.std::string::string(); } // try to construct dm2 

catch(...) { 

　 // if it throws, 

　 dm1.std::string::~string(); // destroy dm1, 

　 Base::~Base(); // destroy base class part, and 

throw; // propagate the exception 

} 

try { dm3.std::string::string(); } // construct dm3 

catch(...) { 

　 // if it throws, 

　 dm2.std::string::~string(); // destroy dm2, 

　 dm1.std::string::~string(); // destroy dm1, 

　 Base::~Base(); // destroy base class part, and 

throw; // propagate the exception 

} 

} 

　　 这些代码并不代表真正的编译器所生成的，因为真正的编译器会用更复杂的方法处理异常。尽管如此，它还是准确地反映了 Derived 的“空”构造函数必须提供的行为。不论一个编译器的异常多么复杂，Derived 的构造函数至少必须调用它的数据成员和基类的构造函数，而这些调用（它们自己也可能是 inline 的）会影响它对于 inline 化的吸引力。


　　 同样的原因也适用于 Base 的构造函数，所以如果它是 inline 的，插入它的全部代码也要插入 Derived 的构造函数（通过 Derived 的构造函数对 Base 的构造函数的调用）。而且如果 string 的构造函数碰巧也是 inline 的，Derived 的构造函数中将增加五个那个函数代码的拷贝，分别对应于 Derived 对象中的五个 strings（两个继承的加上三个它自己声明的）。也许在现在，为什么说是否 inline 化 Derived 的构造函数不是一个不经大脑的决定就很清楚了。类似的考虑也适用于
 Derived 的析构函数，用同样的或者不同的方法，必须保证所有被 Derived 的构造函数初始化的对象被完全销毁。 

库设计者必须评估声明函数为 inline 的影响，因为为库中的客户可见的 inline 函数提供二进制升级版本是不可能的。换句话说，如果 f 是一个库中的一个 inline 函数，库的客户将函数 f 的本体编译到他们的应用程序中。如果一个库的实现者后来决定修改 f，所有使用了 f 的客户都必须重新编译。这常常会令人厌烦。在另一方面，如果 f 是一个非 inline 函数，对 f 的改变只需要客户重新连接。这与重新编译相比显然减轻了很大的负担，而且，如果库中包含的函数是动态链接的，这就是一种对于用户来说完全透明的方法。


　　 为了程序开发的目标，在头脑中牢记这些需要考虑的事项是很重要的，但是从编码期间的实用观点来看，占有支配地位的事实是：大多数调试器会与 inline 函数发生冲突。这不应该是什么重大的发现。你怎么能在一个不在那里的函数中设置断点呢？虽然一些构建环境设法支持 inline 函数的调试，多数环境还是简单地为调试构建取消了 inline 化。


　　 这就导出了一个用于决定哪些函数应该被声明为 inline，哪些不应该的合乎逻辑的策略。最初，不要 inline 任何东西，或者至少要将你的 inline 化的范围限制在那些必须 inline 的和那些实在微不足道的函数上。通过慎重地使用 inline，你可以使调试器的使用变得容易，但是你也将 inline 化放在了它本来应该在的地位：作为一种手动的优化。不要忘记由经验确定的 80-20 规则，它宣称一个典型的程序用 80% 的时间执行 20% 的代码。这是一个重要的规则，因为它提醒你作为一个软件开发者的目标是识别出能全面提升你的程序性能的
 20% 的代码。你可以 inline 或者用其他方式无限期地调节你的函数，但除非你将精力集中在正确的函数上，否则就是白白浪费精力。 

　　 Things to Remember 

　　 ·将大部分 inline 限制在小的，调用频繁的函数上。这使得程序调试和二进制升级更加容易，最小化潜在的代码膨胀，并最大化提高程序速度的几率。 

　　 ·不要仅仅因为函数模板出现在头文件中，就将它声明为 inline。](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)




