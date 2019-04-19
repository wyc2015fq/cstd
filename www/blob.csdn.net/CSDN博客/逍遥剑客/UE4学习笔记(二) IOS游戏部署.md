# UE4学习笔记(二): IOS游戏部署 - 逍遥剑客 - CSDN博客
2014年05月11日 22:11:29[xoyojank](https://me.csdn.net/xoyojank)阅读数：8645
自己的超级本挂了后, 一直没有电脑用
正好休假回来经过香港时赶上macbook降价, 就是入了个
# 过程
UE4最大的改变就是为移动游戏做了专门适配和优化, 所以先拿个demo来跑跑看
以最简单的2D小游戏为例, 首先从市场里下载来下:
![](https://img-blog.csdn.net/20140511215508281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后部署到iPhone
![](https://img-blog.csdn.net/20140511215548468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
运行(本文完)
![](https://img-blog.csdn.net/20140511220857640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 填坑
整个过程其实挺简单, 就是会遇到一些问题, 就一一记录下吧:
- 设备列表里没有iPhone, 打开iTunes一次就可以了
- 第一次部署不成功, 但是xcode的工程文件已经生成了, 打开后用xcode重新运行也不行- ![](https://img-blog.csdn.net/20140511215710953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 看了下我手机是7.0.4(越狱)的, 所以需要在xcode5.1.1里[安装ios7.0的sdk](http://blog.csdn.net/xdrt81y/article/details/8961718)
- 真机调试需要开发者权限, 我只是玩玩, 就[破解个吧](http://bobzy.blog.51cto.com/2109336/1346061)
# 新玩意儿
这是个2D游戏, 也算是UE4新加的东西, 跟Unity一样的思路, 都是用3D空间的层加个正交投影的相机
![](https://img-blog.csdn.net/20140511222227406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这游戏没写代码, 全是拖结点拖出来的-_-, 程序员表示不如写代码
不过Blueprint本来就不是给程序用的, 用来做一些关卡逻辑不错
![](https://img-blog.csdn.net/20140511221750171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
