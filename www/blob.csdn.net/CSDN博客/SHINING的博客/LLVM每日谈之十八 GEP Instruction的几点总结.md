# LLVM每日谈之十八 GEP Instruction的几点总结 - SHINING的博客 - CSDN博客
2013年12月31日 17:39:12[snsn1984](https://me.csdn.net/snsn1984)阅读数：4442
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
In summary, here’s some things to always remember about the GetElementPtr instruction:
- The GEP instruction never accesses memory, it only provides pointer computations.
- The first operand to the GEP instruction is always a pointer and it must be indexed.
- There are no superfluous indices for the GEP instruction.
- Trailing zero indices are superfluous for pointer aliasing, but not for the types of the pointers.
- Leading zero indices are not superfluous for pointer aliasing nor the types of the pointers.
这几条是《The Often Misunderstood GEP Instruction》文档最后对GEP指令的总结，原文档地址为：
[http://llvm.org/docs/GetElementPtr.html](http://llvm.org/docs/GetElementPtr.html)
这几天基本上涵盖了GEP使用中需要注意的问题，现翻译如下：
1. GEP指令绝不允许操作内存，它只能提供指针相关的计算。
2. GEP指令的第一个操作数通常是一个指针，而且它通常是可以索引的。
3. GEP指令没有多余的无用的下标索引。
4. 下标索引尾部的0对指针的混用是没区别的，但是对指针类型的混用确是有区别的。
（译者注：此处指的是下标索引尾部有0和没0的混用。）
5. 下标索引头部的0对指针的混用和指针类型的混用都是有区别的。
（译者注：此处指的是下标索引头部有0和没0的混用。）
