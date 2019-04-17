# 零基础学习GitHub桌面版-6使用pages创建网站 - 博客堂 - CSDN博客





2018年01月18日 22:05:25[最小森林](https://me.csdn.net/u012052268)阅读数：260标签：[git																[github																[博客																[hexo](https://so.csdn.net/so/search/s.do?q=hexo&t=blog)
个人分类：[git](https://blog.csdn.net/u012052268/article/category/6897281)

所属专栏：[零基础学习GitHub桌面版](https://blog.csdn.net/column/details/17109.html)](https://so.csdn.net/so/search/s.do?q=博客&t=blog)




![image](https://img.ctolib.com/uploadImg/AED45344E96ABCF311C2C7421134D170/90.jpeg)


- [使用pages创建网站](#0-使用pages创建网站)
- [Git和GitHub的相关内容略](#1-git和github的相关内容略)- [1开启gh-pages功能](#11开启gh-pages功能)

- [fork现成的blog](#2-fork现成的blog)- [1选择你喜欢的风格](#21选择你喜欢的风格)
- [2 fork项目](#22-fork项目)

- [发布新博文](#3-发布新博文)



# 0 使用pages创建网站

折腾了几天，尝试用github来搭建自己的个人博客，一直没有找到好的方法。今天看到了一种最简单的搭建教程有所进展，现在试试将自己的经历写下来。

网上有很多帖子讲了要怎么使用github的page服务来搭建个人网站。但是涉及到很多东西。这里介绍最简单的办法——直接fork一个现成的blog。

是的，你没看错！你喜欢谁的博客就把谁的copy下来即可！github本就是一个开放分享的平台~！！

# 1 Git和GitHub的相关内容（略）

这里略去了如何创建github账号，以及git、github的基本用法。 

参考本系列前面的文章：[零基础学习GitHub桌面版](http://blog.csdn.net/column/details/17109.html)

## 1.1开启gh-pages功能

点击界面右侧的Settings，你将会打开这个库的setting页面，向下拖动，直到看见GitHub Pages，如图：

![image](https://d33wubrfki0l68.cloudfront.net/14a9e720f5f49408933dc57771017e0c6b09e7f7/e535d/imgs/opinion/github-pages.png)

点击Automatic page generator，Github将会自动替你创建出一个gh-pages的页面。 如果你的配置没有问题，那么大约15分钟之后，yourname.github.io这个网址就可以正常访问了~ 如果yourname.github.io已经可以正常访问了，那么Github一侧的配置已经全部结束了。

# 2 fork现成的blog

下面进行最重要也是最爽的一步，fork其他博主现成的blog。

## 2.1选择你喜欢的风格

其实page博客只有几种基础款：Jekyll 、Hexo等，目前是Node.js编写的Hexo比较流行也更为方便。有很多人基于Hexo修改成了自己的风格，我们下面选择几款优秀的改编介绍一下：

下面介绍几个推荐的模板。
- 效果：[http://huangxuan.me/](http://huangxuan.me/)

![image](https://pic4.zhimg.com/80/4b3c678f7c23067a1975bbc20f6711ea_hd.jpg)

![image](https://pic1.zhimg.com/80/9cf3e3b8938341224d126947e9f25d4b_hd.jpg)
- 
复制地址：[https://github.com/Huxpro/huxpro.github.io](https://github.com/Huxpro/huxpro.github.io)
- 效果：[http://litten.me/](http://litten.me/)


![image](https://cloud.githubusercontent.com/assets/2024949/19027861/92879edc-8967-11e6-8e60-7987b6507c8d.gif)
- 
复制地址：[https://github.com/litten/hexo-theme-yilia](https://github.com/litten/hexo-theme-yilia)
- 效果：[http://notes.iissnan.com/](http://notes.iissnan.com/)


![image](https://camo.githubusercontent.com/7a9f96afc55fa895abef31b27cba7986d245ebb3/687474703a2f2f696973736e616e2e636f6d2f6e657875732f6e6578742f746f6d6f72726f772d6e6f726d616c2e706e67)

复制地址：[https://github.com/iissnan/hexo-theme-next](https://github.com/iissnan/hexo-theme-next)
- 效果：[https://luuman.github.io/](https://luuman.github.io/)

![image](https://camo.githubusercontent.com/2a9ba29b5b8f35ad506629cefc1523b3190ceb62/68747470733a2f2f6865786f2e696f2f6275696c642f73637265656e73686f74732f7370666b2d303235643163643832302e706e67)
- 
复制地址：[https://github.com/luuman/hexo-theme-spfk](https://github.com/luuman/hexo-theme-spfk)
- 效果：[https://blog.viosey.com](https://blog.viosey.com)


![image](https://camo.githubusercontent.com/1e9260f301fe915fb3e4826831fde943229523cb/68747470733a2f2f692e6c6f6c692e6e65742f323031372f30392f30372f353962313336376637366664622e706e67)
- 
复制地址：[https://github.com/viosey/hexo-theme-material](https://github.com/viosey/hexo-theme-material)
- 效果：[https://www.ezlippi.com/](https://www.ezlippi.com/)


复制地址：[https://github.com/EZLippi/EZLippi.github.io](https://github.com/EZLippi/EZLippi.github.io)

我这里选择的是第6个（最后一个），[https://github.com/EZLippi/EZLippi.github.io](https://github.com/EZLippi/EZLippi.github.io)

## 2.2 fork项目

我们首先在GitHub中找到自己想要fork的博客仓库。比如这个 [https://github.com/EZLippi/EZLippi.github.io](https://github.com/EZLippi/EZLippi.github.io)， 

然后我们将它fork到我们自己的GitHub上。

这时你的GitHub仓库中就多了一个叫EZLippi.github.io的仓库。假设你的GitHub用户名是tom，因为GitHub Pages服务的原因，我们需要将刚刚fork的EZLippi.github.io的名字修改为：tom.github.io。修改方式为在当前库中点击setting修改名字。这样修改好之后，就可以通过

> 
[https://tom.github.io/](https://tom.github.io/)


来访问你的博客啦。这里要注意的是，一定要用你的用户名来给仓库命名。

# 3 发布新博文

在_posts文件夹中新建文件，文件命名是有要求的，必须是日期+英文文件名+md。比如：2017-07-07-how-to-build-blog.md。

建立新的文件后就可以开始写作了。但要注意的是每篇博文的最开头一定要插入一段代码，指明博文的相关属性，这样github才能够正确解析它，用户也才能看到正确显示的格式。通常需要设置的信息有如下几个：

```
---
layout: post  
title: 搭建博客  
categories: [blog ]  
tags: [blog, ]  
description: 「这个博客是怎么搭建起来的」   
---
```
- layout: 表示你要用的模版，一般就用默认的就行
- title：就是你的这篇博文的名字
- categories：表示这篇博文所属的类别
- tags：表示这篇博文要打的标签，如果有多个标签，需要用英文半角逗号分隔开
- description：就是概括下整篇博文的主要内容](https://so.csdn.net/so/search/s.do?q=github&t=blog)](https://so.csdn.net/so/search/s.do?q=git&t=blog)




