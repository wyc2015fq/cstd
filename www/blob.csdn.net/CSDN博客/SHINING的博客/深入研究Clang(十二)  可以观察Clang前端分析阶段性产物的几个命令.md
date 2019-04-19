# 深入研究Clang(十二)    可以观察Clang前端分析阶段性产物的几个命令 - SHINING的博客 - CSDN博客
置顶2018年07月12日 13:56:13[snsn1984](https://me.csdn.net/snsn1984)阅读数：633
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
Clang作为整个LLVM框架下关于C、C++和Object-C的前端。其大致环节分为词法分析、预处理、语法分析、语义分析、中间代码生成几个环节。我们在阅读和修改Clang的代码的过程中，需要经常的调试代码，观察改动所带来的各个阶段所发生的改变，这就需要时刻了解各个阶段的输出。
要了解各个阶段的输出，目前常用有两种方法：
1、最朴实最实用但是效率不一定是最高的办法，那就是打log。这是毋庸置疑的。
2、还有一种办法，相对简洁，但是不一定能体现调试过程中的每一次变动，但是大的方面还是可以展现的。这就是直接使用clang的命令行，辅助以参数，来直接dump出各个阶段的产物。这种方法适合阅读代码和理解代码，也适合一部分的代码调试。
下面就第二种方法做简要介绍：
1、词法分析
待分析代码：
```cpp
min.c
int min(int a, int b) {
  if (a < b)
    return a;
  return b;
}
```
命令行输入：clang -cc1 -dump-tokens min.c
输出：
```cpp
int 'int'	 [StartOfLine]	Loc=<../min.c:1:1>
identifier 'min'	 [LeadingSpace]	Loc=<../min.c:1:5>
l_paren '('		Loc=<../min.c:1:8>
int 'int'		Loc=<../min.c:1:9>
identifier 'a'	 [LeadingSpace]	Loc=<../min.c:1:13>
comma ','		Loc=<../min.c:1:14>
int 'int'	 [LeadingSpace]	Loc=<../min.c:1:16>
identifier 'b'	 [LeadingSpace]	Loc=<../min.c:1:20>
r_paren ')'		Loc=<../min.c:1:21>
l_brace '{'	 [LeadingSpace]	Loc=<../min.c:1:23>
if 'if'	 [StartOfLine] [LeadingSpace]	Loc=<../min.c:2:3>
l_paren '('	 [LeadingSpace]	Loc=<../min.c:2:6>
identifier 'a'		Loc=<../min.c:2:7>
less '<'	 [LeadingSpace]	Loc=<../min.c:2:9>
identifier 'b'	 [LeadingSpace]	Loc=<../min.c:2:11>
r_paren ')'		Loc=<../min.c:2:12>
return 'return'	 [StartOfLine] [LeadingSpace]	Loc=<../min.c:3:5>
identifier 'a'	 [LeadingSpace]	Loc=<../min.c:3:12>
semi ';'		Loc=<../min.c:3:13>
return 'return'	 [StartOfLine] [LeadingSpace]	Loc=<../min.c:4:3>
identifier 'b'	 [LeadingSpace]	Loc=<../min.c:4:10>
semi ';'		Loc=<../min.c:4:11>
r_brace '}'	 [StartOfLine]	Loc=<../min.c:5:1>
eof ''		Loc=<../min.c:5:2>
```
2、预处理
待分析代码：
```cpp
pp.c
#define EXIT_SUCCESS 0
int main() {
  return EXIT_SUCCESS;
}
```
命令行输入：clang -E pp.c -o pp2.c && cat pp2.c
输出：
```cpp
# 1 "pp.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 349 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "pp.c" 2
int main() {
  return 0;
}
```
3、语法分析
待分析代码  同上 min.c
命令行输入：clang -fsyntax-only -Xclang -ast-dump min.c
输出：
```cpp
TranslationUnitDecl 0x55eaff416168 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x55eaff4166e0 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
| `-BuiltinType 0x55eaff416400 '__int128'
|-TypedefDecl 0x55eaff416750 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0x55eaff416420 'unsigned __int128'
|-TypedefDecl 0x55eaff416a28 <<invalid sloc>> <invalid sloc> implicit __NSConstantString 'struct __NSConstantString_tag'
| `-RecordType 0x55eaff416830 'struct __NSConstantString_tag'
|   `-Record 0x55eaff4167a8 '__NSConstantString_tag'
|-TypedefDecl 0x55eaff416ac0 <<invalid sloc>> <invalid sloc> implicit __builtin_ms_va_list 'char *'
| `-PointerType 0x55eaff416a80 'char *'
|   `-BuiltinType 0x55eaff416200 'char'
|-TypedefDecl 0x55eaff416d88 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list 'struct __va_list_tag [1]'
| `-ConstantArrayType 0x55eaff416d30 'struct __va_list_tag [1]' 1 
|   `-RecordType 0x55eaff416ba0 'struct __va_list_tag'
|     `-Record 0x55eaff416b18 '__va_list_tag'
`-FunctionDecl 0x55eaff470258 <../min.c:1:1, line:5:1> line:1:5 min 'int (int, int)'
  |-ParmVarDecl 0x55eaff416df8 <col:9, col:13> col:13 used a 'int'
  |-ParmVarDecl 0x55eaff470180 <col:16, col:20> col:20 used b 'int'
  `-CompoundStmt 0x55eaff4704d8 <col:23, line:5:1>
    |-IfStmt 0x55eaff470448 <line:2:3, line:3:12>
    | |-<<<NULL>>>
    | |-<<<NULL>>>
    | |-BinaryOperator 0x55eaff4703c8 <line:2:7, col:11> 'int' '<'
    | | |-ImplicitCastExpr 0x55eaff470398 <col:7> 'int' <LValueToRValue>
    | | | `-DeclRefExpr 0x55eaff470348 <col:7> 'int' lvalue ParmVar 0x55eaff416df8 'a' 'int'
    | | `-ImplicitCastExpr 0x55eaff4703b0 <col:11> 'int' <LValueToRValue>
    | |   `-DeclRefExpr 0x55eaff470370 <col:11> 'int' lvalue ParmVar 0x55eaff470180 'b' 'int'
    | |-ReturnStmt 0x55eaff470430 <line:3:5, col:12>
    | | `-ImplicitCastExpr 0x55eaff470418 <col:12> 'int' <LValueToRValue>
    | |   `-DeclRefExpr 0x55eaff4703f0 <col:12> 'int' lvalue ParmVar 0x55eaff416df8 'a' 'int'
    | `-<<<NULL>>>
    `-ReturnStmt 0x55eaff4704c0 <line:4:3, col:10>
      `-ImplicitCastExpr 0x55eaff4704a8 <col:10> 'int' <LValueToRValue>
        `-DeclRefExpr 0x55eaff470480 <col:10> 'int' lvalue ParmVar 0x55eaff470180 'b' 'int'
```
备注：clang -cc1 -fsyntax-only -ast-dump min.c   也可以输出语法分析后的ast信息，同上面结果一样。不同的是，上面的结果在命令行输出是可以彩色显示的，而这里，是黑白现实的。
4、语义分析
待分析代码  同上 min.c
命令行输入：clang -fsyntax-only -Xclang -print-decl-contexts min.c
输出：
```cpp
[translation unit] 0x55921ed91188
        <typedef> __int128_t
        <typedef> __uint128_t
        <typedef> __NSConstantString
        <typedef> __builtin_ms_va_list
        <typedef> __builtin_va_list
        [function] min(a, b)
            <parameter> a
            <parameter> b
```
5、代码生成
待分析代码  同上 min.c
命令行输入：clang min.c -emint-llvm -S -c -o min.ll && cat min.ll
输出：
```cpp
; ModuleID = '../min.c'
source_filename = "../min.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"
; Function Attrs: noinline nounwind optnone uwtable
define i32 @min(i32, i32) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, i32* %4, align 4
  store i32 %1, i32* %5, align 4
  %6 = load i32, i32* %4, align 4
  %7 = load i32, i32* %5, align 4
  %8 = icmp slt i32 %6, %7
  br i1 %8, label %9, label %11
; <label>:9:                                      ; preds = %2
  %10 = load i32, i32* %4, align 4
  store i32 %10, i32* %3, align 4
  br label %13
; <label>:11:                                     ; preds = %2
  %12 = load i32, i32* %5, align 4
  store i32 %12, i32* %3, align 4
  br label %13
; <label>:13:                                     ; preds = %11, %9
  %14 = load i32, i32* %3, align 4
  ret i32 %14
}
attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
!llvm.module.flags = !{!0}
!llvm.ident = !{!1}
!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
```
参考文献：《Getting Started with LLVM Core Libraries》
