# 基于GRAMMAR的HDecode解码 - xmdxcsj的专栏 - CSDN博客





2015年07月30日 09:07:48[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1609








一、应用场景

离线解码器支持命令式的语音输入

二、原理

Htk解码有两种网络：

1.      Task grammar 语法规则

2.      Word loop 语言模型

网络的描述形式是SLF，通过HParse和HBuild两个工具构建。SLF文件的用途有两种：

1.      用作网络的输入：用于构建解码网络

2.      用作识别结果的输出：输出lattice用于rescore或者nbest

以上的两种HVite采用的解码网络；HDecode采用的解码网络是基于词典构建的，语言模型生成单独的网络结构—trie树，用于获得语言得分，词典网络的节点会生成关于语言模型lookahead的信息。参考[http://blog.csdn.net/xmdxcsj/article/details/45953319](http://blog.csdn.net/xmdxcsj/article/details/45953319)

**HDecode是不支持输入lattice文件进行解码的，但是支持输入lattice文件进行重打分。**HDecode基于lattice解码的网络是使用lattice中出现的词构建的网络，同时根据lattice文件构建新的语言模型网络（结构体如下):

![](https://img-blog.csdn.net/20150730090700895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在解码的过程中，使用lookahead和trans进行剪枝。剪枝的依据是：对于lattice中两个节点之间有连线的语言得分为0，没有连线语言得分为极小值（会被剪枝）。

HDecode之所以不支持直接对grammar进行解码，主要有两方面的限制。一个是grammar必须以<s>开头（因为网络里面第一个词为<s>，如果没有<s>网络没法往下走）；另外一个是网络中不允许有辅助节点NULL。

三、改造HDecode

为了实现HDecode对grammar解码的支持，需要如下操作：

1.      HParse生成grammar对应的lattice

2.      删除lattice中的NULL节点

3.      Lattice读入HDecode进行解码



