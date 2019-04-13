
# LoadRunner运行错误集 - anlcy - 博客园






# [LoadRunner运行错误集](https://www.cnblogs.com/camilla/p/10501902.html)
1.error-27796错误解决方法：
在负载生成器的注册表HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/Tcpip/Parameters里，有如下两个键值：
TcpTimedWaitDelay
MaxUserPort
a,这里的TcpTimedWaitDelay默认值应该中是30s，所以这里，把这个值调小为5s（按需要调整）。
b,也可以把MaxUserPort调大65534（如果这个值不是最大值的话）。
打开loadrunner Controller，打开run-time Setting for script的设置界面，选中Browser，将默认勾选
的simulate a new vuser on each iteration取消勾选，运行场景，不再出现error：27796
2、报如下错误：Action.c(4):Error-27727:Step download timeout(120 seconds) has expired when downloading resource(s).Set the "Resource Page Timeout is Warning"Run-Time Setting to Yes/No have this message as a warning/error,respectively
解决的方法：
Run-Time Setting -- Internet Protocol -- Preferences -- Option -- Step download timeout(sec)改为15000(根据需要可能更大)
3、Error -27728: Step download timeout (120 seconds)
解决的办法：设置了runt time setting中的internet protocol-preferences中的advaced区域有一个winlnet replay instead of sockets选项，选项后再回放就成功了。
4、Loadrunner8.0和IE7.0的兼容问题
录制脚本的时候选择New Multiple Protocol Scipt  -- web -- Program to record 选择ie7即可。
5、Error -26612: HTTP Status-Code=500 (Internal Server Error) for "[http://](http://t-a.jianke.com:9110/Admin_Admin_Main.aspx?action=getdeptlist)XXX“，错误原因汇总：
a、运行的用户数过多，对服务器造成的压力过大，服务器无法响应。
b、该做关联的地方没有去做关联。
c、录制时请求的页面、图片等，在回放的时候服务器找不到，则报HTTP500错误，若该页面无关紧要，则可以在脚本中注释掉。
d、参数化时的取值错误。
e、更换了应用服务器(中间件的更换，如tomcat、websphere、jboss等)，还是利用原先录制的脚本去运行，
则很可能报HTTP500错误。因为各种应用服务器处理的机制不一样，所录制的脚本也不一样，解决办法只有重新录制脚本。
f、Windows xp2 与ISS组件不兼容，则有可能导致HTTP500错误。对ISS组件进行调整后问题解决。
g、系统开发程序写的有问题，则报HTTP500错误。例如有些指针问题没有处理好的，有空指针情况的存在。修改程序后问题解决。
需检查脚本解决问题；
6、错误 -26601: 解压缩函数(wgzMemDecompressBuffer)失败，返回代码=-5，解决方法：
Run-Time Setting -- Internet Protocol -- Preferences -- Option -- Network buffer size改为122880；
7、错误 -27492: “HttpSendRequest”失败，Windows 错误代码=12002，且已超出 URL="http://www.xxx.com/" 的重试限制(0)
解决方法：runtime setting中的preferences- ->options-->http request connect timeout(sec)的值设为999.
8、Failed to send data by channels - post message failed.解决办法：
解决办法1： 在LR的controller负载生成器的菜单栏，单击【Diagnostics】》configuration》
Web Page Diagnostics【Max Vuser Sampling 10%】设置为【Eenable】。
解决办法2：直接去掉勾选Enable the following diagnostics即可。






