# 通过 jstack 与 jmap 分析一次线上故障 - z69183787的专栏 - CSDN博客
2019年01月21日 10:26:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：51
[https://www.cnblogs.com/kingszelda/p/9034191.html](https://www.cnblogs.com/kingszelda/p/9034191.html)
# 一、发现问题
  下面是线上机器的cpu使用率，可以看到从4月8日开始，随着时间cpu使用率在逐步增高，最终使用率达到100%导致线上服务不可用，后面重启了机器后恢复。
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514003417351-1333416698.png)
# 二、排查思路 
简单分析下可能出问题的地方，分为5个方向：
1.系统本身代码问题
2.内部下游系统的问题导致的雪崩效应
3.上游系统调用量突增
4.http请求第三方的问题
5.机器本身的问题
# 三、开始排查 
1.查看日志，没有发现集中的错误日志，**初步排除代码逻辑处理错误。**
2.首先联系了内部下游系统观察了他们的监控，发现一起正常。**可以排除下游系统故障对我们的影响。**
3.查看provider接口的调用量，对比7天没有突增，**排除业务方调用量的问题。**
4.查看tcp监控，TCP状态正常，**可以排除是http请求第三方超时带来的问题。**
5.查看机器监控，6台机器cpu都在上升，每个机器情况一样。**排除机器故障问题。**
**即通过上述方法没有直接定位到问题。**
# 四、解决方案
1.重启了6台中问题比较严重的5台机器，先恢复业务。保留一台现场，用来分析问题。
2.查看当前的tomcat线程pid
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514004234948-269659542.png)
3.查看该pid下线程对应的系统占用情况。top -Hp 384
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514100850741-1821148052.png)
4.发现pid 4430 4431 4432 4433 线程分别占用了约40%的cpu
5.将这几个pid转为16进制，分别为114e 114f 1150 1151
6.下载当前的java线程栈  sudo -u tomcat jstack -l 384>/1.txt
7.查询5中对应的线程情况，发现都是gc线程导致的
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514100916449-1325278223.png)
8.dump java堆数据
sudo -u tomcat jmap -dump:live,format=b,file=/dump201612271310.dat 384
9.使用MAT加载堆文件，可以看到javax.crypto.JceSecurity对象占用了95%的内存空间，初步定位到问题。
MAT下载地址：[http://www.eclipse.org/mat/](http://www.eclipse.org/mat/)
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514101651715-90687757.png)
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514101016818-56682802.png)
10.查看类的引用树，看到BouncyCastleProvider对象持有过多。即我们代码中对该对象的处理方式是错误的，定位到问题。
# 五、代码分析
我们代码中有一块是这样写的
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514101048185-1240846024.png)
这是加解密的功能，每次运行加解密都会new一个BouncyCastleProvider对象,放倒Cipher.getInstance()方法中。
看下Cipher.getInstance()的实现，这是jdk的底层代码实现，追踪到JceSecurity类中
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514101116518-700840740.png)
verifyingProviders每次put后都会remove,verificationResults只会put，不会remove.
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514101143847-178910545.png)
看到verificationResults是一个static的map，即属于JceSecurity类的。
**所以每次运行到加解密都会向这个map put一个对象，而这个map属于类的维度，所以不会被GC回收。这就导致了大量的new的对象不被回收。**
# **六、代码改进**
**将有问题的对象置为static，每个类持有一个，不会多次新建。**
![](https://images2018.cnblogs.com/blog/1196330/201805/1196330-20180514101210937-1764782322.png)
# **七、本文总结**
遇到线上问题不要慌，首先确认排查问题的思路：
- 查看日志
- 查看CPU情况
- 查看TCP情况
- 查看java线程，jstack
- 查看java堆，jmap
- 通过MAT分析堆文件，寻找无法被回收的对象
