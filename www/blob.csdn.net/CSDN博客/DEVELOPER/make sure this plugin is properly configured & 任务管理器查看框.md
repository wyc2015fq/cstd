# make sure this plugin is properly configured & 任务管理器查看框 - DEVELOPER - CSDN博客





2014年08月09日 11:32:30[学术袁](https://me.csdn.net/u012827205)阅读数：870标签：[plugin is configured																[模拟器不能连接Eclipse																[任务管理器页面](https://so.csdn.net/so/search/s.do?q=任务管理器页面&t=blog)
个人分类：[血粼粼的经验终结](https://blog.csdn.net/u012827205/article/category/2467133)





![](https://img-blog.csdn.net/20140809112434495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








































我想会有很多的朋友会遇到和我今天遇到的问题：*make sure this plugin is properly configured*

模拟器不能和Eclipse进行连接！导致自己所写下的项目不能正常的编译。

正确 的解决方法：

***打开cmd.exe<window+R>***

输入下列指令：netstat -ano | findstr "5037"


***找到下列进程***

TCP 127.0.0.1:5037 0.0.0.0:0 LISTENING 5912


***此时把5912这个进程结束就行了。***

如何找到5912对应的是哪个进程呢？继续输入下列指令：TASKLIST | findstr "5912"


***显示下列内容***

XXXX.exe 5912Console 6324K

最后打开任务管理器，把XXXX.exe这个进程给关闭掉就可以了。




***任务管理器查看框***


任务管理器查看框发生变化无法结束想要结束的进程，怎么办？

![](https://img-blog.csdn.net/20140809112859508?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140809112911739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://so.csdn.net/so/search/s.do?q=模拟器不能连接Eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=plugin is configured&t=blog)




