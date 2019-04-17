# R语言自动化报告格式——knitr - 素质云笔记/Recorder... - CSDN博客





2015年08月12日 19:22:40[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4375
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)









R语言自动化报告格式——knitr


相关文献：

[R语言自动化报告格式——knitr](http://blog.csdn.net/sinat_26917383/article/details/47449367)

[资讯 | R Notebooks 即将发布](http://blog.csdn.net/sinat_26917383/article/details/52749108)

## ------------------------------------------------------------------------

## 一、背景

在R的世界里，凡是提到自动化报告，很多人就会想到Sweave，它已经诞生十几年了。它的主要设计思想来自于文学化编程（Literate Programming），这是Knuth大神提出来的一种编程范式，它与传统的结构化编程不同。

knitr (发音为：nit-ter)是由纯文本 (就像你正在阅读的) 和R代码交织在一起的文档文件. 用作者的话来说就是：“knitr 是一头灵活，快速的大象!” 你可能正在想……这玩意和数据挖掘有什么必然的关系呢？ 使用knitr去学习数据挖掘乍看起来是一个奇怪的搭配，但确实是一条很好的学习途径。 

有三个方面的原因: 这是学习R完美的途径。不一定每个人都是这样做的，但是knitr能让你运用学到的东西以及建立一个可重复生成报告的文件。 Knitr和RStudio集成是快速生成报告的最佳组合。简单地说，报告中有文字也有代码，Knitr就是通过markdown的语法在报告中该显示代码的地方显示代码，该显示运行结果（比如图表）的地方显示运行结果，这样整个报告文档就可以直接运行再次生成报告，而不需要把代码单独复制到控制台中执行，然后再把执行结果添加到文档中。 

### 1、文学化编程也是编程

文学化编程（Literate Programming）是整个设计的核心思想，但过去的模式局限在“代码+文档”的简单模型上，knitr使得一份文档变得可编程。

[]

### 2、Markdown 是什么，如何写Markdown[]

Markdown 是一种轻量级标记语言。类似HTML，但是比html简单的多，我在文件夹中放了一个markdown的语法说明。 大家具体可以用一段时间学会。我们这里知道这么几点就可以了。标题，一个#后面跟一个空格代表一级标题，2个## 后面跟一个空格代表二级标题。

1. how are you 斜体

2. how are you 加粗



## 二、创建knitr文档

不要停下来! 通过以下5步，你就可以创建自己的第一个knitr文档：

l 打开 RStudio, 点击File > New File > R Markdown…创建一个新的R Markdown 文件

l 为这个文件起一个有意义的名字。

l 点击OK.

l 删除第二个"---"之后出现的文字

l 点击Knit HTML.

l 你的R Markdown代码应该像下面这样：





title: "Your Title"

output: html_document

在“knitting”你的文件后, 你应该会在查看器窗格看到下面的文字:



恭喜你! 你已经成功创建了第一个knitr文件!







## 三、相关代码——文本输出[]

在文本输出方面，knitr包支持以下功能：

l 代码高亮（highlight=TRUE），增强可读性，有无数的高亮主题可选，仅适用于LaTeX和HTML输出，MD文档在转为HTML文档之后可以用专门的JavaScript库去高亮代码

l 代码重排（tidy=TRUE），对那些不注意代码格式的人来说很有用，再乱的代码，到了这里也会变得相对整齐，本功能由formatR包支持

l 执行或不执行代码（eval=TRUE/FALSE），不执行的代码段将被跳过，原样输出源代码

l 显示/隐藏源代码（echo=TRUE/FALSE），甚至精确控制显示哪几段代码（echo取数值）

l 显示/隐藏普通文本输出或将文本输出以原样形式输出（results='markup', 'hide', 'asis'）

l 显示/隐藏警告文本（warning=TRUE/FALSE）、错误消息（error）和普通消息（message）

l 显示/隐藏整个代码段的输出（include=TRUE/FALSE），比如我们可能想运行代码，但不把结果写入输出中



