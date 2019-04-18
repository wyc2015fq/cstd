# 弹出USB大容量存储设备时出问题的解决方法 - fighting！！！ - CSDN博客
2019年01月21日 22:11:12[dujiahei](https://me.csdn.net/dujiahei)阅读数：720
转自：[https://jingyan.baidu.com/article/5225f26bb92001e6fa0908d1.html](https://jingyan.baidu.com/article/5225f26bb92001e6fa0908d1.html)
- - 
[造成这个问题的原因：是有程序正在占用着U盘或者移动硬盘。有可能是输入法，也有可能是别的什么乱七八糟的程序。](http://jingyan.baidu.com/album/5225f26bb92001e6fa0908d1.html?picindex=14)
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=e1cc146bb212c8fcb4f3f6cdcc0292b4/72f082025aafa40fd8bca896a064034f78f01906.jpg)
- 
2
要解决这个问题，其实也很简单。
首先在桌面找到计算机，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=dc1e2db00146f21fc9345e53c6256b31/0dd7912397dda144a6e00ff9b9b7d0a20df4868b.jpg)
- 
3
选中计算机后，右键，点击“管理”，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=d4c6b3a6ec1190ef01fb92dffe1a9df7/32fa828ba61ea8d3acec174f9c0a304e241f58aa.jpg)
- 
4
点击“管理”之后，会打开一个“计算机管理”窗口，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=f9a0c6540c082838680ddc148898a964/9922720e0cf3d7caa514523df91fbe096a63a99f.jpg)
- 
找到“事件查看器”，点开它，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=1fcdfcb9761ed21b79c92ee59d6fddae/aec379310a55b319739def3e48a98226cffc1710.jpg)
- 
打开“自定义视图->管理事件”，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=c8fea481a451f3dec3b2b964a4eff0ec/314e251f95cad1c8f26e906c743e6709c83d51a7.jpg)
- 
双击“管理事件”里面的警告事件（最新发生的事件），打开它，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=af82e98048166d223877159476220945/3b87e950352ac65cea410838f0f2b21193138a31.jpg)
- 
从下图可以看到，警告事件的“常规”里面记录了到底是哪一个进程占用了U盘或者移动硬盘。
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=a964f5639c2bd40742c7d3fd4b889e9c/728da9773912b31b1011a6628d18367adab4e178.jpg)
- 
也可以看一下详细信息，它也详细记录了到底是哪个进程PID，那个程序占用了U盘或者移动硬盘。
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=ec54fbf9da58ccbf1bbcb53a29d9bcd4/d788d43f8794a4c22294b56005f41bd5ac6e39c1.jpg)
- 
现在知道了是那个“进程PID”占用了U盘或者移动硬盘，那么就好办了。
只要结束掉这个占用进程就ok了
- 
打开Windows 任务管理器，点击“查看->选择列”，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=ceb9b06d0224ab18e016e13705fbe69a/4b90f603738da977a5cdd360bb51f8198718e39f.jpg)
- 
打开之后，勾选“PID（进程标识符）”，确定。如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=0d3cef3fe0f81a4c2632ecc9e72a6029/0df431adcbef76094a39c4d425dda3cc7cd99e4e.jpg)
- 
可以看到Windows 任务管理器里面多出了一列“PID”出来，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=02a657a8de39b6004dce0fb7d9513526/55e736d12f2eb9388c382eb9de628535e5dd6f27.jpg)
- 
这时候，就可以找出刚才那个进程PID出来，把它结束掉，如图所示：
![弹出USB大容量存储设备时出问题的解决方法](https://imgsa.baidu.com/exp/w=500/sign=6d415a69b81bb0518f24b328067bda77/a1ec08fa513d26979dd85e165efbb2fb4316d876.jpg)
