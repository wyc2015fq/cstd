# zabbix_agentd客户端安装与配置(windows操作系统) - DoubleLi - 博客园






**　**********  客户端操作   ************







** 标注：**监控zabbix_agentd客户端安装对象是win server 2008操作系统 64位。







1、  下载zabbix_agentd监控客户端软件安装包(windows操作系统客户端)



**官方下载地址：****[http://www.zabbix.com/download](http://www.zabbix.com/download)**





![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185121207-2121509733.png)









**2、关闭监控主机windows server 2008防火墙或防火墙入放行zabbix_agentd客户端口号  10050 (TPC/UDP)。**









**3、下载后解压zabbix_agents_3.2.0.win.zip 压缩包，里面有两个文件夹，一个是bin文件夹，另一个是conf文件夹。**

**Bin文件夹里面有两个文件夹，一个是win32文件夹里存放zabbix_agentd安装程序应用于windows 32位操作系统，**

**一个是win64文件夹里存放zabbix_agentd安装程序应用于windows 64位操作系统。**

**Conf文件夹里存放是配置文件zabbix_agentd.win.conf**











4、在windows server 2008 操作系统  C盘目录下创建一个zabbix文件夹，把刚下载的zabbix_agentd压缩包里的win64位文件夹的**zabbix_agentd.exe、zabbix_get.exe、zabbix_sender.exe、dev文件夹**复制到zabbix文件夹里



安装程序和conf文件夹**zabbix_agentd.win.conf**复制到windows server 2008操作系统C盘创建的zabbix文件夹里。(文件夹创建路径可以自定义，

但后面zabbix_agentd客户端运行安装命令需要根据创建的路径运行)



![](https://images2017.cnblogs.com/blog/435896/201711/435896-20171108100839544-2141788858.png)







5、右键以文本格式编辑zabbix_agentd.win.conf 配置文件。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185225910-1912791309.png)



修改下面几项



**EnableRemoteCommands=1  **         #允许在本地执行远程命令



**LogRemoteCommands=1 **              #执行远程命令是否保存操作日志



**Server = 10.8.9.155     **                  #填写zabbix服务器IP地址



**Hostname=wintest01  **                  #zabbix_agent客户端计算机名 (被监控主机)













6、桌面 ----  开始 ---- 运行  -----  输入cmd 打开DOS命令窗口---- 输入以下两条命令进行zabbix客户端安装。(必须要以管理员身份运行打开DOS命令窗口)



**C:\zabbix\zabbix_agentd.exe -i -c C:\zabbix\zabbix_agentd.win.conf  **                    #安装zabbix客户端



**C:\ zabbix\ zabbix_agentd.exe -s -c C:\zabbix\zabbix_agentd.win.conf**                  #启动zabbix服务



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185321472-1569773323.png)











**特殊操作：**

**如果你上面的都启动成功了，请忽略下面的内容**



有一些小伙伴由于路径搞错，导致服务起不来，需要卸载服务再次进行安装。

**首先需要在服务界面将Zabbix Agent服务给停掉**

![](https://images2017.cnblogs.com/blog/435896/201711/435896-20171107102437731-2057546357.png)





**然后在CMD窗口下执行删除zabbix客户端操作**



**C:\zabbix\zabbix_agentd.exe  -d  -c  C:\zabbix\zabbix_agentd.win.conf**



**删除成功，接着再次从第一步开始。**























**　　　　**********  服务端操作  ************





1、选择配置 ---- 主机 ---- 创建主机。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185422144-1079892766.png)









2、输入客户端计算机名 --- 可见名称自定义 ---- 群组自行选择 ---- 输入客户端计算IP地址 ---- 勾选已启用 ---- 选择添加。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185443613-902284259.png)









3、添加 zabbix_agentd 客户端监控模版。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185501691-604744601.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185511269-38142273.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185527504-510733062.png)









4、把zabbix_agent客户端添加到拓扑图，以图形方式显示zabbix_agentd客户端主机。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185808847-1620412821.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185820175-1089145596.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185831582-1817246099.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185844644-1102498664.png)

![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170213185858285-117489009.png)



















**特殊操作**





**标注：**Zabbix服务器发送远程关机命令让zabbix_agent客户端主机关机







1、管理 ---- 脚本 ---- 创建脚本。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170214093702925-1128095182.png)







2、  填写脚本相应信息。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170214093735941-1985890727.png)







3、监测中 ----  拓扑图  -----  鼠标左键点击相应服务器 ----  选择 shutdown关机命令。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170214093800316-1139317489.png)









4、zabbix_agent客户端 windows server 2008 系统提示关闭计算机提示。



![](https://images2015.cnblogs.com/blog/435896/201702/435896-20170214093829113-376518035.png)









