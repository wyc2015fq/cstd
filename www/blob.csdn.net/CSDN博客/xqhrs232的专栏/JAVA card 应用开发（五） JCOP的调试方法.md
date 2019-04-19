# JAVA card 应用开发（五） JCOP的调试方法 - xqhrs232的专栏 - CSDN博客
2017年07月01日 17:54:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：217
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://blog.csdn.net/gszhy/article/details/40780057](http://blog.csdn.net/gszhy/article/details/40780057)
相关文章
1、JCOP使用实例----[https://wenku.baidu.com/view/733a0c86172ded630a1cb66e.html](https://wenku.baidu.com/view/733a0c86172ded630a1cb66e.html)
2、[JavaCard开发环境搭建](http://blog.csdn.net/menghnhhuan/article/details/7616690)----[http://blog.csdn.net/menghnhhuan/article/details/7616690](http://blog.csdn.net/menghnhhuan/article/details/7616690)
3、
# [JAVA card 应用开发（一） 创建第一个APPLET](http://blog.csdn.net/gszhy/article/details/40779949)----[http://blog.csdn.net/gszhy/article/details/40779949](http://blog.csdn.net/gszhy/article/details/40779949)
4、[JAVA card 应用开发（二） 在项目增加APPLET](http://blog.csdn.net/gszhy/article/details/40779999)----[http://blog.csdn.net/gszhy/article/details/40779999](http://blog.csdn.net/gszhy/article/details/40779999)
5、[JAVA card 应用开发（三） 把APPLET（CAP文件）装载到卡片](http://blog.csdn.net/gszhy/article/details/40780023)----[http://blog.csdn.net/gszhy/article/details/40780023](http://blog.csdn.net/gszhy/article/details/40780023)

本文讲述如何在仿真环境下，调试[Java](http://lib.csdn.net/base/java)card 的APPLET。
通过JCOP在Eclipse进行仿真调试，启动JCOP调试器，我们可以看到如下图：
![](https://img-blog.csdn.net/20140704110635218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZnJldWRfbHY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
要仿真调试，就是在命令输入框里面输入JCOP相关的指令，通过阅读JCOP的帮助文档，我们看到JCOP提供的命令不算繁多，大约40个。
下面我挑选常用的几个来说明：
1.      复位卡片：“**/atr**”，如果不加任何参数，表示立即复位。
2.      发送指令：“**/send**”，这是最直观的调试指令。可以逐个指令发送给仿真器，类似真实环境里面读卡器与卡片的交互。Send指令后面的参数，不能加空格，如果不加任何控制符，JCOP默认为十六进制。加入“|”，可字符转化为十六进制，加入“#”，可计算后面字符的长度的值。比如：
/send 00A404000E315041592E5359532E4444463031 (不加任何控制符的情况)
/send "0102|ab|03 04|cd"   ==>  /send  01 02 61 62 03 04 63 64
/send "01#(|ab|#{FE FF})"   ==>  /send  01 05 61 62 02 FE FF
3.      查看版本：“**version**”。
以上是逐个指令输入的情况，可以结合Eclipse的断点功能，查看程序运行过程中的变量值的变化。
如果我们有多条指令需要一起执行，可以直接让仿真器执行脚本。比如个人化，或者进行稳定性[测试](http://lib.csdn.net/base/softwaretest)的情况下，可采用下面的方法：
1.      设置内部变量：“/set-var”。设置脚本执行的路径，比如在命令输入框输入：
/set-var path "E:\JCOP_Script"![](https://img-blog.csdn.net/20140704110541578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZnJldWRfbHY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.      在“E:\JCOP_Script”这个路径下，编写相应脚本，比如我这么写：
![](https://img-blog.csdn.net/20140704110923781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZnJldWRfbHY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
请留意几点：
a.      我写文件的名称是“perso1.jcsh”，后缀必须是.jcsh；
b.      注释请以“#”开始；
c.      内容就是各条JCOP指令组成。
3.      由于我写的文件名称是perso1.jcsh，在指令框中直接输入 **perso1**，即可。
图中可以看到JCOP逐条已经执行文件里面的指令。
![](https://img-blog.csdn.net/20140704110548109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZnJldWRfbHY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
脚本的使用还在不断学习中，有新的想法，会在本文更新。
说明：《[java](http://lib.csdn.net/base/java)card 应用开发》系列博文是我在学习[Java](http://lib.csdn.net/base/java)卡过程中的记录，有不对的地方，请指出。
