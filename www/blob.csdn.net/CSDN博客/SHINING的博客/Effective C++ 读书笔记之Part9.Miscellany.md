# Effective C++ 读书笔记之Part9.Miscellany - SHINING的博客 - CSDN博客
2013年05月30日 17:34:16[snsn1984](https://me.csdn.net/snsn1984)阅读数：1536
53. Pay attention to compiler warnings.
总结：
第一，严肃对待编译器发出的警告信息。努力在你的编译器的最高（最严苛）警告级别下争取“无任何警告”的荣誉。
第二，不要过度依赖编译器的报警能力，因为不同的编译器对待事情的态度并不相同。一旦移植到另一个编译器上，你原本依赖的警告信息有可能消失。
54. Familiarize yourself with the standard library, including RT1.
总结：
第一，C++标准程序库的主要机能由STL、iostreams、locales组成。并包含C99标准程序库。
第二，TR1添加了智能指针（例如tr1::shared_ptr）、一般化函数指针（tr1::function）、bash-based容器、正则表达式（regular expressions）以及另外10个组件的支持。
第三，TR1自身只是一份规范。为获得TR1提供的好处，你需要一份实物。一个好的实物来源是Boost。
55.Familiarize yourself with Boost.
总结：
第一，Boost是一个社群，也是一个网站。致力于免费、源码开放、同僚复审的C++程序库开发。Boost在C++标准化过程中扮演深具影响力的角色。
第二，Boost提供许多TR1组件实现品，以及其他许多程序库。
