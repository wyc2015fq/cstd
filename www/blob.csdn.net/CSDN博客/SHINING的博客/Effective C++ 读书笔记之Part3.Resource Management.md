# Effective C++ 读书笔记之Part3.Resource Management - SHINING的博客 - CSDN博客
2013年05月17日 15:06:13[snsn1984](https://me.csdn.net/snsn1984)阅读数：1401
13. Use objects to manage resources.
总结：
第一，为了防止资源泄露，请使用RAII对象，它们在构造函数中获得资源并在西沟函数中释放资源。
第二，两个常被使用的RAII class分别是tr1::shared_ptr和auto_ptr。前者通常是较佳选择，因为其copy行为比较直观。若选择auto_ptr，复制动作会使它（被复制物）指向null。
14.Think carefully about copying behavior in resource-managing classes.
总结：
第一，复制RAII对象必须一并复制它所管理的资源，所以资源的copying行为决定RAII对象的copying行为。
第二，普遍而常见的RAII class copying行为是：抑制copying、施行引用计数法（reference counting）。不过其他行为也都可能被实现。
15.Provide access to raw resources in resource-managing classes.
总结：
第一，APIs往往要求访问原始资源（raw resources），所以每一个RAII class应该提供一个“取得其所管理之资源”的办法。
第二，对原始资源的访问可能经由现实转换或者隐式转换。一般而言显式转换比较安全，但隐式转换对客户比较方便。
16.Use the same form in corresponding uses of new and delete.
总结：
如果你在new表达式中使用[]，必须在相应的delete表达式中也使用[]。如果你在new表达式中不使用[]，一定不要在相应的delete表达式中使用[]。
17.Store newed objects in smart pointers in standalone statements.
总结：
以独立语句将newed对象存储于（置入）职能指针内。如果不则样做，一旦异常被抛出，有可能导致难以觉察的资源泄露。
感悟：这部分的内容，除了new 和delete之前用的比较多之外，就是引用计数用的比较多。但是不是按照这本书的方式，这本书介绍的方式，理解的不是那么透彻，因为之前没这么用过，也没有见过这么用的。这部分内容回头需要重读一下。
