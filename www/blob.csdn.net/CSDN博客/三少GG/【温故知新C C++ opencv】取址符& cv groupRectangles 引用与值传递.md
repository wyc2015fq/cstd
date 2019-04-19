# 【温故知新C/C++/opencv】取址符&||cv::groupRectangles||引用与值传递 - 三少GG - CSDN博客
2011年04月19日 12:21:00[三少GG](https://me.csdn.net/scut1135)阅读数：2232
cv::groupRectangles
- void groupRectangles(vector<Rect>&* rectList*, int* groupThreshold*, double* eps=0.2*)[¶](#groupRectangles)
Groups the object candidate rectangles
Parameters:- *rectList* – The input/output vector of rectangles. On output there will be retained and grouped rectangles 
- *groupThreshold* – The minimum possible number of rectangles, minus 1, in a group of rectangles to retain it. 
- *eps* – The relative difference between sides of the rectangles to merge them into a group 
The function is a wrapper for a generic function [partition()](core_clustering.html#partition) . It clusters all the input rectangles using the rectangle equivalence criteria, that combines rectangles that have similar sizes and similar locations (the similarity is defined by eps ). When eps=0 , no clustering is done at all. If ![/texttt{eps}/rightarrow +/inf](_images/math/48a2cec00f9c9296548c65df451c1ba55c191d29.png)  , all the rectangles will be put in one cluster. Then, the small clusters, containing less than or equal to groupThreshold rectangles, will be rejected. In each other cluster the average rectangle will be computed and put into the output rectangle list.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://www.nowamagic.net/datastructures/ds_UsageOfAddressOperator.php](http://www.nowamagic.net/datastructures/ds_UsageOfAddressOperator.php)
# C语言中地址操作符&的使用
#### 示例一
[view source](#viewSource)
[print](#printSource)[?](#about)
|`01`|```cpp#include <stdio.h>```|
|`02`|```cppint``````cppfind(``````cppint``````cpp*a);```|
|`03`|` `|
|`04`|```cppmain()```|
|`05`|```cpp{```|
|`06`|`    ````cppint``````cppb = 1;```|
|`07`|`    ````cppint``````cppwait, test;```|
|`08`|` `|
|`09`|`    ````cpptest = find(&b);```|
|`10`|` `|
|`11`|`    ````cppprintf``````cpp(``````cpp"内存地址为:%d"``````cpp,&test);```|
|`12`|` `|
|`13`|`    ````cppscanf``````cpp(``````cpp"%d"``````cpp, &wait);```|
|`14`|```cpp}```|
|`15`|` `|
|`16`|```cppint``````cppfind(``````cppint``````cpp*a)```|
|`17`|```cpp{```|
|`18`|`    ````cppreturn``````cpp*a;```|
|`19`|```cpp}```|
int b = 1; int test = find(&b); 这样调用时，实际上是进行了：int* a = &b，创建了一个新的指向b的整型指针a作为find函数范围内的局部变量。 
#### 示例二
函数声明：int find(int& a); 函数调用：int b = 1; int n = find(b); 
这样调用时，实际上是进行了：int& a = b，给变量b起了个别名a，也就是说a和b实际上代表的是同一个变量，调用期间没有创建任何新的变量。 
#### 示例三
函数声明：int find(int a); 函数调用：int b = 1; int n = find(b); 
这样调用时，实际上是进行了：int a = b，也就是普通的变量赋值操作，调用期间创建了在find函数范围内的局部变量a，它的值与b相同。
#### C中*、&用法介绍
&在用于计算时就是取变量地址，如int* a = &b，就是将整型变量b的地址取出，赋值给整型指针a，a中的内容就是b的地址，所以a指向b。
*在用于计算时，就是取指针所指向的地址中的值，如int b = *a，就是将整型指针a指向的地址中存放的内容赋值给整型变量b。
C语言中，&符号大家一定很熟悉吧，它除了可以作为按位运算“与”之外还有更常用的功能——取变量地址。
[view source](#viewSource)
[print](#printSource)[?](#about)
|`01`|```cpp#include <stdio.h>```|
|`02`|` `|
|`03`|```cppmain()```|
|`04`|```cpp{```|
|`05`|`    ````cppint``````cppa = 0;```|
|`06`|`    ````cppint``````cppwait;```|
|`07`|` `|
|`08`|`    ````cppint``````cpp*p = &a;```|
|`09`|` `|
|`10`|`    ````cppprintf``````cpp(``````cpp"The value is: %d/n"``````cpp, *p);```|
|`11`|` `|
|`12`|`    ````cppscanf``````cpp(``````cpp"%d"``````cpp, &wait);```|
|`13`|```cpp}```|
上面代码中，指针p指向变量a的地址。在C/C++中，每个变量都有其相应的地址，通过在变量标识符前加&符号即可获得变量的地址。
那么我们这么写可以吗？int *p = &0x01000;
这显然不行。因为对于一个数值常量，它是没有地址的。而变量之所以有地址就是因为要有一个存储单元对变量进行标识（当然，变量也可以直接映射到某个寄存器）。
我们再看下面的代码：
[view source](#viewSource)
[print](#printSource)[?](#about)
|`1`|```cpp#include "stdio.h"```|
|`2`|```cppint``````cppmain(``````cppvoid``````cpp)```|
|`3`|```cpp{```|
|`4`|`    ````cppint``````cppa = 0;``````cpp// &a = 0x0012ff60```|
|`5`|`    ````cppint``````cpp*p = &*(``````cppint``````cpp*)0x0012ff60;```|
|`6`|`    ````cppprintf``````cpp(``````cpp"The value is: %d/n"``````cpp, *p);```|
|`7`|`    ````cppreturn``````cpp0;```|
|`8`|```cpp}```|
上面代码又是怎么回事呢？
先前已经调查过变量a的地址——0x0012ff60，那么这里的指针p实际上也是指向变量a的地址。
首先，将0x0012ff60作为int*，这时它与&a是等价的。
然后*(int*)0x0012ff60表示取变量a的内容。
最后，&*(int*)0x0012ff60表示再脱去*(int*)0x0012ff60的解引用，相当于又变为(int*)&a。
因此，这里的&与第一个例子中的&是不同的语义。这里的&不是取地址，因为一个*(int*)0x0012ff60不是变量，它是没有地址的。每一个变量标识符在编译期间，编译器会为它们创建一个符号表，其中存放着变量标识符相应的各种属性，如类型、地址标识等。地址标识在连接后即可确定逻辑地址值。简而言之，&作为取地址操作，当且仅当&后面跟着的是变量或函数标识符。所以这里的&表示脱去解引用。
由此我们可以得出：&作为取地址操作时，其行为结果是在编译时就被确定的；而*，解引用操作（或取内容）操作，其行为结果只能在运行时才可被确定。
再看下面的例子，加深印象：
[view source](#viewSource)
[print](#printSource)[?](#about)
|`1`|```cpp#include "stdio.h"```|
|`2`|` `|
|`3`|```cppint``````cppmain(``````cppvoid``````cpp)```|
|`4`|```cpp{```|
|`5`|`    ````cppint``````cppa = 0;```|
|`6`|`    ````cppint``````cpp*p = &*&*&a;```|
|`7`|`    ````cppprintf``````cpp(``````cpp"The value is: %d/n"``````cpp, *p);```|
|`8`|`    ````cppreturn``````cpp0;```|
|`9`|```cpp}```|
**在C++中，&还可以表示引用，这个就不多说了。**
[view source](#viewSource)
[print](#printSource)[?](#about)
|`01`|```cpp#include "iostream"```|
|`02`|```cppusing``````cppnamespace``````cppstd;```|
|`03`|` `|
|`04`|```cppint``````cppmain(``````cppvoid``````cpp)```|
|`05`|```cpp{```|
|`06`|`    ````cppint``````cppa = 0;```|
|`07`|`    ````cppint``````cpp&r = a;```|
|`08`|`    ````cppcout <<``````cpp"The value is: "``````cpp<< r << endl;```|
|`09`|`    ````cppreturn``````cpp0;```|
|`10`|```cpp}```|
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://book.51cto.com/art/201009/224192.htm](http://book.51cto.com/art/201009/224192.htm)
【答案】
指针是一个实体，而引用仅是个别名；
引用使用时无需解引用(*)，指针需要解引用；
引用只能在定义时被初始化一次，之后不可变；指针可变；
引用没有 const，指针有const；
引用不能为空，指针可以为空；
"sizeof引用"得到的是所指向的变量(对象)的大小，而"sizeof指针"得到的是指针本身（所指向的变量或对象的地址）的大小；
和引用的自增（++）运算意义不一样；
在内存分配上，程序为指针变量分配内存区域，而引用不需要分配内存区域。
