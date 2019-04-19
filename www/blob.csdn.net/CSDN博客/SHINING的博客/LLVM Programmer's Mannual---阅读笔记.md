# LLVM Programmer's Mannual---阅读笔记 - SHINING的博客 - CSDN博客
2012年10月17日 23:15:02[snsn1984](https://me.csdn.net/snsn1984)阅读数：3609
                
 文档地址：
[](http://llvm.org/docs/ProgrammersManual.html)
[](http://llvm.org/docs/ProgrammersManual.html)[](http://llvm.org/docs/ProgrammersManual.html)[](http://llvm.org/docs/ProgrammersManual.html)[http://llvm.org/docs/ProgrammersManual.html](http://llvm.org/docs/ProgrammersManual.html)
该文档的主要目的：
该文档主要介绍了LLVM源码的一些重要的类和接口，并不打算解释LLVM是什么，LLVM怎么工作，LLVM代码是什么样的。这个文档主要是面对那些已经了解了LLVM基础，并且对转化、分析和控制LLVM代码有兴趣的人。
重要的和使用的API
The isa<>, cast<> and dyn_cast<> templates
The LLVM source-base makes extensive use of a custom form of RTTI.
RTTI (Run-Time Type Identification)
isa<>:
The isa<> operator works exactly like the Java "instanceof" operator. It returns true or false depending on whether a reference or pointer points to an instance of the specified class.
cast<>:
The cast<> operator is a "checked cast" operation. It converts a pointer or reference from a base class to a derived class, causing an assertion failure if it is not really an instance of the right type.
dyn_cast<>:
The dyn_cast<> operator is a "checking cast" operation. It checks to see if the operand is of the specified type, and if so, returns a pointer to it (this operator does not work with references). If the operand is not of the correct type, a null
 pointer is returned.
Passing strings(the StringRef and Twine classes)
Instead, many LLVM APIs use a StringRef or a const Twine& for
 passing strings efficiently.
The StringRef class
The StringRef data type represents a reference to a constant string (a character array and a length) and supports
 the common operations available on std:string, but does not require heap allocation.
The Twine class
The [Twine](http://llvm.org/doxygen/classllvm_1_1Twine.html) class is an efficient way for APIs
 to accept concatenated strings.
The DEBUG() macro and -debug option
The Statistic class & -stats option
The "llvm/ADT/Statistic.h" file provides a class named Statistic that is used as a unified way to keep track of what the LLVM compiler is doing and how effective various optimizations  are. It is useful to see what optimizations are contributing to making a
 particular program run faster.
Turning an iterator into a class pointer (and vice-versa)
Instead of dereferencing the iterator and then taking the address of the result, you can simply assign the iterator to the proper pointer type and you get the dereference and address-of operation as a result of the assignment (behind the scenes, this is a result
 of overloading casting mechanisms). It's also possible to turn a class pointer into the corresponding iterator, and this is a constant time operation (very efficient).
Iterating over def-use & use-def chains
Function *F = ...;
for (Value::use_iterator i = F->use_begin(), e = F->use_end(); i != e; ++i)
  if (Instruction *Inst = dyn_cast<Instruction>(*i)) {
    errs() << "F is used in instruction:\n";
    errs() << *Inst << "\n";
  }
Instruction *pi = ...;
for (User::op_iterator i = pi->op_begin(), e = pi->op_end(); i != e; ++i) {
  Value *v = *i;
  // ...
}
总结一下：
其实这个文档的目的比较简单，就是介绍一些常用的API和类。虽然在用的时候会发现，这些介绍的东西在LLVM所有的类和API中就是九牛一毛，但是有总胜于无。在实际的编码过程中，只能依赖的就是源码。所以，读源码是必不可少的。
