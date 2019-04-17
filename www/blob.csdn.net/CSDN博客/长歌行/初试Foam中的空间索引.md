# 初试Foam中的空间索引 - 长歌行 - CSDN博客





2018年08月16日 22:57:15[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：169
所属专栏：[区块链杂谈](https://blog.csdn.net/column/details/25507.html)









Foam是一个地理位置证明的协定，并能提供以一种去中心化的地理位置服务从而取代GPS。如果你对Foam或者其他的区块链项目感兴趣，可以参考我其他的[repo](https://github.com/bentan2013/blockchain-project-research/issues/12)

这次我就简单介绍下如何使用Foam的Spatial index explorer。

### 进入空间索引的页面

[https://beta.foam.space/welcome](https://beta.foam.space/welcome)

然后就会看到这个页面了
![这里写图片描述](https://user-images.githubusercontent.com/5510943/44214663-45f18f00-a1a3-11e8-9030-a6b4378de0a6.png)
大概就是说，Foam的Spatial index是为区块链上的地理应用和智能合约提供的可视化浏览器。我们可以把它看做Bloomberg交易终端和谷歌地图之间的桥梁。作为一个开源的网络程序，它是基于purescript-web3库开发，它可以用于：
- 允许用户在视觉上和协议进行交互
- 可以作为构建在顶层的各种dApp的基石

### 安装Metamask

如果你的浏览器安装了Metamask，你就会看到和我的截图一样的内容，如果没有安装，会显示这个：
![image](https://user-images.githubusercontent.com/5510943/44215095-3e7eb580-a1a4-11e8-9ce6-f28ed257f8b6.png)
安装和注册Metamask过程由于网上有很多资料，在这里就略过了（也许以后我会补上）

### 获得测试网Token

应用使用ETH的Rinkeby测试网络，所以我们还要申请一些免费的token，才能部署Beacon。

登录这个网站
[https://www.rinkeby.io/#faucet](https://www.rinkeby.io/#faucet)

下面有介绍，你只需要在Twitter, google+或者fb上面发表一个公开的post，post中包含你的地址。然后把这个post的地址粘贴在空白处，就能收到token了。具体可以参照[这里](https://ethereum.stackexchange.com/questions/34718/how-do-i-buy-tokens-on-the-rinkeby-test-environment)。

![这里写图片描述](https://i.stack.imgur.com/hwMKp.png)

当有Token之后，就可以点击Continue了，然后会出现这个页面，至于其中的乱码，似乎开发者也不知道怎么解决。。

![image](https://user-images.githubusercontent.com/5510943/44215589-763a2d00-a1a5-11e8-8fac-0f492394f332.png)

###使用App

点击Sign进入App，程序跳转到这个页面
[https://beta.foam.space/app](https://beta.foam.space/app)
![image](https://user-images.githubusercontent.com/5510943/44215767-e6e14980-a1a5-11e8-8c4f-43df315e09c6.png)

左边就是我们部署Beacon的属性页面了，右边就是地图浏览器，一个个方格子就是一个个Zone。我们可以在任意地方点击，部署自己的Beacon。

拉近一点看看详细图层
![image](https://user-images.githubusercontent.com/5510943/44215966-6242fb00-a1a6-11e8-9339-50ec435a6c5c.png)
Triangulations是图中绿色的三角形，由每三个相邻Beacon自动组成的三角形，用于定位

Arcs就是图中红色的线，会实时测算与bots的距离，从而定位

Bots是橙色的线条，模拟的移动物体的轨迹，有时候会发出定位的需求。

Pings是蓝色的圆圈，表示发射型号的频率，范围等。

先随便点击一个Beacon看看属性吧。（没细看，里面是不是有这个Beacon赚了多少多少各类代币之类的）
![image](https://user-images.githubusercontent.com/5510943/44215876-2c9e1200-a1a6-11e8-882f-3ca03c3887b0.png)
点击地图上空白处，就可以自己部署一个Beacon了。在这个测试程序上，基本上不花钱：0.34刀。不到一分钟，就可以看到交易被确认，自己的Beacon也显示在地图上了。然后就可以坐等收钱。

![image](https://user-images.githubusercontent.com/5510943/44216336-412eda00-a1a7-11e8-9891-9a0bba4cf9d7.png)

本次试用到此为止。谢谢观看。^ ^








