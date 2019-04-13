
# unity开发android游戏 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity开发android游戏](https://www.cnblogs.com/eniac1946/p/7411703.html)
|Posted on|2017-08-22 15:10|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7411703)|[收藏](#)
|环境搭建：
|Unity+JDK+Android Studio+Android SDK（+NDK）
|教程：|[unity开发android游戏（一）搭建Unity安卓开发环境](http://blog.csdn.net/chenggong2dm/article/details/20654075)
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170822124002511-112442686.png)
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170822151730964-1737865990.png)
|注意“Build System”选项是在Unity 5.5.1版本才支持的，这里表示支持导出的Android项目。|使用Android Studio开发，Build System需选择“Gradle”。
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170822152204246-2055453192.png)
|Scripting Backend需要选择|IL2CPP|，即为mono的AOT模式，Unity Engine运行效率较高。
|按以上设置Unity后可打包出.apk文件。
|项目结构：
|详细介绍、安装设置问题：|[Unity与Android的交互之项目结构](http://blog.csdn.net/qq_15003505/article/details/70156433)
|打开后再整体看下项目结构：
![这里写图片描述](http://img.blog.csdn.net/20170413142427550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTUwMDM1MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
|目录列表|[Unity项目文件目录发布后对应的ios/android应用目录](http://blog.csdn.net/w88193363/article/details/41288449)
|AOT模式下——
|unity engine路径：jniLibs
|脚本路径：assets/bin/
|\#JIT与AOT的说明——
|这里的scriptingBackend就是设置生成游戏的游戏逻辑存放方式，如果选择默认的Mono2x的话，会在反编译后的apk的assets\bin\Data\Managed目录下找到Assembly-CSharp.dll文件，也就是大多数unity游戏逻辑存放的位置，这种情况下，lib文件夹下是没有libil2cpp.so文件的。
|如果是选择IL2CPP的话，会在lib文件夹下生成libil2cpp.so文件，并在assets\bin\Data\Managed\Metadata目录下生成global-metadata.dat配置文件。
|-------------------进阶篇-----------------------------------------------------------------
|Unity编译Android的原理解析和apk打包分析
|apk的程序入口 和|UnityPlayerActivity|详细讲解：|[http://www.cnblogs.com/qcloud1001/p/6650023.html](http://www.cnblogs.com/qcloud1001/p/6650023.html)
|交互
|Android调用Unity：|[[Unity3D]Unity3D游戏开发之在Android视图中嵌入Unity视图](http://blog.csdn.net/qinyuanpei/article/details/39380717)
|Unity调用Android：|[Android与Unity交互研究](http://blog.csdn.net/crazy1235/article/details/46733221)
|安全加固
|对unity engine的加密：
|[Unity3D安卓导出包加密学习--加密libmono.so](http://blog.csdn.net/grimraider/article/details/51217985)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
