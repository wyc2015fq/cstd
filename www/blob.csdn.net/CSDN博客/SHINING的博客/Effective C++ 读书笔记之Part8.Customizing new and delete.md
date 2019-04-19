# Effective C++ 读书笔记之Part8.Customizing new and delete - SHINING的博客 - CSDN博客
2013年05月30日 17:27:40[snsn1984](https://me.csdn.net/snsn1984)阅读数：1344
49. Understand the behavior of the new-handler.
总结：
第一，set_new_handler允许客户指定一个函数，在内存分配无法获得满足时被调用。
第二，Nothrow new是一个颇为局限的工具，因为它只适用于内存分配；后继的构造函数调用还是可能抛出异常。
50. Understand when it makes sense to replace new and delete.
替换编译器提供的operator new或operator delete的理由：
1）用来检测运用上的错误。
2）为了强化效能。
3）为了收集使用上的统计数据。
4）为了增加分配和归还的速度。
5）为了降低缺省内存管理器带来的空间额外开销。
6）为了弥补缺省分配器中的非最佳齐味（suboptimal alignment）。
7）为了将相关对象成簇集中。
8）为了获得非传统的行为。
总结：
有许多理由需要写个自定的new和delete，包括改善效能、对heap运行错误进行调试、收集heap使用信息。
51. Adhere to convention when writing new and delete.
总结：
第一，operator new应该内包含一个无穷循环，并在其中尝试分配内存，如果它无法满足内存需求，就该调用new-handler。它也应该有能力处理0 bytes申请。Class专属版本则还应该处理“比正确大小更大的（错误）申请”。
第二，operator delete应该在收到null指针时不做任何事。Class专属版本则还应该处理“比正确大小更大的（错误）申请”。
52.Write placement delete if you write placement new.
总结：
第一，当你写一个placement operator new，请确定也写出了对应的placement operator delete。如果没有这样做，你的程序可能会发生隐微而时断时续的内存泄露。
第二，当你声明placement new 和placement delete，请确定不要无意识（非故意）地掩盖了他们的正常版本。
批注：
本部分内容主要关注自己实现new和delete的时候需要注意的一些事项，这些功能在平时使用的不多。但是很有助于理解new和delete内部的结构。
