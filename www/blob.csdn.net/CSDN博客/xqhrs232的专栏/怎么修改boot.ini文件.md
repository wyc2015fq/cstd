# 怎么修改boot.ini文件 - xqhrs232的专栏 - CSDN博客
2013年08月21日 09:36:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：748
原文地址::[http://jingyan.baidu.com/article/afd8f4de5aee9334e286e994.html](http://jingyan.baidu.com/article/afd8f4de5aee9334e286e994.html)
boot.ini 文件是系统启动引导程序文件，装多系统或者重装系统的时候会用到它。下面就教大家如何操作该文件。
方法一：直接修改法
1.打开
    boot.ini文件在系统根目录下，默认的情况下是隐藏的，可以用记事本打开这个路径，可以在“运行”中输入“c:\\boot.ini”启动该文件。也可以将此隐藏文件先显示出来，再用记事本打开，具体操作如下：“资源管理器”——“工具”——“文件夹选项”——“查看”去掉“隐藏文件和文件夹”前面的勾，并勾选“显示所有文件及文件夹”，这样就可以在系统根目录下看到boot.ini文件了。
2.修改
打开boot.ini文件，里面的内容如下（这里以我的XP系统boot.ini文件作示范，）
[boot loader]
timeout=0
default=multi(0)disk(0)rdisk(0)partition(1)\WINDOWS
[operating systems]
multi(0)disk(0)rdisk(0)partition(1)\WINDOWS="Microsoft Windows XP Professional" /noexecute=optin /fastdetect
下面就各个内容作解释
1）timeout=0延迟时间设置
延迟时间是系统启动引导菜单后在设定的延迟时间内用户没有进行任何操作时，进入默认的操作系统。默认设置是30，如果你只有一个系统的话就设置为0，多系统的话就根据自己的情况设置一个时间。
2）default=multi(0)disk(0)rdisk(0)partition(1)\\WINDOWS默认系统
默认系统就是延迟时间到了后自动进入的系统。我们经常修改的是partition(1)\\WINDOWS这一块，partition(1）所指的就是第1个分区，通常也就是c:盘，WINDOWS就是你的系统文件夹。如果你要默认的操作系统是d:盘的XP，那么这一块的内容就是partition(2)\\WINDOWS
3）multi(0)disk(0)rdisk(0)partition(1)\\WINDOWS=\"Microsoft Windows XP Professional" /noexecute=optin /fastdetect系统列表 
在这个位置列上你的所有操作系统（一行一个系统）
multi(0)disk(0)rdisk(0)partition(1)\\WINDOWS这一块和上面“默认系统”的修改方法一样。=\"\"\"\"中间的内容就是开机显示列表时本系统的名称，可以任意修改，而且还支持中文！/nodetect /noguiboot这部分是启动参数，在\"后面加一个半角空格/参数，可以加多个。下面是各项参数的详细解释：
　　/fastdetect：快速检测，很多网站都推荐修改为/nodetect
　　/SAFEBOOT：安全启动，只启动HKLM\\System\\CurrentControlSetControl\\SafeBoot中的驱动程序和服务，其后可跟三个参数Minimal、Network或Dsrepair。Minimal和Network允许在网络连接下启动系统。而Dsrepair要求系统从备份设备中调入活动目录的设置。还有一个选项是Minimal（AlternateShell），它让系统调入由HKLM\\System\\CurrentControlSetSafeBoot\\AlternateShell指定的SHELL程序，而不使用默认的Explorer。 
　/NOGUIBOOT:不加载VGA驱动程序，也就不会显示启动过程和失败时的蓝屏信息。
/BOOTLOG：将日志写入Nnbtlog.txt文件。 
　/BASEVIDEO：使用标准VGA方式启动，这种方式主要用于显示驱动程序失效时。
/SOS：在调入驱动程序名时显示它的名称，因驱动问题而无法启动时使用比较好。
方法二、其它方法
1.右键单击电脑桌面，选择“属性”“高级”“启动和故障恢复”设置。
2.电脑“开 始”“运行”，输入“msconfig”，切换到“boot.ini”窗口。
