# Mapping High-Level Constructs to LLVM IR - SHINING的博客 - CSDN博客
2013年12月31日 09:50:42[snsn1984](https://me.csdn.net/snsn1984)阅读数：2890标签：[LLVM																[文档																[compiler																[IR																[类型](https://so.csdn.net/so/search/s.do?q=类型&t=blog)](https://so.csdn.net/so/search/s.do?q=IR&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=LLVM&t=blog)
个人分类：[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)
原文地址：[http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR)
Mapping High-Level Constructs to LLVM IR
## Table of Contents
[Introduction](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#1)
[A Quick Primer](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#2)
[Some Useful LLVM Tools](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#3)
[Mapping Basic Constructs to LLVM IR](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#4)
[Global Variables](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#5)
[Local Variables](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#6)
[Constants](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#7)
[Constant Expressions](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#8)
[Size-Of Computations](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#9)
[Function Prototypes](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#10)
[Function Definitions](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#11)
[Simple Public Functions](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#12)
[Simple Private Functions](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#13)
[Functions with a Variable Number of Parameters](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#14)
[Exception-Aware Functions](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#15)
[Function Pointers](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#16)
[Casts](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#17)
[Bitwise Casts](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#18)
[Zero-Extending Casts (Unsigned Upcasts)](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#19)
[Sign-Extending Casts (Signed Upcasts)](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#20)
[Truncating Casts (Signed and Unsigned Downcasts)](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#21)
[Floating-Point Extending Casts (Float Upcasts)](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#22)
[Floating-Point Truncating Casts (Float Downcasts)](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#23)
[Pointer-to-Integer Casts](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#24)
[Integer-to-Pointer Casts](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#25)
[Address-Space Casts (Pointer Casts)](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#26)
[Incomplete Structure Types](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#27)
[Structures](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#28)
[Nested Structures](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#29)
[Unions](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#30)
[Structure Expressions](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#31)
[Getting a Pointer to a Structure Member](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#32)
[Mapping Control Structures to LLVM IR](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#33)
[Mapping Advanced Constructs to LLVM IR](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#34)
[Lambda Functions](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#35)
[Closures](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#36)
[Generators](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#37)
[Mapping Exception Handling to LLVM IR](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#38)
[Exception Handling by Propagated Return Value](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#39)
[Setjmp/Longjmp Exception Handling](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#40)
[Zero Cost Exception Handling](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#41)
[Resources](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#42)
[Mapping Object-Oriented Constructs to LLVM IR](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#43)
[Classes](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#44)
[Virtual Methods](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#45)
[Single Inheritance](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#46)
[Multiple Inheritance](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#47)
[Virtual Inheritance](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#48)
[Interfaces](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#49)
[Boxing and Unboxing](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#50)
[Class Equivalence Test](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#51)
[Class Inheritance Test](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#52)
[The New Operator](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#53)
[The Instance New Operator](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#54)
[The Array New Operator](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#55)
[Interoperating with a Runtime Library](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#56)
[Interfacing to the Operating System](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#57)
[How to Interface to POSIX Operating Systems](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#58)
[Sample POSIX "Hello World" Application](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#59)
[How to Interface to the Windows Operating System](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#60)
[Sample Windows "Hello World" Application](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#61)
[Resources](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#62)
[Epilogue](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#63)
[Appendix A: How to Implement a String Type in LLVM](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#64)
[Appendix B: Task List](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR#65)
## Introduction
In this document we will take a look at how to map various classic high-level programming language constructs to LLVM IR. The purpose of the document is to make the learning curve less steep for aspiring LLVM users.
For the sake of simplicity, we'll be working with a 32-bit target machine so that pointers and word-sized operands are 32-bits.
Also, for the sake of readability we do not mangle (encode) names. Rather, they are given simple, easy-to-read names that reflect their purpose. A production compiler for any language that supports overloading would generally need to
 mangle the names so as to avoid conflicts between symbols.
## A Quick Primer
Here are a few things that you should know before reading this document:
- LLVM IR is not machine code, but sort of the step just above assembly.
- LLVM IR is highly typed so expect to be told when you do something wrong.
- LLVM IR does not differentiate between signed and unsigned integers.
- LLVM IR assumes two's complement signed integers so that saytruncworks equally well on signed and unsigned integers.
- Global symbols begin with an at sign (@).
- Local symbols begin with a percent symbol (%).
- All symbols must be declared or defined.
- Don't worry that the LLVM IR at times can seem somewhat lengthy when it comes to expressing something; the optimizer will ensure the output is well optimized and you'll often see two or three LLVM IR instructions be coalesced
 into a single machine code instruction.
- If in doubt, consult the Language Reference. If there is a conflict between the Language Reference and this document, this document is wrong!
- All LLVM IR examples are presented without a data layout and without a target triple. You need to add those yourself, if you want to actually build and run the samples. Get them from Clang for your platform.
## Some Useful LLVM Tools
The most important LLVM tools for use with this article are as follows:
|Name|Function|Reads|Writes|Arguments|
|----|----|----|----|----|
|clang|C Compiler|.c|.ll|-c -emit-llvm -S|
|clang++|C++ Compiler|.cpp|.ll|-c -emit-llvm -S|
|llvm-dis|Disassembler|.bc|.ll||
|opt|Optimizer|.bc/.ll|same||
|llc|IR Compiler|.ll|.s||
While you are playing around with generating or writing LLVM IR, you may want to add the option-fsanitize=undefinedto
 Clang/Clang++ insofar you use either of those. This option makes Clang/Clang++ insert run-time checks in places where it would normally output anud2instruction.
 This will likely save you some trouble if you happen to generate undefined LLVM IR. Please notice that this option only works for C and C++ compiles.
## Mapping Basic Constructs to LLVM IR
In this chapter, we'll look at the most basic and simple constructs that are part of nearly all imperative/OOP languages out there.
### Global Variables
Global varibles are trivial to implement in LLVM IR:
intvariable=14;
intmain()
{
returnvariable;
}
Becomes:
[@variable](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i32](http://llvm.org/docs/LangRef.html#t_integer)14
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*[@variable](http://llvm.org/docs/LangRef.html#globalvars)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%1
}
Please notice that LLVM views global variables as pointers; so you must explicitly dereference the global variable using theloadinstruction
 when accessing its value, likewise you must explicitly store the value of a global variable using thestoreinstruction.
### Local Variables
There are two kinds of local variables in LLVM:
- Register-allocated local variables (temporaries).
- Stack-allocated local variables.
The former is created by introducing a new symbol for the variable:
%1=some computation
The latter is created by allocating the variable on the stack:
%2=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)[i32](http://llvm.org/docs/LangRef.html#t_integer)
Please notice thatallocayields a pointer to the allocated type. As is generally the case in LLVM, you must explicitly
 use aloadorstoreinstruction to read
 or write the value respectively.
The use ofallocaallows for a neat trick that can simplify your code generator in some cases. The trick is to explicitly
 allocate all mutable variables, including arguments, on the stack, initialize them with the appropriate initial value and then operate on the stack as if that was your end goal. The trick is to run the "memory to register promotion" pass on your code as part
 of the optimization phase. This will make LLVM store as many of the stack variables in registers as it possibly can. That way you don't have to ensure that the generated program is in SSA form but can generate code without having to worry about this aspect
 of the code generation.
This trick is also described in chapter 7.4,[Mutable
 Variables in Kaleidoscope](http://llvm.org/docs/tutorial/OCamlLangImpl7.html#mutable-variables-in-kaleidoscope), in the OCaml tutorial on the[LLVM website](http://www.llvm.org/).
### Constants
There are two different kinds of constants:
- Constants that do*not*occupy allocated memory.
- Constants that*do*occupy allocated memory.
The former are always expanded inline by the compiler as there is no LLVM IR equivalent of those. In other words, the compiler simply inserts the constant value wherever it is being used in a computation:
%1=[add](http://llvm.org/docs/LangRef.html#i_add)[i32](http://llvm.org/docs/LangRef.html#t_integer)%0,17; 17 is an inlined constant
Constants that do occupy memory are defined using theconstantkeyword:
[@hello](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[6x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"hello\00"
%struct=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer),[i8](http://llvm.org/docs/LangRef.html#t_integer)}
[@struct_constant](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)%struct{[i32](http://llvm.org/docs/LangRef.html#t_integer)16,[i8](http://llvm.org/docs/LangRef.html#t_integer)4}
Such a constant is really a global variable whose visibility can be limited withprivateorinternalso
 that it is invisible outside the current module.
### Constant Expressions
**TODO:**Document the various forms of constant expressions that exist and .. how they can be very useful. For instance,getelementptrconstant
 expressions are almost unavoidable in all but the simplest programs.
#### Size-Of Computations
Even though the compiler ought to know the exact size of everything in use (for statically checked languages), it can at times be convenient to ask LLVM to figure out the size of a structure for you. This is done with the following
 little snippet of code:
%Struct=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i8](http://llvm.org/docs/LangRef.html#t_integer),[i32](http://llvm.org/docs/LangRef.html#t_integer),[i8](http://llvm.org/docs/LangRef.html#t_integer)*}
[@Struct_size](http://llvm.org/docs/LangRef.html#globalvars)=[constant](http://llvm.org/docs/LangRef.html#globalvars)[i32](http://llvm.org/docs/LangRef.html#t_integer)[ptrtoint](http://llvm.org/docs/LangRef.html#i_ptrtoint)(%Struct*[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)(%Struct*[null](http://llvm.org/docs/LangRef.html#simpleconstants),[i32](http://llvm.org/docs/LangRef.html#t_integer)1))to[i32](http://llvm.org/docs/LangRef.html#t_integer)
@Struct_sizewill now contain the size of the structure%Struct.
 The trick is to compute the offset of the second element in the zero-based array starting atnulland that way get the size of the structure.
### Function Prototypes
A function prototype, aka a profile, is translated into an equivalentdeclaredeclaration in LLVM IR:
intBar(intvalue);
Becomes:
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Bar](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%value)
Or you can leave out the descriptive parameter name:
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Bar](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer))
### Function Definitions
The translation of function definitions depends on a range of factors, ranging from the calling convention in use, whether the function is exception-aware or not, and if the function is to be publicly available outside the module.
#### Simple Public Functions
The most basic model is:
intBar(void)
{
return17;
}
Becomes:
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Bar](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)17
}
#### Simple Private Functions
A static function is a function private to a module that cannot be referenced from outside of the defining module:
[define](http://llvm.org/docs/LangRef.html#functionstructure)[private](http://llvm.org/docs/LangRef.html#linkage_private)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Foo](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)17
}
#### Functions with a Variable Number of Parameters
To call a so-called vararg function, you first need to define or declare it using the elipsis (...) and then you need to make use of a special syntax for function calls that allows you to explictly list the types of the parameters of
 the function that is being called. This "hack" exists to allow overriding a call to a function such as a function with variable parameters. Please notice that you only need to specify the return type once, not twice as you'd have to do if it was a true cast:
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,...)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[@.text](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[20x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Argument
 count: %d\0A\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%argc,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%argv)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; printf("Argument count: %d\n", argc)
%1=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,...)*[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)([20x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.text](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0),[i32](http://llvm.org/docs/LangRef.html#t_integer)%argc)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)0
}
#### Exception-Aware Functions
A function that is aware of being part of a larger scheme of exception-handling is called an exception-aware function. Depending upon the type of exception handling being employed, the function may either return a pointer to an exception
 instance, create asetjmp/longjmpframe, or simply
 specify theuwtable(for UnWind Table) attribute. These cases will all be covered in great detail in the chapter on*Exception Handling*below.
### Function Pointers
Function pointers are expressed almost like in C and C++:
int(*Function)(char*buffer);
Becomes:
[@Function](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i32](http://llvm.org/docs/LangRef.html#t_integer)([i8](http://llvm.org/docs/LangRef.html#t_integer)*)*[null](http://llvm.org/docs/LangRef.html#simpleconstants)
### Casts
There are nine different types of casts:
- Bitwise casts (type casts).
- Zero-extending casts (unsigned upcasts).
- Sign-extending casts (signed upcasts).
- Truncating casts (signed and unsigned downcasts).
- Floating-point extending casts (float upcasts).
- Floating-point truncating casts (float downcasts).
- Pointer-to-integer casts (***todo: Document pointer-to-integer casts***).
- Integer-to-pointer casts (***todo: Document integer-to-pointer casts***).
- Address-space casts (pointer casts).
#### Bitwise Casts
A bitwise cast (bitcast) reinterprets a given bit pattern without changing any bits in the operand. For instance, you could make a bitcast of
 a pointer to byte into a pointer to some structure as follows:
typedefstruct
{
inta;
}Foo;
externvoid*malloc(size_tsize);
externvoidfree(void*value);
voidallocate()
{
    Foo*foo=(Foo*)malloc(sizeof(Foo));
    foo.a=12;
free(foo);
}
Becomes:
%Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)}
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer))
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@free](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*)
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@allocate](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)4)
%foo=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%1to%Foo*
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)12,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@free](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%1)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
#### Zero-Extending Casts (Unsigned Upcasts)
To upcast an unsigned value like in the example below:
uint8byte=117;
uint32word;
voidmain()
{
/* The compiler automatically upcasts the byte to a word. */
    word=byte;
}
You use thezextinstruction:
[@byte](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i8](http://llvm.org/docs/LangRef.html#t_integer)117
[@word](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@byte](http://llvm.org/docs/LangRef.html#globalvars)
%2=[zext](http://llvm.org/docs/LangRef.html#i_zext)[i8](http://llvm.org/docs/LangRef.html#t_integer)%1to[i32](http://llvm.org/docs/LangRef.html#t_integer)
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%2,[i32](http://llvm.org/docs/LangRef.html#t_integer)*[@word](http://llvm.org/docs/LangRef.html#globalvars)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
#### Sign-Extending Casts (Signed Upcasts)
To upcast a signed value, you replace thezextinstruction with thesextinstruction
 and everything else works just like in the previous section:
[@char](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i8](http://llvm.org/docs/LangRef.html#t_integer)-17
[@int](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@char](http://llvm.org/docs/LangRef.html#globalvars)
%2=[sext](http://llvm.org/docs/LangRef.html#i_sext)[i8](http://llvm.org/docs/LangRef.html#t_integer)%1to[i32](http://llvm.org/docs/LangRef.html#t_integer)
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%2,[i32](http://llvm.org/docs/LangRef.html#t_integer)*[@int](http://llvm.org/docs/LangRef.html#globalvars)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
#### Truncating Casts (Signed and Unsigned Downcasts)
Both signed and unsigned integers use the same instruction,trunc, to reduce the size of the number in question. This is because
 LLVM IR assumes that all signed integer values are in two's complement format for which reasontruncis sufficient to handle both cases:
[@int](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i32](http://llvm.org/docs/LangRef.html#t_integer)-1
[@char](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i8](http://llvm.org/docs/LangRef.html#t_integer)0
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*[@int](http://llvm.org/docs/LangRef.html#globalvars)
%2=[trunc](http://llvm.org/docs/LangRef.html#i_trunc)[i32](http://llvm.org/docs/LangRef.html#t_integer)%1to[i8](http://llvm.org/docs/LangRef.html#t_integer)
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)%2,[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@char](http://llvm.org/docs/LangRef.html#globalvars)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
#### Floating-Point Extending Casts (Float Upcasts)
Floating points numbers can be extended using thefpextinstruction:
floatsmall=1.25;
doublelarge;
voidmain()
{
/* The compiler inserts an implicit float upcast. */
    large=small;
}
Becomes:
[@small](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[float](http://llvm.org/docs/LangRef.html#t_floating)1.25
[@large](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[double](http://llvm.org/docs/LangRef.html#t_floating)0.0
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[load](http://llvm.org/docs/LangRef.html#i_load)[float](http://llvm.org/docs/LangRef.html#t_floating)*[@small](http://llvm.org/docs/LangRef.html#globalvars)
%2=[fpext](http://llvm.org/docs/LangRef.html#i_fpext)[float](http://llvm.org/docs/LangRef.html#t_floating)%1to[double](http://llvm.org/docs/LangRef.html#t_floating)
[store](http://llvm.org/docs/LangRef.html#i_store)[double](http://llvm.org/docs/LangRef.html#t_floating)%2,[double](http://llvm.org/docs/LangRef.html#t_floating)*[@large](http://llvm.org/docs/LangRef.html#globalvars)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
#### Floating-Point Truncating Casts (Float Downcasts)
Likewise, a floating point number can be truncated to a smaller size:
[@large](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[double](http://llvm.org/docs/LangRef.html#t_floating)1.25
[@small](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[float](http://llvm.org/docs/LangRef.html#t_floating)0.0
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[load](http://llvm.org/docs/LangRef.html#i_load)[double](http://llvm.org/docs/LangRef.html#t_floating)*[@large](http://llvm.org/docs/LangRef.html#globalvars)
%2=[fptrunc](http://llvm.org/docs/LangRef.html#i_fptrunc)[double](http://llvm.org/docs/LangRef.html#t_floating)%1to[float](http://llvm.org/docs/LangRef.html#t_floating)
[store](http://llvm.org/docs/LangRef.html#i_store)[float](http://llvm.org/docs/LangRef.html#t_floating)%2,[float](http://llvm.org/docs/LangRef.html#t_floating)*[@small](http://llvm.org/docs/LangRef.html#globalvars)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
#### Pointer-to-Integer Casts
***todo: Document pointer-to-integer casts.***
#### Integer-to-Pointer Casts
***todo: Document integer-to-pointer casts.***
#### Address-Space Casts (Pointer Casts)
***todo: Find a useful example of an address-space casts, using theaddrspacecastinstruction, to be included here.***
### Incomplete Structure Types
Incomplete types are very useful for hiding the details of what fields a given structure has. A well-designed C interface can be made so that no details of the structure are revealed to the client, so that the client cannot inspect
 or modify private members inside the structure:
voidBar(structFoo*);
Becomes:
%Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes)[opaque](http://llvm.org/docs/LangRef.html#t_opaque)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Bar](http://llvm.org/docs/LangRef.html#globalvars)(%Foo)
### Structures
LLVM IR already includes the concept of structures so there isn't much to do:
structFoo
{
size_t_length;
};
It is only a matter of discarding the actual field names and then index with numerals starting from zero:
%Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)}
### Nested Structures
Nested structures are straightforward:
%Object=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Object*,; 0: above; the parent pointer
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 1: value; the value of the node
}
### Unions
Unions are getting more and more rare as the years have shown that they are quite dangerous to use; especially the C variant that does not have a selector field to indicate which of the union's variants are valid. Some may still have
 a legacy reason to use unions. In fact, LLVM does not support unions at all:
unionFoo
{
inta;
char*b;
doublec;
};
Foo Union;
Becomes this when run through Clang++:
%union.Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes){[double](http://llvm.org/docs/LangRef.html#t_floating)}
[@Union](http://llvm.org/docs/LangRef.html#globalvars)=%union.Foo{0.0}
What happened here? Where did the other union members go? The answer is that in LLVM there are no unions; there are only structs that can be cast into whichever type the front-end want to cast the struct into. So to access the above
 union from LLVM IR, you'd use thebitcastinstruction to cast a pointer to the "union" into whatever pointer you'd want it to be:
%1=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)%union.Foo*[@Union](http://llvm.org/docs/LangRef.html#globalvars)to[i32](http://llvm.org/docs/LangRef.html#t_integer)*
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)1,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
%2=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)%union.Foo*[@Union](http://llvm.org/docs/LangRef.html#globalvars)to[i8](http://llvm.org/docs/LangRef.html#t_integer)**
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[null](http://llvm.org/docs/LangRef.html#simpleconstants),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%2
This may seem strange, but the truth is that a union is nothing more than a piece of memory that is being accessed using different implicit pointer casts.
If you want to support unions in your front-end language, you should simply allocate the total size of the union (i.e. the size of the largest member) and then generate code to reinterpret the allocated memory as needed.
The cleanest approach might be to simply allocate a range of bytes (i8), possibly with alignment padding at the end, and then cast whenever you
 access the structure. That way you'd be sure you did everything properly all the time.
### Structure Expressions
As already told, structure members are referenced by index rather than by name in LLVM IR. And at no point do you need to, or should you, compute the offset of a given structure member yourself. Thegetelementptrinstruction
 is available to compute a pointer to any structure member with no overhead (thegetelementptrinstruction is typically coascaled into the actualloadorstoreinstruction).
#### Getting a Pointer to a Structure Member
The C++ code below illustrates various things you might want to do:
structFoo
{
inta;
char*b;
doublec;
};
intmain(void)
{
    Foo foo;
char**bptr=&foo.b;
    Foo bar[100];
    bar[17].c=0.0;
return0;
}
Becomes:
%Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 0: a
[i8](http://llvm.org/docs/LangRef.html#t_integer)*,;
 1: b
[double](http://llvm.org/docs/LangRef.html#t_floating);
 2: c
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; Foo foo
%foo=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%Foo
; char **bptr = &foo.b
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
; Foo bar[100]
%bar=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%Foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)100
; bar[17].c = 0.0
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%bar,[i32](http://llvm.org/docs/LangRef.html#t_integer)17,[i32](http://llvm.org/docs/LangRef.html#t_integer)2
[store](http://llvm.org/docs/LangRef.html#i_store)[double](http://llvm.org/docs/LangRef.html#t_floating)0.0,[double](http://llvm.org/docs/LangRef.html#t_floating)*%2
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)0
}
***todo: Document theextractvalueandinsertvalueinstructions.***
## Mapping Control Structures to LLVM IR
***todo: Add common control structures such asif,for,switch, andwhile.***
***todo: Explain the purpose of thephiinstruction; show how it becomes obvious that you need it as soon as you encounter multiple blocks that contribute a value through different temporaries. In a way,phiought to have been called "join" as it sort of joins up subexpressions from different basic blocks.***
## Mapping Advanced Constructs to LLVM IR
In this chapter, we'll look at various non-OOP constructs that are highly useful and are becoming more and more widespread in use.
### Lambda Functions
A lambda function is an anonymous function with the added spice that it may freely refer to the local variables (including argument variables) in the containing function. Lambdas are implemented just like Pascal's nested functions,
 except the compiler is responsible for generating an internal name for the lambda function. There are a few different ways of implementing lambda functions (see `Wikipedia on nested functions[Wikipedia
 on Nested Functions](http://en.wikipedia.org/wiki/Nested_function)for more information).
intfoo(inta)
{
autofunction=[](intx){returnx+a;}
returnfunction(10);
}
Here the "problem" is that the lambda function references a local variable of the caller, namelya, even though the lambda function
 is a function of its own. This can be solved easily by passing the local variable in as an implicit argument to the lambda function:
[define](http://llvm.org/docs/LangRef.html#functionstructure)[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@lambda](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%a,[i32](http://llvm.org/docs/LangRef.html#t_integer)%x)[alwaysinline](http://llvm.org/docs/LangRef.html#fnattrs)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[add](http://llvm.org/docs/LangRef.html#i_add)[i32](http://llvm.org/docs/LangRef.html#t_integer)%a,%x
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%1
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@foo](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%a)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@lambda](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%a,[i32](http://llvm.org/docs/LangRef.html#t_integer)10)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%1
}
Alternatively, if the lambda function uses more than a few variables, you can wrap them up in a structure which you pass in a pointer to the lambda function:
intfoo(inta,intb)
{
intc=integer_parse();
autofunction=[](intx){return(a+b-c)*x;}
returnfunction(10);
}
Becomes:
%Lambda_Arguments=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 0: a (argument)
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 1: b (argument)
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 2: c (local)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@lambda](http://llvm.org/docs/LangRef.html#globalvars)(%Lambda_Arguments*%args,[i32](http://llvm.org/docs/LangRef.html#t_integer)%x)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Lambda_Arguments*%args,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%a=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Lambda_Arguments*%args,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%b=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
%3=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Lambda_Arguments*%args,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)2
%c=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%3
%4=[add](http://llvm.org/docs/LangRef.html#i_add)[i32](http://llvm.org/docs/LangRef.html#t_integer)%a,%b
%5=[sub](http://llvm.org/docs/LangRef.html#i_sub)[i32](http://llvm.org/docs/LangRef.html#t_integer)%4,%c
%6=[mul](http://llvm.org/docs/LangRef.html#i_mul)[i32](http://llvm.org/docs/LangRef.html#t_integer)%5,%x
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%6
}
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@integer_parse](http://llvm.org/docs/LangRef.html#globalvars)()
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@foo](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%a,[i32](http://llvm.org/docs/LangRef.html#t_integer)%b)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%args=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%Lambda_Arguments
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Lambda_Arguments*%args,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%a,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Lambda_Arguments*%args,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%b,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
%c=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@integer_parse](http://llvm.org/docs/LangRef.html#globalvars)()
%3=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Lambda_Arguments*%args,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)2
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%c,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%3
%4=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@lambda](http://llvm.org/docs/LangRef.html#globalvars)(%Lambda_Arguments*%args,[i32](http://llvm.org/docs/LangRef.html#t_integer)10)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%4
}
Obviously there are some possible variations over this theme:
- You could pass all implicit as explicit arguments as arguments.
- You could pass all implicit as explicit arguments in the structure.
- You could pass in a pointer to the frame of the caller and let the lambda function extract the arguments and locals from the input frame.
### Closures
***todo: Describe closures.***
### Generators
A generator is a function that repeatedly yields a value in such a way that the function's state is preserved across the repeated calls of the function; this includes the function's local offset at the point it yielded a value.
The most straigthforward way to implement a generator is by wrapping all of its state variables (arguments, local variables, and return values) up into an ad-hoc structure and then pass the address of that structure to the generator.
Somehow, you need to keep track of which block of the generator you are doing on each call. This can be done in various ways; the way we show here is by using LLVM'sblockaddressinstruction
 to save the address of the next local block of code that should be executed. Other implementations use a simple state variable and then do aswitch-like
 dispatch according to the value of the state variable. In both cases, the end result is the same: A different block of code is executed for each local block in the generator.
The important thing is to think of iterators as a sort of micro-thread that is resumed whenever the iterator is called again. In other words, we need to save the address of how far the iterator got on each pass through so that it can
 resume as if a microscopic thread switch had occured. So we save the address of the instruction after the return instruction so that we can resume running as if we never had returned in the first place.
I resort to pseudo-C++ because C++ does not directly support generators. First we look at a very simple case then we advance on to a slightly more complex case:
#include <stdio.h>
generatorintfoo()
{
    yield1;
    yield2;
    yield3;
}
intmain()
{
    foreach(inti in foo())
printf("Value: %d\n", i);
return0;
}
%foo_context=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i8](http://llvm.org/docs/LangRef.html#t_integer)*,;
 0: block (state)
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 1: value (result)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@foo_setup](http://llvm.org/docs/LangRef.html#globalvars)(%foo_context*%context)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; set up 'block'
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[blockaddress](http://llvm.org/docs/LangRef.html#blockaddress)([@foo_yield](http://llvm.org/docs/LangRef.html#globalvars),%.yield1),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
; The boolean returned indicates if a result was available or not.
; Once no more results are available, the caller is expected to not call
; the iterator again.
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i1](http://llvm.org/docs/LangRef.html#t_integer)[@foo_yield](http://llvm.org/docs/LangRef.html#globalvars)(%foo_context*%context)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; dispatch to the active generator block
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
[indirectbr](http://llvm.org/docs/LangRef.html#i_indirectbr)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%2,[[label](http://llvm.org/docs/LangRef.html#t_label)%.yield1,[label](http://llvm.org/docs/LangRef.html#t_label)%.yield2,[label](http://llvm.org/docs/LangRef.html#t_label)%.yield3,[label](http://llvm.org/docs/LangRef.html#t_label)%.done]
.yield1:
; store the result value (1)
%3=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)1,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%3
; make 'block' point to next block to execute
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[blockaddress](http://llvm.org/docs/LangRef.html#blockaddress)([@foo_yield](http://llvm.org/docs/LangRef.html#globalvars),%.yield2),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%4
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)1
.yield2:
; store the result value (2)
%5=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)2,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%5
; make 'block' point to next block to execute
%6=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[blockaddress](http://llvm.org/docs/LangRef.html#blockaddress)([@foo_yield](http://llvm.org/docs/LangRef.html#globalvars),%.yield3),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%6
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)1
.yield3:
; store the result value (3)
%7=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)3,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%7
; make 'block' point to next block to execute
%8=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[blockaddress](http://llvm.org/docs/LangRef.html#blockaddress)([@foo_yield](http://llvm.org/docs/LangRef.html#globalvars),%.done),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%8
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)1
.done:
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)0
}
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,...)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[@.string](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[11x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Value:
 %d\0A\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; allocate and initialize generator context structure
%context=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%foo_context
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@foo_setup](http://llvm.org/docs/LangRef.html#globalvars)(%foo_context*%context)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.head
.head:
; foreach (int i in foo())
%1=[call](http://llvm.org/docs/LangRef.html#i_call)[i1](http://llvm.org/docs/LangRef.html#t_integer)[@foo_yield](http://llvm.org/docs/LangRef.html#globalvars)(%foo_context*%context)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%1,[label](http://llvm.org/docs/LangRef.html#t_label)%.body,[label](http://llvm.org/docs/LangRef.html#t_label)%.[tail](http://llvm.org/docs/LangRef.html#i_call)
.body:
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%3=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
%4=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,...)*[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)([11x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.string](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0),[i32](http://llvm.org/docs/LangRef.html#t_integer)%3)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.head
.[tail](http://llvm.org/docs/LangRef.html#i_call):
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
And now for a slightly more complex example that involves local variables:
#include <stdio.h>
generatorintfoo(intstart,intafter)
{
for(intindex=start;index<after;index++)
{
if(i%2==0)
            yield index+1;
else
            yield index-1;
}
}
intmain(void)
{
    foreach(inti in foo(0,5))
printf("Value: %d\n", i);
return0;
}
This becomes something like this:
%foo_context=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i8](http://llvm.org/docs/LangRef.html#t_integer)*,;
 0: block (state)
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 1: start (argument)
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 2: after (argument)
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 3: index (local)
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 4: value (result)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@foo_setup](http://llvm.org/docs/LangRef.html#globalvars)(%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)%start,[i32](http://llvm.org/docs/LangRef.html#t_integer)%after)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; set up 'block'
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[blockaddress](http://llvm.org/docs/LangRef.html#blockaddress)([@foo_yield](http://llvm.org/docs/LangRef.html#globalvars),%.init),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
; set up 'start'
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%start,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
; set up 'after'
%3=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)2
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%after,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%3
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i1](http://llvm.org/docs/LangRef.html#t_integer)[@foo_yield](http://llvm.org/docs/LangRef.html#globalvars)(%foo_context*%context)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; dispatch to the active generator block
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
[indirectbr](http://llvm.org/docs/LangRef.html#i_indirectbr)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%2,[[label](http://llvm.org/docs/LangRef.html#t_label)%.init,[label](http://llvm.org/docs/LangRef.html#t_label)%.loop_close,[label](http://llvm.org/docs/LangRef.html#t_label)%.end]
.init:
; copy argument 'start' to the local variable 'index'
%3=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%start=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%3
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)3
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%start,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%4
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.head
.head:
; for (; index < after; )
%5=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)3
%index=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%5
%6=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)2
%after=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%6
%again=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)slt[i32](http://llvm.org/docs/LangRef.html#t_integer)%index,%after
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%again,[label](http://llvm.org/docs/LangRef.html#t_label)%.loop_begin,[label](http://llvm.org/docs/LangRef.html#t_label)%.exit
.loop_begin:
%7=[srem](http://llvm.org/docs/LangRef.html#i_srem)[i32](http://llvm.org/docs/LangRef.html#t_integer)%index,2
%8=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)eq[i32](http://llvm.org/docs/LangRef.html#t_integer)%7,0
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%8,[label](http://llvm.org/docs/LangRef.html#t_label)%.even,[label](http://llvm.org/docs/LangRef.html#t_label)%.odd
.even:
; store 'index + 1' in 'value'
%9=[add](http://llvm.org/docs/LangRef.html#i_add)[i32](http://llvm.org/docs/LangRef.html#t_integer)%index,1
%10=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)4
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%9,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%10
; make 'block' point to the end of the loop (after the yield)
%11=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[blockaddress](http://llvm.org/docs/LangRef.html#blockaddress)([@foo_yield](http://llvm.org/docs/LangRef.html#globalvars),%.loop_close),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%11
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)1
.odd:
; store 'index - 1' in value
%12=[sub](http://llvm.org/docs/LangRef.html#i_sub)[i32](http://llvm.org/docs/LangRef.html#t_integer)%index,1
%13=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)4
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%12,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%13
; make 'block' point to the end of the loop (after the yield)
%14=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[blockaddress](http://llvm.org/docs/LangRef.html#blockaddress)([@foo_yield](http://llvm.org/docs/LangRef.html#globalvars),%.loop_close),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%14
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)1
.loop_close:
; increment 'index'
%15=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)3
%16=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%15
%17=[add](http://llvm.org/docs/LangRef.html#i_add)[i32](http://llvm.org/docs/LangRef.html#t_integer)%16,1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%17,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%15
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.head
.exit:
; make 'block' point to the %.end label
%x=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[blockaddress](http://llvm.org/docs/LangRef.html#blockaddress)([@foo_yield](http://llvm.org/docs/LangRef.html#globalvars),%.end),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%x
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.end
.end:
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)0
}
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,...)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[@.string](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[11x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Value:
 %d\0A\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; allocate and initialize generator context structure
%context=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%foo_context
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@foo_setup](http://llvm.org/docs/LangRef.html#globalvars)(%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)5)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.head
.head:
; foreach (int i in foo(0, 5))
%1=[call](http://llvm.org/docs/LangRef.html#i_call)[i1](http://llvm.org/docs/LangRef.html#t_integer)[@foo_yield](http://llvm.org/docs/LangRef.html#globalvars)(%foo_context*%context)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%1,[label](http://llvm.org/docs/LangRef.html#t_label)%.body,[label](http://llvm.org/docs/LangRef.html#t_label)%.[tail](http://llvm.org/docs/LangRef.html#i_call)
.body:
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%foo_context*%context,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)4
%3=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
%4=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,...)*[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)([11x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.string](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0),[i32](http://llvm.org/docs/LangRef.html#t_integer)%3)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.head
.[tail](http://llvm.org/docs/LangRef.html#i_call):
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)0
}
Another possible way of doing the above would be to generate an LLVM IR function for each state and then store a function pointer in the context structure, which is updated whenever a new state/function needs to be invoked.
## Mapping Exception Handling to LLVM IR
Exceptions can be implemented in one of three ways:
- The simple way, by using a propagated return value.
- The bulky way, by usingsetjmpandlongjmp.
- The efficient way, by using a zero-cost exception ABI.
Please notice that many compiler developers with respect for themselves won't accept the first method as a proper way of handling exceptions. However, it is unbeatable in terms of simplicity and can likely help people to understand
 that implementing exceptions does not need to be very difficult.
The second method is used by some production compilers, but it has large overhead both in terms of code bloat and the cost of atry-catchstatement
 (because all CPU registers are saved usingsetjmpwhenever atrystatement
 is encountered).
The third method is very advanced but in return does not add any cost to execution paths where no exceptions are being thrown. This method is the de-facto "right" way of implementing exceptions, whether you like it or not. LLVM directly
 supports this kind of exception handling.
In the three sections below, we'll be using this sample and transform it:
#include <stdio.h>
#include <stddef.h>
classFoo
{
public:
intGetLength()const
{
return_length;
}
voidSetLength(intvalue)
{
        _length=value;
}
private:
int_length;
};
intBar(boolfail)
{
    Foo foo;
    foo.SetLength(17);
if(fail)
thrownewException("Exception requested by caller");
    foo.SetLength(24);
returnfoo.GetLength();
}
intmain(intargc,constchar*argv[])
{
intresult;
try
{
/* The program throws an exception if an argument is specified. */
boolfail=(argc>=2);
/* Let callee decide if an exception is thrown. */
intvalue=Bar(fail);
        result=EXIT_SUCCESS;
}
catch(Exception*that)
{
printf("Error: %s\n", that->GetText());
        result=EXIT_FAILURE;
}
catch(...)
{
puts("Internal error: Unhandled exception detected");
        result=EXIT_FAILURE;
}
returnresult;
}
### Exception Handling by Propagated Return Value
This method is a compiler-generated way of implicitly checking each function's return value. Its main advantage is that it is simple - at the cost of many mostly unproductive checks of return values. The great thing about this method
 is that it readily interfaces with a host of languages and environments - it is all a matter of returning a pointer to an exception.
The C++ sample above maps to the following code:
;********************* External and Utility functions *********************
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer))[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@free](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*[noalias](http://llvm.org/docs/LangRef.html#paramattrs)[nocapture](http://llvm.org/docs/LangRef.html#paramattrs),...)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@puts](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*[noalias](http://llvm.org/docs/LangRef.html#paramattrs)[nocapture](http://llvm.org/docs/LangRef.html#paramattrs))[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
;***************************** Object class *******************************
%Object_vtable_type=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Object_vtable_type*,; 0: above: parent class vtable pointer
[i8](http://llvm.org/docs/LangRef.html#t_integer)*; 1: class: class name (usually mangled)
; virtual methods would follow here
}
[@.Object_class_name](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)[constant](http://llvm.org/docs/LangRef.html#globalvars)[7x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Object\00"
[@.Object_vtable](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)[constant](http://llvm.org/docs/LangRef.html#globalvars)%Object_vtable_type{
%Object_vtable_type*[null](http://llvm.org/docs/LangRef.html#simpleconstants),; This is the root object of the object hierarchy
[i8](http://llvm.org/docs/LangRef.html#t_integer)*[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)([7x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.Object_class_name](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0)
}
%Object=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Object_vtable_type*; 0: vtable: class vtable pointer (always non-null)
; class data members would follow here
}
; returns true if the specified object is identical to or derived from the
; class with the specified name.
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i1](http://llvm.org/docs/LangRef.html#t_integer)[@Object_IsA](http://llvm.org/docs/LangRef.html#globalvars)(%Object*%object,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%name)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
.init:
; if (object == null) return false
%0=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)ne%Object*%object,[null](http://llvm.org/docs/LangRef.html#simpleconstants)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%0,[label](http://llvm.org/docs/LangRef.html#t_label)%.once,[label](http://llvm.org/docs/LangRef.html#t_label)%.exit_false
.once:
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Object*%object,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.body
.body:
; if (vtable->class == name)
%2=[phi](http://llvm.org/docs/LangRef.html#i_phi)%Object_vtable_type**[%1,%.once],[%7,%.next]
%3=[load](http://llvm.org/docs/LangRef.html#i_load)%Object_vtable_type**%2
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Object_vtable_type*%3,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%5=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%4
%6=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)eq[i8](http://llvm.org/docs/LangRef.html#t_integer)*%5,%name
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%6,[label](http://llvm.org/docs/LangRef.html#t_label)%.exit_true,[label](http://llvm.org/docs/LangRef.html#t_label)%.next
.next:
; object = object->above
%7=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Object_vtable_type*%3,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
; while (object != null)
%8=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)ne%Object_vtable_type*%3,[null](http://llvm.org/docs/LangRef.html#simpleconstants)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%8,[label](http://llvm.org/docs/LangRef.html#t_label)%.body,[label](http://llvm.org/docs/LangRef.html#t_label)%.exit_false
.exit_true:
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)[true](http://llvm.org/docs/LangRef.html#simpleconstants)
.exit_false:
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)[false](http://llvm.org/docs/LangRef.html#simpleconstants)
}
;*************************** Exception class ******************************
%Exception_vtable_type=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Object_vtable_type*,; 0: parent class vtable pointer
[i8](http://llvm.org/docs/LangRef.html#t_integer)*; 1: class name
; virtual methods would follow here.
}
[@.Exception_class_name](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)[constant](http://llvm.org/docs/LangRef.html#globalvars)[10x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Exception\00"
[@.Exception_vtable](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)[constant](http://llvm.org/docs/LangRef.html#globalvars)%Exception_vtable_type{
%Object_vtable_type*[@.Object_vtable](http://llvm.org/docs/LangRef.html#globalvars),; the parent of this class is the Object class
[i8](http://llvm.org/docs/LangRef.html#t_integer)*[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)([10x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.Exception_class_name](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0)
}
%Exception=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Exception_vtable_type*,; 0: the vtable pointer
[i8](http://llvm.org/docs/LangRef.html#t_integer)*; 1: the _text member
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Exception_Create_String](http://llvm.org/docs/LangRef.html#globalvars)(%Exception*%this,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%text)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; set up vtable
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Exception*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)%Exception_vtable_type*[@.Exception_vtable](http://llvm.org/docs/LangRef.html#globalvars),%Exception_vtable_type**%1
; save input text string into _text
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Exception*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%text,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%2
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@Exception_GetText](http://llvm.org/docs/LangRef.html#globalvars)(%Exception*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Exception*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%2
}
;******************************* Foo class ********************************
%Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Foo_GetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%2
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
;********************************* Foo function ***************************
[@.message1](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[30x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Exception
 requested by caller\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)%Exception*[@Bar](http://llvm.org/docs/LangRef.html#globalvars)([i1](http://llvm.org/docs/LangRef.html#t_integer)%fail,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%result)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; Allocate Foo instance
%foo=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%Foo
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo)
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)17)
; if (fail)
%1=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)eq[i1](http://llvm.org/docs/LangRef.html#t_integer)%fail,[true](http://llvm.org/docs/LangRef.html#simpleconstants)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%1,[label](http://llvm.org/docs/LangRef.html#t_label)%.if_begin,[label](http://llvm.org/docs/LangRef.html#t_label)%.if_close
.if_begin:
; throw new Exception(...)
%2=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)8)
%3=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%2to%Exception*
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[30x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.message1](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Exception_Create_String](http://llvm.org/docs/LangRef.html#globalvars)(%Exception*%3,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%4)
[ret](http://llvm.org/docs/LangRef.html#i_ret)%Exception*%3
.if_close:
; foo.SetLength(24)
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)24)
%5=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Foo_GetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo)
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%5,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%result
[ret](http://llvm.org/docs/LangRef.html#i_ret)%Exception*[null](http://llvm.org/docs/LangRef.html#simpleconstants)
}
;********************************* Main program ***************************
[@.message2](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[11x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Error:
 %s\0A\00"
[@.message3](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[44x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Internal
 error: Unhandled exception detectd\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%argc,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%argv)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; "try" keyword expands to nothing.
; Body of try block.
; fail = (argc >= 2)
%fail=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)uge[i32](http://llvm.org/docs/LangRef.html#t_integer)%argc,2
; Function call.
%1=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)[i32](http://llvm.org/docs/LangRef.html#t_integer)
%2=[call](http://llvm.org/docs/LangRef.html#i_call)%Exception*[@Bar](http://llvm.org/docs/LangRef.html#globalvars)([i1](http://llvm.org/docs/LangRef.html#t_integer)%fail,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1)
%3=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)ne%Exception*%2,[null](http://llvm.org/docs/LangRef.html#simpleconstants)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%3,[label](http://llvm.org/docs/LangRef.html#t_label)%.catch_block,[label](http://llvm.org/docs/LangRef.html#t_label)%.exit
.catch_block:
%4=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)%Exception*%2to%Object*
%5=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[10x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.Exception_class_name](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%6=[call](http://llvm.org/docs/LangRef.html#i_call)[i1](http://llvm.org/docs/LangRef.html#t_integer)[@Object_IsA](http://llvm.org/docs/LangRef.html#globalvars)(%Object*%4,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%5)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%6,[label](http://llvm.org/docs/LangRef.html#t_label)%.catch_exception,[label](http://llvm.org/docs/LangRef.html#t_label)%.catch_all
.catch_exception:
%7=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[11x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.message2](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%8=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@Exception_GetText](http://llvm.org/docs/LangRef.html#globalvars)(%Exception*%2)
%9=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,...)*[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%7,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%8)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.exit
.catch_all:
%10=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[44x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.message3](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%11=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@puts](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%10)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.exit
.exit:
%result=[phi](http://llvm.org/docs/LangRef.html#i_phi)[i32](http://llvm.org/docs/LangRef.html#t_integer)[0,%0],[1,%.catch_exception],[1,%.catch_all]
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%result
}
### Setjmp/Longjmp Exception Handling
The basic idea behind thesetjmpandlongjmpexception
 handling scheme is that you save the CPU state whenever you encounter atrykeyword and then do alongjmpwhenever
 you throw an exception. If there are fewtryblocks in the program, as is typically the case, the cost of this method is not as high as it might
 seem. However, often there are implicit exception handlers due to the need to release local resources such as class instances allocated on the stack and then the cost can become quite high.
Setjmp/longjmpexception handling is often
 abbreviatedSjLjforSetJmp/LongJmp.
The sample translates into something like this:
; jmp_buf is very platform specific, this is for illustration only...
%jmp_buf=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)}
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@setjmp](http://llvm.org/docs/LangRef.html#globalvars)(%jmp_buf*%env)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@longjmp](http://llvm.org/docs/LangRef.html#globalvars)(%jmp_buf*%env,[i32](http://llvm.org/docs/LangRef.html#t_integer)%val)
;********************* External and Utility functions *********************
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer))[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@free](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*[noalias](http://llvm.org/docs/LangRef.html#paramattrs)[nocapture](http://llvm.org/docs/LangRef.html#paramattrs),...)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@puts](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*[noalias](http://llvm.org/docs/LangRef.html#paramattrs)[nocapture](http://llvm.org/docs/LangRef.html#paramattrs))[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
;***************************** Object class *******************************
%Object_vtable_type=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Object_vtable_type*,; 0: above: parent class vtable pointer
[i8](http://llvm.org/docs/LangRef.html#t_integer)*; 1: class: class name (usually mangled)
; virtual methods would follow here
}
[@.Object_class_name](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)[constant](http://llvm.org/docs/LangRef.html#globalvars)[7x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Object\00"
[@.Object_vtable](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)[constant](http://llvm.org/docs/LangRef.html#globalvars)%Object_vtable_type{
%Object_vtable_type*[null](http://llvm.org/docs/LangRef.html#simpleconstants),; This is the root object of the object hierarchy
[i8](http://llvm.org/docs/LangRef.html#t_integer)*[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)([7x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.Object_class_name](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0)
}
%Object=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Object_vtable_type*; 0: vtable: class vtable pointer (always non-null)
; class data members would follow here
}
; returns true if the specified object is identical to or derived from the
; class with the specified name.
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i1](http://llvm.org/docs/LangRef.html#t_integer)[@Object_IsA](http://llvm.org/docs/LangRef.html#globalvars)(%Object*%object,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%name)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
.init:
; if (object == null) return false
%0=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)ne%Object*%object,[null](http://llvm.org/docs/LangRef.html#simpleconstants)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%0,[label](http://llvm.org/docs/LangRef.html#t_label)%.once,[label](http://llvm.org/docs/LangRef.html#t_label)%.exit_false
.once:
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Object*%object,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.body
.body:
; if (vtable->class == name)
%2=[phi](http://llvm.org/docs/LangRef.html#i_phi)%Object_vtable_type**[%1,%.once],[%7,%.next]
%3=[load](http://llvm.org/docs/LangRef.html#i_load)%Object_vtable_type**%2
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Object_vtable_type*%3,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%5=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%4
%6=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)eq[i8](http://llvm.org/docs/LangRef.html#t_integer)*%5,%name
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%6,[label](http://llvm.org/docs/LangRef.html#t_label)%.exit_true,[label](http://llvm.org/docs/LangRef.html#t_label)%.next
.next:
; object = object->above
%7=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Object_vtable_type*%3,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
; while (object != null)
%8=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)ne%Object_vtable_type*%3,[null](http://llvm.org/docs/LangRef.html#simpleconstants)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%8,[label](http://llvm.org/docs/LangRef.html#t_label)%.body,[label](http://llvm.org/docs/LangRef.html#t_label)%.exit_false
.exit_true:
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)[true](http://llvm.org/docs/LangRef.html#simpleconstants)
.exit_false:
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i1](http://llvm.org/docs/LangRef.html#t_integer)[false](http://llvm.org/docs/LangRef.html#simpleconstants)
}
;*************************** Exception class ******************************
%Exception_vtable_type=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Object_vtable_type*,; 0: parent class vtable pointer
[i8](http://llvm.org/docs/LangRef.html#t_integer)*; 1: class name
; virtual methods would follow here.
}
[@.Exception_class_name](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)[constant](http://llvm.org/docs/LangRef.html#globalvars)[10x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Exception\00"
[@.Exception_vtable](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)[constant](http://llvm.org/docs/LangRef.html#globalvars)%Exception_vtable_type{
%Object_vtable_type*[@.Object_vtable](http://llvm.org/docs/LangRef.html#globalvars),; the parent of this class is the Object class
[i8](http://llvm.org/docs/LangRef.html#t_integer)*[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)([10x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.Exception_class_name](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0)
}
%Exception=[type](http://llvm.org/docs/LangRef.html#namedtypes){
%Exception_vtable_type*,; 0: the vtable pointer
[i8](http://llvm.org/docs/LangRef.html#t_integer)*; 1: the _text member
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Exception_Create_String](http://llvm.org/docs/LangRef.html#globalvars)(%Exception*%this,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%text)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; set up vtable
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Exception*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)%Exception_vtable_type*[@.Exception_vtable](http://llvm.org/docs/LangRef.html#globalvars),%Exception_vtable_type**%1
; save input text string into _text
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Exception*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%text,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%2
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@Exception_GetText](http://llvm.org/docs/LangRef.html#globalvars)(%Exception*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Exception*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%2
}
;******************************* Foo class ********************************
%Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Foo_GetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%2
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
;********************************* Foo function ***************************
[@.message1](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[30x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Exception
 requested by caller\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Bar](http://llvm.org/docs/LangRef.html#globalvars)(%jmp_buf*%throw,[i1](http://llvm.org/docs/LangRef.html#t_integer)%fail)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; Allocate Foo instance
%foo=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%Foo
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo)
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)17)
; if (fail)
%1=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)eq[i1](http://llvm.org/docs/LangRef.html#t_integer)%fail,[true](http://llvm.org/docs/LangRef.html#simpleconstants)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%1,[label](http://llvm.org/docs/LangRef.html#t_label)%.if_begin,[label](http://llvm.org/docs/LangRef.html#t_label)%.if_close
.if_begin:
; throw new Exception(...)
%2=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)8)
%3=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%2to%Exception*
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[30x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.message1](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Exception_Create_String](http://llvm.org/docs/LangRef.html#globalvars)(%Exception*%3,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%4)
%5=[ptrtoint](http://llvm.org/docs/LangRef.html#i_ptrtoint)%Exception*%3to[i32](http://llvm.org/docs/LangRef.html#t_integer)
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@longjmp](http://llvm.org/docs/LangRef.html#globalvars)(%jmp_buf*%throw,[i32](http://llvm.org/docs/LangRef.html#t_integer)%5)
; we never get here
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.if_close
.if_close:
; foo.SetLength(24)
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)24)
%6=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Foo_GetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%6
}
;********************************* Main program ***************************
[@.message2](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[11x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Error:
 %s\0A\00"
[@.message3](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[44x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Internal
 error: Unhandled exception detectd\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%argc,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%argv)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; "try" keyword expands to a call to @setjmp
%env=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%jmp_buf
%status=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@setjmp](http://llvm.org/docs/LangRef.html#globalvars)(%jmp_buf*%env)
%1=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)eq[i32](http://llvm.org/docs/LangRef.html#t_integer)%status,0
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%1,[label](http://llvm.org/docs/LangRef.html#t_label)%.body,[label](http://llvm.org/docs/LangRef.html#t_label)%.catch_block
.body:
; Body of try block.
; fail = (argc >= 2)
%fail=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)uge[i32](http://llvm.org/docs/LangRef.html#t_integer)%argc,2
; Function call.
%2=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Bar](http://llvm.org/docs/LangRef.html#globalvars)(%jmp_buf*%env,[i1](http://llvm.org/docs/LangRef.html#t_integer)%fail)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.exit
.catch_block:
%3=[inttoptr](http://llvm.org/docs/LangRef.html#i_inttoptr)[i32](http://llvm.org/docs/LangRef.html#t_integer)%statusto%Object*
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[10x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.Exception_class_name](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%5=[call](http://llvm.org/docs/LangRef.html#i_call)[i1](http://llvm.org/docs/LangRef.html#t_integer)[@Object_IsA](http://llvm.org/docs/LangRef.html#globalvars)(%Object*%3,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%4)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%5,[label](http://llvm.org/docs/LangRef.html#t_label)%.catch_exception,[label](http://llvm.org/docs/LangRef.html#t_label)%.catch_all
.catch_exception:
%6=[inttoptr](http://llvm.org/docs/LangRef.html#i_inttoptr)[i32](http://llvm.org/docs/LangRef.html#t_integer)%statusto%Exception*
%7=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[11x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.message2](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%8=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@Exception_GetText](http://llvm.org/docs/LangRef.html#globalvars)(%Exception*%6)
%9=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,...)*[@printf](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%7,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%8)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.exit
.catch_all:
%10=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[44x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.message3](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%11=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@puts](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%10)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.exit
.exit:
%result=[phi](http://llvm.org/docs/LangRef.html#i_phi)[i32](http://llvm.org/docs/LangRef.html#t_integer)[0,%.body],[1,%.catch_exception],[1,%.catch_all]
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%result
}
### Zero Cost Exception Handling
***todo: Explain how to implement exception handling using zero cost exception handling.***
### Resources
- [Compiler
 Internals - Exception Handling](http://www.hexblog.com/wp-content/uploads/2012/06/Recon-2012-Skochinsky-Compiler-Internals.pdf).
- [Exception Handling in C without C++](http://www.on-time.com/ddj0011.htm).
- [How a C++
 Compiler Implements Exception Handling](http://www.codeproject.com/Articles/2126/How-a-C-compiler-implements-exception-handling).
- [DWARF standard - Exception Handling](http://wiki.dwarfstd.org/index.php?title=Exception_Handling).
- [Itanium C++ ABI](http://refspecs.linuxfoundation.org/cxxabi-1.86.html).
## Mapping Object-Oriented Constructs to LLVM IR
In this chapter we'll look at various object-oriented constructs and see how they can be mapped to LLVM IR.
### Classes
A class is nothing more than a structure with an associated set of functions that take an implicit first parameter, namely a pointer to the structure. Therefore, is is very trivial to map a class to LLVM IR:
#include <stddef.h>
classFoo
{
public:
    Foo()
{
        _length=0;
}
size_tGetLength()const
{
return_length;
}
voidSetLength(size_tvalue)
{
        _length=value;
}
private:
size_t_length;
};
We first transform this code into two separate pieces:
- . The structure definition.
- . The list of methods, including the constructor.
; The structure definition for class Foo.
%Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)}
; The default constructor for class Foo.
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
; The Foo::GetLength() method.
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Foo_GetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%this
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%2
}
; The Foo::SetLength() method.
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
Then we make sure that the constructor (``Foo_Create_Default``) is invoked whenever an instance of the structure is created:
Foo foo;
%foo=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%Foo
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo)
### Virtual Methods
A virtual method is no more than a compiler-controlled function pointer. Each virtual method is recorded in thevtable, which is
 a structure of all the function pointers needed by a given class:
classFoo
{
public:
virtualintGetLengthTimesTwo()const
{
return_length*2;
}
voidSetLength(size_tvalue)
{
        _length=value;
}
private:
int_length;
};
intmain()
{
    Foo foo;
    foo.SetLength(4);
returnfoo.GetLengthTimesTwo();
}
This becomes:
%Foo_vtable_type=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)(%Foo*)*}
%Foo=[type](http://llvm.org/docs/LangRef.html#namedtypes){%Foo_vtable_type*,[i32](http://llvm.org/docs/LangRef.html#t_integer)}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Foo_GetLengthTimesTwo](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
%3=[mul](http://llvm.org/docs/LangRef.html#i_mul)[i32](http://llvm.org/docs/LangRef.html#t_integer)%2,2
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%3
}
[@Foo_vtable_data](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)%Foo_vtable_type{
[i32](http://llvm.org/docs/LangRef.html#t_integer)(%Foo*)*[@Foo_GetLengthTimesTwo](http://llvm.org/docs/LangRef.html#globalvars)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)%Foo_vtable_type*[@Foo_vtable_data](http://llvm.org/docs/LangRef.html#globalvars),%Foo_vtable_type**%1
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%argc,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%argv)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%foo=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%Foo
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo)
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Foo_SetLength](http://llvm.org/docs/LangRef.html#globalvars)(%Foo*%foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)4)
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo*%foo,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%2=[load](http://llvm.org/docs/LangRef.html#i_load)%Foo_vtable_type**%1
%3=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Foo_vtable_type*%2,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%4=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)(%Foo*)**%3
%5=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)%4(%Foo*%foo)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%5
}
Please notice that some C++ compilers store_vtableat a negative offset into the structure so that things likememcpy(this,
 0, sizeof(*this))work, even though such commands should always be avoided in an OOP context.
### Single Inheritance
Single inheritance is very straightforward: Each "structure" (class) is laid out in memory after one another in declaration order.
classBase
{
public:
voidSetA(intvalue)
{
        _a=value;
}
private:
int_a;
};
classDerived:publicBase
{
public:
voidSetB(intvalue)
{
        SetA(value);
        _b=value;
}
protected:
int_b;
}
Here,aandbwill
 be laid out to follow one another in memory so that inheriting from a class is simply a matter of declaring a the base class as a first member in the inheriting class:
%Base=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 '_a' in class Base
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Base_SetA](http://llvm.org/docs/LangRef.html#globalvars)(%Base*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Base*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
%Derived=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 '_a' from class Base
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 '_b' from class Derived
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Derived_SetB](http://llvm.org/docs/LangRef.html#globalvars)(%Derived*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)%Derived*%thisto%Base*
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Base_SetA](http://llvm.org/docs/LangRef.html#globalvars)(%Base*%1,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Derived*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
So the base class simply becomes plain members of the type declaration for the derived class.
And then the compiler must insert appropriate type casts whenever the derived class is being referenced as its base class as shown above with thebitcastoperator.
### Multiple Inheritance
Multiple inheritance is not that difficult, either, it is merely a question of laying out the multiply inherited "structures" in order inside each derived class.
classBaseA
{
public:
voidSetA(intvalue)
{
        _a=value;
}
private:
int_a;
};
classBaseB:publicBaseA
{
public:
voidSetB(intvalue)
{
        SetA(value);
        _b=value;
}
private:
int_b;
};
classDerived:
publicBaseA,
publicBaseB
{
public:
voidSetC(intvalue)
{
        SetB(value);
        _c=value;
}
private:
int_c;
};
This is equivalent to the following LLVM IR:
%BaseA=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 '_a' from BaseA
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@BaseA_SetA](http://llvm.org/docs/LangRef.html#globalvars)(%BaseA*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%BaseA*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
%BaseB=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 '_a' from BaseA
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 '_b' from BaseB
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@BaseB_SetB](http://llvm.org/docs/LangRef.html#globalvars)(%BaseB*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)%BaseB*%thisto%BaseA*
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@BaseA_SetA](http://llvm.org/docs/LangRef.html#globalvars)(%BaseA*%1,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%BaseB*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
%Derived=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 '_a' from BaseA
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 '_b' from BaseB
[i32](http://llvm.org/docs/LangRef.html#t_integer);
 '_c' from Derived
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Derived_SetC](http://llvm.org/docs/LangRef.html#globalvars)(%Derived*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)%Derived*%thisto%BaseB*
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@BaseB_SetB](http://llvm.org/docs/LangRef.html#globalvars)(%BaseB*%1,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Derived*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)2
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
And the compiler then supplies the needed type casts and pointer arithmentic wheneverbaseBis being referenced as an
 instance ofBaseB. Please notice that all it takes is abitcastfrom
 one class to another as well as an adjustment of the last argument togetelementptr.
### Virtual Inheritance
Virtual inheritance is actually quite simple as it dictates that identical base classes are to be merged into a single occurence. For instance, given this:
classBaseA
{
public:
inta;
};
classBaseB:publicBaseA
{
public:
intb;
};
classBaseC:publicBaseA
{
public:
intc;
};
classDerived:
publicvirtualBaseB,
publicvirtualBaseC
{
intd;
};
Derivedwill only contain a single instance ofBaseAeven
 if its inheritance graph dictates that it should have two instances. The result looks something like this:
classDerived
{
public:
inta;
intb;
intc;
intd;
};
So the second instance ofais silently ignored because it would cause multiple instances ofBaseAto
 exist inDerived, which clearly would cause lots of confusion and ambiguities.
### Interfaces
An interface is nothing more than a base class with no data members, where all the methods are pure virtual (i.e. has no body).
As such, we've already described how to convert an interface to LLVM IR - it is done precisely the same way that you convert a virtual member function to LLVM IR.
### Boxing and Unboxing
Boxing is the process of converting a non-object primitive value into an object. It is as easy as it sounds. You create a wrapper class which you instantiate and initialize with the non-object value:
Unboxing is the reverse of boxing: You downgrade a full object to a mere scalar value by retrieving the boxed value from the box object.
It is important to notice that changes to the boxed value does not affect the original value and vice verse. The code below illustrates both steps:
[@Boxee](http://llvm.org/docs/LangRef.html#globalvars)=[global](http://llvm.org/docs/LangRef.html#globalvars)[i32](http://llvm.org/docs/LangRef.html#t_integer)17
%Integer=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@Integer_Create](http://llvm.org/docs/LangRef.html#globalvars)(%Integer*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; you might set up a vtable and associated virtual methods here
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Integer*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%value,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Integer_GetValue](http://llvm.org/docs/LangRef.html#globalvars)(%Integer*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%Integer*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)%2
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; box @Boxee in an instance of %Integer
%1=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*[@Boxee](http://llvm.org/docs/LangRef.html#globalvars)
%2=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)%Integer
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@Integer_Create](http://llvm.org/docs/LangRef.html#globalvars)(%Integer*%2,[i32](http://llvm.org/docs/LangRef.html#t_integer)%1)
; unbox @Boxee from an instance of %Integer
%3=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@Integer_GetValue](http://llvm.org/docs/LangRef.html#globalvars)(%Integer*%2)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)0
}
### Class Equivalence Test
There are two ways of doing this:
- If you can guarantee that each class a unique vtable, you can simply compare the pointers to the vtable.
- If you cannot guarantee that each class has a unique vtable (because different vtables may have been merged by the linker), you need to add a unique field to the vtable so that you can compare that instead.
The first variant goes roughly as follows (assuming identical strings aren't merged by the compiler, something that they are most of the time):
boolequal=(typeid(first)==typeid(other));
***todo: Finish up class equivalence test sample.***
As far as I know, RTTI is simply done by adding two fields to the _vtable structure:parentandsignature.
 The former is a pointer to the vtable of the parent class and the latter is the mangled (encoded) name of the class. To see if a given class is another class, you simply compare thesignaturefields.
 To see if a given class is a derived class of some other class, you simply walk the chain ofparentfields, while checking if you have found
 a matching signature.
### Class Inheritance Test
A class inheritance test is a question of the form:
*Is class X identical to or derived from class Y?*
To answer that question, we can use one of two methods:
- The naive implementation where we search upwards in the chain of parents.
- The faster implementation where we search a preallocated list of parents.
The naive implementation is documented in the first two exception handling examples as theObject_IsAfunction.
***todo: Document the faster class inheritance test implementation.***
### The New Operator
Thenewoperator is generally nothing more than a type-safe version of the Cmallocfunction
 - in some implementations of C++, they may even be called interchangeably without causing unseen or unwanted side-effects.
#### The Instance New Operator
All calls of the formnew Xare mapped into:
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer))[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
%X=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i8](http://llvm.org/docs/LangRef.html#t_integer)}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@X_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%X*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%X*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)0,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)1)
%2=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%1to%X*
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@X_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%X*%2)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
Calls of the formnew X(Y, Z)are the same, exceptYandZare
 passed into the constructor as arguments.
#### The Array New Operator
New operations involving arrays are equally simple. The codenew X[100]is mapped into a loop that initializes each
 array element in turn:
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer))[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
%X=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer)}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@X_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%X*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%X*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@main](http://llvm.org/docs/LangRef.html#globalvars)()[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%n=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)[i32](http://llvm.org/docs/LangRef.html#t_integer); %n = ptr to the number of elements in the array
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)100,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%n
%i=[alloca](http://llvm.org/docs/LangRef.html#i_alloca)[i32](http://llvm.org/docs/LangRef.html#t_integer); %i = ptr to the loop index into the array
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%i
%1=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%n; %1 = *%n
%2=[mul](http://llvm.org/docs/LangRef.html#i_mul)[i32](http://llvm.org/docs/LangRef.html#t_integer)%1,4; %2 = %1 * sizeof(X)
%3=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%2); %3 = malloc(100 * sizeof(X))
%4=[bitcast](http://llvm.org/docs/LangRef.html#i_bitcast)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%3to%X*; %4 = (X*) %3
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.loop_head
.loop_head:; for (; %i < %n; %i++)
%5=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%i
%6=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%n
%7=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)slt[i32](http://llvm.org/docs/LangRef.html#t_integer)%5,%6
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%7,[label](http://llvm.org/docs/LangRef.html#t_label)%.loop_body,[label](http://llvm.org/docs/LangRef.html#t_label)%.loop_tail
.loop_body:
%8=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%X*%4,[i32](http://llvm.org/docs/LangRef.html#t_integer)%5
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@X_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%X*%8)
%9=[add](http://llvm.org/docs/LangRef.html#i_add)[i32](http://llvm.org/docs/LangRef.html#t_integer)%5,1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%9[i32](http://llvm.org/docs/LangRef.html#t_integer)*%i
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%.loop_head
.loop_tail:
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
## Interoperating with a Runtime Library
It is common to provide a set of run-time support functions that are written in another language than LLVM IR and it is trivially easy to interface to such a run-time library. The use ofmallocandfreein
 the examples in this document are examples of such use of externally defined run-time functions.
The advantages of a custom, non-IR run-time library function is that it can be optimized by hand to provide the best possible performance under certain criteria. Also a custom non-IR run-time library function can make explicit use of
 native instructions that are foreign to the LLVM infrastructure.
The advantages of IR run-time library functions is that they can be run through the optimizer and thereby also be inlined automatically.
## Interfacing to the Operating System
I'll divide this chapter into two sections:
- How to Interface to POSIX Operating Systems.
- How to Interface to the Windows Operating System.
### How to Interface to POSIX Operating Systems
On POSIX, the presence of the C run-time library is an unavoidable fact for which reason it makes a lot of sense to directly call such C run-time functions.
#### Sample POSIX "Hello World" Application
On POSIX, it is really very easy to create theHello worldprogram:
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@puts](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*[nocapture](http://llvm.org/docs/LangRef.html#paramattrs))[nounwind](http://llvm.org/docs/LangRef.html#fnattrs)
[@.hello](http://llvm.org/docs/LangRef.html#globalvars)=[private](http://llvm.org/docs/LangRef.html#linkage_private)unnamed_addr[constant](http://llvm.org/docs/LangRef.html#globalvars)[13x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"hello
 world\0A\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%argc,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%argv){
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[13x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@.hello](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@puts](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%1)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)0
}
#### How to Interface to the Windows Operating System
On Windows, the C run-time library is mostly considered of relevance to the C and C++ languages only, so you have a plethora (thousands) of standard system interfaces that any client application may use.
#### Sample Windows "Hello World" Application
Hello worldon Windows is nowhere as straightforward as on POSIX:
[target datalayout](http://llvm.org/docs/LangRef.html#datalayout)="e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-f80:128:128-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S32"
target triple="i686-pc-win32"
%struct._OVERLAPPED=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer),[i32](http://llvm.org/docs/LangRef.html#t_integer),%union.anon,[i8](http://llvm.org/docs/LangRef.html#t_integer)*}
%union.anon=[type](http://llvm.org/docs/LangRef.html#namedtypes){%struct.anon}
%struct.anon=[type](http://llvm.org/docs/LangRef.html#namedtypes){[i32](http://llvm.org/docs/LangRef.html#t_integer),[i32](http://llvm.org/docs/LangRef.html#t_integer)}
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[dllimport](http://llvm.org/docs/LangRef.html#linkage_dllimport)x86_stdcallcc[i8](http://llvm.org/docs/LangRef.html#t_integer)*@"\01_GetStdHandle@4"([i32](http://llvm.org/docs/LangRef.html#t_integer))#1
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[dllimport](http://llvm.org/docs/LangRef.html#linkage_dllimport)x86_stdcallcc[i32](http://llvm.org/docs/LangRef.html#t_integer)@"\01_WriteFile@20"([i8](http://llvm.org/docs/LangRef.html#t_integer)*,[i8](http://llvm.org/docs/LangRef.html#t_integer)*,[i32](http://llvm.org/docs/LangRef.html#t_integer),[i32](http://llvm.org/docs/LangRef.html#t_integer)*,%struct._OVERLAPPED*)#1
[@hello](http://llvm.org/docs/LangRef.html#globalvars)=[internal](http://llvm.org/docs/LangRef.html#linkage_internal)[constant](http://llvm.org/docs/LangRef.html#globalvars)[13x[i8](http://llvm.org/docs/LangRef.html#t_integer)]c"Hello
 world\0A\00"
[define](http://llvm.org/docs/LangRef.html#functionstructure)[i32](http://llvm.org/docs/LangRef.html#t_integer)[@main](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%argc,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%argv)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
%1=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*@"\01_GetStdHandle@4"([i32](http://llvm.org/docs/LangRef.html#t_integer)-11); -11 = STD_OUTPUT_HANDLE
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[13x[i8](http://llvm.org/docs/LangRef.html#t_integer)]*[@hello](http://llvm.org/docs/LangRef.html#globalvars),[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%3=[call](http://llvm.org/docs/LangRef.html#i_call)[i32](http://llvm.org/docs/LangRef.html#t_integer)@"\01_WriteFile@20"([i8](http://llvm.org/docs/LangRef.html#t_integer)*%1,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%2,[i32](http://llvm.org/docs/LangRef.html#t_integer)12,[i32](http://llvm.org/docs/LangRef.html#t_integer)*[null](http://llvm.org/docs/LangRef.html#simpleconstants),%struct._OVERLAPPED*[null](http://llvm.org/docs/LangRef.html#simpleconstants))
; todo: Check that %4 is not equal to -1 (INVALID_HANDLE_VALUE)
[ret](http://llvm.org/docs/LangRef.html#i_ret)[i32](http://llvm.org/docs/LangRef.html#t_integer)0
}
attributes #1={"less-precise-fpmad"="false""no-frame-pointer-elim"="true""no-frame-pointer-elim-non-leaf"
"no-infs-fp-math"="fa lse""no-nans-fp-math"="false""stack-protector-buffer-size"="8""unsafe-fp-math"="false"
"use-soft-float"="false"
}
# Resources
This chapter lists some resources that may be of interest to the reader:
- Modern Compiler Implementation in Java, 2nd Edition.
- [Alex Darby's series of articles on low-level stuff](http://www.altdevblogaday.com/author/alex-darby/).
# Epilogue
If you discover any errors in this document or you need more information than given here, please write to the author at[Mikael
 Lyngvig](mailto:mikael@lyngvig.org).
Please also remember that you can learn a lot by using the-emit-llvmoption to theclang++compiler.
 This gives you a chance to see a live production compiler in action and precisely how it does things.
# Appendix A: How to Implement a String Type in LLVM
There are two ways to implement a string type in LLVM:
- To write the implementation in LLVM IR.
- To write the implementation in a higher-level language that generates IR.
I'd personally much prefer to use the second method, but for the sake of the example, I'll go ahead and illustrate a simple but useful string type in LLVM IR. It assumes a 32-bit architecture, so please replace all occurences ofi32withi64if
 you are targetting a 64-bit architecture.
We'll be making a dynamic, mutable string type that can be appended to and could also be inserted into, converted to lower case, and so on, depending on which support functions are defined to operate on the string type.
It all boils down to making a suitable type definition for the class and then define a rich set of functions to operate on the type definition:
; The actual type definition for our 'String' type.
%String=[type](http://llvm.org/docs/LangRef.html#namedtypes){
[i8](http://llvm.org/docs/LangRef.html#t_integer)*,;
 0: buffer; pointer to the character buffer
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 1: length; the number of chars in the buffer
[i32](http://llvm.org/docs/LangRef.html#t_integer),;
 2: maxlen; the maximum number of chars in the buffer
[i32](http://llvm.org/docs/LangRef.html#t_integer); 3:
 factor; the number of chars to preallocate when growing
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[fastcc](http://llvm.org/docs/LangRef.html#callingconv)[void](http://llvm.org/docs/LangRef.html#t_void)[@String_Create_Default](http://llvm.org/docs/LangRef.html#globalvars)(%String*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; Initialize 'buffer'.
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[null](http://llvm.org/docs/LangRef.html#simpleconstants),[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
; Initialize 'length'.
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
; Initialize 'maxlen'.
%3=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)2
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%3
; Initialize 'factor'.
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)3
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)16,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%4
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer))
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[void](http://llvm.org/docs/LangRef.html#t_void)[@free](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*)
[declare](http://llvm.org/docs/LangRef.html#functionstructure)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@memcpy](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*,[i8](http://llvm.org/docs/LangRef.html#t_integer)*,[i32](http://llvm.org/docs/LangRef.html#t_integer))
[define](http://llvm.org/docs/LangRef.html#functionstructure)[fastcc](http://llvm.org/docs/LangRef.html#callingconv)[void](http://llvm.org/docs/LangRef.html#t_void)[@String_Delete](http://llvm.org/docs/LangRef.html#globalvars)(%String*%this)[nounwind](http://llvm.org/docs/LangRef.html#fnattrs){
; Check if we need to call 'free'.
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%2=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
%3=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)ne[i8](http://llvm.org/docs/LangRef.html#t_integer)*%2,[null](http://llvm.org/docs/LangRef.html#simpleconstants)
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%3,[label](http://llvm.org/docs/LangRef.html#t_label)%free_begin,[label](http://llvm.org/docs/LangRef.html#t_label)%free_close
free_begin:
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@free](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%2)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%free_close
free_close:
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[fastcc](http://llvm.org/docs/LangRef.html#callingconv)[void](http://llvm.org/docs/LangRef.html#t_void)[@String_Resize](http://llvm.org/docs/LangRef.html#globalvars)(%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%value){
; %output = malloc(%value)
%output=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@malloc](http://llvm.org/docs/LangRef.html#globalvars)([i32](http://llvm.org/docs/LangRef.html#t_integer)%value)
; todo: check return value
; %buffer = this->buffer
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%buffer=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
; %length = this->length
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%length=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
; memcpy(%output, %buffer, %length)
%3=[call](http://llvm.org/docs/LangRef.html#i_call)[i8](http://llvm.org/docs/LangRef.html#t_integer)*[@memcpy](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%output,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%buffer,[i32](http://llvm.org/docs/LangRef.html#t_integer)%length)
; free(%buffer)
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@free](http://llvm.org/docs/LangRef.html#globalvars)([i8](http://llvm.org/docs/LangRef.html#t_integer)*%buffer)
; this->buffer = %output
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%output,[i8](http://llvm.org/docs/LangRef.html#t_integer)**%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
[define](http://llvm.org/docs/LangRef.html#functionstructure)[fastcc](http://llvm.org/docs/LangRef.html#callingconv)[void](http://llvm.org/docs/LangRef.html#t_void)[@String_Add_Char](http://llvm.org/docs/LangRef.html#globalvars)(%String*%this,[i8](http://llvm.org/docs/LangRef.html#t_integer)%value){
; Check if we need to grow the string.
%1=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)1
%length=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
%2=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)2
%maxlen=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%2
; if length == maxlen:
%3=[icmp](http://llvm.org/docs/LangRef.html#i_icmp)eq[i32](http://llvm.org/docs/LangRef.html#t_integer)%length,%maxlen
[br](http://llvm.org/docs/LangRef.html#i_br)[i1](http://llvm.org/docs/LangRef.html#t_integer)%3,[label](http://llvm.org/docs/LangRef.html#t_label)%grow_begin,[label](http://llvm.org/docs/LangRef.html#t_label)%grow_close
grow_begin:
%4=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)3
%factor=[load](http://llvm.org/docs/LangRef.html#i_load)[i32](http://llvm.org/docs/LangRef.html#t_integer)*%4
%5=[add](http://llvm.org/docs/LangRef.html#i_add)[i32](http://llvm.org/docs/LangRef.html#t_integer)%maxlen,%factor
[call](http://llvm.org/docs/LangRef.html#i_call)[void](http://llvm.org/docs/LangRef.html#t_void)[@String_Resize](http://llvm.org/docs/LangRef.html#globalvars)(%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)%5)
[br](http://llvm.org/docs/LangRef.html#i_br)[label](http://llvm.org/docs/LangRef.html#t_label)%grow_close
grow_close:
%6=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)%String*%this,[i32](http://llvm.org/docs/LangRef.html#t_integer)0,[i32](http://llvm.org/docs/LangRef.html#t_integer)0
%buffer=[load](http://llvm.org/docs/LangRef.html#i_load)[i8](http://llvm.org/docs/LangRef.html#t_integer)**%6
%7=[getelementptr](http://llvm.org/docs/LangRef.html#i_getelementptr)[i8](http://llvm.org/docs/LangRef.html#t_integer)*%buffer,[i32](http://llvm.org/docs/LangRef.html#t_integer)%length
[store](http://llvm.org/docs/LangRef.html#i_store)[i8](http://llvm.org/docs/LangRef.html#t_integer)%value,[i8](http://llvm.org/docs/LangRef.html#t_integer)*%7
%8=[add](http://llvm.org/docs/LangRef.html#i_add)[i32](http://llvm.org/docs/LangRef.html#t_integer)%length,1
[store](http://llvm.org/docs/LangRef.html#i_store)[i32](http://llvm.org/docs/LangRef.html#t_integer)%8,[i32](http://llvm.org/docs/LangRef.html#t_integer)*%1
[ret](http://llvm.org/docs/LangRef.html#i_ret)[void](http://llvm.org/docs/LangRef.html#t_void)
}
# Appendix B: Task List
This chapter serves as an informal task list and is to be updated as new to-do items are completed:
- How to enable debug information? (Line and Function, Variable)
- How to interface with a garbage collector? (link to existing docs)
- How to express a custom calling convention? (link to existing docs)
- Representing constructors, destructors, finalization
- How to examine the stack at runtime? How to modify it? (i.e. reflection, interjection)
- Representing subtyping checks (with full alias info), TBAA, struct-path TBAA.
- How to exploit inlining (external, vs within LLVM)?
- How to express array bounds checks for best optimization?
- How to express null pointer checks?
- How to express domain specific optimizations? (i.e. lock elision, or matrix math simplification) (link to existing docs)
- How to optimize call dispatch or field access in dynamic languages? (ref new patchpoint intrinsics for inline call caching and field access caching)
***todo: Ask various front-end implementors (Rust, Haskell (GHC), Rubinius, and more) to review and/or contribute so as to make the document great.***
