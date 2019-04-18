# Web001-Win7中安装IIS7 - 小灰笔记 - CSDN博客





2018年01月06日 00:32:01[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：348








       我本身对微软的IIS兴趣不大，我喜欢的Web架构是LAMP。不过，由于公司领导不允许反驳的原因，现在不得不在这方面花点时间。不过也好，反正也算得上是小hack经验。对自己来说没有什么太大的损失，只是做的东西多少有点不是自己很中意的而已。

       IIS在Win7中应该可以说是内置的，只是没有开启而已。我是这么理解，但是我自己的机器是联网的，具体是不是有联网下载的过程我还真说不清楚。也不去做这个确认了，低性价比。

1.    打开控制面板，点击程序（标注的时候确实有点偏了，应该是程序而不是卸载）。

![](https://img-blog.csdn.net/20180106003202169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2.    点击打开或关闭Windows功能（从这个描述看，这个功能应该是内置了）。

![](https://img-blog.csdn.net/20180106003232218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.    找到这个Internet信息服务，把子项全都打钩后点击确认。

![](https://img-blog.csdn.net/20180106003242527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4.    点击确认后，会出现以下提示。

![](https://img-blog.csdn.net/20180106003253011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

5.    安装完成后，可以进行IIS的配置。

![](https://img-blog.csdn.net/20180106003309637?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



       最后，说一下选择的时候子项中的IIS6。我个人的理解是这些相关子项其实是IIS7对IIS6的兼容性支持。至于安装好的IIS的版本，其实从帮助中就能够看到。相应的版本是IIS7，确切说是7.5。



