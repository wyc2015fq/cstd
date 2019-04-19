# 关于GitHub客户端上传代码的使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月04日 10:07:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3408
### 什么是GitHub
在我们实际开发中，经常需要用到Git，我们一般都是直接选择 **downloadzip** ,但是有时候我们需要将自己的项目上传到Git上面。所以，需要我们配置一下。原来都是使用终端的，现在我们可以使用**GitHub客户端**来实现。
### GitHub配置
1.下载github客户端
[github客户端](https://desktop.github.com/)
2.登录GitHub客户端并且进行相关设置
![](https://ooo.0o0.ooo/2016/05/02/57277915563aa.png)
登录
![](https://ooo.0o0.ooo/2016/05/02/57277a6a926d1.png)
设置邮箱
3.创建库->create Repository
![](http://upload-images.jianshu.io/upload_images/1186277-2f36514f7b4fcb83.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建资源库
**PS:本地项目选择时一定要选择文件根目录若如上图所示，提示A repository already exists here则可能是你重复上传github，则会在本地产生相应的.git文件，该文件为隐藏文件，需要删除所有.git目录，可在终端执行以下命令：显示Mac隐藏文件的命令：defaults write com.apple.finder AppleShowAllFiles YES；隐藏Mac隐藏文件的命令：defaults write com.apple.finder AppleShowAllFiles NO；重启finder（command+option+esc，选中finder重启），找到相应工程删除所有文件夹下的.git文件，重新Create Repository即可。**
### 4.提交GitHub
![](http://upload-images.jianshu.io/upload_images/1186277-ecef78478cef3a43.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
同步到github
![](http://upload-images.jianshu.io/upload_images/1186277-0e69b2186da9ee35.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
发布到GitHub
**5、到此为止上传github完毕，登录githu即可看到刚才上传的工程**
这样，我们就可以把一个项目添加到Github上了。
> 
*PS:一定要先填写提交说明信息，在同步到Github上。*
如果大家还有什么问题，可以联系我的[微博](http://weibo.com/2276371262/profile?rightmod=1&wvr=6&mod=personinfo&is_all=1)
文／YanniLiu（简书作者）
原文链接：http://www.jianshu.com/p/92f157e11994
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
