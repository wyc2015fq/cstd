# Win10安装ST-Link驱动 - 小灰笔记 - CSDN博客





2017年09月03日 10:40:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2054








            从今天起，算是正是开启了ARM探索之路了。首先准备一下自己需要的PC环境，根据手头这个小玩意儿调试器的要求安装一下驱动。    

1，驱动包下载后，解压如下：

![](https://img-blog.csdn.net/20170903103905880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2，执行dpinst_amd64.exe启动安装

            说明：从驱动包看，驱动应该支持32位和64位的系统。我个人的电脑是64位的，先尝试安装一下64位软件试试。

3，安装

![](https://img-blog.csdn.net/20170903103918081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            从上面的结果看，安装还是十分顺利的。没有出现什么兼容性之类的问题。

4，查看

![](https://img-blog.csdn.net/20170903103928933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            如上图所示，安装结束后，电脑链接调试期后在设备管理器中能够看到相应的设备。



