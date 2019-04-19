# 如何创建本地的LLVM SVN镜像库 - SHINING的博客 - CSDN博客
2013年02月27日 10:40:27[snsn1984](https://me.csdn.net/snsn1984)阅读数：2681标签：[LLVM																[SVN																[本地镜像																[sync																[学习](https://so.csdn.net/so/search/s.do?q=学习&t=blog)](https://so.csdn.net/so/search/s.do?q=sync&t=blog)](https://so.csdn.net/so/search/s.do?q=本地镜像&t=blog)](https://so.csdn.net/so/search/s.do?q=SVN&t=blog)](https://so.csdn.net/so/search/s.do?q=LLVM&t=blog)
个人分类：[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
创建本地的LLVM SVN镜像，有一些好处，但是需要花费很多的时间，但是一旦建立好了，后面用起来就方便了很多。
可以随意的在没有网络或者网络状态不好的时候去使用LLVM的代码。
1.首先，需要创建一个空的库，使用命令：
$> svnadmin create /home/...../llvm
 '/home/...../' 是想存放该库的路径，此处注意使用绝对路径，使用相对路径会报错。llvm在这里是这个库的名字。可以根据自己的需要，设置自己想要的路径和库名。
2.需要新建一个hook，使用如下的命令：
$> echo '#!/bin/sh' > /home/...../llvm/hooks/pre-revprop-change
$> chmod +x /home/...../llvm/hooks/pre-revprop-change
3.初始化该库
$> svnsync init file:///home/...../llvm  http://llvm.org/svn/llvm-project/llvm/trunk
http://llvm.org/svn/llvm-project/llvm/trunk是LLVM主干SVN的地址。
4. 同步SVN：
$> svnsync sync file:///home/...../llvm
这会花很多时间，取决于网络和机器的速度。
5.更新镜像
隔一段时间需要运行如下命令去更新本地的LLVM镜像
$> svnsync sync file:///home/...../llvm
参考文章：[https://www.coderesort.com/about/wiki/HowTo/Subversion/SvnSync](https://www.coderesort.com/about/wiki/HowTo/Subversion/SvnSync)
