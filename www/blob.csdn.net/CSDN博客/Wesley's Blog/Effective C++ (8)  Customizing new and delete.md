
# Effective C++ (8): Customizing new and delete - Wesley's Blog - CSDN博客


2018年09月05日 11:23:47[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：48


## Introduction
本章主要讨论了自定义 new 和 delete 的目的和需要注意的问题
## Rule 49: Understand the behavior of the new-handler
在 new 抛出 bad_alloc 异常之前, 会先调用一个客户指定的错误处理函数, 就是 new_handler:
`namespace std{
    typedef void (*new_handler) ();
    new_handler set_new_handler(new_handler p) throw();
}`一个好的 new_handler 函数应该做以下事情:
让更多内存可被使用
安装另一个 new-handler
卸除 new-handler
抛出 bad_alloc
不返回, 常用 abort 或 exit
Remeber:
**set_new_handler 允许客户指定一个函数, 在内存分配无法获得满足时被调用**
## Rule 50: Understand when it makes sense to replace new and delete
自定义 new , delete 通常出于以下原因:
为了检测运用错误
为了收集动态分配内存的使用统计信息
为了增加分配和归还速度
为了降低缺省内存管理器带来的空间额外开销
为了弥补缺省分配器中的非最佳齐位
为了将相关对象成簇集中
为了获得非传统行为
Remeber:
**有许多理由写个自定义的 new 和 delete, 包括改善效能, 对heap运用错误进行调试, 收集 heap 使用信息**
## Rule 51: Adhere to convention when writing new and delete
Remeber:
operator new 应该内含一个无穷循环, 在其中尝试分配内存, 无法满足就调用 new-handler, 它也应该有能力处理 0 bytes 的申请
operator delete应该在收到 null 指针时不做任何事. Class 专属版本则还应该处理 “比正确大小更大(错误)申请”
## Rule 52: Write placement delete if you write placement new
Remeber:
当你写一个 placement operator new, 请确定也写出了对应的 placement operator delete.
当你声明 placement new 和 placement delete, 请确定不要无意识(非故意)地遮蔽了它们的正常版本.
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

