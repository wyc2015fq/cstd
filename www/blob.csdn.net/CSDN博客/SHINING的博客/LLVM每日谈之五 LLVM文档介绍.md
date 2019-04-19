# LLVM每日谈之五 LLVM文档介绍 - SHINING的博客 - CSDN博客
2012年11月09日 11:07:35[snsn1984](https://me.csdn.net/snsn1984)阅读数：10286
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
LLVM的文档挺多，而且都是官方的文档，刚开始接触的时候很容易让人摸不着头脑，这里就简单的介绍下这些文档。希望能给学习LLVM的人提供一些帮助。
当前版本和发布版本的文档：
首先，因为LLVM分发布版本和当前版本，发布版本是正式的对外发布的版本，当前版本是不断更新的最新版本。
目前最新的发布版本是3.1.而LLVM的文档也分发布版本和当前版本。
每个发布版本都会对应一套文档，当前版本也会对应一套文档。这些文档总体来说，差别不大，但是每个版本的文档还是会有一些更新的。
当前版本的文档：http://llvm.org/docs/index.html
每个发布版本和当前版本列表：http://llvm.org/releases/
点击列表中对应版本的doc就可以跳转到相应的文档列表。
文档的分类：
LLVM的文档主要分为一下几类：
1.Design & Overview
这个类别里的文档主要是几篇介绍LLVM架构和设计的论文和会议报告。
2.Publications
这个类别主要是一个已经发布的基于LLVM的项目列表。
3.User Guides
这个类别的文档主要是针对刚接触LLVM，打算使用LLVM的用户，并且此类用户打算接触LLVM的中间表示。
如果只是打算把LLVM作为一个编译器使用，可以考虑clang :  http://clang.llvm.org/
或者DragonEgg:  http://dragonegg.llvm.org/
4.API Clients
这个类别主要是针对以LLVM为基础进行工程开发的人。提供一些介绍编程需要的文档。比如:编程手册、编程规范等文档。
5.Subsystems
这个类别里的文档主要是介绍子系统的。相对前一个类别，这个列别里的文档都是针对的是LLVM里的某一块。比如介绍如何写Pass,如何写 Backend等。
6. Development Process
这个类别里主要介绍的是开发过程中的几个动作需要的文档。如何构建工程，如果编译工程，如何发布等。
7. Mailing Lists
这里主要是几个邮件列表，有开发的还有报告bug的，还有关于提交代码的。
PS：后续会更深入的对这些文档进行介绍，计划每个文档都用一段文件进行简要的概括和总结。
