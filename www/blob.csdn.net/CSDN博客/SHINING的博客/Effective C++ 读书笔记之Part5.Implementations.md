# Effective C++ 读书笔记之Part5.Implementations - SHINING的博客 - CSDN博客
2013年05月23日 11:23:08[snsn1984](https://me.csdn.net/snsn1984)阅读数：1460标签：[读书笔记																[C++																[语言																[文档																[实现](https://so.csdn.net/so/search/s.do?q=实现&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)](https://so.csdn.net/so/search/s.do?q=读书笔记&t=blog)
个人分类：[C++](https://blog.csdn.net/snsn1984/article/category/488621)
                
 26. Postpone variable definitions as long as possible.
总结：
尽可能延后变量定义式的出现。这样做可增加程序的清晰度并改善程序效率。
批注：
纯C语言此处有冲突，C语言要求变量定义出现在代码快的开始部分。

27. Minimize casting.
1）const_cast 通常被用来将对象的常量性转除(cast away the constness) 。它也是唯一有此能力的 C++-style 转型操作符。
2）dynamic_cast主要用来执行"安全向下转型"( safe downcasting) ,也就是用来决定某对象是否归属继承体系中的某个类型。它是唯一无法由旧式语法执行的动作,也是唯一可能耗费重大运行成本的转型动作。
3）reinterpret_cast 意图执行低级转型,实际动作(及结果)可能取决于编译器,这也就表示它不可移植。例如将一个 pointer to int 转型为一个 int 。这一类转型在低级代码以外很少见。本书只使用一次,那是在讨论如何针对原始内存 (rawmemory) 写出一个调试用的分配器 (debugging allocator) 时,见条款50。
4）static_cast 用来强迫隐式转换 (implicit conversions) ,例如将 non-const对象转为 const 对象(就像条款3 所为) ,或将 int 转为 double 等等。它也可以用来执行上述多种转换的反向转换,例如将void*指针转为 typed 指针,将pointer-to-base 转为 pointer-to-derived。但它无挂将 const 转为 non-canst'一一这个只有const_cast 才办得到。
总结：
第一，如果可以，尽量避免转型，特别是在注重效率的代码中避免dynamic_casts。如果有个设计需要转型动作，试着发展无需转型的替代设计。
第二，如果转型是必要的，试着将它隐藏于某个函数背后。客户随后可以调用该函数，而不需要将转型放进他们自己的代码内。
第三，宁可使用C++-style（新式）转型，不要使用旧式转型。前者很容易辨识出来，而且也比较有着分们别类的职掌。

28.Avoid returning "handles" to object internals.
总结：
避免返回handles（包括references、指针、迭代器）指向对象内部。遵守这个条款可增加封装性，帮助const成员函数的行为像个const，并将发生“虚吊号码牌”（dangling handles）的可能性将至最低。
批注：
虚吊号码牌应该指的就是指向的对象已经被销毁了，但是返回的指针或者引用还在使用。
29.Strive for exception-safe code.
当异常被抛出时。带有异常安全性的函数会：
1）不泄露任何资源。
2）不允许数据被毁坏。
异常安全函数（Exception-safe functions）提供以下三个保证之一：
1）基本承诺：如果异常被抛出，程序内的任何事物仍然保持在有效状态下。
2）强烈保证：如果异常被抛出，程序状态不改变。
3）包抛掷（nothrow）保证：承诺绝不抛出异常，因为他们总能完成它们原先承诺的功能。
copy and swap原则：为你打算修改的对象（原件）做出一份副本，然后在那副本身上做一切必要修改。若有任何修改动作抛出异常，原对象仍保持未改变状态。待所有改变都成功后，再将修改过的那个副本和原对象在一个不抛出异常的操作中置换（swap）。
总结：
第一，异常安全函数（Exception-safe functions）即使发生异常也不会泄漏资源或允许任何数据结构败坏。这样的函数区分为三种可能的保证：基本型、强烈型、不抛异常型。
第二，“强烈保证”往往能够以copy-and-swap实现出来，单“强烈保证”并非对所有函数都可实现或具备现实意义。
第三，函数提供的“异常安全保证”通常最高只等于其所调用哪个之各个函数的“异常安全保证”中的最弱者。
批注：
在现实程序的使用中，一般要求达到强烈型即可。因为基本型可能会导致一些不再使用的数据残留下来。而不抛异常型因为实现过于复杂的原因，一般也不达到这个级别。
30.Understand the ins and outs of inlining.
inline函数北周的整体观念是，将“对此函数的每一个调用”都以函数本体替换之。
inline函数通常一定被置于头文件内，大多数编译器会在编译期时候完成inlining，有些编译器会在链接期完成inlining，甚至有些编译器会在运行的时候完成inlining.
Templates通常也被置于头文件中，因为它一旦被使用，编译器为了将它具体化，需要知道它长什么样子。有些编译器会在链接期才执行template具体化，但是大多数情况下还是在编译器实现的具体化。
一个表面上看似inline的函数是否真是inline，取决于你的建置环境，主要取决于编译器。大多数的编译器提供了一个诊断级别：如果它们无法将你要求的函数inline化，会给你一个警告信息。
总结：
第一，将大多数inlining限制在小型、被频繁调用的函数身上。这可使日后的调试过程和二进制升级（binary upgradability）更容易，也可使潜在的代码膨胀问题最小化，使程序的速度提升机会最大化。
第二，不要只因为function templates出现在头文件，就将它们声明为inline。
批注：
templates和inline没有必然关系，需要加以区分。
31.Minimize compilation dependencies between files.
编译依存关系   compilication dependency
连串编译依存关系  cascading compilation dependencies
总结：
第一，支持“编译依存性最小化”的一般构思是：相依于声明式，不要相依于定义式。基于此构想的两个手段是Handle classes和Interface classes.
第二，程序库头文件应该以“完全且仅有声明式”（full and declaration-only forms）的形势存在。这种做法不论是否涉及templates都适用。
感悟：
这本书对于有一定的代码量的人会有很大的帮助，如果代码量不够的话，看起来会觉的很不好理解，也不容易记住这么多条款。当然，即使代码量够了，这本书也不是看一遍就可以扔了的，是属于可以放在案头不断的翻阅，直到这些条款的内容都融入骨髓。这部分的内容，我最大的感受就是很多东西在带来有利的一面的时候也带来了不利的一面，所以在使用的过程中要仔细的推敲，到底如何去用。没有哪种用法就一定万无一失，就一定会更好，要看具体使用的环境。

