# 如何成为一名优秀的 C 语言程序员 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [bigship](http://www.jobbole.com/members/bigship) 翻译。未经许可，禁止转载！
英文出处：[Fabien Sanglard](http://www.fabiensanglard.net/c/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
**问题的提出**
每过一段时间我总会收到一些程序员发来的电子邮件，他们会问我是用什么编程语言来编写自己的游戏的，以及我是如何学习这种编程语言的。因此，我认为在这篇博文里列出一些有关C语言的最佳读物应该能帮到不少人。如果你知道其它的优秀读物，请给我发邮件或者直接在评论栏中告诉我吧。
**问题的解答（你可以略过这部分）**
我在之前的一篇[博文](http://www.fabiensanglard.net/dEngine/)中已经提到过了，目前为止，所有我所编写的商业3D引擎95%都是C89（也称作标准C，或ANSI C）。我之所以选择C89而不是C99是因为有些编译器仍然不能完美支持C99标准。而且从第一天起，我就强制规定游戏需要能在iOS、Windows和Xbox 360平台上都能编译。根据我所选择的编译平台，剩下5%的代码是Objective-C（iOS）或者C++（Windows，Mac OS X），这是为了将引擎绑定到原生的输入/输出设备上。没想到的是，选择C还是C++这个主题在[reddit](http://www.reddit.com/r/gamedev/comments/ctiqm/iphone_3d_engine_experience_from_2009_especially/)曾上引发了很大的争议。我选择C的两个真正原因是：
● 我写引擎时对C++掌握的并不算好。平台资源非常有限（iPhone 2G），而且我知道我得对我的工具（编程语言）相当熟悉才能使游戏达到每秒60帧的速度。
● 因为我研读过id Software（著名游戏制作公司，Doom系列、Quake系列是其代表作，开创了FPS类型游戏的先河）发布过的源代码，用纯C完成一个商业游戏是我的梦想。
这算是“好”的选择吗？我想，到最后唯一合理的问题是：“你的游戏发布了吗？”以及“它跑的够快吗？”看看那令人难以置信的帧速吧（有人提到在玩[Shmup](http://www.fabiensanglard.net/shmupLite/)时会有眩晕的感觉），我想我做了正确的选择。
**糟糕的C读物（不要忽略这一段）**
我将以一些我认为不必太认真的读物说起：网络教程，博客以及大部分Google上得到的东西都算（没错，也包括我这篇）。通常我认为这些资料来源是不可信的，而且有潜在的危害。和这一行里大部分人一样，我曾是重度Google搜索使用者。过了一段时间之后，我发现不准确的答案反而对生产力的提高有反效果。这让我有了一种速度上的错觉：答案找的越快，活干的越慢。没有什么网站会比一本好书更有价值，没有什么好书比反汇编输出更有价值。
**优秀的C读物**
[1.《C程序设计语言》](http://www.amazon.cn/gp/product/B0011425T8/ref=as_li_qf_sp_asin_il_tl?ie=UTF8&tag=vastwork-23&linkCode=as2&camp=536&creative=3200&creativeASIN=B0011425T8)
![](http://jbcdn2.b0.upaiyun.com/2012/03/the-c-programming-language.jpg)
[《C程序设计语言》](http://www.amazon.cn/gp/product/B0011425T8/ref=as_li_qf_sp_asin_il_tl?ie=UTF8&tag=vastwork-23&linkCode=as2&camp=536&creative=3200&creativeASIN=B0011425T8)，又称 K&R C。这是你应该首先去看的有关C语言编程的经典著作。读这本书会很轻松，它只有272页，386幅插图。由C语言之父Kernighan和Ritchie亲自编写的短小且有清晰解释的代码示例遍布全书各个角落。在刚开始的几周里，所有你需要了解的有关C语言的知识都在这里。这本书读起来很有趣，主题都比较短小，而且可以使你很快上手。你可能会跳过附录A（有关C语言中比较晦涩的阴暗角落，比如类型提升、 类型转换、型别退化以及其它一些无用的东西）以及有关C标准库的附录B。我认为刚开始学习C语言时，这些就足够了。这本书使C显得很娇小、简洁，因此强烈推荐学习这本书。
如果你继续学习并坚持练习，很快你就会触到一些诡异的情况，比如下面的例子：

C
```
unsigned int ui_one         = 1;
signed int i_one            = 1;
signed short s_minus_one    = -1;
if (s_minus_one > ui_one)
    printf(“-1 > 1 \n”);
if (s_minus_one < i_one)
    printf(“-1 < 1 \n”);
#./run
#
# -1 > 1
# -1 < 1
```
在上面的代码示例中，由于整型提升的缘故，-1先被计算为大于1，然后又小于1。C语言里有很多这种阴暗面，此时将产生不正确的结果。
这里还有许多细节之处：

C
```
extern void foo(void);
void (*f)();
f = &foo;     // 合法
f = foo;      // 同样合法！（语法糖）
f();          // 调用f
(*f)();       // 同样调用了f（语法糖）
```
或者有关数组/指针/类型退化的例子：

C
```
int array[] = {0, 1, 2, 3, 4};
int *pointer = array;
if (sizeof array == sizeof pointer)
    printf(“This will never be printed!!”);
if (sizeof(int *) == sizeof &array[0])
    printf(“This will be printed!!\n”);
if (&array[2] - &array[0] == 8 )
    printf(“This will never be printed either, result is 2 not 8!!”);
```
当你发现自己原来对C语言的了解还很不够时（附录A真的太轻量级了），那这时就是拿起第二本书的时候了。
2. 《C专家编程》
![](http://jbcdn2.b0.upaiyun.com/2012/03/expert-c-programming.jpg)
这是本神奇的书，因为该书以一种非常娱乐化的方式告诉你在代码背后究竟都发生了些什么。通过介绍了几个著名bug的奇闻异事（大部分跟NASA有关）后，读者将再次领略整型提升、下标、类型退化以及许多其他的C语言犄角。这本书是如此的迷人，你很可能会一晚上埋头读完353页，然后失望地说：怎么这么短？
现在你想挖的更深些并成为一个优秀的C程序员，你应该拿起这本你所需要的最后一本C语言书籍：
3. 《C语言参考手册》
![](http://jbcdn2.b0.upaiyun.com/2012/03/C-manual.jpg)
这本书全面涵盖C/C89/C99标准。从今以后你面对的就是这些冰冷无聊的语言标准。你可以把K&R 和C专家编程放在书架上，而把这本C语言参考手册摆放在显示器旁，任何你想知道的C语言知识都在这里。
**附加阅读**
1. [id Software 代码库](ftp://ftp.idsoftware.com/idstuff/)
![](http://jbcdn2.b0.upaiyun.com/2012/03/id.jpg)
只看书当然是不够的。阅读优秀的开源代码将带来极大的帮助。我的最爱是id Software的3D引擎代码库：Doom、Quake、Quake2、Quake3、Wolfenstein 3D iPhone以及Doom iPhone。当我阅读这些代码时，我会自己撰写备忘日志。之后我会整理这些日志，写成技术文章（[Doom](http://www.fabiensanglard.net/doomIphone/doomClassicRenderer.php)、 [Quake](http://www.fabiensanglard.net/quakeSource/index.php)、[Wolfenstein 3D iPhone](http://www.fabiensanglard.net/wolf3d/index.php) 以及 [Doom iPhone](http://www.fabiensanglard.net/doomIphone/index.php)）。
2.  《[Sh*t My Dad Says](http://www.amazon.cn/gp/product/0062067281/ref=as_li_qf_sp_asin_il_tl?ie=UTF8&tag=vastwork-23&linkCode=as2&camp=536&creative=3200&creativeASIN=0062067281)》
![](http://jbcdn2.b0.upaiyun.com/2012/03/dad-says1.jpg)
试着保持健康的生活，改变一下，读些有趣的东西吧 : )
**注**
原文评论栏中推荐的一些补充书目：
1. C Interfaces and Implementations: Techniques for Creating Reusable Software（C语言接口与实现：创建可重用软件的技术）
![](http://jbcdn2.b0.upaiyun.com/2012/03/C-interface-and-implementations.jpg)
2. [Object-Oriented Programming With ANSI-C](http://www.planetpdf.com/codecuts/pdfs/ooc.pdf)
![](http://jbcdn2.b0.upaiyun.com/2012/03/ooc.jpg)
3. 《 Code Complete 2nd edition / 代码大全 第二版》
![](http://jbcdn2.b0.upaiyun.com/2012/03/cc2.jpg)
