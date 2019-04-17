# winedit 永久试用的办法 - 战斗蜗牛的专栏 - CSDN博客





2014年01月22日 02:01:28[vbskj](https://me.csdn.net/vbskj)阅读数：1838









软件把安装信息放在注册表里面这种做法真是方便了我等草民啊。


打开CMD。


运行下面的命令。

```
reg delete "HKEY_CURRENT_USER\Software\WinEdt 7" /v "Install Build" /f
reg delete "HKEY_CURRENT_USER\Software\WinEdt 7" /v "Install Root" /f
```

![image](http://images.cnitblog.com/blog/495450/201303/23210011-34b1a8511f2446e2aab773fbb4fb944a.png)


这样就行啦。


当然为了方便自己使用，可以把上面两条命令存进文本文档，改后缀为bat，变成批处理。

![image](http://images.cnitblog.com/blog/495450/201303/23210017-c9a70b931bad4cc28cdd32bb3f6e8163.png)


就像这样~


然后在计划任务中启用定时任务。

![image](http://images.cnitblog.com/blog/495450/201303/23210017-bc141230d2514973b4bd6c16cd0f34b0.png)

![image](http://images.cnitblog.com/blog/495450/201303/23210021-3ca10246c3a940e39f91ee15aaac254e.png)


新建基本任务就可以了。

![image](http://images.cnitblog.com/blog/495450/201303/23210023-0fff8f66a869447a8a2efe6daa82db5b.png)

![image](http://images.cnitblog.com/blog/495450/201303/23210025-32ecd48495114aa3bc1d17ec6fea3397.png)![image](http://images.cnitblog.com/blog/495450/201303/23210027-6b688d143a27404f89a21da27d5108be.png)

![image](http://images.cnitblog.com/blog/495450/201303/23210028-56f52000e9664b74a4be9ccf852cf5b0.png)


最后在属性里面设置。

![image](http://images.cnitblog.com/blog/495450/201303/23210032-a1c92ed9e8004288b32eeed94b268e7d.png)


呐。享受免维护的Winedt 试用期吧~~



