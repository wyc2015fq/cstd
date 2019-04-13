
# Windows数据类型 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:29:52[seven-soft](https://me.csdn.net/softn)阅读数：300


Windows使用`typedef`或`#define`定了很多新的数据类型，前面几节中我们已经领略到了。它们虽然多，但是都有规律可循，很多都是对C/C++中数据类型的简单加工，而且很容易“见名知意”。要想学习Windows编程，必须要了解常用的数据类型。
如果你对C语言或者C++的数据类型比较熟悉的话，那么对于一些基础的内容这个过程就是慢慢熟悉的过程。
这些数据类型在windows.h头文件中定义：typedefintINT;/* 整形 */
typedefunsignedintUINT;/* 无符号整形 */
typedefunsignedint*PUINT;/* 无符号整形指针 */
typedefintBOOL;/* 布尔类型 */
typedefunsignedcharBYTE;/* 字节 */
typedefunsignedshortWORD;/* WORD (无符号短整型) */
typedefunsignedlongDWORD;/* DOUBLE WORD (无符号长整形)*/
typedeffloatFLOAT;/* 浮点型 */
typedefFLOAT*PFLOAT;/* 指向float类型指针 */
typedefBOOLnear*PBOOL;/* 指向布尔类型指针 */
typedefBOOLfar*LPBOOL;
typedefBYTEnear*PBYTE;/* 指向字节类型指针 */
typedefBYTEfar*LPBYTE;
typedefintnear*PINT;/* 整形指针 */
typedefintfar*LPINT;
typedefWORDnear*PWORD;/* 指向WORD类型的指针 */
typedefWORDfar*LPWORD;
typedeflongfar*LPLONG;/* 指向长整形的指针 */
typedefDWORDnear*PDWORD;/* 指向DWORD类型的指针 */
typedefDWORDfar*LPDWORD;
typedefvoidfar*LPVOID;/* 指向void类型的指针 */
typedefCONSTvoidfar*LPCVOID;/* 指向void类型的常指针 */

简单说下，大部分类型都没有什么离奇的地方，可能很多读者会对那个 far 和 near 觉得有疑问，实际上 F12 查看定义会发现他们就定义上方：\#definefar
\#definenear

看到这里你是否还是有疑问？其实，这个 far 和 near 只是用来标识变量的新旧的（预处理阶段 far 和 near 会被替换成空字符串）。 例如PINT和LPINT实际上都是int
 *，只不过一个是老式写法，一个是新式写法，这都是为了兼容问题。
简单的看下这些数据类型，就可以总结出：但凡是以 “P” 开头的都是指针（pointer）类型（"LP"是老式写法）。撇开这些不谈，那么实际上这些 Windows API 常用的一些数据结构跟我们原本所学的 C/C++
 变量差别就是一个typedef而已，基础好的熟悉一下就行了。不过，也不要想得这么简单，Windows
 编程还有一些比较复杂的类型比如 HWND、HANDLE 等。
在碰到不熟悉的类型时请熟练的使用 “右键转到定义” 或者 F12。大家不用太过害怕这些复杂的类型名称，Windows
 数据类型并不是内建的数据类型类型，而都是从C类型重定义得到的。
Windows 数据类型名命名的规律无符号类型：一般是以“U”开头，比如“INT”对应的“UINT”。
指针类型：其指向的数据类型前加“LP”或“P”，比如指向 DWORD 的指针类型为“LPDWORD”和“PDWORD”。
句柄类型：以“H”开头。比如，HWND 是window（WND简写）也就是窗口的句柄，菜单(MENU)类型对应的句柄类型为 “HMENU” 等等。

