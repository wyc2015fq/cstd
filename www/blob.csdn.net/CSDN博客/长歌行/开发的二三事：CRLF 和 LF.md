# 开发的二三事：CRLF 和 LF - 长歌行 - CSDN博客





2018年01月08日 11:22:46[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：624








最近使用VS打开某些代码文件的时候，突然发现有这么一个提示： 
![这里写图片描述](https://img-blog.csdn.net/20180108111024859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
赶紧去查git的log日志。检查log时，发现很多提交的代码，两个文件之间，貌似没有任何的改动。但仔细一看，原来区别是每一行末尾的换行符。有些换行符是下箭头↓，有些换行符是键盘上回车键的那个符号，先向下，再向左的箭头。如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20180108105701788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上网一搜，原来不同操作系统的换行符是不一样的啊。 unix是b’\n’， windows是b’\r\n”，mac是b’\r’。真是一个操作系统一个模样，内部的存储byte不一样，从外部的编辑器还看不出来。unix 的 \n 就是 LF(Line Feed)类型了。windows是cr(Carriage Return)。现在知道区别了，在项目里只需要统一标准就行了。粗略一想：项目是c#开发，目前还是在windows平台，当然是使用CRLF了。 但是，如何对每一个成员提交的代码，都审核一下，使用\r\n的换行符提交呢？

还好，对于这个问题，GIT已经有标准的解决办法。通过git的设置，可以自动完成上面的操作。 

设置如下即可：

```
git config --global core.autocrlf true
git config --global core.safecrlf true
```

上面第一行的意思是，如果有CRLF的换行符，在提交git时会自动转换成LF，但是在检出代码的时候会转换为CRLF。这样，VS就不会弹出提示了。 

上面第二行的意思是，不允许提交包含混合换行符的文件。

也许有的同学就要问了。我按照CRLF提交，CRLF检出，不是更好么？但是毕竟上面的是一个global的设置，谁能保证组里面每一个人都在windows上开发，不做其他项目，谁能保证项目永远只在windows上运行呢？

后续的处理就很简单了。把我们项目中所有文件的换行都转为CRLF，提交到git中即可。从网上找了个脚本，修改了下，效果还不错，能够在三个系统标准之间相互转换，支持后缀名过滤。我放到gist上了，有需要的下载来看看：[gist 地址](https://gist.github.com/bentan2013/1b43fb38da0ffc3bb4794a5db489d55e)




