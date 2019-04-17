# 在win7环境下oracle10g安装问题 - 博客堂 - CSDN博客





2015年05月21日 15:45:47[最小森林](https://me.csdn.net/u012052268)阅读数：5296








虽然oracle10g这个版本已经比较老，但是他比较经典。并且许多高校都用这个版本。所以把安装过程中所有问题贴出来。



ORACLE 10g下载地址现在直接点击不能下载了要经过oracle许可才可以下载如果嫌麻烦可以用迅雷直接下载

32位：(经检验64位电脑也可以用)

Oracle Database 10g Release 2 (10.2.0.1.0) Enterprise/Standard Edition forMicrosoft Windows (32-bit)


  http://download.oracle.com/otn/nt/oracle10g/10201/10201_database_win32.zip









2、 然后解压

3、 退出360等，最好关闭防火墙。



4、  让陈旧的oracle10g支持高大上的win7，呵呵：



更改解压好的文件夹里面：

.. \database\stage\prereq\db下的refhost.xml文件，在任意一个</CERTIFIED_SYSTEMS>之后增加


**<!--MicrosoftWindows 7-->**

**    <OPERATING_SYSTEM>**

**      <VERSION VALUE="6.1"/>**

**</OPERATING_SYSTEM>**



5、  更改.. \database\install下的oraparam.ini文件，增加如下项：



Windows=5.0,5.1,5.2,6.0,**6.1[Windows-6.1-required]#Minimum display colours for OUI to runMIN_DISPLAY_COLORS=256#Minimum CPU speed required for OUI#CPU=300[Windows-6.1-optional]**

6、  右击“stuep.exe”文件--》选择“属性”--》兼容性--》选中兼容系统

然后就可以开始安装了，（以管理员身份运行此程序）首先会弹出一个框说是找不到先决条件(这个很正常，因为上面被你更改了配置)，不要紧张，耐心等待一会就会开始安装了，千万不要关闭这个窗口。



7、 ERROR 2正在检查网络配置要求…

检查完成。此次检查的总体结果为: 失败 <<<< 

问题: 安装检测到系统的主 IP 地址是 DHCP 分配的地址。 

建议案: Oracle 支持在具有 DHCP 分配的 IP 地址的系统上进行安装。但在安装之前, 必须将Microsoft LoopBack Adapter 配置为系统的主网络适配器。有关在配置有 DHCP 的系统上安装软件的详细信息, 请参阅 Installation Guide。 
解决方案：
1.单击开始图标，在智能搜索中输入“hdwwiz”，在搜索结果中鼠标右键单击该程序，使用“以管理员身份运行”方式来启动。 

2. 根据操作系统向导，选择“**安装我手动从列表选择的硬件**（高级）”。 

3.在硬件列表中，选择“**网络适配器**”。 

4.选择“**Microsof**t”厂商，并在右边网络适配器列表中选中“**MicrosoftLoopback Adapter**”，下一步按照向导完成安装。 

5 然后进入 控制面板\网络和 Internet\网络连接 把当前的本地连接禁止，然后给新增的Microsoft LoopBack Adapter本地连接指定个IP:192.168..0.1，然后启动。 

OK,然后点Oracle中的重试，this’s Problem Resolve！





8、ERROR 2正在检查网络配置

出现错误："无法确定主机的IP地址时产生异常错误"

问题原因：找不到本机系统的IP地址，无法解析机器名。

解决办法：在hosts文件下手动修改。

 （1）打开c:\windows\system32\drivers\etc\hosts文件



手动加入：      127.0.0.1       localhost

192.168.0.1   lenovothink




                   第二行，每台机器不一样！那么 怎么找到自己机器的IP地址和机器名？



（1）      IP地址网络连接->本地连接






（2）      本机的机器名：右键 我的电脑 即可看到。




























情况3:

如果企业管理控制器打不开怎么办呢？

1.      检查自己的IP地址与host文件中的是否一致。

2.      如果不一致，更改host

3.      如果一致重启电脑即可。

情况四：

出现错误时登录企业管理器时出现的界面






出现这种错误一般是因为没有设置时区，一般默认的是agentTZRegion=GMT，也就是GMT。所以大家只要设置了这个东西，然后重新启动dbconsole就可以了。下面是设置以及重新启动dbconsole的全过程：

**第一步，**在Oracle安装目录中找打这个文件emd.properties（以往大家都是找不到这个文件在哪里），D:\oracle\product\10.2.0\db_1\lenovothink_orcl\sysman\config，我的这个文件就是在这个路径下。估计大家都能找到这里D:\oracle\product\10.2.0\db_1，再往下就是**lenovothink_orcl**文件夹，这个是**我的计算机名**_数据库全局变量，大家只要找到以自己计算机命名的这个文件即可，然后依次找到sysman\config这个路劲下，然后就找到了emd.properties这个文件。然后用记事本打开这个文件，在此文件的最后一行你就可以看到agentTZRegion=GMT。



**第二步**，将agentTZRegion=GMT中的GMT改成Asia/Shanghai，也就是agentTZRegion=Asia/Shanghai，这里有个问题就是Asia一定首字母大写，Shanghai的首字母也要大些，你也可以用其他的地区，比如Asia/Beijing，不过我试过这个在我的机器上行不通，而改成Shanghai之后便可以正确显示，这个我也不清楚为什么，关于时区的列表参考：10.2.0\db_1\sysman\admin\supportedtzs.lst这个路径的文件去查找下中查找。改完后保存就行。



**第三步，**在cmd下输入输入此命令，>setoracle_sid=orcl（orcl也就是数据库全局变量名，也就是数据库名），当然回车之后没任何反应。如下图：





**第四步，**继续输入命令>emctlstop dbconsole（大家一定要注意emctl，最后一个字母是L，不是1，笔者因为把l弄成1，费了好大劲，希望大家不要犯我的错误）。这一步就是关闭dbconsole，回车之后如下图：





**第五步，**最后一步，启动原来关闭的dbconsole服务。键入命令>emctl
 start dbconsole，回车之后如下图：






好了，经过这些操作之后大家重新启动浏览器，然后重新登录企业管理器就会发现java.lang.Exception:Exception in sending Request :: null这个错误没有了。我重新启动浏览器，重新登录后的界面如下图：



















**Fujia附加的，卸载过程：**

1.      停止所有与ORACLE相关的服务。

Ctrl+alt+delete…..



2. 使用OUI（OracleUniversal Installer）卸载Oracle软件。

　“开始”->“程序”->“Oracle-OraDb110g_home1/Oracle installation product/Universalinstaller.

或者安装的那个程序：setup.exe也可以卸载。



3.删除注册表内容。

　     运行regedit命令，删除下面内容：

HKEY_LOCAL_MACHINE/SOFTWARE/ORACLE注册表键，删除此键。



　　HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services，删除Services键下所有以oracle为首的键。



　　HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/Eventlog/Application， 删除此键下所有以oracle为首的键。



HKEY_CLASSES_ROOT，删除此键下所有以Ora，Oracle，Orcl，EnumOra 为前缀的键。



HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Explorer/MenuOrder/StartMenu/Programs，删除此键下所有以oracle为首的键。



　　HKEY_LOCAL_MACHINE/SOFTWARE/ODBC/ODBCINST.INI注册表键，删除了MicrosoftODBC FOR ORACLE注册表键以外的所有有Oracle字样的键值。



4.最后在文件系统内删除ORACLE相关的文件及目录：如果无法删除，就用360粉碎删除。

5.重新启动。






