
# 可在广域网部署运行的QQ高仿版 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:16:14[Jlins](https://me.csdn.net/dyllove98)阅读数：1683


前段时间看到园子里有朋友开发了QQ高仿版的程序，我也非常有兴趣，以前一直有个做即时聊天程序的梦，趁这段时间工作不是很忙，就开始动手来做这个事情。根据我以往积累下来的项目经验，实现QQ的基本功能，问题应该不大。当然，我的目标并不是做一个QQ高仿版的玩具，而是希望做成一个能能够真正使用的产品（当然是免费的），并持续维护下去。想为这个聊天工具取个名字，就抽取了“叽叽歪歪”中的“叽叽”(GG)作为暂时的名称吧。
## 一.GG的功能规划
我想分成几个阶段来开发GG，每个阶段一个版本，从简单到复杂，从玩具到产品。大体规划如下：
**1.GG V1.0**
（1）登录、好友列表。
（2）文字聊天、字体设置、表情、窗口震动、截图、手写板
（3）文件传送、文件夹传送
目前，V1.0版本的功能已经全部实现，文末将提供下载。
**2.GG V2.0**
（1）语音聊天
（2）视频聊天
（3）远程协助
**3.GG V3.0**
（1）GG群
（3）视频会议
**4.GG V4.0**
（1）聊天记录
（2）离线消息
（3）离线文件
（4）增加持久化支持
**5.GG V5.0 & more...**
## 二.框架选型
如果要完全从头开发GG，其难度可想而知，所以，利用现有的成熟框架是必要的，我们必须站在别人的肩膀上，才能做得更快更好。GG V1.0 所采用的框架和素材如下：
（1）通信框架：ESFramework
（2）语音视频：OMCS
（3）皮肤框架：SkinForm
（4）截屏类库：ImageCapturerLib
（5）图片素材：90%以上来自QQ。
ESFramework和OMCS都是园子里[zhuweisky](http://www.cnblogs.com/zhuweisky/)的作品，已经相当成熟，不多说。SkinForm是我从网上找的一套类似QQ界面的皮肤框架（没有专门的项目网址），有些小bug，能将就用。ImageCapturerLib也是网上找的，并有源码，我把发现的一个bug已经修复了。由于是高仿QQ，所以，GG中用到的绝大多数图片都是从QQ里面拿过来的（商用也许会有版权问题）。
## 三.GG V1.0 功能展现
GG整个解决方案分为三个项目：服务端GG.Server（直接将ESFramework的demo的服务端搬了过来）、客户端GG、定义消息类型和协议的GG.Core。
至于GG V1.0是如何实现的，内部并不是很难（最困难的事情都让上述框架给做了），大家可以下载源码自己研究，我就不多费口舌了。下面就截几张运行时的图片吧，跟QQ是不是非常像了？
1.登录框
![](https://img-blog.csdn.net/20130807181602656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.主窗体
![](https://img-blog.csdn.net/20130807181604843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3.聊天窗口
![](https://img-blog.csdn.net/20130807181608031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 四.GG V1.0 源码下载
[GG V1.0 源码](http://files.cnblogs.com/justnow/GG2013.rar)
注意：如果要将GG部署到广域网，则可以在服务端的配置文件中设置监听的端口；而在客户端的配置文件中，则可以指定服务器的IP和Port。
大家有什么问题和建议，可以留言，也可以发送email到我邮箱：ggim2013@163.com。
如果有人有兴趣来参与GG后续版本的开发，那就更好了，尽管email给我。
如果你觉得还不错，请粉我，顺便再顶一下啊，呵呵



