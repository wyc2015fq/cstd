# Arduino Uno安装设备时，出现了一个错误，这个INF中的服务安装段落无效 - V__KING__的专栏 - CSDN博客





2013年11月07日 14:34:26[v__king__](https://me.csdn.net/V__KING__)阅读数：10949









**问题：在windows系统下，Uno安装设备时，出现了一个错误，这个INF中的服务安装段落无效**

**原因：缺少系统文件**

**解决方案：**


1. 先下载：usbser.zip。下载地址：[http://download.csdn.net/detail/v__king__/6549415](http://download.csdn.net/detail/v__king__/6549415)

2. 解压

3.将里面的usbser.sys移动到  C:\WINDOWS\system32\drivers目录下面


4.重新启动




**问题2: 如果问题解决了，下面就不用看了。如果还是不行，那么就按照下面方法做**

**原因：还是缺少系统文件**

**解决方案：**


**1、**打开 C:\Windows\inf\setupapi.dev 打开硬件安装日志,搜索arduino，出现：

Copying driver package files to 'C:\Users\ADMINI~1\AppData\Local\Temp\{507fd0d4-6b4c-429e-e53a-a4040be2ef30}'.

     inf:                Opened INF: 'c:\users\administrator\desktop\arduino-1.0.5-r2\drivers\arduino.inf' ([strings])

!    inf:                Unable to load INF: 'C:\Windows\System32\DriverStore\FileRepository\mdmcpq.inf_amd64_neutral_fbc4a14a6a13d0c8\mdmcpq.inf'(00000003)

!    inf:                Error 3: The system cannot find the path specified.

!    inf:                Could not find include INF file "mdmcpq.inf". Error = 0x00000003

**2、**在C:\Windows\System32\DriverStore\FileRepository\处建立文件夹mdmcpq.inf_amd64_neutral_fbc4a14a6a13d0c8

**3、**将mdmcpq.inf_amd64（自己下载，其它帖子里面有[http://download.csdn.net/detail/jilong17/6320297](http://download.csdn.net/detail/jilong17/6320297)）文件夹里面的文件复制到mdmcpq.inf_amd64_neutral_fbc4a14a6a13d0c8文件夹中。


**4、**X:\Program Files\Arduino\drivers，去这里，解压Old_Arduino_Drivers，里面有UNO的驱动程序

然后去设备管理器中找到那个有叹号的UNO，更新驱动，指向刚才解压得到的文件夹。



总之：各个电脑安装文件夹名称可能有出入，但原理相同！我的版本是arduino 1.0.5 r2，装在D盘！





[http://blog.csdn.net/v__king__/article/details/14451101](http://blog.csdn.net/v__king__/article/details/14451101)





**问题3:终极解决，一定是能够解决的，关键是在window 64位系统下哦**

**原因：还是缺少系统文件，文件系统不对的原因。32位和64位系统是由区别的。windows加载的顺序，**

**在C:\Windows\inf\setupapi.dev中搜索arduino。发现：**

**1. 先是到C:\WINDOWS\system32\drivers下面导入Driver Package（**可能是**usbser.sys）。**

**2. 然后再Importing driver package into Driver Store，就是**C:\Windows\System32\DriverStore\FileRepository\****

**3.然后就是去C:\Windows\System32\DriverStore\FileRepository\mdmcpq.inf_amd64_neutral_fbc4a14a6a13d0c8\mdrivers\arduino.inf**


所以：**C:\WINDOWS\system32\drivers和**C:\Windows\System32\DriverStore\FileRepository\mdmcpq.inf_amd64_neutral_fbc4a14a6a13d0c是有先后顺序的****

**解决方案：**


1. 将[http://download.csdn.net/detail/jilong17/6320297](http://download.csdn.net/detail/jilong17/6320297) 文件夹mdmcpq.inf_amd64_neutral_fbc4a14a6a13d0c8里面的usbser.sys复制到**C:\WINDOWS\system32\drivers**，


2. 文件夹mdmcpq.inf_amd64_neutral_fbc4a14a6a13d0c8里面的mdmcpq.inf和mdmcpq.PNF复制到C:\Windows\inf中

3. ****mdmcpq.inf_amd64_neutral_fbc4a14a6a13d0c文件直接复制到****C:\Windows\System32\DriverStore\FileRepository中********





