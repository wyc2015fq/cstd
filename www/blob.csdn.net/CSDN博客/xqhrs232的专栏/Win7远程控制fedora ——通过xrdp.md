# Win7远程控制fedora ——通过xrdp - xqhrs232的专栏 - CSDN博客
2016年04月14日 15:47:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2061
原文地址::[http://blog.sohu.com/s/MTU5MTY3OTE1/302888160.html](http://blog.sohu.com/s/MTU5MTY3OTE1/302888160.html)
最近要分析RDP协议的相关东西，然后需要抓包，实验室空着的电脑只有Fedora系统了，就找了一下windows可以通过RDP协议远程控制linux的办法，网上找到很多资料。自己整理了下自己实现的步骤和资源，还有遇到的问题。
    1、 现在Fedora下安装xrdp:
     yum install xrdp
     不过发现源里没有，就直接在网上下载的安装包。网址：[http://sourceforge.net/projects/xrdp/files/](http://sourceforge.net/projects/xrdp/files/)
     下载下来之后放在 /usr/local/src/目录下，解压：
     tar -xzvf xrdp-v0.6.1.tar.gz
     cd xrdp-v0.6.1
     正常接下里就是make 编译然后 make install，不过这里没看到makefile，自然就不能make  然后看到目录里有个文档 readme.txt，点进去看了下，里面有安装说明。原来要先运行其他的。
     ./bootstarp
     ./configure
     make
     make  install
     2、配置
     安装好xrdp以后就是配置了，在/usr/local/src/xrdp-v0.6.1  目录下有个文档，file-loc.txt,他是软件所包含的文件位置。还有安装文档 install.txt，他包含相关配置文件的位置和作用。
     主要修改一个配置文件：/etc/xrdp/sesman.ini
     在[Globals] 中的这一行:
     ListenAddress=127.0.0.1
     表示只允许从本机连结RDP Server，如果要从别的主机进行操作(配合登入xrdp 的"sesman-any" 选项)，可将"127.0.0.1″ 改成"0.0.0.0″。
     3、设置开机启动
     可以将xrdp_control.sh start加入到文件/etc/rc.d/rc.local中实现开机自启动，在install.txt中也有说明，不过我只是抓几个包就可以了，所以就没设置这个了。
     4、开启xrdp
     进入目录 /etc/xrdp/
     cd /etc/xrdp
     开启：./xrdp.sh start
     关闭：./xrdp.sh stop
     重启：./xrdp.sh restart
    然后就从windows那一端开始远程登陆了：输入mstsc；输入IP地址
    到这里以后我这就出现了问题，说是连接端口5910的时候出现了问题，连接不上。
connecting to sesman ip 127.0.0.1 port 3350
sesman connect ok
sending login info to sesman
login successful for display 10
starting connecting
connecting to 127.0.0.1 5910
error -problem connecting
以上错误是由于缺少vnc-server
    在网上查了一下，说是Fedora的vncserver的问题，就更新了以下
     yum install vnc-server
    然后重启xrdp，问题就解决了。可以远程了，而且速度还挺快的~
![](http://sucimg.itc.cn/sblog/oTeZd4kiH48)
![](http://sucimg.itc.cn/sblog/jTeZTN2KO9t)
![](http://sucimg.itc.cn/sblog/oTeZTvSt2ef)
![](http://sucimg.itc.cn/sblog/oTeZTGyihNL)
![](http://sucimg.itc.cn/sblog/jTeZjNvjApx)
还在网上看到说连线后，首先会看到xrdp 的登入视窗，你可以选择:
因为在远程时，系统出现英文，解决办法是：
进入vim /usr/local/xrdp/startwm.sh
要添加:
# defaults to running kde
. /etc/profile
export LANG=zh_CN.UTF-8
export XMODIFIERS="@im=SCIM"
export GTK_IM_MODULE="scim"
scim -d
不过这个没试过，没有验证是否可行。
