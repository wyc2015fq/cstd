# Android Studio开发第一篇QuickStart - weixin_33985507的博客 - CSDN博客
2015年06月21日 14:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
为什么把as的环境开发放在gradle里呢，因为eclipse里装gradle插件还是不够方便，as直接集成了，然后正好as也是大势所趋，学习一下正好。
![](https://images0.cnblogs.com/blog2015/307289/201506/152242110137212.jpg)
看到右边Quick Start快速启动栏下面
1.开始一个新studio项目
2.打开已存在的studio项目 
3.导入sample示例项目
4.从版本控制器检出
5.导入eclipse/gradle项目
6.配置
一个一个点点看。
**2.打开已存在的studio项目 ：**
先不看创建新项目，看第二个，打开一个已存在as项目:
![](https://images0.cnblogs.com/blog2015/307289/201506/152250567163207.jpg)
这这个截图可以看出as还是很贴心的，在as项目所在目录会显示一个淡绿色和深绿色图案的文件夹，一个断了的环中间一点的就代表是Gradle项目，一个圆规盘的就是as项目，如果你的目录非常多的话一眼就看出哪个目录有as项目，而as项目工程目录会以android studio图标来显示，这样就很容易区分哪些目录是studio项目哪些不适studio项目，简直太赞啊。
**3.导入sample示例项目**
这个每次都会失败啊，可能天朝网络你懂的。
**4.从版本控制器检出**
as支持github/cvs/git/google cloud/svn等
 　 其实版本控制器是需要设置的，要不然第一次就是会提示 git.exe未找到，SVN或者其他就没去试
![](https://images0.cnblogs.com/blog2015/307289/201506/211436360294563.jpg)
　　上面也有GitHub设置
![](https://images0.cnblogs.com/blog2015/307289/201506/211437523268920.jpg)
**5.导入eclipse/gradle项目**
当然as肯定还是要兼容eclipse项目的，还有gralde。
![](https://images0.cnblogs.com/blog2015/307289/201506/211442112329355.jpg)
![](https://images0.cnblogs.com/blog2015/307289/201506/211442529519910.jpg)
　　如果从eclipse项目结构会弹出提示转换成as的项目结构
![](https://images0.cnblogs.com/blog2015/307289/201506/211443540297151.jpg)
最前沿Android技术分享尽在Android技术分享社，拿起你们的手机打开微信扫一扫，关注我的公众号就给你推荐优秀的知识文章或技术分享了！
![](https://images0.cnblogs.com/blog2015/307289/201506/222210184709415.jpg)
