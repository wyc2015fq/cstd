# Deep Water: printf float in int type - game3108的专栏 - CSDN博客
2016年11月30日 22:32:57[game3108](https://me.csdn.net/game3108)阅读数：809
个人分类：[转载																[iOS](https://blog.csdn.net/game3108/article/category/2844789)](https://blog.csdn.net/game3108/article/category/2926393)
转载地址：http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html
这片文章是与一个朋友聊天，聊起了一个问题，然后他研究完写了一篇文章，写的非常好，转载过来，记录一下。
Abstract
For a programmer, as a user of the interface `printf` in
 C language, he or she should assure the string specifier matches the types of the variables or the result isundefined. In fact, the result from unmatched types may be defined from the prospective of the implementor
 of this interface.
Table of Contents
- [Background](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#background)
- [Solving](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#solving)
- [Conclusions](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#conclusions)
- [Take-away Tips](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#take-away-tips)
## [Background](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#id1)
Last week, a friend of mine showed me an Obj-C code snippet and we want to figure out what is the output exactly.
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2cb0d8d0b2c348c8b29d752c12fca79c-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2cb0d8d0b2c348c8b29d752c12fca79c-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2cb0d8d0b2c348c8b29d752c12fca79c-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2cb0d8d0b2c348c8b29d752c12fca79c-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2cb0d8d0b2c348c8b29d752c12fca79c-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2cb0d8d0b2c348c8b29d752c12fca79c-6)|```float price = 1.5;NSLog(@"%f\n", price);NSLog(@"%d\n", (int)price);NSLog(@"%d\n", price);NSLog(@"%d\n", (int *)&price);NSLog(@"%d\n", *(int *)&price);```|
The output from the Xcode IDE is as follows, running on a iPhone 6s simulator :
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-6)[7](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-7)[8](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-8)[9](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-9)[10](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-10)[11](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-11)[12](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-12)[13](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-13)[14](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-14)[15](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-15)[16](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-16)[17](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-17)[18](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-18)[19](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-19)[20](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2f6878818f1545a3b32b9169a22b8ea7-20)|// first run1.500000        // %f <- price1               // %d <- (int)price-209625088      // %d <- price1431444172      // %d <- (int *)&price1069547520      // %d <- *(int *)&price// second run1.500000        // %f <- price1               // %d <- (int)price1518649344      // %d <- price1430264524      // %d <- (int *)&price1069547520      // %d <- *(int *)&price// third run1.500000        // %f <- price1               // %d <- (int)price-1056837632     // %d <- price1457400524      // %d <- (int *)&price1069547520      // %d <- *(int *)&price|
Note that after running three times, some numbers in the outputs keep the same while others change every time. Especially, the outputs by printing `price` of `float` type
 in `int`type without type casting is indeterminate
 at the first sight.
This Obj-C code snippet seems too easy to give a quick answer. To verify my first thought, I just quickly translate them into C language to look them in a lower level, assuming`NSLog` is
 a macro wrapped C `printf` ( Obj-C is a superset of C anyway ~ ). Here is
 the C code snippet:
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-6)[7](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-7)[8](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-8)[9](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-9)[10](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-10)[11](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-11)[12](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-12)[13](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-13)[14](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-14)[15](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-15)[16](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-16)[17](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_db20ad06076e4729bcd52188b5995661-17)|//// sample.c//#include<stdlib.h>#include<stdio.h>intmain(intargc,char**argv){floata=1.5;printf("%specifier casting input\n");printf("%%f             %f\n",a);printf("%%d (int)       %d\n",(int)a);printf("%%d             %d\n",a);printf("%%d (int *)     %d\n",(int*)&a);printf("%%d *(int *)    %d\n",*(int*)&a);return;}|
The output is as follows. Note the line of printing `%d` without
 casting, it seems unpredictable yet:
// compile sample.c$ gcc -g -o sample sample.c
// run for once$ ./sample
specifier casting input%f             1.500000
%d (int)       1
%d             2147483630
%d (int *)     -493699412
%d *(int *)    1095237632
## [Solving](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#id2)
The output astonished and puzzled me instantly AND for the week. The question haunted around: is it true that the output of printf is undefined?
To study further and thoroughly, I decide to check a concise snippet focusing on the 
```
printf("%d\n",
 price)
```
. In my opinion, this is key point to demystify the hood.
Before we begin to check the code, it is necessary to make clear under which environment the programs will run. Actually the x86-64 Linux OS and gcc environment are on a Debian virtual guest.
// programming environment$ uname -a
Linux debian 3.16.0-4-amd64 #1 SMP Debian 3.16.36-1+deb8u2 (2016-10-19) x86_64 GNU/Linux$ gcc --version
gcc (Debian 4.9.2-10) 4.9.2Copyright (C) 2014 Free Software Foundation, Inc.This is free software; see the source for copying conditions.  There is NOwarranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
The first C snippet code here is used to show the sizes of different data types.
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-6)[7](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-7)[8](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-8)[9](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-9)[10](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-10)[11](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-11)[12](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-12)[13](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_96dc544dd8b34f209a5fe1d8c8a39c2c-13)|//// size.c//#include<stdlib.h>#include<stdio.h>intmain(intargc,char**argv){printf("sizoef(int) is %zu\n",sizeof(int));printf("sizoef(long) is %zu\n",sizeof(long));printf("sizoef(float) is %zu\n",sizeof(float));printf("sizoef(double) is %zu\n",sizeof(double));return;}|
// compile size.c$ gcc -g -o size size.c
// and run$ ./size
sizoef(int) is 4sizoef(long) is 8sizoef(float) is 4sizoef(double) is 8
The concise C code demonstrating 
```
printf("%d\n",
 price)
```
 and it's output is shown below:
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-6)[7](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-7)[8](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-8)[9](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-9)[10](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-10)[11](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-11)[12](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_2c1ad3d3d67f4874908efbff58da582b-12)|//// printf_int.c//#include<stdlib.h>#include<stdio.h>intmain(intargc,char**argv){floatprice=1.5;printf("%d\n",price);return;}|
// compile printf_int$ gcc -g -o printf_int printf_int.c
// and run for three times$ ./printf_int
-1044129512$ ./printf_int
559638616$ ./printf_int
1742869704
No surprise at all, the output is weird and it changes in every run.
I know a few gotchas that the specifier format string in the `printf` function
 should be matched with the types of the `var_arg` list or the behavior is undefined.
 Now the output makes me to think in deep and hard way why and how the undefined behavior comes.
The typical memory layout for a C program is composed of
- text segment : containing machine instructions
- data segment : initialized data
- bss segment : uninitialized data
- heap : dynamic allocated memory
- stack : memory area for storing variable during function calls
Usually, on Linux running on x86 intel CPU, the stack area starts from higher memory address and expands to lower address when it grows. The heap area starts from the
 top ofbss segment and grows to the bottom of the stack area.
When calling a function, the arguments passed from the caller is stored in the stack if needed (pushed onto the stack). The order for pushing arguments is reverse with that in source code.
 That is to say, the last argument in the source code will be the first pushed. The is true for stackless function calling. For x86-64 programs on Linux, some reigsters are involved storing the arguments.
As many as six arguments should be loaded into respective registers directly without being pushed onto stack. The registers and the arguments are specified by the CPU ABI which is well introduced in [this
 article](http://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64) written by Eli Bendersky. I refer the illustrating image here:
The illustrating image is showing calling a function `myfunc` like:
long myfunc(long a, long b, long c, long d,
        long e, long f, long g, long h)
![x64 frame nonleaf (illustrating)](http://eli.thegreenplace.net/images/2011/08/x64_frame_nonleaf.png)
Fig. x64 frame nonleaf (illustrating) (copied from Eli)
By the x86-64 assembly specification, the first argument is loaded into register %rdi and the second into %rsi. Another important exception for function `printf` is
 that its arguments are examined by the compiler for type checking and type promotion. Thus the type float is promoted to type double. Regarding the statement 
```
printf("%d\n",
 price);
```
, the first argument is the format string and the second argument `price` (float
 number 1.5) is promoted into double. ( [See
 more](http://stackoverflow.com/questions/4264127/correct-format-specifier-for-double-in-printf) about type promote).
If this is true, the output should be the content interpreted as type `int` of
 the `price` . Unfortunately, it is not. The binary representation of double 
```
price
 = 1.5
```
 is
// 64-bit binary representation of double 1.5
// as hex
0x3FF8000000000000
// as binary
00111111 11111000 00000000 00000000
00000000 00000000 00000000 00000000
If you do verify yourself, neither the upper 32 bits or lower 32 bits matches the %d output if interpreted as an `int`.
Things get more complicated. Thanks to Eli Bendersky again, the article referenced above also indicates that float arguments are stored into xmm registers while only arguments of integer type
 or pointer are handled by the common six registers. This gives a clue to examine the xmm0 register, the first xmm register. To examine registers, I have to use the powerful debugging tool `gdb`.
Tip
- In order to debug with `gdb`, the executable should be compiled
 with the `-g` option of `gcc`.
- The gdb command `n` (line 22 and line 25) is short for `next`,
 which is to execute the next step indicated by latest output statement (line 19 and line 23 respectively).
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-6)[7](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-7)[8](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-8)[9](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-9)[10](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-10)[11](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-11)[12](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-12)[13](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-13)[14](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-14)[15](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-15)[16](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-16)[17](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-17)[18](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-18)[19](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-19)[20](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_44863bc794d148088f857ad047705c06-20)|// compile printf_int.c with option -g$ gcc -g -o printf_int printf_int.c// run with gdb$ gdb printf_int(gdb) startTemporary breakpoint 1 at 0x400515: file d.c, line 5.Temporary breakpoint 1, main (argc=1, argv=0x7fffffffdf78) at d.c:55       float a = 1.5;(gdb) display $esi1: $esi = -8328(gdb) n6       printf("%d\n", a);1: $esi = -8328(gdb) n-83287       return 0;1: $esi = 2147483642(gdb)|
Line 26 is the output integer -8328 of the `printf` function.
 Pay close attention to line 20: the int value -8328 represented by the lower 32 bits(%esi) of the %rsi register. To learn more about the x86-64 registers, follow [this
 link](http://stackoverflow.com/questions/20637569/assembly-registers-in-64-bit-architecture)
What a coincident! or is it deterministic? Yes, it is and deterministic and defined. I will explain soon.
Recall that the %rsi register holds the second argument of integer or pointer(address), but of which function? Here it is the `main` function!
 If you look closely at Line 18:
Temporary breakpoint 1, main (argc=1, argv=0x7fffffffdf78) at printf_int.c:5
The argument `argv` is the second
 argument and its content is an 64-bit address `0x7fffffffdf78` . It is a
 pointer, so it's content is hold by register %rsi.
I use python to manipulate numbers. If we convert the lower 32 bits of this address into integer.
```python
# address hex
ffffdf78
# binary representation of the address
0b10000010001000
# interpret the binary address as integer
-   // -0b10000010001000
```
Amazing!!!
Now we explain the these two statements.
floatprice=1.5;printf("%d\n",price);
When we want to print it with a `%d` format
 specifier, the compiler does in such steps:
- Parsing the arguments. There are two. The first is pointer to the format string, so it(as address) is loaded into %rdi register; The second is `price` of
 float type(but promoted to double), so it( `1.5` )
 is loaded into %xmm0 register and the content of the %rsi register remains unchanged. Here is the black magic.
- When `printf` is called, it parse the specifier
 format string to determine the value type will be printed. Here the %d specifier is first encountered during the parsing, so the `printf` considers
 it to be an integer(as the %d indicates). The `printf` then
 fetches the value from the %rsi register and prints the content as integer.
- The %rsi register is not alerted by `printf` here,
 so the output is not determined by the call of `printf("%d\n", price);`.
 It is determined by the last call which changes the%rsi register.
In general, we can summary:
- Type promoting is checked first. Type `char` is promoted into `int`,
 type `float` is promtped into `double` and
 so on.
- For the arguments of integer type or pointer, as many as six should be loaded into specified registers( 
```
%rdi
 %rsi %rdx %rcx %r8 %r9
```
 ), floating(single or double) arguments are loaded into `xmm` registers,
 and others should be pushed onto the stack frame.
- For the arguments of double type, they are loaded into %xmm registers which is designed for holding float numbers.
- Before `printf` function is called, the arguments is stored according
 their declared types (after type promoting if needed); When executing, the value is fetched according to the format specifier string.
Let's verify these conclusions. Here we add a function `sum` which
 expects two `int` arguments. By taking in two integer, calling the `sum` function
 would make a side effect: the%rsi register will be loaded with the second argument and remains intact by a following call `printf`.
 The following `printf` function will take the integer value according to
 the `%d`specifier from the %rsi register set by the
 previous `sum`. We can expect that the output of the `printf` should
 be determined by the second integer argument by the previous `sum`function
 and it varies with this second int argument.
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-6)[7](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-7)[8](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-8)[9](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-9)[10](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-10)[11](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-11)[12](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-12)[13](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-13)[14](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-14)[15](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-15)[16](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-16)[17](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-17)[18](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-18)[19](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-19)[20](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-20)[21](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_341228fe506f4da2991d37d47528bdd6-21)|//// printf_int_sum.c//#include<stdlib.h>#include<stdio.h>voidsum(inta,intb);// declaring the function prototypeintmain(intargc,char**argv){floatprice=1.5;sum(,);printf("%%d price: %d\n",price);return;}// function implementationvoidsum(inta,intb){// actually we can do nothing...intc=a+b;}|
Here is the output
// sum(2, 5)// compile$ gcc -g -o printf_int_sum printf_int_sum.c
// run for three times$ ./printf_int_sum
%d price: 5
$ ./printf_int_sum
%d price: 5
$ ./printf_int_sum
%d price: 5
// after we change to sum(2, 7)// compile$ gcc -g -o printf_int_sum printf_int_sum.c
// run$ ./printf_int_sum
%d price: 7
$ ./printf_int_sum
%d price: 7
$ ./printf_int_sum
%d price: 7
Observing the above outputs, when we call 
```
sum(2,
 5);
```
, `price` is printed as 5; When we call 
```
sum(2,
 7);
```
, `price` is printed as 7. The behavior is
 what we expect: the first integer or pointer argument of `sum` determines
 the output of `price`. It is defined! Hooray!
Until now, we have figured out how to expect the output of the `printf`.
 One mystery is still remained that the result `sample.c` program changes
 in every run. According to the conclusions, the output should be determined by the first integer or pointer argument of `main` function,
 I add a line in the file printf_int_argv.c to print the `argv` which
 is a pointer. This program `printf_int_argv` runs on a real machine and with `gcb` respectively.
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-6)[7](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-7)[8](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-8)[9](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-9)[10](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-10)[11](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-11)[12](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-12)[13](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_9e19d90fb5fd4b8a919ddbd89c2231cf-13)|//// printf_int_argv.c//#include<stdlib.h>#include<stdio.h>intmain(intargc,char**argv){floatprice=1.5;printf("%%d price: %d\n",price);printf("%%p argv: %p\n",argv);return;}|
The output from running on a real machine is shown as follows:
// run on real machine, with default system settings$ ./printf_int_argv
%d price: 1520235144
%p argv: 0x7ffd5a9cf288
$ ./printf_int_argv
%d price: -1531767544
%p argv: 0x7ffea4b31508
$ ./printf_int_argv
%d price: 1047310888
%p argv: 0x7ffe3e6cb228
The output from running with`gdb` is
 shown as follows:
|[1](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-1)[2](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-2)[3](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-3)[4](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-4)[5](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-5)[6](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-6)[7](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-7)[8](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-8)[9](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-9)[10](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-10)[11](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-11)[12](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-12)[13](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-13)[14](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-14)[15](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-15)[16](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-16)[17](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-17)[18](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-18)[19](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-19)[20](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-20)[21](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-21)[22](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-22)[23](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-23)[24](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-24)[25](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-25)[26](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-26)[27](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-27)[28](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-28)[29](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-29)[30](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-30)[31](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-31)[32](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#rest_code_fb65f5d3db7a4b5ab383363f5712d89b-32)|// run with gdb$ gdb printf_int_argv(gdb) runStarting program: ./printf_int_argv%d price: -6536%p argv: 0x7fffffffdf68[Inferior 1 (process 22351) exited normally](gdb) runStarting program: ./printf_int_argv%d price: -6536%p argv: 0x7fffffffdf68[Inferior 1 (process 22355) exited normally](gdb) run%d price: -6536%p argv: 0x7fffffffdf68[Inferior 1 (process 22356) exited normally](gdb)// another run with gdb to show content of %esi register(gdb) startTemporary breakpoint 1, main (argc=1, argv=0x7fffffffe678) at printf_int_argv.c:55       float price = 1.5;(gdb) n6       printf("%%d price: %d\n", price);(gdb) display $esi1: $esi = -6536(gdb) n%d price: -6536// and so on|
The outputs differ in these two different environments. The output from `gdb` can
 be explained well according to previous conclusions: they are determined by and varies with the value of `argv`.
 See line 22 - 32: the content of %esi is exactly the same the print of `price`.
 The difference is attributed to that `argv` stay unchanged when debugging
 using `gdb`while it varies on real machine.
Why and how `argv` changes?
 I goolge for c why argv address changes every time and get a useful link [Environment
 variable's address is changing?](http://superuser.com/questions/552803/environment-variables-address-is-changing). I get some key concepts:
ASLR
/proc/sys/kernel/randomize_va_space
Continuing to search with these concepts, I get these from [ASLR
 @wikipedia](https://en.wikipedia.org/wiki/Address_space_layout_randomization):
> 
Address space layout randomization (ASLR) is a computer security technique involved in protection from buffer overflow attacks. In order to prevent an attacker from reliably jumping to, for example, a particular exploited function in memory, ASLR randomly arranges
 the address space positions of key data areas of a process, including the base of the executable and the positions of the stack, heap and libraries.
As far as I know, the `argv` list
 is just above the stack, so it should also change in every run. I finally decide to disable ASLR and run the `printf_int_argv` again,
 with the fresh output here:
Warning
You need root permission to disable or enable the ASLR. Here is a guide step by step from the link:
The following values are supported:
0 – No randomization. Everything is static.
1 – Conservative randomization. Shared libraries, stack, mmap(), VDSO and heap are randomized.
2 – Full randomization. In addition to elements listed in the previous point, memory managed through brk() is also randomized.
So, to disable it, run
    echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
and to enable it again, run
    echo 2 | sudo tee /proc/sys/kernel/randomize_va_space
// when the ALSR is disabled$ ./printf_int_argv
%d price: -6504
%p argv: 0x7fffffffe698
$ ./printf_int_argv
%d price: -6504
%p argv: 0x7fffffffe698
$ ./printf_int_argv
%d price: -6504
%p argv: 0x7fffffffe698
Hooray! Hooray! The `argv` stays
 unchanged when the ALSR is disabled, so the output of `printf` as a result
 of interpreting the `argv` as an integer keeps the same in every run now.
Warning
You should enable ASLR after this experiment. Do NOT forget it.
## [Conclusions](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#id3)
For a programmer, as a user of the interface `printf` in
 C language, he or she should assure the string specifier matches the types of the variables or the result is undefined. In fact, the result from unmatched types may be defined as follows from the prospective of the
 implementor of this interface.
Function calls are modeled as stack frames and the arguments passed from the caller are stored according to their types(maybe after undergoing type checking and type promotion). As for x86-64 CPU, the first six
 arguments of integer type or pointer are loaded into respective registers ( 
```
%rdi
 %rsi %rdx %rcx %r8 %r9
```
 ), floating(single or double) arguments are loaded into `xmm` registers,
 and others should be pushed onto the stack frame.
Specifically, when `printf` function
 is called to print something, the first `%d` in the format
 specifier string indicates that it is the content in the register %rsi should be fetched. When trying to print out a float variable `price` in
 a %d format `printf("%d\n", price)`, the %rsi remains
 unchanged by `printf` since the second argument(`price`)
 is NOT ofinteger type or pointer; the %rsi keeps the value set by the last function call with a second argument of that proper type.
If the `printf` function is
 the first to be called in a `int main(int argc, char **argv)` program, the
 content of register %rsi is the the second argument is value of `argv` which
 is a pointer(holding an address). When ASLR is enabled(in a Linux system), the value of `argv` changes
 randomly for enhancing security, so does content of %rsi. That is why the output of 
```
printf("%d\n",
 price)
```
 changes in every run.
## [Take-away Tips](http://www.xiesiyi.com/posts/deep-water-printf-float-in-int-type.html#id4)
- Always use the right format specifier for `printf` function,
 or you will get unexpected results.
- In fact, the output by an unmatched format specifier string is defined in some way if you examine the registers and function call stack. When you understand this magic, you should alway refer to the previous tip ~.
# [simple qrcode chrome extension](http://www.xiesiyi.com/posts/simple-qrcode-chrome-extension.html)
 Siyi Xie 
[2016-08-04
 07:28](http://www.xiesiyi.com/posts/simple-qrcode-chrome-extension.html)
[Comments](http://www.xiesiyi.com/posts/simple-qrcode-chrome-extension.html#disqus_thread)
Polishing the Simple Qrcode
More to do
The work simple-qrcode is so simple that some efforts are needed to polish it. Here I figure them out and will polish the code in the future. Ofcause you could have a try yourself.
- 
It should be avoided to update the QR code image synchronously for it blocks UI.
> 
To do: use Ajax to request the QR code asynchronously. Adding a loading indicator is more user-friendly.
- 
A new request for QR code is sent on every user click even the page does not refresh. This is a waste of extra traffic and undesirable UI updating.
> 
To do: cache a certain amount of requested QR images for sometime.
- 
The request for the QR code does not be sent until the badge(extension icon) is clicked even the page has been loaded for a long time.
> 
To do: pre-fetch the QR code image as soon as possible. DOMContentLoaded should be a better trigger event.
- 
When in poor network condition, the HTTP request for QR code may fail or be interrupted.
> 
To do: use a local js library to generate the QR code image.
- 
The server for returning the QR code may be not responsive. A number of servers could assure more reliable service.
> 
To do: provide an option page for users to customize their own QR code servers or/and emmed a list of servers in the extension.
