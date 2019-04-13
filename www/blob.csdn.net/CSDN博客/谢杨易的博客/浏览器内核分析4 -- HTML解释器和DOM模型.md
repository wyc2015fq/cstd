
# 浏览器内核分析4 -- HTML解释器和DOM模型 - 谢杨易的博客 - CSDN博客

2017年02月16日 17:17:32[谢杨易](https://me.csdn.net/u013510838)阅读数：1281



# 1 概述
Browser进程完成HTML CSS等资源文件的加载后，Renderer进程就需要开始渲染流程了。下面这张图描述了整个渲染流程。
![这里写图片描述](http://pic002.cnblogs.com/images/2011/265173/2011110316264892.png)
主要过程分为以下几步：
读取HTML文件，解析并生成DOM树
读取CSS文件，解析生成样式规则表，并根据选择器找到元素的匹配规则
由DOM树和匹配规则生成RenderObject树，并进行布局和绘制
执行JavaScript时，可能会改变DOM结构和CSS样式，此时需要针对改变部分重新布局和绘制。
后面几篇文章针对上述四个步骤一一讲解。本篇文章会讲解HTML解析和DOM树生成部分。
# 2 DOM模型
DOM定义了文档上的对象，对象的行为和属性，以及这些对象之间的关系。DOM树可以认为是页面上数据和结构的一个树形表示。HTML DOM定义了所有HTML文档元素的对象和属性，以及访问他们的方法。
## 节点
HTML文档每一个成分都是一个节点。整个文档对应HTMLDocument，称为文档节点。每个HTML标签是一个元素节点HTMLElement，又分为HTMLBodyElement，HTMLDIvElement, HTMLImageElement等对象。每一个属性是一个属性节点Attr。下图是一个简单的DOM树
![](http://pic002.cnblogs.com/images/2011/265173/2011110316280265.png)
# 3 HTML解析流程分析
解析读取HTML文件字节流，最终生成DOM树，大概分为以下四个步骤：
字节流转变为字符流。根据编码时候的格式来相应解码，如UTF-8 GBK
词法分析，可以简单理解为将字符流解析为一个个单词。这些单词在HTML语言的词汇表中（其实对于所有编程语言的词法分析阶段都是将符号解析为相应语言的词汇表中的单词）。这个过程会跳过空格，换行等内容。词法分析由HTMLTokenizer完成，循环调用nextToken()方法，解析出所有词语。HTMLToken定义了6种词语，DOCTYPE, StartTag, EndTag, Comment, Character, EndOfFile。
语法分析，利用HTML语言的语法对单词进行语法匹配。这个过程是迭代式的。获取单词，然后进行语法匹配。如果匹配，则将其添加到解析树上。如果未匹配，则保存在堆栈中，然后去获取下一个单词，直到匹配为止。如果最终还未匹配，则抛出异常。
转换，将解析树转变为DOM树。根据DOM模型，创建相关的DOM节点对象。HTMLDocument对象是DOM树的根节点，HTMLBodyElement代表body节点，HTMLDivElement代表div元素节点，Attr代表属性节点。
![Markdown](http://i1.piimg.com/1949/9c08fb608ee2d7ef.png)
为了提高HTML的容错性，解析器还必须修复无效内容。如未闭合的标签<br>。这无疑会增加解析器的负担，故建议严格按照W3C语法来编写代码。

