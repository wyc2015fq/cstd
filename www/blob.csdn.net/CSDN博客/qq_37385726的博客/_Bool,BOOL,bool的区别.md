# _Bool,BOOL,bool的区别~ - qq_37385726的博客 - CSDN博客





2017年03月01日 20:35:47[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：267
个人分类：[C语言](https://blog.csdn.net/qq_37385726/article/category/6697679)









**【 bool、BOOL 和 _Bool 的区别 】**

**bool 类型**在 C++ 中以关键字的形式被支持，表示布尔类型，其对应变量的值只有真（true）和假（false）两种值。

BOOL类型 ·字节长度在头文件<windef.h> 中定义为 typedef int BOOL；在头文件 <wtypes.h> 中定义为 typedef long BOOL；


BOOL 类型的长度视实际环境来定，一般可认为是 4 个字节。

**·取值范围**BOOL 是微软定义的表达布尔逻辑的类型。与 C++ 中的 bool 类型不同是，它是一个三值逻辑：TRUE、FALSE 和 ERROR。当返回值为大于 0 的整数时为 TRUE，返回值为 0 时为 FALSE，返回值为 -1 时为 ERROR。

**_Bool**是 C99 标准中定义的一个新关键字，以提供布尔类型。C2008 草案中只规定了 _Bool 类型的大小至少应能够存放 0 和 1 这两个值。而并没有规定具体的大小。这交给编译器自由发挥了。需要包括头文件stdbool.h。

**指的注意的是，这个头文件里面的宏定义是这样的 #define bool _Bool,所以如果要使用的话，就直接用bool而不是用_Bool~~**

==============================================================



### 我们自己定义的“仿布尔型”




        在C99标准被支持之前，我们常常自己模仿定义布尔型，方式有很多种，常见的有下面两种：



**[cpp]**[view
 plain](http://blog.csdn.net/daheiantian/article/details/6241893#)[copy](http://blog.csdn.net/daheiantian/article/details/6241893#)

[print](http://blog.csdn.net/daheiantian/article/details/6241893#)[?](http://blog.csdn.net/daheiantian/article/details/6241893#)

- /* 第一种方法 */
- #define TRUE 1
- #define FALSE 0
- 
- 
- /* 第二种方法 */
- enumbool{false, true};
   








