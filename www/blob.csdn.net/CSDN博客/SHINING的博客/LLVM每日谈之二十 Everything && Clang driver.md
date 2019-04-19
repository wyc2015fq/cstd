# LLVM每日谈之二十 Everything && Clang driver - SHINING的博客 - CSDN博客
2014年11月06日 11:28:25[snsn1984](https://me.csdn.net/snsn1984)阅读数：3963
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[史宁宁（snsn1984）](http://blog.csdn.net/snsn1984)
最近在读<Getting Started with LLVM Core Libraries>，这是读的第一本LLVM的书，很多地方虽然讲的是自己知道的东西，但是也给人耳目一新的感觉，让人感觉之前有些不确定的东西，或者没有联系起来的知识点一下子贯通了，那感觉非常酸爽。![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)
**1.先谈谈题目里的Everything。**
LLVM每日谈之十六中，曾经提到过：Everything is a value.主要讲的是重要的LLVM IR的C++类都是Value的子类，同时在对IR的具体操作中，因为LLVM IR使用的是SSA，所以可以把很多的东西直接看成Value，并且也可以当做Value去操作，这样就很方便了。我觉的这句话非常经典，是属于点睛之笔的一句话，理解了这句话，有种豁然开朗，境界猛一下提升的感觉。具体内容可以去参照LLVM每日谈之十六。
在读<Getting ...>这本书的时候，遇到了另外一个Everything：Everything is a lib.这句话同样的经典，同样的高境界。之前我根据源码结构，自己推测LLVM整个架构下的所有产品基本上都是按照库的形式去组织代码的，甚至有的工具在是一个工具的同时，也是一个库。这句话证实了我的看法，LLVM的源码是按照库的形式去组织的。这对理解整个LLVM的源码，有很大的帮助，虽然只是一个架构方面的知识，但是可以让人对架构的理解提升不少。
所以：
Everything is a lib.
Everything is a value.
**2. Clang driver。**
Clang是LLVM框架下面的一个前端，这是无需置疑的。但是很多人容易忽略Clang driver，这个方面之前我也有过考虑，也是从读<Getting ...>这本书彻底的理清楚了。把LLVM作为编译器使用的时候，命令行使用的“clang”其实并不是前端Clang，而是指的是Clang driver；在命令行使用的“Clang cc1”也指的不是前端Clang，而指的是Clang编译器，这其中不仅包含了前端Clang，还包含了LLVM Core部分等。Clang driver不仅仅调用了前端Clang，同时还调用了LLVM Core，并且应为LLVM没有自己的Linker的缘故，在编译的最后阶段，还调用了系统的linker。LLVM的linker产品lld仍然在开发中。
所以如果把前端Clang和Clang driver分清楚，就好像分清楚了LLVM Core和LLVM框架一样，不同的概念指代的是不同范围的东西。如果不搞清楚，那么对之后的学习和工作，会造成很多的困扰。
所以：
Clang driver是一个驱动，一个包含编译全部阶段的驱动，不同于前端Clang，也不同于编译器Clang。
