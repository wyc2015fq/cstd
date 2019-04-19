# Ubuntu11.10--->unity中彻底清除最近打开文档（视频）的历史记录 - 三少GG - CSDN博客
2011年11月07日 08:51:54[三少GG](https://me.csdn.net/scut1135)阅读数：5680
在11.10+unity中可行 ![:em01](http://forum.ubuntu.org.cn.sixxs.org/images/smilies/em01.gif)
1.清除unity中最近打开的文档的历史记录
**代码:**
rm ~/.local/share/zeitgeist/activity.sqlite && zeitgeist-daemon --replace
如果你想永远禁用unity的这一功能，可以用ubuntu tweak。
附件:
![选区_008.png](http://forum.ubuntu.org.cn.sixxs.org/download/file.php?id=136895&t=1&sid=1b05e587f76802ee4b2f5f2580e52d6a)
选区_008.png [ 95.26 KiB | 被浏览 1215 次 ]
2.清除gnome-mplayer和totem等播放器中的“最近打开”的历史记录
**代码:**
rm -v ~/.local/share/recently-used.xbel
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
有一个软件叫activity-log-manager
可以设置删除特定时间段内的记录，设置特定的文件夹内容永远不记录，等等。。。推荐一下。 ![:em09](http://forum.ubuntu.org.cn.sixxs.org/images/smilies/em09.gif)
