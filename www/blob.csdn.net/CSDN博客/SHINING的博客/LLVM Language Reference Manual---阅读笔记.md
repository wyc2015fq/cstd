# LLVM Language Reference Manual---阅读笔记 - SHINING的博客 - CSDN博客
2012年10月15日 21:09:28[snsn1984](https://me.csdn.net/snsn1984)阅读数：3682标签：[LLVM																[llvm																[文档																[编译器																[读书笔记																[重点](https://so.csdn.net/so/search/s.do?q=重点&t=blog)](https://so.csdn.net/so/search/s.do?q=读书笔记&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=llvm&t=blog)](https://so.csdn.net/so/search/s.do?q=LLVM&t=blog)
个人分类：[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)
文档地址：http://llvm.org/docs/LangRef.html
LLVM IR的标示符有两种基本类型，全局的和局部的。全局标示符以@开头，局部标示符以%开头。LLVM IR的标示符有三种形式：命名的，未命名的，常量。
每一个Moudule都是由函数、全局变量和符号表组成的。
全局变量表现为指向为一块内存的指针。
全局变量和函数都会有一个linkage type, 就像：private, internal, linkonce等。
一个函数的声明只能使用external, dellimport, extern_weak这三种linkage type.
引用(aliases)只能使用external, internal, weak, weak_odr这四种linkage type.
Calling Converntions
所有的全局变量和函数都有一个visibility styles: default, hidden, protected.
Named Types:
Note that type names are aliases for the structural type that they indicate, and that you can therefore specify multiple names for the same type. This often leads
 to confusing behavior when dumping out a .ll file. Since LLVM IR uses structural typing, the name is not part of the type. When printing out LLVM IR, the printer will pick one name to
 render all types of a particular shape. This means that if you have code where two different source types end up having the same LLVM type, that the dumper will sometimes print the "wrong" or unexpected type. This is an important design point and isn't going
 to change.
随手记得一些东西，没有仔细的总结和分析，后续有时间的话进行总结。
