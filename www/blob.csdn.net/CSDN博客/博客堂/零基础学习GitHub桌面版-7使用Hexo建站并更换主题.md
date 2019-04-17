# 零基础学习GitHub桌面版-7使用Hexo建站并更换主题 - 博客堂 - CSDN博客





2018年02月01日 21:59:12[最小森林](https://me.csdn.net/u012052268)阅读数：465
所属专栏：[零基础学习GitHub桌面版](https://blog.csdn.net/column/details/17109.html)










- [0什么是 Hexo](#0什么是-hexo)
- [安装Hexo](#1-安装hexo)- [1安装前提](#11安装前提)
- [2安装 Git](#12安装-git)
- [3安装 Nodejs](#13安装-nodejs)
- [4安装 Hexo](#14安装-hexo)
- [5 遇到的问题](#15-遇到的问题)

- [2建站](#2建站)- [1简单的配置](#21简单的配置)

- [3发布](#3发布)
- [HEXO主题](#4-hexo主题)- [1 安装主题](#41-安装主题)
- [2更新](#42更新)




# 0什么是 Hexo？

Hexo 是一个快速、简洁且高效的博客框架。Hexo 使用 [Markdown](http://daringfireball.net/projects/markdown/)（或其他渲染引擎）解析文章，在几秒内，即可利用靓丽的主题生成静态网页。

配合Hexo与github可以免费、快速配置出强大、绚丽的个人网站！哈哈，动心了把？一起来做把。

# 1 安装Hexo

安装 Hexo 只需几分钟时间，若您在安装过程中遇到问题或无法找到解决方式，请提交问题，我会尽力解决您的问题。

## 1.1安装前提

安装 Hexo 相当简单。然而在安装前，您必须检查电脑中是否已安装下列应用程序：
- Node.js
- Git

如果您的电脑中尚未安装所需要的程序，请根据以下安装指示完成安装。

## 1.2安装 Git：
- Windows：下载并安装 [git](https://git-scm.com/download/win).
- Mac：使用 Homebrew, MacPorts ：brew install git;或下载 [安装程序](http://sourceforge.net/projects/git-osx-installer/) 安装。
- Linux (Ubuntu, Debian)：sudo apt-get install git-core
- Linux (Fedora, Red Hat, CentOS)：sudo yum install git-core

## 1.3安装 Node.js

安装过程参考：[http://www.runoob.com/nodejs/nodejs-install-setup.html](http://www.runoob.com/nodejs/nodejs-install-setup.html)

linux下安装： 

安装 Node.js 的最佳方式是使用 nvm。

cURL:
`$ curl https://raw.github.com/creationix/nvm/master/install.sh | sh`
Wget:
`$ wget -qO- https://raw.github.com/creationix/nvm/master/install.sh | sh`
安装完成后，重启终端并执行下列命令即可安装 Node.js。
`$ nvm install stable`
Windows直接下一步下一步安装：可以下载 [安装程序](http://nodejs.org/) 来安装。

## 1.4安装 Hexo

所有必备的应用程序安装完成后，即可使用 npm 安装 Hexo。

> 
$ npm install -g hexo-cli


## 1.5 遇到的问题
- 报错： /usr/bin/env: node: No such file or directory

解决办法：

```bash
sudo rm /usr/bin/node
sudo ln -s /usr/bin/nodejs /usr/bin/node
```
- 此时若出现如下报错：

```
ERROR Local hexo not found in ~/blog
ERROR Try runing: 'npm install hexo --save'
```

则执行命令：

```bash
sudo npm install hexo --save
```
- 发布文章中出现的错误：

```
$ hexo deploy
ERROR Deployer not found: git
```

解决办法：安装  hexo-deployer-git:
`sudo npm install hexo-deployer-git --save`
然后设置你的github地址和名字：

```
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
```

# 2建站

Hexo安装好了之后，就开始进行建站。打开终端cd到桌面并使用如下命令即可建好:

```bash
hexo init yourname
cd yourname
npm install
```

其中yourname是你的文件夹名字可随意取（本文章假设yourname的文件夹名称是Hexo）。

建站好了之后需要了解更多的信息和其他步骤请参考官网的这篇文档。[https://hexo.io/zh-cn/docs/setup.html](https://hexo.io/zh-cn/docs/setup.html)

这里需要特别提一下，官方的文档里并没讲解如何配置与Github pages进行关联，在此特意说一下配置信息。进入到你的站点（使用hexo init yourname命令时，这里的yourname文件夹目录，刚假设yourname是Hexo，所以我们进入Hexo目录），然后以文本编辑器打开_config.yml文件，并滚动到最下面添加如下配置信息（注意最下边有deploy和type字段，覆盖这两个字段或者删除这两个字段然后复制下面的四个字段也行。）：

```
deploy:
type: git
repo: https://github.com/xqtbox/xqtbox.github.io.git
branch: master
```

把其中repo字段的值替换成你的github pages提交代码的git地址。

> 
你的github pages的git提交地址。。。如图：


![image](https://upload-images.jianshu.io/upload_images/660127-b37a60a1ce6ac073.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

好吧，到此你使用终端，然后进入到你的站点文件夹使用hexo s命令，如果成功会打印类似Hexo is running at [http://localhost:4000/](http://localhost:4000/). Press Ctrl+C to stop的一句话，再打开你的浏览器输入localhost:4000地址见证神奇的一刻吧。

## 2.1简单的配置

您可以在 _config.yml 中修改大部份的配置。
- 网站

```
参数  描述
title   网站标题
subtitle    网站副标题
description 网站描述
author  您的名字
language    网站使用的语言
timezone    网站时区。Hexo 默认使用您电脑的时区。时区列表。比如说：America/New_York, Japan, 和 UTC 。
```

其中，description主要用于SEO，告诉搜索引擎一个关于您站点的简单描述，通常建议在其中包含您网站的关键词。author参数用于主题显示文章的作者。 

2. 网址

```
参数  描述  
url 网址  
root    网站根目录   
permalink   文章的 永久链接 格式 :year/:month/:day/:title/
permalink_defaults  永久链接中各部分的默认值
```

网站存放在子目录

> 
如果您的网站存放在子目录中，例如 [http://yoursite.com/blog](http://yoursite.com/blog)，则请将您的 url 设为 [http://yoursite.com/blog](http://yoursite.com/blog) 并把 root 设为 /blog/。


其他集体配置参考官网：[https://hexo.io/zh-cn/docs/configuration.html](https://hexo.io/zh-cn/docs/configuration.html)

# 3发布

此时。我们的博客只是本地跑起来了，而你的github pages服务器上并没有，所以你就需要在你的站点里使用终端命令进行发布：

```
hexo clean
hexo g 
hexo d
```

命令详解，第一条是清楚缓存，第二条命令是生成本地发布文件夹，第三条命令才是最后的发布到github pages上。更多的hexo命令操作请参考官方文档即可。不过一般用来用去无非就是创建页面、发布这么几条命令而已。Hexo[官方命令参考文档](https://link.jianshu.com/?t=https://hexo.io/zh-cn/docs/commands.html)

> 
其实学习一个新东西有事没事多去官方看文档，比在网上查资料要来的更靠谱的多。


其中出现的错误：

```
$ hexo deploy
ERROR Deployer not found: git
```

解决办法：安装  hexo-deployer-git:
`sudo npm install hexo-deployer-git --save`
然后设置你的github地址和名字：

```
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
```

这样在你的网站中，就出现了第一篇文章helloworld。 下一篇我再写如果写文章。

# 4 HEXO主题

如果你到了这里没有任何问题，那么恭喜你已经成功了，不过这才刚刚开始。

当你成功的看到自己博客搭建好的那一刻又是激动又是失望，激动的是博客总算折腾出来了，失望的是，为何如此的丑。。。说实话Hexo默认的主题自我感觉还过得去，如果你想换风格,Hexo的主题网上随便一搜也有很多。

在此笔者使用的博客主题是icarus。这个网站 [https://github.com/iissnan/hexo-theme-next](https://github.com/iissnan/hexo-theme-next) 给了详细的主题安装过程。

## 4.1 安装主题
- 进入你的网页文件夹，里面必须有这些文件：node_modules, source, themes and other directories:

```bash
$ cd myblog
$ ls
_config.yml  node_modules  package.json  public  scaffolds  source  themes
```
- 从github获取主题：

在主题文件夹里面建立一个主题文件夹，然后下载最新：

```
$ mkdir themes/icarus
$ curl -s https://api.github.com/repos/ppoffice/hexo-theme-icarus/releases/latest | grep tarball_url | cut -d '"' -f 4 | wget -i - -O- | tar -zx -C themes/icarus --strip-components=1
```

## 4.2更新

```bash
cd themes/icarus
git pull
```

在站点配置文件_config.yml中更改：
`theme: icarus`
到这里你的博客就初出茅庐啦~哈哈 欣赏一下： 
![](http://oqy7bjehk.bkt.clouddn.com/18-1-18/92475206.jpg)



