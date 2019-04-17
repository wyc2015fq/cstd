# 【TinyOS】Ubuntu12.04下 TinyOS2.1.2的搭建与配置 - CD's Coding - CSDN博客





2015年06月08日 04:01:42[糖果天王](https://me.csdn.net/okcd00)阅读数：638








被坑了好久……好久……好久……

终于搭建好了，不写个博客记(qing)录(zhu)一下么？




前言自然要写最重要的东西：

不要完全照抄2.1.1的教程去装2.1.2！

2.1.2的/opt/tinyos-2.1.2/下并没有tinyos.sh文件！




咳咳，来来来我们从头来说~ 图文说明哟

![](https://img-blog.csdn.net/20150608034215775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


第一件事自然是要加个tinyOS的源，这里用的是[斯坦福的tinyOS](http://tinyos.stanford.edu/tinyos-wiki/index.php/Installing_TinyOS_2.1.1#Two-step_install_on_your_host_OS_with_Debian_package)源：


`gedit /etc/apt/sources.list`
需要添加的行如下：


` deb http://tinyos.stanford.edu/tinyos/dists/ubuntu lucid main`
![](https://img-blog.csdn.net/20150608034352557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

源改好了之后，需要更新一下源~ 就是


`apt-get update`这项操作会在list文件中读取，如果有新的会把新的报头摘取下来

![](https://img-blog.csdn.net/20150608034704656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们输入
`sudo apt-get install tinyos`的时候，可以获得当前已有的各种版本的清单，选择一个，这里我选择的是最新的


`sudo apt-get install tinyos-2.1.2`
![](https://img-blog.csdn.net/20150608034720930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

会询问是否安装，虽然空间很吃紧但咱还是得装呀~

![](https://img-blog.csdn.net/20150608034921531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


于是开始了漫长的等待

![](https://img-blog.csdn.net/20150608034827418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下载完成，自动开始解包

![](https://img-blog.csdn.net/20150608034855716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



安装完成，返回用户操作

![](https://img-blog.csdn.net/20150608035056862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


通过修改bashrc来添加tinyos.sh的操作


`gedit ~/.bashrc`
这个在2.1.1都是有效的，然而——

2.1.2 根本没有tinyos.sh这个文件！

![](https://img-blog.csdn.net/20150608035217390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


所以我们需要将bashrc的末尾添加如下文本



```
#installation by substituting /opt for the actual tinyos tree
#installation point
TOSROOT="/opt/tinyos-2.1.2"
TOSDIR="$TOSROOT/tos"
CLASSPATH="$TOSROOT/support/sdk/java/tinyos.jar:."
MAKERULES="$TOSROOT/support/make/Makerules"
export TOSROOT
export TOSDIR
export CLASSPATH
export MAKERULES
#Sourcing the tinyos environment variable setup script
source /opt/tinyos-2.1.2/tinyos.sh
```

添加成功之后记住要重启终端

其实不重启也是可以的，输入这段指令就可以了——


`source ~/.bashrc`
![](https://img-blog.csdn.net/20150608035308608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

打开sim.extra，对python的版本号作更改


`sudo gedit /opt/tinyos-2.1.1/support/make/sim.extra`

什么，不知道python版本号？


`python --version`

什么，你还没有python？


`apt-get install python`
![](https://img-blog.csdn.net/20150608035646490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

获取JNI和G++并给予用户权限




```
sudo tos-install-jni
sudo apt-get install g++
```

`sudo chown okcd00 -R /opt/tinyos-2.1.2/`

此处的okcd00是我的ID哦，你们要改成自己的username哦

![](https://img-blog.csdn.net/20150608035740688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在线down了一些实例下来，打算尝试能不能在TinyOS下跑，也算一种验证。


`wget http://github.com/tinyos/tinyos-release/archive/tinyos-2_1_2.tar.gz`![](https://img-blog.csdn.net/20150608040005547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
解压之后覆盖了tinyos-2.1.2的apps文件夹，然后进入Blink目录下，尝试


`make telosb`

出现上述信息，即代表成功编译运行，验证完毕，TinyOS2.1.2至此完全搭建完成

倘若有什么没有写的完善的不容易懂的，请在下方留言。当然如果有什么好的建议或者意见也欢迎告诉我~



















