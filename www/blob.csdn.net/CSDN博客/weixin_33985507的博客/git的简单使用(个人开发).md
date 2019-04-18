# git的简单使用(个人开发) - weixin_33985507的博客 - CSDN博客
2017年03月24日 16:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
git  分布式版本控制
1.每个本地仓库都有备份版本
![2673147-b6928c66b65b3fae.png](https://upload-images.jianshu.io/upload_images/2673147-b6928c66b65b3fae.png)
31.png
2.多人开发下的模式
![2673147-4bd4620d530e0e45.png](https://upload-images.jianshu.io/upload_images/2673147-4bd4620d530e0e45.png)
30.png
3.多了一个工作暂缓区所以要先添加(复制)到工作暂缓区add.
![2673147-f7a487313a9a0287.png](https://upload-images.jianshu.io/upload_images/2673147-f7a487313a9a0287.png)
35.png
4.暂缓区再提交到本地仓库（剪切）
![2673147-1acc2aad4f33567d.png](https://upload-images.jianshu.io/upload_images/2673147-1acc2aad4f33567d.png)
33.png
一.命令行
git help (会显示出git 所有的命令行操作,像我这样看不懂英文的同学求助有道吧)
git help init (会显示出git 的命令行操作用法)
步骤:
1.git init 初始化本地仓库
查看隐藏的.git 文件                                      (打开终端，输入：
defaults write com.apple.finder AppleShowAllFiles -bool true
此命令显示隐藏文件
defaults write com.apple.finder AppleShowAllFiles -bool false
此命令关闭显示隐藏文件
命令运行之后需要重新加载Finder：快捷键option+command+esc，
选中Finder，重新启动即可)
![2673147-95f669c0a4f404fa.png](https://upload-images.jianshu.io/upload_images/2673147-95f669c0a4f404fa.png)
2.png
2.git config 配置仓库
git config user.name "admin" (配置本地名字)
git config [user.email](https://link.jianshu.com?t=http://user.email)  "[568986318@qq.com](https://link.jianshu.com?t=mailto:568986318@qq.com)"
git config -l                                 查看本地配置
![1.png]([http://upload-images.jianshu.io/upload_images/2673147-](https://link.jianshu.com?t=http://upload-images.jianshu.io/upload_images/2673147-)  b22ae4deba9445c4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
(如果没配置 他就会读区默认的)
git status 查看git 状态
![2673147-3c0ac85caa483822.png](https://upload-images.jianshu.io/upload_images/2673147-3c0ac85caa483822.png)
3.png
3.git add .                     把文件添加到暂缓区
![2673147-347045a4dabe0c5d.png](https://upload-images.jianshu.io/upload_images/2673147-347045a4dabe0c5d.png)
20.png
4.git commit -m "111"  文件提交到本地仓库
111 和svn 里面commit要提交的时候写的东西一样是说明这次做了什么修改
![2673147-84449c8c7d4db16f.png](https://upload-images.jianshu.io/upload_images/2673147-84449c8c7d4db16f.png)
21.png
5.git log
和svn log 一样 是查看了本机做了什么操作
![2673147-1e6e9bc4ad049da6.png](https://upload-images.jianshu.io/upload_images/2673147-1e6e9bc4ad049da6.png)
22.png
- 
git rm
手动删除实体文件 和 命令行操作 删除文件的区别是
手动删除手你还要自己commit 到本地仓库
![2673147-4a8818698c8fc7a4.png](https://upload-images.jianshu.io/upload_images/2673147-4a8818698c8fc7a4.png)
23.png
![2673147-2a5a27e7366edff0.png](https://upload-images.jianshu.io/upload_images/2673147-2a5a27e7366edff0.png)
24.png
- 
git reset --hard HEAD 和svn 顶部rever 的功能一样
git reset --hard HEAD^^(回退到向上2个版本 几个^就是几个版本)
git reset --hard 7位版本号(回退到指定版本)
git reflog 查看所有修改版本的7位版本号
![2673147-2ff041f9ac7e4d35.png](https://upload-images.jianshu.io/upload_images/2673147-2ff041f9ac7e4d35.png)
27.png
