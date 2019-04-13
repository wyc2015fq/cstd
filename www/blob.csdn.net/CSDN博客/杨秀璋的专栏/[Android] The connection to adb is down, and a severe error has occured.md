
# [Android] The connection to adb is down, and a severe error has occured - 杨秀璋的专栏 - CSDN博客

2014年10月20日 01:49:38[Eastmount](https://me.csdn.net/Eastmount)阅读数：4183标签：[Android																](https://so.csdn.net/so/search/s.do?q=Android&t=blog)[adb is down																](https://so.csdn.net/so/search/s.do?q=adb is down&t=blog)[Eclipse																](https://so.csdn.net/so/search/s.do?q=Eclipse&t=blog)[配置环境																](https://so.csdn.net/so/search/s.do?q=配置环境&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Eclipse&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=adb is down&t=blog)个人分类：[Android																](https://blog.csdn.net/Eastmount/article/category/2645447)[Error报告																](https://blog.csdn.net/Eastmount/article/category/1929669)[
							](https://blog.csdn.net/Eastmount/article/category/2645447)
所属专栏：[Android实例开发与学习](https://blog.csdn.net/column/details/eastmount-android.html)
[
																	](https://so.csdn.net/so/search/s.do?q=adb is down&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=Android&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=Android&t=blog)

**一.ADB server didn't ACK**
在配置完Android环境后,运行第一个程序时,遇到了如下错误：
ADB server didn't ACK
* failed to start daemon *
它可能的原因是端口5037被占用,需要查找占用5037（Android默认）端口的PID,在cmd中输入：
netstat -ano | findstr "5037"
输出进程：
TCP    127.0.0.1:5037         0.0.0.0:0              LISTENING       9292
TCP    127.0.0.1:5037         127.0.0.1:49422        ESTABLISHED     9292
TCP    127.0.0.1:49422        127.0.0.1:5037         ESTABLISHED     3840
此时把该进程“9292”结束,代码如下：
TASKLIST | findstr "9292"
**二.The connection
 to adb is down, and a severe error has occured**
在执行上面的结果后可能还会遇到一个错误,经常出现的错误：
The connection to adb is down, and a severe error has occured.
You must restart adb and Eclipse.
Please ensure that adb is correctly located at 'D:\..\platform-tools\adb.exe' and can be executed.
如下图所示：
![](https://img-blog.csdn.net/20141020011547796)
解决方法是在cmd中调用adb kill-server,再调用adb start-server开启服务.但是可能会出现错误：
adb server is out of date.  killing...
ADB server didn't ACK
* failed to start daemon *
![](https://img-blog.csdn.net/20141020012357246)
这是因为端口号被占用,查看豌豆荚或手机助手占用端口号,将kadb\qadb进程关闭.
具体方法如下：
1.先将Eclipse和豌豆荚关掉,同时结束进程kadb.
![](https://img-blog.csdn.net/20141020012719033)
2.使用cd去到platform-tools文件夹下(含有adb.exe),并开启adb服务如我的指令是：
G:
cdG:\software\Program software\Android\adt-bundle-windows-x86_64-20140321\sdk\platform-tools
adb start-server
成功时输出：
* daemon not running.Starting it now on port 11888*
* daemon started successfully *
![](https://img-blog.csdn.net/20141020012825015)
最后点击platform-tools文件夹中adb.exe应用程序,再运行程序即可.
注意上面的程序上面的程序输出端口号为11888,如果你的还是不成功,可以修改环境的端口号,如下：
![](https://img-blog.csdn.net/20141020012944328)
修改端口号再按上面的步骤实现即可.这是配置Android+Eclipse的错误解决方案,希望对你有用~
(By:Eastmount 2014-10-20 夜2点http://blog.csdn.net/eastmount/)

