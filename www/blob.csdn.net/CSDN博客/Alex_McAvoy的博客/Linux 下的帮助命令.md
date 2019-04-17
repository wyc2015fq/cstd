# Linux 下的帮助命令 - Alex_McAvoy的博客 - CSDN博客





2018年07月03日 16:34:56[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：2350








# 【概述】

在 Linux 环境中，如果遇到困难，可以使用帮助命令来取得帮助。

常见的帮助命令有：man 命令、help 命令、info 命令

# 【man 命令】

1.概述

man 命令，它是 Manual pages 的缩写，它是 Linux 系统中在线软件文档的一种普遍的形式，其内容包括计算机程序（包括库和系统调用）、正式的标准和惯例，抽象的概念等。

man 工具是显示系统手册页中的内容，也就是一本电子版的字典，这些内容大多数都是对命令的解释信息，还有一些相关的描述，通过查看系统文档中的 man 还可以得到程序的更多相关信息和 Linux 的更多特性。

2.作用：通常用来获得某个命令的说明和使用方式的详细介绍。

3.格式：**man 命令名**

注：可以使用 man 命令查看其本身的使用说明，即：man man

4.man 手册的分区区段

1）man 手册的内容很多，涉及了 Linux 使用过程中的方方面面，为了便于查找，man 手册被进行了分册（分区段）处理，手册通常被分为以下9个区段：


|1|Standard commands （标准命令）|
|----|----|
|2|System calls （系统调用）|
|3|Library functions （库函数）|
|4|Special devices （设备说明）|
|5|File formats （文件格式）|
|6|Games and toys （游戏和娱乐）|
|7|Miscellaneous （杂项）|
|8|Administrative Commands （系统管理命令和守护进程）|
|9|other（其他，用来存放内核例行程序的文档）|
2）如要查看相应区段的内容，就在 man 后面加上相应区段的数字即可。
例：![](https://img-blog.csdn.net/2018070316114148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3）打开手册之后可以通过鼠标滚轮或上下键来进行上下翻看，查看完毕后按 q 退出当前页面。


![](https://img-blog.csdn.net/20180703161310125?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5.man 手册页布局

所有的手册页遵循一个常见的布局，一般包括以下部分内容：

1）手册名称与所在章节，如上图最上方的 LS（1），其中 LS 表示手册名称，（1）表示该手册位于第一章节。


2）NAME（名称）：该命令或函数的名称，接着是一行简介。

3）SYNOPSIS（概要）：对于命令，正式的描述它如何运行，以及需要什么样的命令行参数。对于函数，介绍函数所需的参数，以及哪个头文件包含该函数的定义。

4）DESCRIPTION（说明）：命令或函数功能的文本描述。

5）EXAMPLES（示例）：常用的一些示例。

6）SEE ALSO（参见）：相关命令或函数的列表。

除以上内容外，不同命令存在其它部分内容，但这些部分没有得到跨手册页的标准化。

常见的例子有：OPTIONS（选项）、EXIT STATUS（退出状态）、ENVIRONMENT（环境）、BUGS（程序漏洞）、FILES（文件）、AUTHOR（作者）、REPORTING BUGS（已知漏洞）、HISTORY（历史）、COPYRIGHT（版权）等。

6.man 手册的搜索

通常 man 手册中内容很多，不太容易找到想要的结果，可以在 man 中使用搜索：**/ <要搜索的关键字>。**

当查找完毕后，可以按 n 切换到下一个关键字所在处，shift+n 切换到上一个关键字所在处。

![](https://img-blog.csdn.net/20180703163025258?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


# 【help 命令】

1.作用

help 命令是用于显示 shell 内建命令的简要帮助信息，帮助信息中显示有该命令的简要说明以及一些参数的使用以及说明。

![](https://img-blog.csdn.net/20180703162920311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




2.外部命令的 help

外部命令基本上都有一个参数 --help，通过这个参数可以得到相应的帮助。

![](https://img-blog.csdn.net/20180703163013189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


3.何时选用 help 命令



当非常紧急只是忘记该用哪个参数的时候，help 这种显示简单扼要的信息就特别实用，若是不太紧急的时候就可以用 man 这种详细描述的查询方式。

# 【info 命令】

如果觉得 man 显示的信息都还不够，满足不了需求，那么可使用 info 命令来获取帮助。



info 来自自由软件基金会的 GNU 项目，是 GNU 的超文本帮助系统，能够更完整的显示出 GNU 信息。

man 和 info 就像两个集合，它们有一个交集部分，但与 man 相比，info 工具可显示更完整的 GNU 工具信息。 

![](https://img-blog.csdn.net/20180703163441518?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






