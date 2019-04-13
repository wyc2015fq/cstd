
# centos6.5 firefox升级 flash player安装 adblock plus广告屏蔽插件安装 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月29日 23:57:37[Jlins](https://me.csdn.net/dyllove98)阅读数：2919标签：[centos6.5 firefox																](https://so.csdn.net/so/search/s.do?q=centos6.5 firefox&t=blog)


firefox升级
1.到官网https://www.mozilla.org/en-US/firefox/new/?from=getfirefox上下载最新的firefox版本，我下载的是firefox-33.1.1.tar.bz2
2.删除原有的Firefox 浏览器
|1
|\#yum erase firefox
|

也可以使用rpm的形式删除
3.解压到你希望安装的目录，例如我的是/usr/local，解压后会生成一个firefox的文件夹
|1
|\#tar -jxvf firefox-33.1.1.tar.bz2 -C /usr/local/
|

解压后我们会在/usr/local/firefox目录下看到一个firefox的可执行文件，cd到该目录下执行
|1
|\#./firefox
|

即可运行firefox
4.以上可以说是安装完成了，但是我们在终端下直接输入firefox时会提示找不到该命令，必须输入/usr/local/firefox/firefox才能启动firefox，所以我们在这就给它做个链接
|1
|\#ln -s /usr/local/firefox/firefox /usr/local/bin/ /usr/local/sbin/
|

这样，我们在终端输入firefox时就会启动firefox啦
5.添加桌面快捷方式以便在桌面单击使用，可以参考《CentOS6.5安装eclipse》中快捷方式的创建方法

adobe flash player安装
火狐浏览器默认情况下是不带flash_player，一般情况下有flash的网页打开就会提示安装flash_player插件，先根据火狐的提示，去Adobe的网站下载最新的flash插件，我下载的是install_flash_player_11_linux.x86_64.tar.gz
1.下载install_flash_player_11_linux.x86_64.tar.gz
2.解压
|1
|tar -zxvf install_flash_player_11_linux.x86_64.tar.gz
|

解压后会生成三个文件(Everything is file.)libflashplayer.so，readme.txt，usr(目录)
3.将libflashplayer.so拷贝到Firefox的plugins目录
|1
|cp libflashplayer.so /usr/lib/mozilla/plugins/
|

4.将usr目录下的所有文档拷贝到系统的/usr目录下
|1
|cp -r ./usr/* /usr/
|

5.重启firefox就生效啦

adblock plus插件安装
本来安装插件是很容易的事情，可是最近出了个ocsp回应包含过期信息的错误，这样就无法直接安装插件了，据说又关GFW的事，证书过期什么的，也不是很清楚，可以参考下火狐解决ocsp回应包含过期信息的问题
根据以上帖子的内容，我们在hosts文件中添加相关条目即可
|1
|vi /etc/hosts
|

在文件中插入如下条目
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|\#mozilla
|117.18.232.191 addons.cdn.mozilla.net
|117.18.232.191 mozorg.cdn.mozilla.net
|117.18.232.191 developer.cdn.mozilla.net
|117.18.232.191 fhr.cdn.mozilla.net
|117.18.232.191 support.cdn.mozilla.net
|117.18.232.191 marketplace.cdn.mozilla.net
|117.18.232.191 air.cdn.mozilla.net
|117.18.232.191 activations.cdn.mozilla.net
|117.18.232.191 cdn.mozilla.net
|117.18.232.191 download-installer.cdn.mozilla.net
|117.18.237.191 getpersonas-cdn.mozilla.net
|117.18.232.191 snippets.cdn.mozilla.net
|117.18.232.191 telemetry-experiment.cdn.mozilla.net
|117.18.232.191 code.cdn.mozilla.net
|117.18.232.191 videos.cdn.mozilla.net
|117.18.232.191 glow.cdn.mozilla.net
|117.18.237.29   ocsp.digicert.com
|72.21.81.109 df6a.https.cdn.softlayer.net
|\#mozilla
|

这下重启浏览器后，选择【插件】搜索adblock plus安装即可
以上完成后就可以屏蔽广告了，接下来讲一下怎么屏蔽优酷和土豆的广告呢？
打开 home/你的用户名 文件夹找到 .macromedia 文件夹一般是隐藏的你需要按 ctrl+h 使其显示出来（root用户到/root下查找），然后相应进入Flash_Player/\#SharedObjects/（8位随机名字）文件夹下 如果看到一个叫 static.youku.com 的文件夹那就删掉（没有就直接无视）然后创建个叫 static.youku.com 的文档更改权限为只读，至此完结。
本来是到上面是可以的了，如果你安装的是英文版的firefox就还有一步，我们到adblock plus中添加过滤规则Easylist China+Easylist并更新。
好啦～以上重启浏览器后打开优酷看看是不是生效了～

![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)

