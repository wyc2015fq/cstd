# 深入研究Clang（四） Clang编译器的简单分析 - SHINING的博客 - CSDN博客
2014年11月13日 14:53:59[snsn1984](https://me.csdn.net/snsn1984)阅读数：19509
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
作者：[史宁宁（snsn1984）](http://blog.csdn.net/snsn1984)
首先我们确定下Clang编译器的具体内容和涵盖范围。之前在《[LLVM每日谈之二十 Everything && Clang driver](http://blog.csdn.net/snsn1984/article/details/40857967)》中曾经提到过，Clang driver（命令行表示是clang）和Clang前端（按照具体实现来说就是Clang的那些库所实现的前端）是不同的，同时还存在一个Clang编译器（命令行表示是clang -cc1）。Clang编译器不仅仅包含了Clang前端，还包括使用LLVM的哭实现的编译器的中间阶段以及后端，同时也集成了assembler。
Clang driver有一系列的frontend action，这些frontend action定义于clang/include/clang/Frontend/FrontendOptions.h中的ActionKind枚举中。其中一些frontend action就会触发Clang编译器（clang -cc1），比如：ASTView, EmitBC, EmitObj等。一旦触发了Clang编译器（clang -cc1），就会执行函数cc1_main()（clang/tools/driver/cc1_main.cpp），从名字上就可以看出来，这个函数是Clang编译器（clang -cc1）的入口主函数。
举个具体的例子来看一下：
min.c
```cpp
int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}
```
运行命令： clang -### min.c -o min
```cpp
clang version 3.5.0 (tags/RELEASE_350/final)
Target: x86_64-unknown-linux-gnu
Thread model: posix
 "/home/shining/llvm-3.5/build/bin/clang-3.5" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-emit-obj" "-mrelax-all" "-disable-free" "-main-file-name" "min.c" "-mrelocation-model" "static" "-mdisable-fp-elim" "-fmath-errno" "-masm-verbose" "-mconstructor-aliases" "-munwind-tables" "-fuse-init-array" "-target-cpu" "x86-64" "-dwarf-column-info" "-resource-dir" "/home/shining/llvm-3.5/build/bin/../lib/clang/3.5.0" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/home/shining/llvm-3.5/build/bin/../lib/clang/3.5.0/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-fdebug-compilation-dir" "/home/shining/llvm-3.5/build/bin" "-ferror-limit" "19" "-fmessage-length" "80" "-mstackrealign" "-fobjc-runtime=gcc" "-fdiagnostics-show-option" "-o" "/tmp/min-75c13b.o" "-x" "c" "min.c"
 "/usr/bin/ld" "-z" "relro" "--hash-style=gnu" "--build-id" "--eh-frame-hdr" "-m" "elf_x86_64" "-dynamic-linker" "/lib64/ld-linux-x86-64.so.2" "-o" "min" "/usr/lib/gcc/x86_64-linux-gnu/4.8/../../../x86_64-linux-gnu/crt1.o" "/usr/lib/gcc/x86_64-linux-gnu/4.8/../../../x86_64-linux-gnu/crti.o" "/usr/lib/gcc/x86_64-linux-gnu/4.8/crtbegin.o" "-L/usr/lib/gcc/x86_64-linux-gnu/4.8" "-L/usr/lib/gcc/x86_64-linux-gnu/4.8/../../../x86_64-linux-gnu" "-L/lib/x86_64-linux-gnu" "-L/lib/../lib64" "-L/usr/lib/x86_64-linux-gnu" "-L/usr/lib/gcc/x86_64-linux-gnu/4.8/../../.." "-L/home/shining/llvm-3.5/build/bin/../lib" "-L/lib" "-L/usr/lib" "/tmp/min-75c13b.o" "-lgcc" "--as-needed" "-lgcc_s" "--no-as-needed" "-lc" "-lgcc" "--as-needed" "-lgcc_s" "--no-as-needed" "/usr/lib/gcc/x86_64-linux-gnu/4.8/crtend.o" "/usr/lib/gcc/x86_64-linux-gnu/4.8/../../../x86_64-linux-gnu/crtn.o"
```
-###参数是为了查看，clang driver到底调用了哪些命令，并且不会执行这些命令。从这里可以实际看到，实际上调用的是 clang-3.5 -cc1，其中3.5是版本号，所以其实调用的就是clang编译器。之后又调用了系统的ld loader,因为LLVM架构的loader还在开发之中。
所以，对于那些我们已经明确需要clang编译器去作的工作，我们可以不通过clang driver去隐式调用（比如上面的例子），而是直接在命令行调用clang -cc1去执行，并且在clang -cc1之后跟clang编译器接受的参数。也可以通过clang -Xclang就可以直接将参数传递给clang编译器（clang -cc1）。下面的具体实现，将同时给出这两种的命令行形式，其实运行结果几乎是完全相同的。不同的是，使用clang -Xclang的时候，如果不加强制的参数，这里虽然-Xclang将参数传递给了clang -cc1，但是这里的clang driver依然会继续工作的。在下面的例子中会进行分别的展示
编译器首先进行的是词法分析，我们可以通过命令行去查看进行词法分析之后的token序列到底是怎么样的，仍然以上面的min.c为例，运行命令：
clang -cc1 -dump-tokens min.c
运行之后得到如下输出：
```cpp
int 'int'	 [StartOfLine]	Loc=<min.c:1:1>
identifier 'min'	 [LeadingSpace]	Loc=<min.c:1:5>
l_paren '('		Loc=<min.c:1:8>
int 'int'		Loc=<min.c:1:9>
identifier 'a'	 [LeadingSpace]	Loc=<min.c:1:13>
comma ','		Loc=<min.c:1:14>
int 'int'	 [LeadingSpace]	Loc=<min.c:1:16>
identifier 'b'	 [LeadingSpace]	Loc=<min.c:1:20>
r_paren ')'		Loc=<min.c:1:21>
l_brace '{'	 [LeadingSpace]	Loc=<min.c:1:23>
if 'if'	 [StartOfLine] [LeadingSpace]	Loc=<min.c:2:5>
l_paren '('	 [LeadingSpace]	Loc=<min.c:2:8>
identifier 'a'		Loc=<min.c:2:9>
less '<'	 [LeadingSpace]	Loc=<min.c:2:11>
identifier 'b'	 [LeadingSpace]	Loc=<min.c:2:13>
r_paren ')'		Loc=<min.c:2:14>
l_brace '{'	 [LeadingSpace]	Loc=<min.c:2:16>
return 'return'	 [StartOfLine] [LeadingSpace]	Loc=<min.c:3:9>
identifier 'a'	 [LeadingSpace]	Loc=<min.c:3:16>
semi ';'		Loc=<min.c:3:17>
r_brace '}'	 [StartOfLine] [LeadingSpace]	Loc=<min.c:4:5>
return 'return'	 [StartOfLine] [LeadingSpace]	Loc=<min.c:5:5>
identifier 'b'	 [LeadingSpace]	Loc=<min.c:5:12>
semi ';'		Loc=<min.c:5:13>
r_brace '}'	 [StartOfLine]	Loc=<min.c:6:1>
eof ''		Loc=<min.c:6:2>
```
或者选用： clang -Xclang -dump-tokens min.c
输出信息如下：
```cpp
int 'int'	 [StartOfLine]	Loc=<min.c:1:1>
identifier 'min'	 [LeadingSpace]	Loc=<min.c:1:5>
l_paren '('		Loc=<min.c:1:8>
int 'int'		Loc=<min.c:1:9>
identifier 'a'	 [LeadingSpace]	Loc=<min.c:1:13>
comma ','		Loc=<min.c:1:14>
int 'int'	 [LeadingSpace]	Loc=<min.c:1:16>
identifier 'b'	 [LeadingSpace]	Loc=<min.c:1:20>
r_paren ')'		Loc=<min.c:1:21>
l_brace '{'	 [LeadingSpace]	Loc=<min.c:1:23>
if 'if'	 [StartOfLine] [LeadingSpace]	Loc=<min.c:2:5>
l_paren '('	 [LeadingSpace]	Loc=<min.c:2:8>
identifier 'a'		Loc=<min.c:2:9>
less '<'	 [LeadingSpace]	Loc=<min.c:2:11>
identifier 'b'	 [LeadingSpace]	Loc=<min.c:2:13>
r_paren ')'		Loc=<min.c:2:14>
l_brace '{'	 [LeadingSpace]	Loc=<min.c:2:16>
return 'return'	 [StartOfLine] [LeadingSpace]	Loc=<min.c:3:9>
identifier 'a'	 [LeadingSpace]	Loc=<min.c:3:16>
semi ';'		Loc=<min.c:3:17>
r_brace '}'	 [StartOfLine] [LeadingSpace]	Loc=<min.c:4:5>
return 'return'	 [StartOfLine] [LeadingSpace]	Loc=<min.c:5:5>
identifier 'b'	 [LeadingSpace]	Loc=<min.c:5:12>
semi ';'		Loc=<min.c:5:13>
r_brace '}'	 [StartOfLine]	Loc=<min.c:6:1>
eof ''		Loc=<min.c:6:2>
/usr/bin/ld: cannot find /tmp/min-3cce9d.o: No such file or directory
clang-3.5: error: linker command failed with exit code 1 (use -v to see invocation)
```
明显可以看到，使用-Xclang的时候，把-dump-tokens参数传递给了clang -cc1，但是clang driver依然工作，并且调用了ld.
可以使用clang -### -Xclang -dump-tokens min.c命令进行验证。
看过了词法分析阶段，我们再看下clang编译器语法分析来的AST nodes。
使用命令：clang -cc1 -fsyntax-only -ast-dump min.c
或者：clang -fsyntax-only -Xclang -ast-dump min.c
输出结果一样：
```cpp
TranslationUnitDecl 0x6bc3a40 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x6bc3f40 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
|-TypedefDecl 0x6bc3fa0 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
|-TypedefDecl 0x6bc42f0 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list '__va_list_tag [1]'
`-FunctionDecl 0x6bc4490 <min.c:1:1, line:6:1> line:1:5 min 'int (int, int)'
  |-ParmVarDecl 0x6bc4350 <col:9, col:13> col:13 used a 'int'
  |-ParmVarDecl 0x6bc43c0 <col:16, col:20> col:20 used b 'int'
  `-CompoundStmt 0x6bc46f8 <col:23, line:6:1>
    |-IfStmt 0x6bc4668 <line:2:5, line:4:5>
    | |-<<<NULL>>>
    | |-BinaryOperator 0x6bc45c0 <line:2:9, col:13> 'int' '<'
    | | |-ImplicitCastExpr 0x6bc4590 <col:9> 'int' <LValueToRValue>
    | | | `-DeclRefExpr 0x6bc4540 <col:9> 'int' lvalue ParmVar 0x6bc4350 'a' 'int'
    | | `-ImplicitCastExpr 0x6bc45a8 <col:13> 'int' <LValueToRValue>
    | |   `-DeclRefExpr 0x6bc4568 <col:13> 'int' lvalue ParmVar 0x6bc43c0 'b' 'int'
    | |-CompoundStmt 0x6bc4648 <col:16, line:4:5>
    | | `-ReturnStmt 0x6bc4628 <line:3:9, col:16>
    | |   `-ImplicitCastExpr 0x6bc4610 <col:16> 'int' <LValueToRValue>
    | |     `-DeclRefExpr 0x6bc45e8 <col:16> 'int' lvalue ParmVar 0x6bc4350 'a' 'int'
    | `-<<<NULL>>>
    `-ReturnStmt 0x6bc46d8 <line:5:5, col:12>
      `-ImplicitCastExpr 0x6bc46c0 <col:12> 'int' <LValueToRValue>
        `-DeclRefExpr 0x6bc4698 <col:12> 'int' lvalue ParmVar 0x6bc43c0 'b' 'int'
```
通过clang -### -fsyntax-only -Xclang -ast-dump min.c查看实际执行命令，其实跟使用clang -cc1是相同的。
参考资料：
1. 《Getting Started with LLVM Core Libraries》
2.  Code of clang

