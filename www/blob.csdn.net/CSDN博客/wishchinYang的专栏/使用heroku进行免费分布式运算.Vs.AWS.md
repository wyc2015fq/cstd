# 使用heroku进行免费分布式运算.Vs.AWS - wishchinYang的专栏 - CSDN博客
2014年12月28日 18:20:42[wishchin](https://me.csdn.net/wishchin)阅读数：1389
原文链接：[http://www.freehao123.com/heroku/](http://www.freehao123.com/heroku/)
     Heroku这是一个老牌的[免费云空间](http://www.freehao123.com/tag/yunkongjian/)了，无限流量和无限容量，但是它的受欢迎程度远不如[OpenShift空间](http://www.freehao123.com/tag/openshift/)。部落直到今天才介绍Heroku免费空间也是有原因的，就是Heroku不支持PHP，数据库只有大小5MB。
目前[Heroku空间](http://www.freehao123.com/tag/heroku/)只支持Ruby, Node.js, Clojure, Java, Python, and Scala，创建项目和管理项目都需要安装Heroku Toolbelt工具，操作的难度都比较高，英文网站界面，不能绑定域名，空间没有广告。
对于一个不支持PHP的空间来说，它的实用性就已经大大折扣了，但是国人的力量还是强大的，Heroku空间可以说是继GAE空间又一个“广泛被应用”的[免费空间](http://www.freehao123.com/category/freekongjian/)，由于它的不限制流量，支持Https，很多人都已经将它代替GAE空间来使用了。
免费云空间不仅可以用来建站玩博客，还可以用来做一些“另类用途”： 
- 1、盛大空间：[盛大云引擎CAE免费空间2GB:申请使用、绑定域名和运行WordPress体验](http://www.freehao123.com/cae/)
- 2、百度空间：[百度BAE空间安装WordPress-邀请码数据库Url重写伪静态发送邮件云存储](http://www.freehao123.com/bae-wordpress-url/)
- 3、谷歌空间：[谷歌免费空间GAE国内广泛流行的应用安装和使用教程](http://www.freehao123.com/gae-kongjian/)
**Heroku免费空间申请方法和应用创建使用教程:无限流量无限容量 **
**一、Heroku免费空间注册账号**
1、Heroku官网链接：
- 1、官方网站：http://www.heroku.com
- 2、应用包下载：[Heroku Toolbelt下载地址](http://www.freehao123.com/dl-heroku-toolbelt/)
2、Heroku官网做得非常漂亮，你只需要点击“sign up” 直接输入你的邮箱账号。
![Heroku注册账户](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_01.gif)
3、接下来就是填写密码完成注册了。
![Heroku填写账号注册](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_02.gif)
4、注册完了就是进入到了Heroku空间用户管理界面了。这里有一个Heroku Toolbelt下载链接。
![Heroku下载链接地址](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_03.gif)
**二、Heroku免费空间安装Toolbelt和上传应用服务器端**
**PS：2012年12月19日更新，**感谢 chenxuefeng.net.cn 站长的提醒，如果你的电脑已经安装Ruby软件直接执行下面的代码安装heroku的组件就行，不用安装Heroku Toolbelt了。
`gem install heroku `
1、Heroku Toolbelt是一个用来创建和管理Heroku空间项目的工具，我已经将下文中要说到了应用都全部打包在其中了：
2、启动Heroku Toolbelt，然后输入：heroku login，再输入你的邮箱和密码登录进去。
![Heroku登录到工具中](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_04.gif)
3、将你下载的Heroku Toolbelt压缩包的War文件放在你的硬盘中一个新建的文件夹中。
![Heroku解压放在一个文件夹中](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_05.gif)
4、用CD的命令进入到这个文件夹中，然后执行以下命令。
```
heroku plugins:install https://github.com/heroku/heroku-deploy
```
![Heroku进入该文件夹](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_06.gif)
5、再执行新建应用的命令，新生成的Heroku二级域名的地址需要记下来。
```
heroku apps:create
```
![Heroku新建一个应用](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_07.gif)
6、再执行以下命令将War文件上传到Heroku空间当中。注意：freehao123.war是那个文件名，后面的是得到的二级域名地址去掉.herokuapp.com，你需要换成你自己的。
```
heroku deploy:war --war ./freehao123.war --app peaceful-mountain-8384
```
![Heroku上传文件](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_08.gif)
7、最后打开你的Heroku二级域名，如果能看到如下提示就表示成功了。
![Heroku成功创建了应用](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_09.gif)
**三、Heroku免费空间修改本地客户端配置连接服务器**
1、将Heroku Toolbelt应用包中的GSN压缩文件包解压，打开里面的配置文件，将GAE的Enable设置为0。
![Heroku设置为0](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_10.gif)
2、将C4的Enable设置为1，WorkerNode[0]后面写上你的Heroku二级域名，如下图：
![Heroku设置为1](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_11.gif)
3、SPAC的Enable设置为0，确定保存。
![Heroku关闭了Spac](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_12.gif)
4、最后双击打开GSN可执行文件。
![Heroku双击运行](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_13.gif)
5、接下来客户端就会连接服务器了，看到下图就表示成功了。
![Heroku成功连接服务器](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_14.gif)
**四、Heroku应用浏览器配置和速度测试**
1、看到了上面的内容，相信很多朋友知道接下来该如何做了，常用的IE、Chrome和Firefox都已经有了专用的插件供大家使用。
2、如果你是第一次使用，建议参考操作教程：[利用OpenShift免费空间的SSH免费挂Vagex赚美元的详细操作步骤](http://www.freehao123.com/openshift-vagex/)
3、记得在设置端口时要与你在配置文件中设置的端口是一致的。
![Heroku设置好端口](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_16.gif)
4、连接成功后看到了IP地址就变化了。
![Heroku变换IP地址](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_17.gif)
5、完美支持Https。
![Heroku支持加密链接](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_18.gif)
6、看视频不需要等待，速度很好。
![Heroku看视频速度](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_19.jpg)
**五、Heroku免费空间相关演示**
- 1、主页演示：http://peaceful-mountain-8384.herokuapp.com
**六、Heroku空间申请使用小结**
1、上次部落介绍了用OpenShift空间挂Vagex的方法，建议大家改用Heroku空间挂[Vagex](http://www.freehao123.com/tag/vagex/)，速度更快，且用的人更少，IP不冲突。
![Heroku挂Vagex赚美元](http://img.freehao123.com/wp-content/uploads/2012/12/heroku_20.gif)
2、OpenShift空间应该算是2012年度部落介绍的最好的一个适合建站的免费空间了，在某些地方已经远远超过一些付费空间，大家尽量不要用它做建站之外的事情了。
3、因为我看到好多朋友已经将正规的博客放在这个空间上了，如果大家无节制地去“榨干”OpenShift空间，哪一天大树倒下了，最后损失的还是我们自己。
