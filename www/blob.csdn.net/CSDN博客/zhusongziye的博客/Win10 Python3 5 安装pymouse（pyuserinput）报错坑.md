# Win10 Python3.5 安装pymouse（pyuserinput）报错坑 - zhusongziye的博客 - CSDN博客





2018年02月02日 17:36:44[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1441








看到python可以控制鼠标和键盘，所以自己也试了下。结果遇到数个坑。

我的电脑环境：

python3.5.4

Win10 64位 商业版

问题描述：

1、由于python3.5把pymouse和pykeyboard放在了pyuserinput这个包里面，所以我们只要安装这个包就可以了。

2、如果直接pip install pyuserinput，会提示要先安装pyhook这个包。

解决办法：

1、安装pywin32，下载地址 https://github.com/mhammond/pywin32/releases

2、在这个上面下载pyhook进行安装https://www.lfd.uci.edu/~gohlke/pythonlibs/#pyhook

![](https://img-blog.csdn.net/20180202172524572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

根据我的环境我用的是（根据你自己的环境选择）

![](https://img-blog.csdn.net/20180202172756108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3、使用pip install pyuserinput 命令安装pyuserinput，大家注意下版本

![](https://img-blog.csdn.net/20180202173052791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我发现如果pyuserinput为0.1.12时，就会说版本不匹配。大家请注意。谢谢！






