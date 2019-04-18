# 80老翁谈人生（297）：自然语言处理与逻辑程序Prolog语言 - 袁萌专栏 - CSDN博客
2016年12月28日 04:27:26[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：570
80老翁谈人生（297）：自然语言处理与逻辑程序Prolog语言
2011年3月31日，IBM研究人员发表文章“Natural LanguageProcessing With Prolog in the IBM Watson System”，文章详细地说明了IBM沃森系统（认知平台）的工作原理，读后便可知晓一切。
**    原来IBM科学家设计了一种语法解析器“Parser”，本领很大。    **
实际上，Watson’s NLP（自然语言处理） begins byapplying a “parser”（语法解析器）  thatconverts each text sentence into a more structured form: a tree（语法树）that shows bothsurface structure and deep, logical structure.
说句大白话，语法解析器“Parser”本质上就是用逻辑程序Prolog语言老表达自然语言的语法结构，使其便于计算机处理。
我们以前已经讲过，Prolog语言就是把自然语言描述的对象（Entity）之间的关系（Relationship）用谓词演算的逻辑形式表达出来，便于计算机处理。比如：
GiveI(我，你，书）
这里“Give”是一个对象之间的三元关系，计算机Prolog语言就理解为“我给你书”。
沃森超级电脑肚子里面全是Prolog子程序在并行运行。实际上，沃森超级电脑就是一台Prolog程序大型处理器，其实沃森电脑什么也不懂。
袁萌   12月28日
