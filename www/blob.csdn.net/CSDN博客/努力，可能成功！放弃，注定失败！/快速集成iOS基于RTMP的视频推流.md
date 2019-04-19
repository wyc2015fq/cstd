# 快速集成iOS基于RTMP的视频推流 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月22日 14:50:45[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4343
## 效果图
iTools有点卡, 但是推到服务器倒是很快的.
![](http://upload-images.jianshu.io/upload_images/1038348-95b0e4d6d64422ec.gif?imageMogr2/auto-orient/strip)
推流
## 前言
这篇blog是[iOS视频直播初窥:<喵播APP>](http://www.jianshu.com/p/b8db6c142aad)的一个补充. 因为之前传到[github](https://github.com/SunLiner/MiaowShow)上的项目中没有集成视频的推流.有很多朋友`简信`和`微博`上问我推流这部分怎么实现的.
 所以, 我重新集成了`RTMP`的推流, 合并到了之前的[项目](https://github.com/SunLiner/MiaowShow)中.
## 下载链接
[github(完整版视频直播)](https://github.com/SunLiner/MiaowShow)
[blog详解(建议大家结合这篇一起看)](http://www.jianshu.com/p/b8db6c142aad)
## Mac搭建nginx+rtmp服务器
> 
我个人电脑里面很早以前就搭建过`nginx+rtmp`服务器, 但是昨天在公司电脑上搭建的时候, 手动输入命令的时候, 还是出现了bug(所以, 建议大家直接复制命令, 不要手动输入命令). 所以记录一份详细的搭建步骤, 防止再次跳坑...
此段教程参考自[推酷:Mac搭建nginx+rtmp服务器](http://www.tuicool.com/articles/muya6rz)
- - 打开终端, 查看是否已经安装了`Homebrew`, 直接终端输入命令
`man brew`
如果Mac已经安装了, 会显示一些命令的帮助信息. 此时输入Q退出即可, 直接进入第二步.
反之, 如果没有安装,执行命令
```bash
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
如果安装后, 想要卸载
```bash
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/uninstall)"
```
- 2.安装nginx
先clone nginx项目到本地
`brew tap homebrew/nginx`
执行安装：
``brew install nginx-full --with-rtmp-module``
此时, nginx和rtmp模块就安装好了
输入命令:
`nginx`
在浏览器里打开[http://localhost:8080](http://localhost:8080)
如果出现下图, 则表示安装成功
![](http://upload-images.jianshu.io/upload_images/1038348-a7171240f3eae6c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
安装成功
如果终端上提示
> 
nginx: [emerg] bind() to 0.0.0.0:8080 failed (48: Address already in use)
nginx: [emerg] bind() to 0.0.0.0:8080 failed (48: Address already in use)
nginx: [emerg] bind() to 0.0.0.0:8080 failed (48: Address already in use)
nginx: [emerg] bind() to 0.0.0.0:8080 failed (48: Address already in use)
.....
则表示`8080`端口被占用了, 查看端口PID
```cpp
```cpp
lsof -i tcp:8080
```
```
根据端口PID, kill掉(这儿的`9603`换成你自己`8080`端口的PID)
```bash
kill 9603
```
然后重新执行`nginx`, 打开[http://localhost:8080](http://localhost:8080)
- 3.配置nginx和ramp
首先我们查看`nginx`安装到哪了
`brew info nginx-full`
如图, 找到`nginx.conf`文件所在位置
![](http://upload-images.jianshu.io/upload_images/1038348-f63fe72acb11319c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
nginx.conf
通过`vim`或者点击`Finder`->`前往`->`前往文件夹`->`输入/usr/local/etc/nginx/nginx.conf`->用记事本工具(推荐`Sublime Text`)打开`nginx.conf`.
直接滚到最后一行, 在最后一个`}`(即最后的空白处, 没有任何`{}`)后面添加
```
# 在http节点后面加上rtmp配置：
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
然后重启nginx(其中的1.10.1要换成你自己安装的`nginx`版本号, 查看版本号用`nginx -v`命令即可)
```bash
/usr/local/Cellar/nginx-full/1.10.1/bin/nginx -s reload
```
昨天, 就在执行这段命令的时候, 坑出现了.(如果你命令执行成功, 重启成功了, 没有没有任何异常, 请直接跳转到下一步)
执行命令的时候, 报异常
```bash
nginx: [emerg] unknown directive “rtmp” in /usr/local/etc/nginx/nginx.conf:119
```
`rtmp`成了未知指令. 我赶紧Google, 将其中的答案都试了一遍, 这个bug涛声依旧. 最后实在没办法了, `brew list`查看所有安装的模块, 才发现是自己手动敲命令的时候, 将`nginx-full`变成了`nginx -full`.
上解决方案:
卸载`nginx`
``brew uninstall nginx``
再次安装`nginx`
``brew install nginx-full --with-rtmp-module``- - 安装ffmpeg
执行命令
``brew install ffmpeg``
安装`ffmpeg`时间就要长一点了. 如果速度过慢, 建议翻墙. 不过也才50多M的东西, 耐心一点就好. 等待的时间里, 再安装一个支持rtmp协议的视频播放器`VLC`
- - ffmpeg推流
以我桌面的一个`loginmovie.mp4`(自己准备一个视频文件, 也可以用我[github](https://github.com/SunLiner/MiaowShow)项目中`Resource`文件下的两个视频文件)为例, 执行推流命令
> 
ffmpeg -re -i /Users/sunlin/Desktop/loginmovie.mp4 -vcodec libx264 -acodec aac -strict -2 -f flv rtmp://localhost:1935/rtmplive/room
将视频推流到服务器后，打开VLC，然后File->open network->输入：
> 
rtmp://localhost:1935/rtmplive/room
效果如图
![](http://upload-images.jianshu.io/upload_images/1038348-10fd8e48e415eed3.gif?imageMogr2/auto-orient/strip)
推流
## 快速集成直播推流
前提条件: 
- 去[github](https://github.com/SunLiner/MiaowShow)上下载最新的喵播项目.
- 根据上面教程配置好nginx+rtmp服务器
> 
原先准备自己实现一下直播推流,但是今天在技术群聊的时候, 有朋友推荐了优酷土豆旗下`来疯直播`前不久开源的iOS推流框架[LFLiveKit](https://github.com/LaiFengiOS/LFLiveKit). 是用OC写的, 很适合学习(我也准备深入研究一下这个框架, 有什么疑问大家可以一起探讨), 集成也非常简单, 几句代码就OK了.
下载好我写的[喵播](https://github.com/SunLiner/MiaowShow)后, 在Xcode左下方直接搜索`ShowTimeViewController`, 进入`ShowTimeViewController.m`. 搜索`- (IBAction)living:(UIButton *)sender`, 将里面的`stream.url`替换成你自己的rtmp地址即可.
> 
tip: `LFLiveKit`已经集成了`GPUImage`, 如果项目中有集成`GPUImage`, 需要将之前的移除掉. 且集成`LFLiveKit`需要关闭`Bitcode`.
## 结案陈词
现在的[喵播](https://github.com/SunLiner/MiaowShow)算得上是一个比较完整的视频直播项目了, 虽然用了很多第三方, 但是也节省了开发的时间, 提升了开发效率.
后期这个项目我还会不断的完善, 如果你有什么好的idea或者效果, 可以微博私信我, 如果我能搞定, 我会尽快加上去的.
## github下载地址
[完整版视频直播:喵播下载地址](https://github.com/SunLiner/MiaowShow)
## 联系我
[github](https://github.com/SunLiner)
[简书](http://www.jianshu.com/users/9723687edfb5/latest_articles)
文／Monkey_ALin（简书作者）
原文链接：http://www.jianshu.com/p/8ea016b2720e
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
