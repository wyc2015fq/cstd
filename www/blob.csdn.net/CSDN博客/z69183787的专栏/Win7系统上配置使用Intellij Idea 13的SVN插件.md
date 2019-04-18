# Win7系统上配置使用Intellij Idea 13的SVN插件 - z69183787的专栏 - CSDN博客
2014年11月29日 12:43:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1269
个人分类：[IDE-Idea](https://blog.csdn.net/z69183787/article/category/2695809)
前言：最近打算在团队开发环境中采用Idea，然后我自己用的是mac，下载最新的Idea之后一切正常，但是同样的windows下的最新版的Idea的SVN就一直有问题，没办法，正好搜索了下，然后根据这篇博客(猛戳阅读原文：[Win7系统上配置使用Intellij Idea 13的SVN插件](http://blog.csdn.net/zhyh1986/article/details/39227979))，如果转载请标注原文出处，尊重作者的知识版权。
以下是解决的教程原文：
以前开发工具一直用的是Eclipse/MyEclipse，虽然早就听说过Idea而且也尝试用过几次，但由于使用不习惯每次都是不久就放弃了，这次因为使用了FreeMarker，而Idea对FreeMarker的支持很好，因此下决心换IDE
说实话一开始使用idea真是很不习惯，不只是快捷键不同；比如项目和模块、服务器（如Tomcat）配置等也完全不同，但坚持用过一段时间后感觉很棒，比使用Eclipse/MyEclipse理有效率。
开始的时候因为不熟悉，没有使用SVN插件，仅是使用idea作为开发工具，然后TortoiseSVN客户端来作版本控制，今天尝试配置了一下idea的svn插件，其实也没有想像中的那么麻烦。
我使用的idea是13.0.4版本，这个版本中支持了最新的Subversion 1.8；相对于之前版本，Subversion 1.8提供了SVN最新的使用方法 -- 客户端命令行来代替SVNKit，这种方法更灵活，使对未来版本的支持更加容易。
这里有这个问题的官方说明：[http://blog.jetbrains.com/idea/2013/12/subversion-1-8-and-intellij-idea-13/](http://blog.jetbrains.com/idea/2013/12/subversion-1-8-and-intellij-idea-13/)
[](http://blog.jetbrains.com/idea/2013/12/subversion-1-8-and-intellij-idea-13/)
既然需要使用客户端命令行，那我们就需要在机器上安装一个SVN客户端命令行程序，可以到这里下载对应的安装程序：[http://subversion.apache.org/packages.html#windows](http://subversion.apache.org/packages.html#windows)
我选择的是[SlikSVN](http://www.sliksvn.com/en/download)，因为其它的好像都是客户端和服务器安装程序，只有它仅包含SVN客户端。
安装成功后配置一下环境变量PATH（其实不配也没关系，对idea来说没关系，仅在使用命令行时需要），配置好环境变量后测试一下：
**[plain]**[view
 plain](http://blog.csdn.net/zhyh1986/article/details/39227979#)[copy](http://blog.csdn.net/zhyh1986/article/details/39227979#)
- C:\Users\Administrator>svn --version --quiet  
- 1.8.10-SlikSvn-1.8.10-X64  
想启用idea的SVN插件还需要在idea配置一下（Ctrl + Alt + S），如下图所示：
![](https://img-blog.csdn.net/20140912121540218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh5aDE5ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接下来启用idea的版本控制插件（这里当然是启用Subversion了），打开“VCS”菜单项然后点击“Enable Version Control Integration...”菜单，如下图所示：
![](https://img-blog.csdn.net/20140912122206578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh5aDE5ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
选择Subversion项：
![](https://img-blog.csdn.net/20140912122508176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh5aDE5ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
VCS菜单会变成下面这个样子，SVN所有相关的操作这里都有
![](https://img-blog.csdn.net/20140912122750213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh5aDE5ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样就可以使用SVN插件了，虽然与Eclipse的SVN插件使用有差别，但也很简单了，具体就不再详细记述了.
