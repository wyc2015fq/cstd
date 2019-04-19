# Symbol Overloading in C - Koma Hub - CSDN博客
2019年01月31日 21:39:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：33
个人分类：[ANSI C](https://blog.csdn.net/Rong_Toa/article/category/8653945)
|Symbol|Meaning|
|----|----|
|static|Inside a function, retains its value between calls			At the function level, visible only in this file|
|extern|Applied to a function definition, has global scope (and is redundant)			Applied to a variable, defined elsewhere|
|void|As the return type of a function, doesn't return a value			In a pointer declaration, the type of a generic pointer			In a parameter list, takes no parameters|
|*|The multiplication operator			Applied to a pointer, indirection			In a declaration, a pointer|
|&|Bitwise AND operator			Address-of operator|
|=|Assignment operator|
|==|Comparison operator|
|<=|Less-than-or-equal-to operator|
|<<=|Compound shift-left assignment operator|
|<|Less-than operator|
|<|Left delimiter in #include directive|
|()|Enclose formal parameters in a function definition			Make a function call			Provide expression precedence			Convert (cast) a value to a different type			Define a macro with arguments			Make a macro call with arguments			Enclose the operand of the sizeof operator when it is a typename|
