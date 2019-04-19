# 解决android模拟器上不了网的问题 - xqhrs232的专栏 - CSDN博客
2011年03月15日 17:03:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1156
原文地址::[http://hi.baidu.com/hbzha/blog/item/797b41cdea9d390400e92865.html](http://hi.baidu.com/hbzha/blog/item/797b41cdea9d390400e92865.html)
Windows下，配置好Adroid环境变量后(如将d:android-sdk-windows-1.0_r1 ools加入系统变量PATH)，在命令行窗口输入：
　　emulator
　　启动Android 模拟器后，　输入：
　　adb shell
　　进入adb shell模式：
　　将网络连接代理设置写入配置数据库，假如你的上网代理IP是10.193.xx.xx：
　　sqlite3 /data/data/com.android.providers.settings/databases/settings.db "INSERT INTO system VALUES(99,'http_proxy','10.193.xx.xx:1080')"
　　查询一下是否成功更改了系统设置：
　　sqlite3 /data/data/com.android.providers.settings/databases/settings.db "SELECT * FROM system"
　　结果中应有：99|http_proxy|10.193.xx.xx:1080
　　重启模拟器，应该可以使用Browser上 Internet了.
　　删除刚刚写入的配置信息方法：
　　sqlite3 /data/data/com.android.providers.settings/databases/settings.db "DELETE FROM system WHERE _id=99"
Android模拟器默认的地址是10.0.2.3，默认的DNS也是10.0.2.3，对于在家里上网学习Android的人（像我）来讲，一般电脑的IP都是192.168.1.100之类的，不在同一个网段。所以就会出现电脑可以上网但是模拟器不能上网的情况。其实设置方法很简单，只要把模拟器的默认DNS设置成电脑的DNS地址即可。
第一步：用系统的命令进入Android开发包的tools目录
 cd X:/.../android-sdk-windows/tool
第二布：使用adb的shell，确认系统的各项属性
adb shell
getprop 
getprop会列出系统当前的各项属性
第三步：得到模拟器的DNS地址
在结果里可以看到：
[net.dns1]: [10.0.2.3]
[net.dns2]: [10.0.2.4]
[net.dns3]: [10.0.2.5]
[net.dns4]: [10.0.2.6]
第四步：把dns改成我们自己的DNS
setprop net.dns1 192.168.1.1
注：一般TP-LINK及D-LINK的DNS都是192.168.1.1，如果不是请使用ipcnofig /all命令确认。
大功告成，再去使用模拟器的浏览器的话，你就可以发现你已经可以上网了。
我在网上查了一下，发现有以下四种方式： 
一：将网络连接代理设置写入配置数据库 
1、通过命令行或者通过双击emulatoer可执行文件，打开模拟器 
2、在命令行执行adb shell 打开android的控制台 （确保环境变量设置正确，即已经把Android_SDK/tools 添加到了PATH(Linux)/path(Windows)）
3、执行 ls -l /data/data/com.android.providers.settings/databases/settings.db ，查看系统设置数据库文件是否存在
4、如果数据库文件存在，执行 sqlite3 /data/data/com.android.providers.settings/databases/settings.db，以settings.db为当前scheme启动SQLITE3
5、执行 INSERT INTO system VALUES(99,’http_proxy’, ‘192.168.0.245:8080′); 语句插入数据，其中192.168.0.245是我用的代理服务器IP，8080是端口，请根据具体情况替换
6、执行 SELECT * FROM system; 来查看刚才插入的数据 
7、重启模拟器，以使代理服务器设置生效 
8、可以使用 DELETE FROM system WHERE _id=99; 语句来删除代理设置，或者使用 UPDATE system SET value = ‘192.168.0.245:8070′ WHERE _id=99; 来修改代理服务器设置
二：可能是DNS的问题 
1、用系统的命令进入Android开发包的tools目录 
cd E:/android-sdk-windows/tools 
2、使用adb的shell，确认系统的各项属性 
adb shell 
getprop 
getprop会列出系统当前的各项属性 
3、得到模拟器的DNS地址 
在结果里可以看到： 
[net.dns1]: [10.0.2.3] 
[net.dns2]: [10.0.2.4] 
4、把dns改成我们自己的DNS 
setprop net.dns1 192.168.1.1 
注：一般TP-LINK及D-LINK的DNS都是192.168.1.1，如果不是请使用ipcnofig /all命令确认。 
三：直接带参数启动模拟器 
emulator @default -http-proxy 10.16.xx.xx:8080 
其中default是模拟器的名称 
四：在模拟器中设置 
启动模拟器，然后进入 Settings->Wireless & networks->Mobile networks->Access Point Names   
然后打开出现在列表中的access point; 
然后下面这样设置： 
- Proxy : your proxy address   
- Port : your proxy port 
- Username : your username if needed, or <Not set> 
- Password : your password if needed, or <Not set> 

