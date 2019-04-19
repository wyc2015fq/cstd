# 我的高仿bilibili项目成长之路 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月01日 14:49:55[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：176
![(logo)](http://img2.imgtn.bdimg.com/it/u=2951591626,706941868&fm=21&gp=0.jpg)
(logo)
## 高仿bilibili项目
Github链接:[(https://github.com/MichaelHuyp/Bilibili_Wuxianda)](https://link.jianshu.com?t=https://github.com/MichaelHuyp/Bilibili_Wuxianda)
目前完成了如下功能：
- 启动页的业务逻辑
- 带有阻力效果的轮播图
- 直播、推荐、番剧界面的搭建
- 基于ijkplayer的视频播放器（普通视频、直播视频）
七月三十号更新:
- 
个人中心模块的搭建
- 
集成基于来疯直播开源的[LFLiveKit](https://link.jianshu.com?t=https://github.com/LaiFengiOS/LFLiveKit)的手机直播功能
- RTMP推流
- 镜像 、美颜、手电筒、摄像头方向
- 
更新展示
![](https://upload-images.jianshu.io/upload_images/695203-1f874a12ed703ef3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/639/format/webp)
![](https://upload-images.jianshu.io/upload_images/695203-b85c761c3739c565.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
关于电脑上如何查看直播：
- 首先确保你的电脑是否安装了Homebrew
```
man brew
```
- 如果没有安装，则在终端输入以下命令安装
```
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
- 第一步
```
brew tap homebrew/nginx
```
- 第二步
```
brew install nginx-full --with-rtmp-module
```
- 第三步,在终端输入
```
nginx
```
- 第四步，打开浏览器打开[http://localhost:8080](https://link.jianshu.com?t=http://localhost:8080) 如果出现以下所示，那么环境就搭建好了
![](https://upload-images.jianshu.io/upload_images/695203-e6cf6f76009c004a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/546/format/webp)
demo
- 第五步,在终端输入
```
brew info nginx-full
```
- 第六步，在终端 Command+F 搜索nginx.conf
![](https://upload-images.jianshu.io/upload_images/695203-45cec11a4ab0aa1f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/570/format/webp)
demo
- 第七步，进入该路径,以文本编辑形式打开配置文件，直接在最后面插入以下代码
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
![](https://upload-images.jianshu.io/upload_images/695203-f42f7f087c9eadaf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/708/format/webp)
demo
![](https://upload-images.jianshu.io/upload_images/695203-e13e520e95b8edbe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/607/format/webp)
demo
- 第八步，查看nginx版本号，在终端输入:
```
nginx -v
```
![](https://upload-images.jianshu.io/upload_images/695203-feeafff912f6a1d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/229/format/webp)
demo
- 第九步，重启ngix,把版本号替换成你电脑的nginx的版本号
```
/usr/local/Cellar/nginx-full/1.10.1/bin/nginx -s reload
```
接下来就可以试下直播的效果了**
- 下载VLC客户端
![](https://upload-images.jianshu.io/upload_images/695203-fa3a6a16733d091c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/747/format/webp)
demo
- 打开项目代码，找到YPPhoneLiveViewController.m文件，将你电脑的ip替换到此处
![](https://upload-images.jianshu.io/upload_images/695203-6159a1cac3a5c09c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
demo
- 运行项目，点击个人信息界面的我要直播,然后点击开始直播,然后打开VLC客户端，然后File->open network->输入：rtmp://(你的ip):1935/rtmplive/room
![](https://upload-images.jianshu.io/upload_images/695203-9d1cc0e1bc4e1ce4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/622/format/webp)
demo
![](https://upload-images.jianshu.io/upload_images/695203-21d0c2cb3220b0b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/574/format/webp)
demo
- 好了如果顺利了话。。。。 就可以测试一下直播了[](￣▽￣)*
项目展示
启动页
![](https://upload-images.jianshu.io/upload_images/695203-3896c93b0b0dcac5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/639/format/webp)
![](https://upload-images.jianshu.io/upload_images/695203-b0231314002a83ee.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
推荐界面
![](https://upload-images.jianshu.io/upload_images/695203-0086e7337c32c951.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
![](https://upload-images.jianshu.io/upload_images/695203-b4004dbff4ec58a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/639/format/webp)
直播以及番剧界面
![](https://upload-images.jianshu.io/upload_images/695203-bc529cdba271f442.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
![](https://upload-images.jianshu.io/upload_images/695203-ee799f3c46eddfdd.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
## 基于ijkplayer的播放器
![](https://upload-images.jianshu.io/upload_images/695203-308a6ad47fb60b42.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/695203-210c16c75cad24a9.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
![](https://upload-images.jianshu.io/upload_images/695203-d188131bbd57c36d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
# 写在最后
- 希望能把这个项目写完，还有许许多多的功能点没有写，持续更新中...
### 每天进步一点点... Bilibili - ( ゜- ゜)つロ乾杯~
作者：MichaelHuyp
链接：https://www.jianshu.com/p/09eebb030331
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
