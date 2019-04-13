
# Debian 平台 NoMachine 安装配置后 无法连接 - David Dai -- Focus on Oracle - CSDN博客


2015年02月15日 23:53:12[Dave](https://me.csdn.net/tianlesoftware)阅读数：11271




以前写过一篇Redhat/CentOS 平台下的NoMachine 远程工具的安装，链接如下：
**远程桌面工具****-- NoMachine**
[http://www.cndba.cn/Dave/article/668](http://www.cndba.cn/Dave/article/668)

今天在Debian 平台下又安装了一下，安装配置没有问题，但是客户端无法链接。

# 1  Debian 平台下安装

直接从官网下载deb 包。
dave@dave:~$ sudo wget[http://download.nomachine.com/download/4.4/Linux/nomachine_4.4.6_7_amd64.deb](http://download.nomachine.com/download/4.4/Linux/nomachine_4.4.6_7_amd64.deb)
//然后安装：
dave@dave:~$ sudo dpkg -inomachine_4.4.6_7_amd64.deb
安装完后可以直接在Debian的GNOME中查看。
最新版与之前版本的NoMachine有2个区别：
（1）  默认端口改成了4000.
（2）  只有一个安装文件，默认就安装服务端和客户端。
这点可以直接从GNOME中确认：
![](https://img-blog.csdn.net/20150215235108229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

打开服务进行配置：
![](https://img-blog.csdn.net/20150215235104379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从GUI上，可以看出来，可以很直接的配置端口，开机自启动等信息。


# 2  远端连接
在我本机的Win7 平台去连接Debian，结果发现无法连接，也没有报任何错误，起初怀疑是端口问题，修改端口以后还不行，因为是Debian的系统，我直接卸载了防火墙，所以也排除了防火墙的因素。
![](https://img-blog.csdn.net/20150215235121364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

折腾了半天之后，才发现，原来两端的NoMachine版本不一样。低版本的NoMachine 无法正常连接高版本的NoMachine。
![](https://img-blog.csdn.net/20150215235128743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


重新安装了Win7 的NoMachine之后，正常连接了。
![](https://img-blog.csdn.net/20150215235126702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20150215235144172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

整体风格与之前的版本，变化还是非常大。  随笔小记。



--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**AboutDave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1: 62697850 (空)    CNDBA_2: 62697716 (满)  CNDBA_3: 283816689
CNDBA_4: 391125754   CNDBA_5:104207940    CNDBA_6: 62697977   CNDBA_7: 142216823（满）

