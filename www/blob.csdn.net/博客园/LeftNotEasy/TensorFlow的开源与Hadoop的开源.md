# TensorFlow的开源与Hadoop的开源 - LeftNotEasy - 博客园







# [TensorFlow的开源与Hadoop的开源](https://www.cnblogs.com/LeftNotEasy/p/differences-types-of-opensource-of-hadoop-and-tensorflow.html)






最近看TensorFlow代码的时候，用Git pull下来最新的master一看，哇好多的更新，然后点击去之前看到一半的cc文件继续看，好多地方都改变了。但是一看Git log，有好多巨大的commit叫什么 "Merge commit for internal changes", "Merge for github"，没有任何的其他描述...，这样基本上不知道到底改动是为了什么。

相对来说Hadoop的开源要实诚得多，每个改动都有对应的JIRA来跟踪，从JIRA里面可以看到改动的原因、设计（对于大的改变来说）和讨论。任何一个关注项目的人都可以在代码merge之前提出自己的质疑，（只要不是无理取闹）基本上能够得到很详细的回答。

出现这种区别最大的原因是来源于Google与Apache本质上不同，Google作为一个商业公司，有的是人来维护代码，基本上不在乎来自公司外的贡献。对于Google来说，开源TF（目测k8s也差不多）最大的目的是制定标准，推进自己的cloud，另外如果有更多的人来测试和修修补补那些外围一点的东西那是再好不过了。

而Apache基金会本来就是为了开源而生，公司把项目贡献给Apache基金会的目的除了为了提升自身形象以外，主要是为了吸引用户和更多的开发者，也就是造成杠杆的效应，那么为了得到更多真诚的帮助，肯定得把自己的肠肠肚肚都剖出来给人看。不然外部的人怎么去贡献。

当然两种方式各有优劣，对Google这种级别的公司这种方式可能更快也避免了很多讨论的时间但是也会让很多潜在参与者走掉，而Apache的方式更民主更开放。作为Hadoop的一员，我还是真心喜欢Apache的开放的工作方式...












