# WINDOWS2003 安全设置 - 左直拳的马桶_日用桶 - CSDN博客
2012年03月27日 12:11:07[左直拳](https://me.csdn.net/leftfist)阅读数：2577
因为安全方面请教机房的人，他们手把手教我一点窍门，整理如下：
1、重命名管理员administrator；停用Guests帐号；设置强密码
2、在运行这里，输入gpedit.msc，打开windows设置。windows设置-》安全设置-》账户策略-账户锁定策略：时间锁定5分钟，次数为5次，复位计数器未3分钟，防止暴力破解。然后打开账户策略-》密码策略，设置强制要求用户设置密码，可以防止其他用户安全意识不够而设置了简单密码，遗留一下漏洞。
3、升级到SP2。因为sp1以及以下版本远程信息不是加密的。
4、打开我的电脑-c盘-右键属性：打开安全选项，查看everyone是否可以读取/写，是的话去掉勾选。
5、运行，输入cmd，运行net share，查看是否有默认共享，有的话，如果没必要请禁用。这个是一个风险，入侵者可以使用默认共享存储/获取文件
如需开启共享，请手动开启文件共享，不要整个盘共享。管理的方法是，在计算机管理里面，计算机管理-系统工具-共享文件夹
6、在计算机里面查找 cmd.exe这个文件，请为有需要用到的用户才勾选权限，最好就是只可以system和管理员有权限，这个是非法控制的一个关键点
7、如有可能，禁用以下服务
alerter 
clipbook
computer browser
messenger
remote registry service
routing and remote acess 
print spooler （打印机服务，一般服务器都不会接打印机的，如果网路上伪造一个打印机地址这样就可以连接到我们的服务器，然后通过警告、错误等提示获取服务器的一些信息，判断这些信息是否可被利用）
automatic updates 最好不要启用自动更新，因为自动更新会造成不可预见的风险，最好是手动更新
8、指定可远程登录的ip地址。在本地连接-属性-tcp/ip协议属性》高级》选项>tcp/ip筛选 输入您指定的可以远程登录的ip地址。
9、修改远程桌面端口。
数据库端口1433要修改，众所周知，其实远程桌面的默认端口3389也应该修改。
“修改服务器端的端口设置 ，注册表有2个地方需要修改。 [HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server\Wds\rdpwd\Tds\tcp] PortNumber值，默认是3389，修改成所希望的端口，就如你所说该成6222。第二个地方： [HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp] PortNumber值，默认是3389，修改成所希望的端口，该成6222，重起一下就OK了。这样就可以了　　
要注意的是：当你的计算机修改完端口号后要想继续使用远程桌面，并且计算机有启用防火墙，则必须在防火墙例外中添加所修改的端口号。否则用3389与修改后的端口号都将连不上远程桌面。”
修改端口之前，应该看一下新端口是否已经被使用了。命令：
netstat -aon|findstr "3389"
如果没有被使用，应该没有什么返回。
远程桌面带端口的服务器，格式是这样子滴：IP:PortNumber
