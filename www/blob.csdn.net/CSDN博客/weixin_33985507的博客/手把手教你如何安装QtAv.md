# 手把手教你如何安装QtAv - weixin_33985507的博客 - CSDN博客
2017年07月26日 13:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
**首先说说我的开发环境：**
**   Qt5.4.0库**
**   MinGW 4.9.1编译器**
**   QtCreator 3.3**
**   我编译所用到的QtAv材料在这里可以下载  链接: http://pan.baidu.com/s/1hqhAeqw 密码: j7bf**
**   里面有QtAv源码，FFmpeg库，Portaudio库**
#     第一步：把上面所说的东西解压到某个盘的跟目录。 
      这里提醒一下，必须要有FFmpeg库，Portaudio库可以有也可以没有，因为作者第一次编译的时候都没有用到Portaudio库，但是出来的效果是视频播放有声音但是一卡一卡的。所以还是要加上那个库。至于你需不需要自己编译这两个库呢，我个人觉得你把时间用在编译这两个库特别是FFmpeg，不如把时间陪你老爸老妈好。所以还是下载人家已经编译好的库吧~~~
#      第二步：开始编译QtAv
      因为作者渣渣的，不会用命令行这个高大上的方法编译，只好打开creator，然后再打开QtAv.pro。如下图：
![](https://img-blog.csdn.net/20150227163822318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHJ1c3RndWFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
       然后添加环境变量，添加CPATH和LIBRARY_PATH两个变量，两个值分别为FFmpeg库，Portaudio库的include路径和lib路径，如下图：
![](https://img-blog.csdn.net/20150227164634169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHJ1c3RndWFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     最后当然就是编译啦，大概6分钟左右吧~~~下图就是编译好的库文件了：
![](https://img-blog.csdn.net/20150227165449706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHJ1c3RndWFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
          果断打开一个example运行一下啊~~~
![](https://img-blog.csdn.net/20150227165934188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHJ1c3RndWFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     是不是太简单了！！！搞到我都不好意思写这个教程了，不过我还是比较喜欢写这些傻瓜式的教程~~~
