# VS2008 程序打成CAB包 程序安装后始终是旧版本的exe - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年02月13日 14:20:49[boonya](https://me.csdn.net/boonya)阅读数：1858标签：[vs2008																[解决方案																[磁盘																[调试																[CAB](https://so.csdn.net/so/search/s.do?q=CAB&t=blog)
个人分类：[C#](https://blog.csdn.net/boonya/article/category/1776321)





**    VS2008 将程序打成CAB包 打了一上午的包 程序始终是旧版本的exe，很明显打包出了问题。最终多种尝试终于解决，现记于此。**

**处理步骤如下：**

1、清除解决方案，并重新生成解决方案。

2、将本地磁盘的解决方案下的：**bin\Release和obj\Release**目录的文件全部清空（据说此处有Bug，在Release路径下如果存在文件则不进行更新，故此导致打包CAB文件安装的*.exe保持旧的版本）。

3、部署重新生成的解决方案。

4、启动Release的调试。

![](https://img-blog.csdn.net/20140213141522468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5、查看本地磁盘**bin\Release和obj\Release**目录的文件是否重新生成，注意看生成时间。

6、按照CAB打包流程进行后续操作，例如：[http://blog.csdn.net/boonya/article/details/19082747](http://blog.csdn.net/boonya/article/details/19082747)。](https://so.csdn.net/so/search/s.do?q=调试&t=blog)](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=解决方案&t=blog)](https://so.csdn.net/so/search/s.do?q=vs2008&t=blog)




