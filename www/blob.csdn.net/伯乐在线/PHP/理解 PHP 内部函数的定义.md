# 理解 PHP 内部函数的定义 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [hoohack](http://www.jobbole.com/members/hector) 翻译。未经许可，禁止转载！
英文出处：[nikic](https://nikic.github.io/2012/03/16/Understanding-PHPs-internal-function-definitions.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
欢迎来到”给PHP开发者的PHP源码”系列的第二部分。
在上一篇中，ircmaxell说明了你可以在哪里找到PHP的源码，它的基本目录结构以及简单地介绍了一些C语言（因为PHP是用C语言来写的）。如果你错过了那篇文章，在你开始读这篇文章之前也许你应该读一下它。
在这篇文章中，我们谈论的是定位PHP内部函数的定义，以及理解它们的原理。
## 如何找到函数的定义
作为开始，让我们尝试找出strpos函数的定义。
尝试的第一步，就是去PHP 5.4根目录然后在页面顶部的搜索框输入strpos。搜索的结果是一个很大的列表，展示了`strpos`在PHP源码中出现的位置。
因为这个结果对我们并没有太大的帮助，我们使用一个小技巧：我们搜索”PHP_FUNCTION strpos”（不要漏了双引号，它们很重要），而不是`strpos`.
现在我们得到两个入口链接：

```
/PHP_5_4/ext/standard/
    php_string.h 48   PHP_FUNCTION(strpos);
    string.c     1789 PHP_FUNCTION(strpos)
```
第一个要注意的事情是，两个位置都是在`ext/standard`文件夹。这就是我们希望找到的，因为strpos函数（跟大部分string，array和文件函数一样）是standard扩展的一部分。
现在，在新标签页打开两个链接，然后看看它们背后藏了什么代码。
你会看到第一个链接带你到了php_string.h文件，它包含了下面的代码：

```
// ...
PHP_FUNCTION(strpos);
PHP_FUNCTION(stripos);
PHP_FUNCTION(strrpos);
PHP_FUNCTION(strripos);
PHP_FUNCTION(strrchr);
PHP_FUNCTION(substr);
// ...
```
这就是一个典型的头文件（以.h后缀结尾的文件）的样子：单纯的函数列表，函数在其他地方定义。事实上，我们对这些并不感兴趣，因为我们已经知道我们要找的是什么。
第二个链接更有趣：它带我们到`string.c`文件，这个文件包含了函数真正的源代码。
在我带你一步一步地查阅这个函数之前，我推荐你自己尝试理解这个函数。这是一个很简单的函数，尽管你不知道真正的细节，但大多数代码看起来都很清晰。
## PHP函数的骨架
所有的PHP函数都使用同一个基本结构。在函数顶部定义了各个变量，然后调用`zend_parse_parameters`函数，然后到了主要的逻辑，当中有`RETURN_***`和`php_error_docref`的调用。
那么，让我们以函数的定义来开始：

```
zval *needle;
char *haystack;
char *found = NULL;
char  needle_char[2];
long  offset = 0;
int   haystack_len;
```
第一行定义了一个指向`zval`的指针`needle`。zval是在PHP内部代表任意一个PHP变量的定义。它真正是怎么样的会在下一篇文章重点谈论。
第二行定义了指向单个字符的指针`haystack`。这时候，你需要记住，在C语言里面，数组代表指向它们第一个元素的指针。比如说，`haystack`变量会指向你所传递的`$haystack`字符串变量的第一个字符。`haystack + 1`会指向第二个字符，`haystack + 2`指向第三个，以此类推。因此，通过逐个递增指针，可以读取整个字符串。
那么问题来了，PHP需要知道字符串在哪里结束。不然的话，它会一直递增指针而不会停止。为了解决这个问题，PHP也保存了明确的长度，这就是`haystack_len`变量。
现在，在上面的定义中，我们感兴趣的是offset变量，这个变量用来保存函数的第三个参数：开始搜索的偏移量。它使用long来定义，跟int一样，也是整型数据类型。现在这两者的差异并不重要，但你需要知道的是在PHP中，整型值使用long来存储，字符串的长度使用int来存储。
现在来看看下面的三行：

```
if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz|l", &haystack, &haystack_len, &needle, &offset) == FAILURE) {
    return;
}
```
这三行代码做的事情就是，获取传递到函数的参数，然后把它们存储到上面声明的变量中。
传递给函数的第一个参数是传递参数的数量。这个数字通过`ZEND_NUM_ARGS()`宏提供。
下一个函数是`TSRMLS_CC`宏，这是PHP的一种特性。你会发现这个奇怪的宏分散在PHP代码库的很多地方。是线程安全资源管理器（TSRM）的一部分，它保证PHP不会在多线程之间混乱变量。这对我们来说不是很重要，当你在代码中看到`TSRMLS_CC`（或者`TSRMLS_DC`）的时候，忽略它就行。（有一个奇怪的地方你需要注意的是，在”argument”之前没有逗号。这是因为不管你是否使用线程安全创建函数，该宏会被解释为空或者, trsm_ls。因此，逗号是宏的一部分。）
现在，我们来到重要的东西：”sz|l”字符串标记了函数接收的参数。：

```
s  // 第一个参数是字符串
z  // 第二个参数是一个zval结构体，任意的变量
|  // 标识接下来的参数是可选的
l  // 第三个参数是long类型（整型）
```
除了s，z，l之外，还有更多的标识类型，但是大部分都能从字符中清楚其意思。例如b是boolean，d是double（浮点型数字），a是array，f是回调（function），o是object。
接下来的参数`&haystack`，`&haystack_len`，`&needle`，`&offset`指定了需要赋值的参数的变量。你可以看到，它们都是使用引用（&）传递的，意味着它们传递的不是变量本身，而是指向它们的指针。
这个函数调用之后，`haystack`会包含haystack字符串，`haystack_len`是字符串的长度，needle是needle的值，offset是开始的偏移量。
而且，这个函数使用FAILURE（当你尝试传递无效参数到函数时会发生，比如传递一个数组赋值到字符串）来检查。这种情况下`zend_parse_parameters`函数会抛出警告，而此函数马上返回（会返回null给PHP的用户层代码）。
在参数解析完毕以后，主函数体开始：

```
if (offset < 0 || offset > haystack_len) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Offset not contained in string");
    RETURN_FALSE;
}
```
这段代码做的事情很明显，如果offset超出了边界，一个E_WARNING级别的错误会通过php_error_docref函数抛出，然后函数使用RETURN_FALSE宏返回false。
`php_error_docref`是一个错误函数，你可以在扩展目录找到它（比如，ext文件夹）。它的名字根据它在错误页面中返回文档参考（就是那些不会正常工作的函数）定义。还有一个`zend_error`函数，它主要被Zend Engine使用，但也经常出现在扩展代码中。
两个函数都使用sprintf函数，比如格式化信息，因此错误信息可以包含占位符，那些占位符会被后面的参数填充。下面有一个例子：

```
php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to write %d bytes to %s", Z_STRLEN_PP(tmp), filename);
// %d is filled with Z_STRLEN_PP(tmp)
// %s is filled with filename
```
让我们继续解析代码：

```
if (Z_TYPE_P(needle) == IS_STRING) {
    if (!Z_STRLEN_P(needle)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty delimiter");
        RETURN_FALSE;
    }
    found = php_memnstr(haystack + offset,
                        Z_STRVAL_P(needle),
                        Z_STRLEN_P(needle),
                        haystack + haystack_len);
}
```
前面的5行非常清晰：这个分支只会在needle为字符串的情况下执行，而且如果它是空的话会抛出错误。然后到了比较有趣的一部分：`php_memnstr`被调用了，这个函数做了主要的工作。跟往常一样，你可以点击该函数名然后查看它的源码。
`php_memnstr`返回指向needle在haystack第一次出现的位置的指针（这就是为什么found变量要定义为char *，例如，指向字符的指针）。从这里可以知道，偏移量（offset）可以通过减法被简单地计算，可以在函数的最后看到：

```
RETURN_LONG(found - haystack);
```
最后，让我们来看看当needle作为非字符串的时候的分支：

```
else {
    if (php_needle_char(needle, needle_char TSRMLS_CC) != SUCCESS) {
        RETURN_FALSE;
    }
    needle_char[1] = 0;
    found = php_memnstr(haystack + offset,
                        needle_char,
                        1,
                        haystack + haystack_len);
}
```
我只引用在手册上写的”如果 needle 不是一个字符串，那么它将被转换为整型并被视为字符顺序值。”这基本上说明，除了写`strpos($str, 'A')`，你还可以写`strpos($str, 65)`，因为A字符的编码是65。
如果你再查看变量定义，你可以看到`needle_char`被定义为`char needle_char[2]`，即有两个字符的字符串，`php_needle_char`会将真正的字符（在这里是’A’）到needle_char[0]。然后strpos函数会设置needle_char[1]为0。这背后的原因是因为，在C里面，字符串是使用’’结尾，就是说，最后一个字符被设置为NUL（编码为0的字符）。在PHP的语法环境里，这样的情况不存在，因为PHP存储了所有字符串的长度（因此它不需要0来帮助找到字符串的结尾），但是为了保证与C函数的兼容性，还是在PHP的内部实现了。
## Zend functions
我对strpos这个函数感觉好累，让我们找另一个函数吧：strlen。我们使用之前的方法：
从PHP5.4源码根目录开始搜索strlen。
你会看到一堆无关的函数的使用，因此，搜索“PHP_FUNCTION strlen”。当你这么搜索的时候，你会发现一些奇怪的事情发生了：没有任何的结果。
原因是，strlen是少数通过Zend Engine而不是PHP扩展定义的函数。这种情况下，函数不是使用`PHP_FUNCTION(strlen)`定义，而是`ZEND_FUNCTION(strlen)`。因此，我们也要搜索“ZEND_FUNCTION strlen”。
我们都知道，我们需要点击没有分号结尾的链接跳到源码的定义。这个链接带我们到下面的函数定义：

```
ZEND_FUNCTION(strlen)
{
    char *s1;
    int s1_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &s1, &s1_len) == FAILURE) {
        return;
    }
    RETVAL_LONG(s1_len);
}
```
这个函数实现太简单了，我不觉得我还需要进一步的解释。
## 方法
我们会谈论类和对象如何工作的更多细节在其他文章里，但作为一个小小的剧透：你可以通过在搜索框搜索`ClassName::methodName`来搜索对象方法。例如，尝试搜索`SplFixedArray::getSize`。
## 下一部分
下一部分会再次发表在。会谈论到zval是什么，它们是怎么工作的，以及它们是怎么在源码中被使用的（所有的Z_***宏）。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7aa70eaa311d5a5dec82ca0d49624741.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/1a4b335ec981d013f4002205a23fedd0.jpg)
