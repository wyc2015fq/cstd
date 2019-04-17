# 零基础学习GitHub桌面版-4 怎么给开源项目贡献代码？ - 博客堂 - CSDN博客





2017年09月03日 14:43:02[最小森林](https://me.csdn.net/u012052268)阅读数：939
所属专栏：[零基础学习GitHub桌面版](https://blog.csdn.net/column/details/17109.html)









![](http://ww1.sinaimg.cn/large/006qDjsOgy1g0tdplblplj30ne07q7cc.jpg)




### 文章目录
- [怎么给开源项目贡献代码？](#_4)
- [1点击fork喜欢的项目](#1fork_16)
- [2修改代码](#2_27)
- [2.1 克隆clone的方法](#21_clone_32)
- [2.2 更新到github仓库](#22_github_43)
- [3修改完成后Pull request](#3Pull_request_52)
- [4小问题：原项目已修改？](#4_67)




# 怎么给开源项目贡献代码？

给别人贡献代码，特别是一些开源组织，是件光荣的事。

有两种方式：
- 成为项目合作者，直接改别人的。
- fork别人的项目到自己本地，更改后请求原作者接纳自己的修改。

其中第一方式，给的权限过大（等同于原作者），所以几乎不可能，除非同学同事之间彼此信任。一个大型开源项目往往有几千支持者，不可能都给权限。

所以本章我们着重介绍第二种方法，下面是方法的具体步骤：

## 1点击fork喜欢的项目

在github上找到自己想要贡献代码的地址,如图1点击fork。

![image](https://img-blog.csdn.net/20161229232339422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF2YWdhb3J1aTU5NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后回到自己github主页查看仓库,可以看到我们仓库里也躺着一个一样的项目。

而且还写着原作者是谁，图中红箭头标记了fork的地址
![image](https://img-blog.csdn.net/20161229230038361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF2YWdhb3J1aTU5NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2修改代码

既然已经fork了别人的项目到了自己的主页，那么就可以实现早前的想法了。先用客户端把代码下载clone到本地，把自己想改的都改了。

### 2.1 克隆clone的方法

进入自己的github网站，找到想要复制的项目，进入浏览器的地址栏，复制这个项目的链接。

打开github客户端，点击file菜单 --> 选第三个 clone repsitory，出现以下图片：

![image](https://upload-images.jianshu.io/upload_images/1159344-1e576a3415989bbf.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

在一个栏目填入你刚刚复制的链接，

第二个栏目填入想要存放的本地目录，点击clone即可。

这样项目就下载到本地了。

### 2.2 更新到github仓库

在本地拿到代码之后，修改你想要修改的地方。然后做成版本commit。然后更新到云端。

不会修改和上传的请看我之前的文章：

[零基础学习GitHub桌面版-1 GitHub桌面版的下载安装与使用](http://blog.csdn.net/u012052268/article/details/77720900)

## 3修改完成后Pull request

等你觉得想要把你改的发给原项目同步，就在你的项目上点Pull request按钮.

原有项目就会收到推送消息，原作者查看你的修改之后，如果觉得不好会给你提出意见，觉得好，就你的扯进主分支了。

这样，你也就完成了对开源项目的贡献。

我们来复习一下整个逻辑：
- fork别人的项目到自己帐户
- 从自己账户下载到本地
- 在本地修改
- 把修改上传到自己的github账户
- 从自己的账户向原主人发起pull request

## 4小问题：原项目已修改？

如果你更改代码比较慢，那么有个问题出来了，在你fork他的项目之后，你还没来得及修改完成。而原主人又更新了代码，如果你直接修改老版本，会造成与原作者新版冲突。那么你自己fork的项目怎么做到和原项目同步呢？

**有一个小技巧来解决：**

就是，在本地弄俩仓库分支，一个是对应原作者的远程库，一个对应自己folk出来的远程库。第一个库专管拉原作改动（fetch），第二个库管开发和提交到自己的远程库。

等你改好了第二个分支，就先合并到第一个分支中去（因为第一个与原作者同步），然后再把合并后的整个项目Pull request。




