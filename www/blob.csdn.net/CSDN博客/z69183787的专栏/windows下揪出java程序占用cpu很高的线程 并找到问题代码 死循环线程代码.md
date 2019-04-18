# windows下揪出java程序占用cpu很高的线程 并找到问题代码 死循环线程代码 - z69183787的专栏 - CSDN博客
2018年06月26日 10:49:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：164
[https://blog.csdn.net/baidu_18607183/article/details/62235657](https://blog.csdn.net/baidu_18607183/article/details/62235657)
http://blog.csdn.net/hexin373/article/details/8846919
我的一个[Java](http://lib.csdn.net/base/javase)程序偶尔会出现cpu占用很高的情况
一直不知道什么原因
今天终于抽时间解决了
系统是win2003
jvisualvm 和 jconsole貌似都只能看到总共占用的cpu 看不到每个线程分别占用的cpu呢
所以在windows平台上要找出到底是哪个线程占用的cpu还不那么容易，[Linux](http://lib.csdn.net/base/linux)用top就简单多了
最后的解决方法:
1.找到java进程对应的pid。
找pid的方法是:打开任务管理器，然后点击 "查看" 菜单，然后点击 "选择列"，把pid勾上，然后就可以在任务管理器里面看到所有进程的pid值了。(也可以用第三步中提到的工具直接查看)
![](https://img-my.csdn.net/uploads/201304/24/1366812746_4122.jpg)
2.然后把java进程导出快照。直接运行命令。
[java][view plain](http://blog.csdn.net/hexin373/article/details/8846919#)[copy](http://blog.csdn.net/hexin373/article/details/8846919#)
- jstack -l 31372 > c:/31372.stack  
![](https://img-my.csdn.net/uploads/201304/24/1366812708_7430.jpg)
我这里是指定把java所有的信息导出到c盘的31372.stack的文件里。
3.在windows下只能查看进程的cpu占用率，要查看线程的cpu占用率要借助其他的工具，我这里用的是微软提供的 Process Explorer v15.3
下载地址http://technet.microsoft.com/en-us/sysinternals/bb896653.aspx
下载完后解压运行
![](https://img-my.csdn.net/uploads/201304/24/1366812781_8489.jpg)
右键点击需要查看的进程---properties
4.然后选择 Threads 选项卡，找到占用cpu的线程的tid，比如我这里是 31876 的线程
![](https://img-my.csdn.net/uploads/201304/24/1366812856_2454.jpg)
5.把pid转换成16进制，我这里直接用系统自带的计算器转换，置于为什么要转换，是因为先前用jstack导出的信息里面线程对应的tid是16进制的。
![](https://img-my.csdn.net/uploads/201304/24/1366813002_5839.jpg)
![](https://img-my.csdn.net/uploads/201304/24/1366813046_6511.jpg)
![](https://img-my.csdn.net/uploads/201304/24/1366813007_1254.jpg)
最后得到的线程pid的16进制的值为 7C84
6.在 c盘的31372.stack文件中查找 7C84
![](https://img-my.csdn.net/uploads/201304/24/1366813122_3788.jpg)
由于是我的程序已经该过了，这里没有异常的东西，所以这里没有什么异常内容。

我的问题没解决之前，找到到这里的内容为:
[java][view plain](http://blog.csdn.net/hexin373/article/details/8846919#)[copy](http://blog.csdn.net/hexin373/article/details/8846919#)
- "Thread-23" prio=6 tid=0x03072400 nid=0x1b68 runnable [0x0372f000]  
-    java.lang.Thread.State: RUNNABLE  
-     at com.horn.util.MyEncrypt.encode(MyEncrypt.java:17)  
-     at com.horn.common.OrderUtil.hisExp(OrderUtil.java:228)  
-     at com.horn.util.MsgManage.receiveMsg(MsgManage.java:961)  
-     at com.horn.util.PollMessageThread.run(PollMessageThread.java:74)  
- 
-    Locked ownable synchronizers:  
-     - None  
于是 打开 t com.horn.util.MyEncrypt.encode(MyEncrypt.java:17)
分析了下代码，问题找到了。
问题代码为:
[java][view plain](http://blog.csdn.net/hexin373/article/details/8846919#)[copy](http://blog.csdn.net/hexin373/article/details/8846919#)
- // 100-999的随机数
- int random = (int) (Math.random() * 1000);  
- while (random < 100) {  
-     random = random * 10;  
- }  
这样票眼看 是没问题
当时我写这段代码也没注意
关键在于 Math.random()的取值范围是大于0小于1 是吧?
如果Math.random() 的值为 0.00009以下... 就成死循环了...
现在修改为了
[java][view plain](http://blog.csdn.net/hexin373/article/details/8846919#)[copy](http://blog.csdn.net/hexin373/article/details/8846919#)
- // 100-999的随机数
- int random = new Random().nextInt(900) + 100;  
希望能给遇到问题的朋友一些帮助。
