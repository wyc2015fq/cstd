# 10款常用的Linux音乐播放器 - xqhrs232的专栏 - CSDN博客
2017年02月09日 16:45:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：395
原文地址::[http://blog.csdn.net/bamboolsu/article/details/43063575](http://blog.csdn.net/bamboolsu/article/details/43063575)
相关文章
1、Linux 下五款轻量级音乐播放器（开源）----[http://www.oschina.net/news/13947/five-lightweight-music-players/](http://www.oschina.net/news/13947/five-lightweight-music-players/)
不管你使用的是windows还是linux，抑或是Mac OS，音乐播 放器都基本是必须安装的。我这几天一直在ubuntu下折腾，尝试了很多款音乐播放器，老实说，我还是更喜欢windows里的 foobar2000，可惜foobar2000没有linux版本，但是，linux下的音乐播放器不必windows下的少，而且有很多还很优秀，很
 华丽。这里介绍10款我在ubuntu里安装的音乐播放器，同时提供一个解决MP3中文乱码的方法。 
需要注意的是，大多数播放器都是可以安装皮肤和插件的，所以，这里展示的不一定是你安装后的界面。 
1、 [Audacious](http://audacious-media-player.org/index.php?title=Main_Page)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/81e6639a24fc63f5c8eaf4bf.jpg)
很像winamp吧？非常遗憾的是，在写这篇文章的是，Audacious的官网上不去。Audacious与其它播放器的不同之处在于，它除了支持UTF-8字符集之外，还提供其它字符集的支持，这样能比较好的解决中文乱码的问题。后文将介绍一种更为有效的去除乱码的方法。 
2、[GmusicBrowser](http://squentin.free.fr/gmusicbrowser/gmusicbrowser.html)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/aae90cb4e02d2d8e37d3cabf.jpg)
GmusicBrowser是一款简洁实用的linux音乐播放器，直接安装后UI如上图所示，不过我建议你不要盯着唱片封面的[池玲子](http://blog.kenengba.com/2008/10/08/144/)看。 
3、[Rhythmbox](http://www.gnome.org/projects/rhythmbox/)
Rhythmbox是我重点推荐的音乐播放器，它的易用性很高，支持全局快捷键。最重要的是，它让我有foobar2000的感觉。 
![](http://hiphotos.baidu.com/bamboolsu/pic/item/603842cf1d14ed79b700c8bf.jpg)
4、[Listen
 Project](http://gmpcwiki.sarine.nl/index.php?title=GMPC)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/9f0182389bc702b33b87cebf.jpg)
Listen Music Player和Rhythmbox的UI比较相似，其实功能也差不多，但LMP支持从[维基百科](http://www.kenengba.com/post/361.html)读取歌手资料。 
5、[Bmpx](http://bmpx.backtrace.info/site/BMPx_Homepage)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/1fd26df21540d110342accbf.jpg)
Bmpx相对前面两者的特色在于最小化到托盘后的播放提示。其它播放器都只是提示下一首歌的歌名和歌手，而Bmpx则会像下图这样显示歌曲信息： 
![](http://hiphotos.baidu.com/bamboolsu/pic/item/9355be5104edfe1c1138c2bf.jpg)
6、[Juk](http://developer.kde.org/~wheeler/juk.html)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/9cf8915d23ff6b03faf2c0bf.jpg)
Juk是一个很易用的播放器，而且界面方面做得比较美观。但似乎它不能直接读取windows分区里的音乐文件，因为我是双系统安装ubuntu的，音乐都在NTFS分区里，所以Juk给我带来不少不便。 
7、[Banshee](http://banshee-project.org/)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/5220e510a4d4883ecb80c4bf.jpg)
功能与界面都属于Rythmbox那一派系的，也支持全局快捷键，不知道是不是我电脑的问题，当Banshee启动大概3分钟后，CPU资源占用一直在90%以上。 
8、[Exaile](http://www.exaile.org/)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/94bf02faba3c7b566c22ebbf.jpg)
Exaile的特色在于使用折叠菜单来选择专辑，同时也可以从维基百科读取歌手信息，支持歌曲黑名单。 
9、[SongBird](http://getsongbird.com/)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/1b29141767e5770a21a4e9bf.jpg)
SongBird给我的第一印象是很“苹果”，很iTune。它不但有linux版本，还有windows和Mac版本。相对于其它音乐播放器，Songbird可以说是最不像音乐播放器的，它集成web浏览器，也就是说你可以在songbird里一边上网一边浏览网页，当网页中出现音乐时，你可以用songbird将其捕抓下来。 
10、[Amarok](http://amarok.kde.org/)
![](http://hiphotos.baidu.com/bamboolsu/pic/item/897611220730a8129922edbf.jpg)
Amarok的功能很强大，而且界面也可以与SongBird媲美。 
解决Linux下音乐乱码的问题：
MP3文件乱码的原因： 
这个问题出现在mp3文件里，由于大陆大多数MP3文件都是用GBK/GB18030编码写入标签信息的，而大多数的linux播放器默认以utf-8编码读取，这就产生了乱码。 
解决方法： 
使用Mutagen来修改Mp3文件的标签信息，具体方法如下（只针对GBK/GB18030编码的情况）： 
安装Mutagen（ubuntu下终端运行 sudo apt-get install python-mutagen）后，在终端执行： 
mid3iconv -e gbk *.mp3 
如果想转换当前目录下的所有 mp3 (包括子目录)： 
find . -iname \"*.mp3\" -execdir mid3iconv -e gbk {} ; 
这里只介绍最常见的情况，更多信息可以查看[Nicky的文章](http://www.osxcn.com/ubuntu/mp3-tag-encoding.html)，他分析得很详细。
