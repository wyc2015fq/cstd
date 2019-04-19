# Effective C++ 读书笔记之Part6.Inheritance and Object-Oriented Design - SHINING的博客 - CSDN博客
2013年05月27日 15:41:43[snsn1984](https://me.csdn.net/snsn1984)阅读数：1779标签：[C++																[读书笔记																[类																[入门																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=入门&t=blog)](https://so.csdn.net/so/search/s.do?q=类&t=blog)](https://so.csdn.net/so/search/s.do?q=读书笔记&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/snsn1984/article/category/488621)
32.Make sure public inheritance models "is-a".
所谓的最佳设计，取决于系统希望做什么事，包括现在与未来。
需要解决的问题：其中关于两个assert都通过的地方有些疑惑。
总结：
“public继承”意味着is-a。适用于base classes身上的每一件事情一定也适用于derived classes身上，因为每一个derived class对象也都是一个base class对象。
33.Avoid hiding inherited names.
总结：
第一，derived classes内的名称会遮掩base class内的名称。在public继承下从来没有人希望如此。
第二，为了让被遮掩的名称再见天日，可使用using声明式或转交函数（forwarding functions）。
批注：
转交函数就是在子类中的成员函数中跳转到父类的成员函数。仅仅是加了一层包装。
34. Differentiate between inheritance of interface and inheritance of implementation.
1）声明一个pure virtual函数的目的是为了让derived classes只继承函数接口。
2）声明简朴的（非纯）impure virtual函数的目的，是让derived classes继承该函数的接口和缺省实现。
3）声明non-virtual函数的目的是为了令derived classes继承函数的接口及一份强制性实现。
总结：
第一，接口继承和实现继承不同。在public继承之下，derived classes总是继承base class的接口。
第二，pure virtual 函数只具体制定接口继承。
第三，简朴的（非纯）impure virtual函数具体指定接口继承及缺省实现继承。
第四，non-virtual函数具体制定接口继承以及强制性实现继承。
35.Consider alternatives to virtual functions.
令客户通过public non-virtual成员函数间接调用private virtual函数，称为non-virtual interface(NVI)手法。它是所谓Template Method设计模式（与C++ templates并无关联）的一个独特表现形式。
1）使用non-virtual interface(NVI)手法，那是Template Method设计模式的一种特殊形式。它以public non-virtual 成员函数包裹较低访问性（private或protected）的virtual函数。
2）将virtual函数替换为“函数指针成员变量”，这是Strategy设计模式的一种分解表现形式。
3）以tr1::function成员变量替换virtual函数，因而允许使用任何可调用物（callable entity）搭配一个兼容于需求的签名式。这也是Strategy设计模式的某种形式。
4）将继承体系内的virtual函数替换为另一个继承体系内的virtual函数。这是Strategy设计模式的传统实现手法。
总结：
第一，virtual函数的替代方案包括NVI手法及Strategy设计模式的多种形式。NVI手法自身是一个特殊形式的Template Method设计模式。
第二，将机能从成员函数移到class外部函数，带来的一个缺点是，非成员函数无法访问class的non-public成员。
第三，tr1::function对象的行为就像一般函数指针。这样的对象可接纳“与给定之目标签名式（target signature）兼容”的所有可调用物（callable entities）。
批注：该条款主要介绍了针对virtual函数实现功能的集中实现方式，主要是分为两个方面，一个方面是NVI，一个方面是设计模式中的Strategy的几种实现方式。
36. Never redefine an inherited non-virtual function.
总结：
绝对不要重新定义继承而来的non-virtual函数。
批注：
主要是因为如果你这么做了，首先，你破坏了non-virtual函数应该被完全继承的初衷；其次，这样做会导致混乱。所以这是一个不合理的情况，要不声明为virtual，要不就不要重新定义。否则这样带来的混乱是对象执行成员函数的时候，取决于它们的声明类型，而不是本身自己所指向的类型。
37. Never redefine a functions's inherited default parameter value.
对象的所谓静态类型（static type），就是它在程序中被声明时所采用的类型。
对象的所谓动态类型（dynamic type），就是指“目前所指对象的类型”。
总结：
绝对不要重新定义一个继承而来的缺省参数数值，因为缺省参数数值都是静态绑定，而virtual函数---你唯一应该覆写的东西---却是动态绑定。
批注：
由此可见很大一部分问题都是由于动态绑定和静态绑定的原因造成的。由此可类推到virtual函数以及non-virtual函数。
38. Model "has-a" or "is-implemented-in-terms-of" through composition.
总结：
第一，复合（composition）的意义和public继承完全不同。
第二，在应用域（application domain），复合意味着has-a（有一个）。在实现域（implementation domain），复合意味is-implemented-in-terms-of（根据某物实现出）。
39. Use private inheritance judiciously.
复合和private继承都意味is-implemented-in-terms-of，但复合比较容易理解，所以无论什么时候，只要可以，你还是应该选择复合。
总结：
第一，private继承意味is-implemented-in-term-of（根据某物实现出）。它通常比复合（composition）的级别低。但是当derived class需要访问protected base class的成员，或需要重新定义继承而来的virtual函数时，这么设计是合理的。
第二，和复合（composition）不同，private继承可以造成empty base最优化。这对致力于“对象尺寸最小化”的程序库开发者而言，可能很重要。
批注：
主要关注了private继承下的情况。
40. Use multiple inheritance judiciously.
总结：
第一，多重继承比单一继承复杂。它可能导致新的歧义性，以及对virtual继承的需要。
第二，virtual继承会增加大小、速度、初始化（及赋值）复杂度等等成本。如果virtual base class不带任何数据，将是最具实用价值的情况。
第三，多重继承的确有正当用途。其中一个情节涉及“public继承某个Interface class”和“private继承某个协助实现的class”的两组结合。
