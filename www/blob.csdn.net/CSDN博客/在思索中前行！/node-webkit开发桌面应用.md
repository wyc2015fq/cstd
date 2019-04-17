# node-webkit开发桌面应用 - 在思索中前行！ - CSDN博客





2014年12月26日 22:51:52[_Tham](https://me.csdn.net/txl16211)阅读数：1258标签：[node-webkit																[nodejs																[桌面应用](https://so.csdn.net/so/search/s.do?q=桌面应用&t=blog)
个人分类：[开发篇](https://blog.csdn.net/txl16211/article/category/2492171)





# [Node-Webkit能够做什么呢？](http://www.oschina.net/question/658403_131072)（打开链接看discuss）

github 项目源：https://github.com/rogerwang






## 导言

node-webkit 是一个很神奇的桌面客户端项目,正如这个项目的名字,这个项目是由node 和 webkit 构成,简单来说,就是你可以用HTML 5和 node 进行桌面客户端开发,而且客户端还是同时支持在 WIN,MAC,LINUX运行. 下面,就用一个简单的例子来展示一下node-webkit的魅力.

## 创建项目

### 本例子基于Grunt构建

如果,你用过grunt,要创建一个node-webkit 非常简单,我已经写好了一个node-webkit的`grunt-init`的项目模板. 至于怎么安装这个模板,跟官方的教程一样. 如果你是windows 用户

> 
md %USERPROFILE%.grunt-init\node-webkit 

git clone git@github.com:youxiachai/grunt-init-node-webkit.git %USERPROFILE%.grunt-init\node-webkit


linux or mac

> 
git clone git@github.com:youxiachai/grunt-init-node-webkit.git ~/.grunt-init/node-webkit


你只需要用

> 
`grunt-init node-webkit`


就可以创建完毕.

```
├─app.nw
└─test
```

app.nw 这个目录就是我们准备要开始写的pc 客户端的项目文件夹,运行node-webkit项目很简单,只需要把node-webkit 的运行环境配置到环境变量,然后运行

> 
`nw app.nw` 就可以运行起来了. ![nodewebkit1](http://images.cnitblog.com/blog/199034/201307/03092303-77b44d687ee34336b615be0af7523657.jpg)


PS: 如果你不想接触grunt,不过我建议还是学一下grunt,你可以到[https://github.com/rogerwang/node-webkit#quick-start](https://github.com/rogerwang/node-webkit#quick-start) 学习如何启动一个node-webkit应用.

## 效果图

![oscdesk1](http://images.cnitblog.com/blog/199034/201307/03092304-49ea35bc61ab43c581f9505ff10d7b76.jpg)

![oscdesk2](http://images.cnitblog.com/blog/199034/201307/03092305-8397ae61131643678b9d6b9848b954e8.jpg)

这个就是所谓的 win 8 风格的客户端了吧....界面用的框架是这货:[http://aozora.github.io/bootmetro/](http://aozora.github.io/bootmetro/) 90%
 的时间都是调整界面...蛋疼死了......连个 win8 风格的progress 都没..让我非常伤心..也可能是alpha 的原因吧. 不过最后的效果,还是很难看,就凑合着过去吧....

### 开发

基于node-webkit 开发pc 客户端语言支持 `c/c++`,`html5`,`css3`, `js`,`node api`.好了,现在我们开始用html 5 + css3 写一个pc 客户端吧. `node-webkit`本质就是一个可以跑node 的浏览器,所以,我们用web 开发的技巧来开发pc
 客户端毫无问题.

首先,打开`toolbar`,在`package.json`文件里面有个`toolbar`的参数,设置为`true`即可,就会见到如下图所示:

> 
![toolbar](http://images.cnitblog.com/blog/199034/201307/03092306-63d80e246d2e4b8cb692e1e89b6e0b19.jpg)


点击那个三横线的按钮,一个chrome 风的调试窗口就出来了.

> 
![console](http://images.cnitblog.com/blog/199034/201307/03092306-7fe4f31caabe4c12a9a01611471b0b4a.jpg)


开发的时候,我们修改完文件,并不需要重新运行程序来看结果,我们,可以直接点击左边的刷新按钮即可看到我们修改的运行结果.用`node-webkit`开发客户端是不是很方便了!

那么接下来,要开发一个oschina pc 客户端,我们只需要知道,相关api 就行了,从android 客户端源码里面可以得到相关api...具体代码在`app/model/oschinaApi.js` 文件里面.

node-webkit,已经吧相关的安全限制已经去掉,所以说,用node-webkit开发pc客户端,用webkit 发的请求不受同源限制. 用node-webkit 开发一些restful 应用是非常舒服的事情,只要有个不错的界面.关于`node-webkit`的东西也就这么些了,剩下的就是web 开发,不在本文`node-webkit`范围内,所以就不再啰嗦..

## 使用的开源项目

界面:

[http://aozora.github.io/bootmetro/](http://aozora.github.io/bootmetro/)

[https://github.com/cubiq/iscroll](https://github.com/cubiq/iscroll)

模板引擎:

[https://github.com/visionmedia/ejs](https://github.com/visionmedia/ejs)

## 项目地址

Github:

> 
[https://github.com/youxiachai/osChinaDesktopClient](https://github.com/youxiachai/osChinaDesktopClient)


git@osc:

> 
[http://git.oschina.net/youxiachai/oschinadesktopclient](http://git.oschina.net/youxiachai/oschinadesktopclient)


程序运行: windows用户之间去到`app.nw` 目录下运行 nw.exe 即可.

> 
cd app.nw 

nw.exe


linux 或者mac 用户 把除 index.html,package.json,app 以外的文件删除,然后将`node-webkit` 运行环境配到环境变量中运行

> 
nw app.nw








  之前有一篇博文提到过node-webkit和有道的hex，今天就node-webkit开发做一个详细介绍吧。 顾名思义，node-webkit就是nodejs+webkit。这样做的好处显而易见，核心奥义在于，用nodejs来进行本地化调用，用webkit来解析和执行HTML+JS。





## 快速上手

### 下载node-webkit



        点击这里：[https://github.com/rogerwang/node-webkit](https://github.com/rogerwang/node-webkit)

        找到Downloads这一小节，然后下载对应平台的node-webkit预编译包。（为了介绍起来方便，本文后续内容全部针对windows平台，OSX和linux平台上的操作类似，就不做特意说明了。）

        下载完之后解压，可以看到如下内容：




![](http://dl2.iteye.com/upload/attachment/0093/6163/e630aaba-3bb0-3133-8d6c-5bc71fb48c68.jpg)

        双击nw.exe直接运行，看到如下界面说明一切都ok，在你的机器上可以运行：



![](http://dl2.iteye.com/upload/attachment/0093/6165/ff965764-0ff7-3e79-a884-49ccd3498af3.jpg)


### Hello nw

        老规矩，先来Hello World!

        建一个example1.html，内容如下：



**[html]**[view plain](http://blog.csdn.net/xiebaochun/article/details/39078041#)[copy](http://blog.csdn.net/xiebaochun/article/details/39078041#)



- <!DOCTYPE html>
- <html>
- <head>
- <title>Hello World!</title>
- </head>
- <body>
- <h1>Hello node-webkit!</h1>
-     We are using node.js <script>document.write(process.version)</script>.  
- </body>
- </html>
- 
-         在同一级目录下再建一个package.json，内容如下：  
- 
- {  
-   "name": "nw-demo",  
-   "main": "example1.html"  
- }  
- 

        好了，用你机器上的压缩工具，rar或者7z或者神马神马的，把这两个文件压缩成exampl1.zip，如下图所示：
![](http://dl2.iteye.com/upload/attachment/0093/6169/a08d3b4d-138f-33d7-9c72-ea8c757e039f.jpg)
![](http://dl2.iteye.com/upload/attachment/0093/6167/ef1ae0c7-8484-3fe7-9d1c-536ed263a78c.jpg)




**        注意，用什么工具压缩无所谓，但是一定要压缩成ZIP格式。**

        把example1.zip拷贝到与nw.exe同级的目录下，如下图：




![](http://dl2.iteye.com/upload/attachment/0093/6171/c5ae6a4a-32a2-3156-b73d-28698e7c00ba.jpg)


        然后用鼠标把example1.zip拖动到nw.exe上运行（用命令行也可以，不过在windows下还是直接拖比较爽一点），效果如下图：

![](http://dl2.iteye.com/upload/attachment/0093/6173/2c280ae2-0c63-35b6-84a0-605df70343a6.jpg)


        这样，第一个例子就完成了，然后你自己可以去玩儿了。

        按照官方的搞法，example1.zip可改名成example1.nw，这样显得更加高大上一些。不过由于小僧的BIG一向比较低，私认为用.zip作为后缀也没啥不可以。



### 来点儿JS

        第一个例子代码非常简单，你可以自己加点儿你最熟悉的JS代码，比如这样：
``


**[html]**[view plain](http://blog.csdn.net/xiebaochun/article/details/39078041#)[copy](http://blog.csdn.net/xiebaochun/article/details/39078041#)



- <!DOCTYPE html>
- <html>
- <head>
- <title>Example2</title>
- <metahttp-equiv="content-type"content="text/html;charset=UTF-8"/>
- </head>
- <body>
- <h1>Example2</h1>
- <script>
-         alert("This is example2!");  
- </script>
- </body>
- </html>


        显然，这里有很多HTML5的习惯写法，那当然是必须的对吧？因为webkit在google的调教之下早已成了HTML5的开路先锋了！

## 打包和分发

![](http://dl2.iteye.com/upload/attachment/0093/6163/e630aaba-3bb0-3133-8d6c-5bc71fb48c68.jpg)


        如上图，很显然，要想让我们的应用跑起来，以上这些dll和pak文件是少不了的（至于每个文件是用来干嘛的，请参见官方的描述文档）。

        那么，对于我们做的example1这个简单的应用，怎么让我们的用户能运行它呢？当然，让我们的用户自己去下载node-webkit然后解压运行，这也算一种办法，这样的话我们只要把example1.zip文件扔给他就可以了。

        不过这样总觉得有点儿奇怪，要是能把所有相关的内容都打包成一个独立的exe文件就好了，比如打包成example1.exe，这样用户拿到之后直接双击就可以运行，这样显然是极好的。

        OK,let's go!

        首先，我们要合并example1.zip和nw.exe，形成一个新文件，名为example1.exe，这一点我们通过windows的命令行下的copy命令可以实现：
``


**[html]**[view plain](http://blog.csdn.net/xiebaochun/article/details/39078041#)[copy](http://blog.csdn.net/xiebaochun/article/details/39078041#)



- copy /b nw.exe+example1.zip example1.exe  


**注意，nw.exe必须放在+号前面。**



       这样就生成了一个名为example1.exe的文件，双击这个文件，看到的效果和第一个例子相同。很显然，这个example1.exe还是要依 赖那些dll文件才能运行，所以，这时候我们需要一款打包工具来帮助我们把这些example1.exe和dll文件都打包到一起，它就是[Enigma
 Virtual Box](http://enigmaprotector.com/en/aboutvb.html)，大家可以点击这里下载安装：



[http://enigmaprotector.com/assets/files/enigmavb.exe](http://enigmaprotector.com/assets/files/enigmavb.exe)

        安装完成之后启动，看到如下界面：



![](http://dl2.iteye.com/upload/attachment/0093/6177/2f016af1-f9e4-3c14-8c28-76c4d71083de.jpg)


        打包过程截图（请按照图中的配置操作）：



![](http://dl2.iteye.com/upload/attachment/0093/6175/fbf9f797-cba7-39cb-a358-8a27df9014ae.jpg)



        点击右下角的Process按钮开始打包，最终获得了一个名为example1_boxed.exe的文件。把这个文件拷贝到其它任意目录中双击，如果能顺利运行，说明打包成功！你可以把它分发给你的用户了！

        官方的文档上还介绍了其它几种打包分发方式，不过看起来都不是很实用，有兴趣的参见这里：

[https://github.com/rogerwang/node-webkit/wiki/How-to-package-and-distribute-your-apps](https://github.com/rogerwang/node-webkit/wiki/How-to-package-and-distribute-your-apps)



## 未竟的主题

        以上就是关于node-webkit用法的一些简单示例，你可以沿着这个方向自己去探索了。有热心的网友把我们的Ext4.1中文文档打了一个包出来，可以到脚本娃娃的群共享（号83163037）去下载（比较大，不好放上来），入群口令nw




### 如何最小化发布？

        在官方发布的内容里面，icudt.dll这个文件有9724k，nw.exe有38396k，很显然，这个体积略显庞大了一些，如果我们的应用本身很小，只有几百K，而打包出来的东西有几十M显然是不合适的，这样非常不方便拷贝和网络传播。

        很显然，我们还需要一款工具，能够根据需要进行最小化的打包发布。

        不过目前我还没找到这样的好东东，各路大神如有知道的，求指教。




### 如何利用nodejs调用本地接口？

        很显然，既然nodejs都被打包进来了，网上众多的npm模块都是可以拿过来使用的，具体做法官方主页上都有比较详细的介绍，参见这里[https://github.com/rogerwang/node-webkit](https://github.com/rogerwang/node-webkit)

### 其它案例

        官方的主页上还展示了众多使用node-webkit制作的应用，各位自行点击查看即可。

## 展望未来

### 其实这条路老早就有人在走

        网上有很多人在争论，未来究竟是原生的应用会胜出，还是WEB APP会胜出，实际上这两者并不是你死我活的关系，而是会互相融合，走向【混合型应用】这样一种形态。

先看几个界面：




![](http://dl2.iteye.com/upload/attachment/0093/6179/995a7cbb-8bdc-364e-a1db-2d49c396cb50.jpg)

![](http://dl2.iteye.com/upload/attachment/0093/6181/7d7307f4-e375-33b7-91d4-643f083d897c.jpg)

![](http://dl2.iteye.com/upload/attachment/0093/6183/3a69db6a-5df7-35a4-9533-75ead0ae15d2.jpg)

![](http://dl2.iteye.com/upload/attachment/0093/6185/93b7fc30-784e-3b32-80e0-d3d4c8c5a674.jpg)



![](http://dl2.iteye.com/upload/attachment/0093/6396/9b110a31-234b-3764-be62-3d5d8ab7c9d2.jpg)


        大家可以把自己的网络断掉测试以上界面，QQ有时候甚至能爆出JS错误，哈哈。

        2011年的时候，我发过一篇文章，《未来五年，桌面应用和WEB走向融合》，当时只是因为看到诸如酷我音乐、腾讯QQ这些桌面应用的开发方式而有 所感悟，对于桌面应用会以什么样的形式和B/S开发模式融合在一起并没有确切的想法。而最近由于打包Ext4.1中文文档的缘故知道了node- webkit，突然有一种恍然大悟的赶脚！

        当然，酷我音乐盒和疼讯目前是用什么样的技术实现的小僧不知道其中的细节（人家也不想让外人知道对吧？）。不过，小僧相信，对于屌丝单兵作战来说，疼讯的实现方式肯定会比node-webkit更加复杂，目测个人无法模仿。



### 跨平台+多终端的神器



        毫无疑问，这货会成为2014年的一款神器。因为它具备了以下这些无可比拟的优势：



- **人力成本最低**：招一个会C++的码农要花多少钱？招一个牛逼哄哄的既会C++还对QT、MFC等了如指掌的码 农又要花多少钱？对比一下，招一个会JS+CSS的呢？如果你是老板，你怎么想？再说了，市面上会C++的码农更多，还是会JS+CSS的码农更多？无需 小僧多言，其中的奥妙您自个儿体会。
- **APP的体积更小**：列位一定还记得当年大明湖畔的疼讯QQ只有十几M的体积，俗话说，岁月是把杀猪刀，在集成了疼讯的各种应用之后，如今的QQ身材早已发福走形。可以想象，如果QQ不使用嵌入浏览器内核的方式，恐怕体积早就超过100M了吧！
- **不需要整体更新APP**：如果是按照传统的方式开发桌面应用，每次升级的时候都需要用户重新下载整个应用，比如疼讯QQ。但是用node-webkit的这种方式，可以很好地避免这个问题，就像网站一样，网站内容可以不断更新，但是浏览器本身的更新不需要那么频繁。
- **跨平台**：这一点是由NodeJS来保证的。
- **多终端**：由于nodejs和chromium本身都是可以在桌面平台和移动平台上运行的，因此用这货来支持多终端开发，保持技术栈的一致性是非常合适的。
- **完善的工具链**：由于nodejs的兴起，它下面的npm模块数量已经有几十万个；而各种基于chrome内核的定制版浏览器就不用再介绍了。加上传统B/S开发模式下已经具备的完整开发和设计工具，用node-webkit来开发桌面应用已经具备了完善的工具链。




## 参考资料

        关于node-webkit作者的介绍参见这里：

[http://www.csdn.net/article/2014-01-08/2818066-Node-Webkit](http://www.csdn.net/article/2014-01-08/2818066-Node-Webkit)

        node-webkit在github上的地址：

[https://github.com/rogerwang/node-webkit](https://github.com/rogerwang/node-webkit)

        由于node-webkit的作者提供的文档都是E文的，为了方便大中华局域网内的一众屌丝，我的好基友@老男孩对文档做了一些翻译，参见这里：

[https://github.com/liu78778/node-webkit](https://github.com/liu78778/node-webkit)](https://so.csdn.net/so/search/s.do?q=nodejs&t=blog)](https://so.csdn.net/so/search/s.do?q=node-webkit&t=blog)




