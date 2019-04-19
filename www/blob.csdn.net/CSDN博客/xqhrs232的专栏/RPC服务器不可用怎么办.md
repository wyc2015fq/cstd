# RPC服务器不可用怎么办 - xqhrs232的专栏 - CSDN博客
2013年11月04日 16:27:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1558
原文地址::[http://jingyan.baidu.com/article/b87fe19ea671c752183568d7.html](http://jingyan.baidu.com/article/b87fe19ea671c752183568d7.html)
相关文章
1、** RPC服务器不可用的解决办法----[http://lixingyu.blog.51cto.com/907028/195561/](http://lixingyu.blog.51cto.com/907028/195561/)**
步骤
- ![rpc服务器不可用怎么办](http://d.hiphotos.baidu.com/exp/scrop=50/sign=2f956aade7cd7b89ed327ddc7f197797/14ce36d3d539b600c16165d0e850352ac65cb72e.jpg)1[](http://jingyan.baidu.com/album/b87fe19ea671c752183568d7.html?picindex=1)
- ![rpc服务器不可用怎么办](http://c.hiphotos.baidu.com/exp/scrop=50/sign=0e9b0cd6810a19d8cf5dc35a43c7b7bb/1c950a7b02087bf4c8f5028ef3d3572c11dfcfb8.jpg)2[](http://jingyan.baidu.com/album/b87fe19ea671c752183568d7.html?picindex=2)
- ![rpc服务器不可用怎么办](http://g.hiphotos.baidu.com/exp/scrop=50/sign=7aced16232fa828bd57ddabc8d227404/0bd162d9f2d3572cd72fc9bb8b13632763d0c38c.jpg)3[](http://jingyan.baidu.com/album/b87fe19ea671c752183568d7.html?picindex=3)
- ![rpc服务器不可用怎么办](http://b.hiphotos.baidu.com/exp/scrop=50/sign=460cf1b877c6a7efbd78ef798dc79a61/cefc1e178a82b901eb8d9754728da9773912efb1.jpg)4[](http://jingyan.baidu.com/album/b87fe19ea671c752183568d7.html?picindex=4)
- ![rpc服务器不可用怎么办](http://h.hiphotos.baidu.com/exp/scrop=50/sign=98afd845c9fcc3ceb09e8e6ce278e3b8/9922720e0cf3d7caa4746b11f31fbe096a63a9d3.jpg)5[](http://jingyan.baidu.com/album/b87fe19ea671c752183568d7.html?picindex=5)
- ![rpc服务器不可用怎么办](http://c.hiphotos.baidu.com/exp/scrop=50/sign=818fd0469213b07eb9e317577ceaa413/38dbb6fd5266d0167359c94f962bd40735fa3510.jpg)6[](http://jingyan.baidu.com/album/b87fe19ea671c752183568d7.html?picindex=6)
我们在使用电脑进行时间同步，安装打印机或者其它的操作的时候可能会遇到同样一个问题，那就是提示“RPC服务器不可用”，很多朋友可能对于RPC并不了解，更不知道如何解决，下面就跟随小编一起来看看这个问题怎么解决吧。
## 方法/步骤
- 
1
RPC就是英文Remote Procedure Call Protocol的简写，中文释义为远程过程调用协议，是一种通过网络从远程计算机程序上请求服务，而不需要了解底层网络技术的协议。对于它我们并不需要了解的十分清楚，解决这个问题才是我们的首要任务。
![rpc服务器不可用怎么办](http://d.hiphotos.baidu.com/exp/w=500/sign=f3680e6a279759ee4a5060cb82fa434e/14ce36d3d539b600c16165d0e850352ac65cb72e.jpg)
- 
2
我们右击我的电脑，找到管理选项并点击，此时我们便会来到计算机管理界面，我们看到有三个大的选项，而今天我们的需要进行设置的就是服务和应用程序，我们展开它，此时可以看到两个小的选项，我们双击服务选项即可。
![rpc服务器不可用怎么办](http://c.hiphotos.baidu.com/exp/w=500/sign=6de719660b55b3199cf9827573a98286/1c950a7b02087bf4c8f5028ef3d3572c11dfcfb8.jpg)
- 
3
然后我们在服务列表中找到Remote Procedure Call (RPC)选项，这个就是我们今天的主角。一般出现RPC服务器不可用的原因就是这个服务没有启动所致，我们可以看看它是否处于禁用的状态，如果是，我们启用它。
![rpc服务器不可用怎么办](http://g.hiphotos.baidu.com/exp/w=500/sign=2d34d71242a98226b8c12b27ba83b97a/0bd162d9f2d3572cd72fc9bb8b13632763d0c38c.jpg)
- 
4
RPC服务应该算是我们经常使用的一个服务了，如果每次用到都手动启动的话难免有些麻烦，这样的话我们可以把它设置为自动，这样在开机的时候就可以自己启动不用我们手动去开启它了，方法很简单，我们双击此服务，然后选择自动，点击确定即可。
![rpc服务器不可用怎么办](http://b.hiphotos.baidu.com/exp/w=500/sign=f0c7e09c38dbb6fd255be5263924aba6/cefc1e178a82b901eb8d9754728da9773912efb1.jpg)
- 
5
当然除了上述方法进入到服务界面之外，还有另一种比较简单的方法就是命令行法。我们打开开始菜单，点击运行，然后输入services.msc点击确定，是不是直接跳转到了服务界面，接下来的操作就跟刚才的相同了。
![rpc服务器不可用怎么办](http://h.hiphotos.baidu.com/exp/w=500/sign=f8c0ff7806082838680ddc148898a964/9922720e0cf3d7caa4746b11f31fbe096a63a9d3.jpg)
- 
6
当然有时候即便是开启了RPC服务问题仍然可能没有解决，此时我们就要考虑是不是病毒或者其它的原因造成的，我们可以开启杀毒软件进行全盘的查杀，看看是否有病毒的存在，亦或者搜索下相关问题，是不是还牵扯到其它的原因以及解决办法。
![rpc服务器不可用怎么办](http://c.hiphotos.baidu.com/exp/w=500/sign=d2ae9f484034970a4773102fa5cbd1c0/38dbb6fd5266d0167359c94f962bd40735fa3510.jpg)
- 
7
如果我们平时在使用电脑的时候出现了某某服务不可用的时候我们仍然可以进入到这个界面，然后找到对应的服务把它开启即可，当然有些服务开启之后可能会造成一些不安全因素，如非必要还是不要胡乱开启服务为好。
## 注意事项
- 
对于RPC我们并不需要知道的很多，只要知道这个问题的解决方法即可。
- 
使用电脑的过程中如果遇到了服务不可用，都可以到这个界面把它开启。
- 
以上就是rpc服务器不可用怎么办的全部内容，希望对有需要的朋友有所帮助。
//=======================================================================================================
备注::
1>使用VS2005进行同步调试下载的时候经常会出现这样的问题，实在设置后还是不管用就重启PC吧，把RPC服务置为初始状态就可以解决这个问题

