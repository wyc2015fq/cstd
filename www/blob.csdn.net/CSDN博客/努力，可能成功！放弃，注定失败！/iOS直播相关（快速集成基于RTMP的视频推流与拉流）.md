# iOS直播相关（快速集成基于RTMP的视频推流与拉流） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月05日 09:30:58[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2064
![](http://upload-images.jianshu.io/upload_images/1881947-ab8bd12c72df333c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
关于直播技术的讲解已经有很多文章做过介绍了，我也只是参照前辈们的研究做了一个初步的入门，此文涉及的内容是在自己亲自实践过的基础上记一个笔记。
原理性的东西就不做记录了，毕竟我也不是很懂。。
### 一.推流部分
- 搭建nginx+rtmp服务器
（1）安装Homebrew
打开终端, 查看是否已经安装了Homebrew, 直接终端输入命令
`man brew`
如果已经安装, 直接进入第二步。 如果没有安装,执行命令
```bash
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
（2）安装nginx
clone nginx项目到本地
`brew tap homebrew/nginx`
安装
``brew install nginx-full --with-rtmp-module``
（3）运行nginx
此时, nginx和rtmp安装好了,继续输入命令:
`nginx`
然后再浏览器打开[http://localhost:8080](http://localhost:8080/)
如果出现下图, 则表示安装成功
![](http://upload-images.jianshu.io/upload_images/1881947-29d35adbb8de7859.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
成功之后的图片
（4）配置nginx和rtmp
查看nginx.conf文件路径：
`brew info nginx-full`
![](http://upload-images.jianshu.io/upload_images/1881947-39837ff25a95dcad.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
nginx.conf文件路径
用vim或者其他工具找到这个路径并且打开这个文件
拉到最最下面，添加如下配置:
```
rtmp {
 server {
     listen 1935;
     application rtmplive {
         live on;
         record off;
     }
 }
}
```
然后重启nginx
查看版本号
`nginx -v`
下面的1.10.2就是我的版本号
```bash
/usr/local/Cellar/nginx-full/1.10.2/bin/nginx -s reload
```
（5）安装ffmpeg
执行命令(耐心等待一会)
``brew install ffmpeg``
下载一个支持rtmp协议的视频播放器，推荐`VLC`
（6）ffmpeg推流
事先准备好一个视频，以我桌面上的
以我桌面的一个movie.mp4为例,下面的路径要打对，执行推流命令
```cpp
```cpp
ffmpeg -re -i /Users/lzc/Desktop/movie.mp4 -vcodec libx264 -acodec aac -strict -2 -f flv rtmp://localhost:1935/rtmplive/room
```
```
将视频推流到服务器后，打开VLC，然后File->open network->输入：
`rtmp://localhost:1935/rtmplive/room`
![](http://upload-images.jianshu.io/upload_images/1881947-bc87b94c513314fc.gif?imageMogr2/auto-orient/strip)
效果图
到这里的话视频推流就完成了。
### 二、拉流部分
上面只是实现了推流，拉流的话相当于是用了现成的播放器来实现的，现在记录一下用框架来实现。
直播的框架有很多，这里用的是B站开源的`ijkplayer`，下载地址[https://github.com/Bilibili/ijkplayer](https://github.com/Bilibili/ijkplayer)
关于如何集成看此文[iOS中集成ijkplayer视频直播框架](http://www.jianshu.com/p/1f06b27b3ac0)，按照流程来基本上是可以成功的.
这里如此麻烦的集成是为了制作`IJKMediaFramework.framework`，我的demo里已经做好了，实在不想动手的可以直接去我项目里把这个拖到你自己的工程里使用。
这里我是自己推流，然后同时用VLC和我自己demo运行的结果。
![](http://upload-images.jianshu.io/upload_images/1881947-eec5f7598574e944.gif?imageMogr2/auto-orient/strip)
VLC&demo
这里因为文件过大传不到Github上面（ 限制100M），想看demo的可以去这里看，地址换一下就能用了[Demo地址](https://coding.net/u/wanglei123/p/ijkplayerDemo/git)
如果想自己推流就用下面这个地址，如果只想看拉流效果用上面这个地址.
```
//直播视频
    //self.url = [NSURL URLWithString:@"http://live.hkstv.hk.lxdns.com/live/hks/playlist.m3u8"];
    self.url = [NSURL URLWithString:@"rtmp://localhost:1935/rtmplive/room"];
```
### 结语
感谢前辈们无私的分享,参考文献
- [](http://www.cnblogs.com/oc-bowen/p/5896245.html)[快速集成iOS基于RTMP的视频推流](http://www.cnblogs.com/oc-bowen/p/5896245.html)
- [Mac搭建nginx+rtmp服务器](http://www.cnblogs.com/oc-bowen/p/5896245.html)
- [iOS中集成ijkplayer视频直播框架](http://www.jianshu.com/p/1f06b27b3ac0)
文／zcLu（简书作者）
原文链接：http://www.jianshu.com/p/eafdaa0daa8b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
