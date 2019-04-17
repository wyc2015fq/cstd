# QString乱谈(2) - DoubleLi - 博客园





- 长期以来，很多人都清楚，一旦C++源码中直接使用了中文，这样的源码想要跨平台(I18N)会非常困难。

随着：
- Windows下：MSVC2010成为主流
- Linux下：GCC升级到4.6

C++中的中文问题 才算有了一个比较优雅的、跨平台的Workaround。

(本文讨论编译器范围：GCC4.6+, MSVC2010sp1+ 。本文属于QString系列，但暂不涉及QString)

## C++ 中文问题

要在C++中正确使用中文，必须要了解下面两个概念：

|源码字符集(the source character set)|源码文件是使用何种编码保存的|
|----|----|
|执行字符集(the execution character set)|可执行程序内保存的是何种编码(程序执行时内存中字符串编码)|


C++98的问题： **既没有规定源码字符集，也没有规定执行字符集**

这个... 如何理解？不妨看个例子

### 例子

这个要求高么？
- 一个简单的C++程序，只是希望它能在简体中文Windows、正体中文Windows、英文版Windows、Linux、MAC OS...下的结果一致。


//main.cpp
int main()
{
    char mystr[] = "老老实实的学问，来不得半点马虎";
    return sizeof mystr;
}
可以试着反问自己两个问题
- 这个源码文件是何种编码保存的？(有确定答案么？)
- mystr中是什么内容？(有确定答案么？)

对C++来说，这两个都不确定。
- 固定平台的话，还能忍忍
- 要跨平台的话，这种东西...

### GCC

在GCC下，这两个都可以使用你自己喜好的编码(如果不指定，默认都是UTF8)
-finput-charset=charset
-fexec-charset=charset
除了前两个选项外，还有一个：
-fwide-exec-charset=charset
wide? 不妨先猜一下它是干嘛的

### MSVC

MSVC没有类似前面的选项。

|源码字符集如何解决？|有BOM么，有则按BOM解释，无则使用本地Locale字符集(随系统设置而变)|
|----|----|
|执行字符集如何解决?|使用本地Locale字符集(随系统设置而变)|


挺霸道哈（当然，源码中可以使用#pragma setlocale("...")，但功能很有限，比如Windows没有utf8的locale，所以...）。

另外，和GCC对应的wide-exec-charset呢？

|宽执行字符集如何解决?|不妨先考虑一下|
|----|----|


### 怎么办?

这才两个编译器，看起来就这么复杂了。而C++编译器的数目远大于2.

要想跨平台，必须确保这两个字符集都是“确定”的，而能胜任该任务的字符集，似乎理想的也只能是...

## UTF-8方案
- 
如果我们将源码保存成utf8，执行字符集也选为utf8，那么天下将太平了。使用非ASCII字符的源码文件也就可以在不同国家的用户间无障碍流通了 ;-).


源码保存成UTF-8没有什么困难，但是，**执行字符集需要是UTF-8**。没那么简单

对GCC来说，这个问题很简单(默认的编码选项足够了)：
- 只要源码文件保存成utf8即可(带或不带BOM均可)
- 早期的gcc不接收带BOM的utf8源码文件，现在，至少在GCC4.6中，这一限制不再存在。

对MSVC来说，这个问题异常复杂：
- 对MSVC2003来说，只要源码保存成不带BOM的utf8即可
- 对MSVC2005、(没在SP1基础上装热补丁的)MSVC2008来说。完全没办法
- 直到MSVC2010sp1，才算提供了一个解决方案。源码保存成带BOM的utf8，utf16，...，然后添加


#pragma execution_character_set("utf-8")
要想跨GCC4.6+和MSVC2010sp1+，我们需要取它们的交集：也就是
- 源码保存成带BOM的utf8
- 
为MSVC添加#pragma



//main.cpp
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
int main()
{
    char mystr[] = "老老实实的学问，来不得半点马虎";
    return sizeof mystr;
}
### C++11

等到MSVC支持C++11的String Literals之时，我们就没必要用那个蹩脚的pragma了，直接
    char mystr[] = u8"老老实实的学问，来不得半点马虎";
即可(尽管现在在GCC下没问题，但要跨平台，估计要等到Visual C++ 12了)。

## 有个问题？

C++98中不是有个wchar_t么，它不是用来表示unicode字符的么？

Unicode 4.0标准的5.2节是如何说的：


The width of wchar_t is compiler-specific and can be as small as 8 bits. Consequently, programs that need to be portable across any C or C++ compilershould not use wchar_t for storing Unicode text. The wchar_t type is intended forstoring compiler-defined wide characters, which may be Unicode characters in some compilers.
在回头看看GCC的选项
-fwide-exec-charset=charset
尽管GCC为其提供的默认编码是UTF16或UTF32(取决于wchar_t的宽度)，但该编码是可以随意设置的。

尽管这个东西不保证跨平台，也很不好玩， 但是，由于在windows下面wchar_t用来表示utf16字符，而且直接对应系统API接口，所以在类型char16_t普及之前，还是很重要的。

## C++11执行字符集

前面提到的u8就是C++11为“执行字符集”所做的努力之一。

新明确规定了utf8、utf16和utf32这3种执行字符集。

|char*|u8"中文"|
|----|----|
|char16_t*|u"中文"|
|char32_t*|U"中文"|


可是C++11并没有规定源码字符集
const char* mystr=u8"中文";
C++标准对编译器说，我不管这个文件的具体编码是什么，但你必须给我生成对应utf8编码的字节流。

编译器似乎有点傻了吧？不知道源文件的编码，我如何转换

于是：

MSVC说：源码文件必须有BOM，不然我就认为你是本地locale的编码

GCC说：我认为你就是utf8编码，除非通过命令行通知我其他编码

在C++11标准下，对源码编码 简单的处理办法还是，使用带BOM的UTF8保存。

## 参考
- 
[http://gcc.gnu.org/onlinedocs/cpp/Character-sets.html#Character-sets](http://gcc.gnu.org/onlinedocs/cpp/Character-sets.html#Character-sets)

- 
[http://support.microsoft.com/kb/980263](http://support.microsoft.com/kb/980263)

- from:http://blog.csdn.net/dbzhang800/article/details/7540905#comments









