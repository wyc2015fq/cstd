# 面试点：C语言经典面试题(C语言面试题分析之五) - 2019跟着小虎玩着去软考 - CSDN博客
2011年05月24日 14:18:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：3018
**面试点：****C****语言经典面试题**
**C****语言面试题分析之五**
# 一.关于C语言指针的经典面试题
a) 一个整型数（An integer）
b) 一个指向整型数的指针（A pointer to an integer）
c) 一个指向指针的指针，它指向的指针是指向一个整型数的（A pointer to a pointer
to an integer）
d) 一个有10个整型数的数组（An array of 10 integers）
e) 一个有10个指针的数组，该指针是指向一个整型数的（An array of 10 pointers to
integers）
f) 一个指向有10个整型数数组的指针（A pointer to an array of 10 integers）
g) 一个指向函数的指针，该函数有一个整型参数并返回一个整型数（A pointer to a fu
nction that takes an integer as an argument and returns an integer）
h) 一个有10个指针的数组，该指针指向一个函数，该函数有一个整型参数并返回一个整型
数（ An array of ten pointers to functions that take an integer argument and r
eturn an integer ）
答案是：
a) int a; // An integer
b) int *a; // A pointer to an integer
c) int **a; // A pointer to a pointer to an integer
d) int a[10]; // An array of 10 integers
e) int *a[10]; // An array of 10 pointers to integers
f) int (*a)[10]; // A pointer to an array of 10 integers
g) int (*a)(int); // A pointer to a function a that takes an integer argument
and returns an integer
h) int (*a[10])(int); // An array of 10 pointers to functions that take an int
eger argument and return an integer
