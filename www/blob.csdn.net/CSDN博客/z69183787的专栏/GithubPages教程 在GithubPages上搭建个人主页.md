# GithubPages教程 在GithubPages上搭建个人主页 - z69183787的专栏 - CSDN博客
2018年02月01日 15:54:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：157
个人分类：[版本管理-Git](https://blog.csdn.net/z69183787/article/category/2857957)
[http://blog.csdn.net/yanzhenjie1003/article/details/51703370](http://blog.csdn.net/yanzhenjie1003/article/details/51703370)
# GithubPages教程 在GithubPages上搭建个人主页
> 
版权声明：转载请注明本文转自严振杰的博客: [http://blog.csdn.net/yanzhenjie1003](http://blog.csdn.net/yanzhenjie1003)
　　对我的博客感情趣的同学可以加[我博客](http://blog.csdn.net/yanzhenjie1003)左侧的群来一起交流学习，共同进步。本篇博客公有三个目的，同样也会举三个实际操作的例子来和大家一起玩
```
Github
 pages
```
。
　　第一个目的，有的开发者平常会做自己的网站，大部分是还是买的空间吧，有人买的阿里云、新浪SEA或者linode、aws加nginx，今天看完这篇博客，你就可以用`Github Pages`免费搭建自己的主页了。
　　第二个目的，如何在Github为自己的帐号（个人或者组织）搭建个人主页，比如我的Github地址是：[https://github.com/yanzhenjie](https://github.com/yanzhenjie)，我为自己的帐号搭建的主页就是[https://yanzhenjie.github.io](https://yanzhenjie.github.io/)。
　　第三个目的，如何为自己在Github上的开源项目搭建项目主页，比如我的开源项目[NoHttp](https://github.com/yanzhenjie/NoHttp)的Github地址是：[https://github.com/yanzhenjie/NoHttp](https://github.com/yanzhenjie/NoHttp)，我为NoHttp搭建的项目主页就是[https://yanzhenjie.github.io/NoHttp](https://yanzhenjie.github.io/)。 
　　（由于篇幅太长，[如何为自己在Github上的开源项目搭建项目主页传送门](http://blog.csdn.net/yanzhenjie1003/article/details/51703374)。）
　　我的个人主页就放在`Github Pages`并且绑定了我的域名：[yanzhenjie.com](http://www.yanzhenjie.com/)，非常好记：[严振杰.com](http://www.yanzhenjie.com/)，是不是一下就记住了，如果没有请再记一次，记不住就不要往下看了，哈哈哈开个玩笑啦。
# Github Pages是什么
`Github Pages`主页：[https://pages.github.com](https://pages.github.com/)，其实官网这里已经有教程了，怕好多同学看不懂，所以来一篇博客。 
`Github Pages`是什么，来自官网的解释：[https://help.github.com/articles/what-are-github-pages](https://help.github.com/articles/what-are-github-pages)，不想打开官网的人看图片：
不要夸我，我就是这么贴心，提大家翻译好了中文版：
![Github Pages是什么](https://img-blog.csdn.net/20160617144523492)
　　按我的理解，Github Pages是[Github](https://github.com/)免费提供给开发者的一款托管个人网站的产品，这绝对是个好东西啊。目前来看只能托管静态内容，但是这已经足够我们用了，如果咱发挥的好，犹如鲤鱼跃龙门啊。
# Github Pages怎么玩
　　我刚开始做这事之前也不知道怎么办，我有个习惯就是对于不明白的事情习惯去官网看帮助，于是乎我看了Github Helper：[https://help.github.com/](https://help.github.com/)，我看到这里的时候心中大喜，其实这就是教程，不够是英文版的，喜欢的同学也可以直接去看：
![Github Pages使用帮助](https://img-blog.csdn.net/20160617145112322)
　　我是一步步看过来的，为了不浪费大家时间，我们只要看这条就行：[User, Organization, and Project Pages](https://help.github.com/articles/user-organization-and-project-pages/)。这里我把最重要的信息贴出来：
![仓库名称规则](https://img-blog.csdn.net/20160617145520304)
　　看到这里我就基本猜到了要怎么做了，后面的就不一一带各位看了，我直接说原理。Github的个人主页是可以绑定一个域名的，后面带各位看官一步步实现。在没有绑定自己的域名之前，这里的地址都是有规律的，比如我的帐号是`yanzhenjie`，那么我的主页地址就是`yanzhenjie.github.io`，比如我的帐号是`xxoo`，那么我的主页地址就是`xxoo.github.io`；比如我的项目叫`NoHttp`，那么项目对应的地址就是`yanzhenjie.github.io/NoHttp`。如果你是组织帐号也一样。
# 用Github Pages为自己的帐号搭建个人主页
　　做这件事首先你得有一个Github的帐号，没有的人[去Github申请](https://github.com/)一个吧，作为一个程序员我擦。如果你已经有一个Github的帐号了那么恭喜你继续往下看。
## 新建个人主页的仓库
　　打开Github首页，登录后新建一个仓库，这里再次提醒要注意仓库的名称，比如我的帐号是yanzhenjie，那么仓库名称应该是：`yanzhenjie.github.io`。 
![新建仓库演示](https://img-blog.csdn.net/20160617152346289)
按照如步骤创建仓库：
![新建个人主页仓库注意](https://img-blog.csdn.net/20160617152507720)
## 添加个人主页仓库内容
　　仓库创建完成后，就要添加网站内容到仓库咯。提交文件到github仓库，这里简单的介绍下用Git怎么提交内容，首先你电脑必须安装了Git并配置好了环境（看了还是不会的人去Google下）。 
　　如果熟练使用Git命令、TortoiseGit或者SourceTree的人请不要见怪，因为毕竟有人不会。
### 一、clone仓库到本地
　　注意你clone的时候要把连接换成你的喔。
```php
git clone https://github.com/yanzhenjie/NoHttp.git
```
- 1
### 二、添加内容到本地仓库
　　刚才clone下来的仓库就是一个本地仓库啦，若是你熟悉html的话随便弄个html就好，我们知道网站默认的首页都是index.html，所以我们在本地仓库建立一个index.html，目录一定要在`.git`所在的目录，也就是我们项目的root目录：
```
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="text/html;charset=utf-8"/>
    <title>严振杰</title>
</head>
<body>
我叫严振杰，软件工程师，英文名Yolanda，尤兰达，他们叫我达哥。
</body>
</html>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
　　文件都建好内容填好保存，一个简单的首页就完成了，假设我们的网站内容都建好了。
![index.html演示](https://img-blog.csdn.net/20160617183826808)
### 三、commit push内容到我们刚才新建的仓库
　　在命令模式下进入到刚才的仓库文件夹下，用下面的命令提交，先别急着复制哦，看下下面的解释：
```
git add .
git commit -m "初始化个人主页仓库"
git push
Username for 'https://github.com': yanzhenjie
Password for 'https://yanzhenjie@github.com':
```
- 1
- 2
- 3
- 4
- 5
　　命令要一行一行执行喔，第一行命令是添加所有文件到git，第二行是commit提交的内容到本地仓库，第三行是push本次仓库的内容到服务器，第四行是执行了第三行自动出来的，输入帐号确定，第四行是输入第三行后出来的，出入密码确定就完成了提交。
## 恭喜你，个人网站搭好了
　　如果你的提交都完成了没有问题了，那么这个时候恭喜，你的个人主页已经搭建好了，不信你可以在浏览器访问下，比如我的：[https://yanzhenjie.github.io](https://yanzhenjie.github.io/)，根据刚才说的规则输入你的地址看看喔。
![我的个人主页](https://img-blog.csdn.net/20160617181829972)
## Github Pages个人主页绑定个人域名
　　我们刚才的域名可能不好记，比我的：`yanzhenjie.github.io`=`严振杰.github.io`，但是还是不如`严振杰.com`。so我们绑定一个好记的域名，所以我又去找[Github
 Helper](https://help.github.com/)，又被我找到了哈哈哈：
![如何绑定个性域名](https://img-blog.csdn.net/20160617182126871)
　　打开看了看，果然找到了答案，我还是贴一个最重要的图：
![如何绑定个性域名](https://img-blog.csdn.net/20160617182835989)
![推荐使用www二级域名](https://img-blog.csdn.net/20160617183143962)
　　这里再把上图中的流程叙述下，我们需要一个域名，没有的人[去万网申请](https://wanwang.aliyun.com/)一个，然后需在刚才的仓库root中添加一个CNAME的文件，文件内容是我们的域名，在Github上看到推荐我们使用www的二级域名。
### 申请域名
　　现在去阿里云·万网申请到域名，我的其中其中两个域名如下：
![申请到的域名](https://img-blog.csdn.net/20160617185555113)
### 解析域名到github pages的ip上
　　域名申请好之后就是解析域名到`github pages`的个人主页的ip，我们ping一下自己的主页的ip，在命令行：
```
ping
 yanzhenjie.github.io
```
，你的地址和我肯定不一样也许就是`ping xxoo.github.io`哈哈哈：
![ping到的ip地址](https://img-blog.csdn.net/20160617185802451)
　　为我们的域名添加解析，记录类型都是A记录，记录值全部对应刚才ping出来的ip：
![申请到的域名](https://img-blog.csdn.net/20160617190014985)
![添加两条A记录](https://img-blog.csdn.net/20160617190643320)
　　这里的www是一个二级域名，@或者不填是一级域名，两条记录的记录值都是刚才ping出来的ip。这样就能把我们的域名和github上的主页关联起来了。不要着急喔，现在在浏览器输入域名你会发现访问不到，继续往下看。
### 添加CNAME文件到github仓库
　　在我们刚才创建`index.html`的地方创建一个名为CNAME的文件，里边的内容填你的域名，例如我的是’www.yanzhenjie.com’，这样就能做到访问`yanzhenjie.com`和`www.yanzhenjie.com`都redirect到`www.yanzhenjie.com`这个二级域名了，上文中说过这种方式是Github推荐的。 
　　CANME文件的内容填好按照刚才提交`index.html`的方法把CNAME提交到Github，这个时候我们的域名绑定也就完成啦。可以在浏览器输入你的个性域名看看是不是OK啦。比如我的严振杰.com：[www.yanzhenjie.com](http://www.yanzhenjie.com/)：
![严振杰的主页](https://img-blog.csdn.net/20160617223227119)
> 
版权声明：转载请注明本文转自严振杰的博客: [http://blog.csdn.net/yanzhenjie1003](http://blog.csdn.net/yanzhenjie1003)
