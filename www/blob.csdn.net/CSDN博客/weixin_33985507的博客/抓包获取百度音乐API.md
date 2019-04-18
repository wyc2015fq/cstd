# 抓包获取百度音乐API - weixin_33985507的博客 - CSDN博客
2017年01月17日 14:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
这次抓包是获取手机APP中的数据包，共分为三个部分：
1.win7建立wifi
2.PC架设代理服务器
手机设置代理
3.抓包分析
**一、win7建立wifi**
在win7下搭建wifi非常简单，网上的教程也非常多，下面简要介绍一下方法：
首先要保证笔记本开启了无线网卡。
1）以管理员身份打开cmd
2）输入命令：netsh wlan set hostednetwork mode=allow ssid=账号 key=密码，其中“账号“和”密码“是你想设置的wifi的账号和密码，密码限定8个字符以上。mode：是否启用虚拟网卡。输入完成，回车，可以看到设置成功的提示。
3）打开网络和共享中心-->更改适配器设置，可以看到多了一个名称为：”Microsoft Virtual WiFi Miniport Adapter”的无线网络连接2
4）右键已经联网的网络连接，选择“属性”->"共享"，进行如下设置
![](https://img-blog.csdn.net/20140817202551281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
此时联网的网卡图标上会显示“共享”字样，表示宽度连接已经共享至虚拟wifi。
5）继续在cmd中运行：netsh wlan start hostednetwork
发现虚拟网卡上的红叉消失，此时可以用手机连接该wifi
二、PC架设代理服务器
本文使用CCproxy代理服务器。以管理员方式运行。
1）设置-高级-网络，取消“禁止局域网外部用户”
![](https://img-blog.csdn.net/20140817204954656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2）设置，将本机局域网ip地址设置为刚刚建立的无线网络的IP地址。
![](https://img-blog.csdn.net/20140817205126515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3）账号，如下图设置
![](https://img-blog.csdn.net/20140817205921655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4）点击“新建”按钮。先将本机PC的IP账号添加至服务器
![](https://img-blog.csdn.net/20140817210801464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140817210857046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140817210925875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140817210940555?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
打开手机的网络设置，找到连接到的wifi网络，进行代理的IP和端口设置：
![](https://img-blog.csdn.net/20140817211433492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再将手机的IP账号添加至代理服务器
![](https://img-blog.csdn.net/20140817211021677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140817211034500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
此时，用手机打开网页和应用，可以正常联网，同时代理服务器显示出已经连接的设备数。
![](https://img-blog.csdn.net/20140817211050427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
至此代理服务器设置完毕，下面开始抓包。
三、抓包分析
目前的抓包分析工具很多，像wireshark、SmartSniff等，下面介绍使用SmartSniff进行抓包。
开始抓包。打开千千静听百度音乐版，随便点一首歌播放，然后停止抓包，结果如下：
![](https://img-blog.csdn.net/20140817212512609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl3X2Jsb2c=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
API一目了然了，下面总结出几个比较有用的API：
**[html]**[view plain](http://blog.csdn.net/xyw_blog/article/details/38641793?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/xyw_blog/article/details/38641793?utm_source=tuicool&utm_medium=referral#)
[print](http://blog.csdn.net/xyw_blog/article/details/38641793?utm_source=tuicool&utm_medium=referral#)[?](http://blog.csdn.net/xyw_blog/article/details/38641793?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/506187/fork)
- 1）搜索建议  
- http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.search.catalogSug&format=json&query=%E5%B0%8F%E8%8B%B9%E6%9E%9C  
- 搜索建议：只有歌名  
- http://tingapi.ting.baidu.com/v1/restserver/ting?method=baidu.ting.search.suggestion&query=%E5%B0%8F%E8%8B%B9%E6%9E%9C&format=json&from=ios&version=2.1.1  
- 2）搜索结果  
- http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.search.common&format=json&query=%E5%B0%8F%E8%8B%B9%E6%9E%9C&page_no=1&page_size=30  
- 3）  
-  http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.artist.getList&format=json&order=1&offset=0&limit=5  
-  4）新歌榜  
-  http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.billboard.billList&format=json&type=1&offset=0&size=50  
-  5）热歌榜  
- http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.billboard.billList&format=json&type=2&offset=0&size=50  注意这个和上边的区别，type=1  
-  6）Billboard  
-  http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.billboard.billList&format=json&type=8&offset=0&size=507）Hito中文榜  
- 7）Hito中文榜  
- http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.billboard.billList&format=json&type=18&offset=0&size=50  
- 8）KTV热歌榜  
- http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.billboard.billList&format=json&type=6&offset=0&size=50  
- 9）电台列表  
-  http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.radio.getCategoryList&format=json  
-  10）获取某个电台下的歌曲列表  
-  http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.radio.getChannelSong&format=json&pn=0&rn=10&channelname=public_tuijian_ktv  
-  11）获取songid的歌曲信息  
-   http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.song.getInfos&format=json&songid=8059247&ts=1408284347323&e=JoN56kTXnnbEpd9MVczkYJCSx%2FE1mkLx%2BPMIkTcOEu4%3D&nw=2&ucf=1&res=1   
-   12）获取登陆用户的喜爱歌曲列表，其中bduss参数用来标示唯一的用户  
-   http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.favorite.getFavoriteSong&format=json&pn=0&rn=50&bduss=UlXZ1dWbm9icDBrMm13aFcwZ282ejlTM1dyS1NEd2JPWXpQcDgyT0w0Vn5SUmhVQVFBQUFBJCQAAAAAAAAAAAEAAAB0L~cOeHl3MDQzNzM1AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH-48FN~uPBTd  
-   13)歌手列表  
-   http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.artist.get72HotArtist&format=json&order=1&offset=0&limit=50  
-   14）歌手简介，tinguid为歌手id  
-   http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.artist.getinfo&format=json&tinguid=7994  
-   15）歌手歌曲列表，tinguid为歌手id  
-   http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.artist.getSongList&format=json&order=2&tinguid=7994&offset=0&limits=50  
- 16）新碟上架  
- http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.plaza.getRecommendAlbum&format=json&offset=0&limit=50  
- 17）专辑信息  
- http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.album.getAlbumInfo&format=json&album_id=122314357  
- 18）新歌速递  
- http://tingapi.ting.baidu.com/v1/restserver/ting?from=qianqian&version=2.1.0&method=baidu.ting.plaza.getNewSongs&format=json&limit=50  
本文为Eliot原创，转载请注明出处：[http://blog.csdn.net/xyw_blog/article/details/38641793](http://blog.csdn.net/xyw_blog/article/details/38641793)
