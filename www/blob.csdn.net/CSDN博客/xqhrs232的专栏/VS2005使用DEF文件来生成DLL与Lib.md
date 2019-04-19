# VS2005使用DEF文件来生成DLL与Lib - xqhrs232的专栏 - CSDN博客
2012年06月28日 15:38:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：872标签：[dll																[sqlite](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/boise/article/details/1448749](http://blog.csdn.net/boise/article/details/1448749)
VS2005使用DEF文件来生成DLL与Lib
VS2005使用DEF文件来生成DLL与Lib时，与VC6的设置是不一样 的，我费了一个小时才发现其中的原因，嘿嘿。
VC6工程中，只要有DEF文件并将其添加到工程中，VC6就可以自动地生成DLL与其相应的Lib文件了。
但是VS2005不一样，需要指定DEF文件，方法如下：
把 sqlite[3].def 添加到工程中. 
选择 工程 > 属性中的链接器，然后找到"输入"这一项. 在 "模块定义文件" 中输入 a.def.
注意: 你需要在 Debug 和 Release 中都输入该项才行. 
