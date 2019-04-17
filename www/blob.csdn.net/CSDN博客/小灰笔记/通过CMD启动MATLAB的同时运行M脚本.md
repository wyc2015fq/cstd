# 通过CMD启动MATLAB的同时运行M脚本 - 小灰笔记 - CSDN博客





2016年12月28日 22:27:35[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：5606








之前写过一个这个知识点的总结，当时有点疑惑，不知道为什么我测试的时候有时候可以有时候不行。今天终于发现了我之前一直忽略的一个细节，MATLAB中M脚本可以当做一个命令来使用，但是使用的时候是不带有后缀名称的。而MATLAB的命令参数中，-r选项是用于启动的时候运行命令的选项。

做一下测试如下：

1，D盘根目录下放一个my_test.m的小脚本，内容如下

![](https://img-blog.csdn.net/20161228222756779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2，输入以下命令进行测试


![](https://img-blog.csdn.net/20161228222851842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3，测试的时候提示没有找到变量或者函数，其实就是脚本命令没有识别，而正确的命令应该是不带后缀名的文件名。


![](https://img-blog.csdn.net/20161228222927027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





4，再次测试


![](https://img-blog.csdn.net/20161228223003733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5，这次测试，脚本运行成功，而两者之间的差异只在后缀名。其实，这种差异在MATLAB环境下直接运行脚本的时候也是存在的。


![](https://img-blog.csdn.net/20161228223031403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




