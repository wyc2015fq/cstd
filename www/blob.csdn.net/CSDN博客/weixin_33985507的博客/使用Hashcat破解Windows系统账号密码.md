# 使用Hashcat破解Windows系统账号密码 - weixin_33985507的博客 - CSDN博客
2016年01月08日 16:48:54[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：45
使用Hashcat破解Windows系统账号密码
Simeon
笔者最近对oclHashcat工具破解密码进行了研究，偶有所得，因此撰文跟大家一起分享，本次破解对象选择破解Windows7用户密码。oclHashcat号称世界上最快的密码破解，世界上第一个和唯一的基于GPGPU规则引擎，免费多GPU（高达128个GPU），多哈希，多操作系统（Linux和Windows本地二进制文件），多平台（OpenCL和CUDA支持），多算法，资源利用率低，基于字典***，支持分布式破解等等。oclHashcat for AMD下载地址：http://hashcat.net/files/oclHashcat-1.31.7z，oclHashcatfor NVidia下载地址：http://hashcat.net/files/cudaHashcat-1.31.7z，oclHashCat系列软件在硬件上支持使用CPU、NVIDIA GPU、ATI GPU来进行密码破解。在操作系统上支持Windows、Linux平台，并且需要安装官方指定版本的显卡驱动程序，如果驱动程序版本不对，可能导致程序无法运行。NV users  GPU破解驱动需要ForceWare331.67以及更高版本，AMD 用户则需要Catalyst 14.9以及更高版本，可以通过Catalyst 自动侦测和下载检测工具来检测系统应该下载那个版本，下载地址：[http://support.amd.com/en-us/download/auto-detect-tool](http://support.amd.com/en-us/download/auto-detect-tool)，还可以通过360软件管理直接搜索Catalyst，选择合适的版本安装即可。
0x00准备工作
（1）kali linux操作系统或者虚拟机
（2）windows7操作系统或者虚拟机
（3）准备字典，可以自己生成字典工具，也可以从互联网获取字典，推荐一个字典下载网站：[http://contest-2010.korelogic.com/wordlists.html](http://contest-2010.korelogic.com/wordlists.html)，http://www.insidepro.com/dictionaries.php
（4）在windows7中新增一个用户antian365，密码为password。在单击“开始”-“运行”中输入“cmd”并按“Shift+Ctrl+Enter”组合键，输入命令“net user antian365 password /add”。或者以管理员权限启动“cmd.exe”程序也可，执行成功后如图1所示。测试完毕后可以通过命令删除该帐号“net user antian365 /del”
![wKiom1aPdzzDDPFdAAE_mdGif4s611.jpg](http://s4.51cto.com/wyfs02/M00/79/62/wKiom1aPdzzDDPFdAAE_mdGif4s611.jpg)
图1添加测试帐号
（5）下载saminside
官方网站目前已经停止saminside软件的开发了，可以到华军软件园下载：[http://gwbnsh.onlinedown.net/down2/saminside.v2.6.1.0.chs.rar](http://gwbnsh.onlinedown.net/down2/saminside.v2.6.1.0.chs.rar)
0x01获取并整理密码hashes值
（1）获取操作系统hash值
获取Windows7操作系统的hash值有多个软件，比如wce，mimikatz，cain以及saminside等，在windows vista以及以上版本都会有UAC权限控制机制。UAC（User Account Control，用户帐户控制）是微软为提高系统安全而在Windows Vista中引入的新技术，它要求用户在执行可能会影响计算机运行的操作或执行更改影响其他用户的设置的操作之前，提供权限或管理员密码。通过在这些操作启动前对其进行验证，UAC 可以帮助防止恶意软件和间谍软件在未经许可的情况下在计算机上进行安装或对计算机进行更改。因此获取密码的工具都需要以管理员身份运行，选择saminside.exe程序，右键单击在弹出的菜单中选择“以管理员身份运行”，然后在saminside程序主界面中从左往右选择第三个图标，下来菜单第二个选项（Import local user using Scheduler）来获取密码，如图2所示，即可获取本机所有帐号的密码hash值等信息。
![wKioL1aPd3nTE-H2AAN_g6Cy9II700.jpg](http://s4.51cto.com/wyfs02/M00/79/60/wKioL1aPd3nTE-H2AAN_g6Cy9II700.jpg)
图2获取密码hash值
（2）整理hash值
在saminside中可以导出所有帐号的hash值，也可以复制单个帐号的hash值。单击“File”菜单中的“导出用户到pwdump文件”即可导出获取的hash值，也可以选择hash，右键单击“复制”-“NT hash”获取NT hash值。对于WindowsVista以上操作系统即使是普通的密码也以“AAD3B”开头的一串字符，这个值目前在“ophcrack”等工具中无法进行破解，在saminside中会显示为一串“0”字符，将NT hash值整理到一个文件中，并命名为win2.hash，如图3所示。
![wKiom1aPd2KQvpksAADn8SwI3S0520.jpg](http://s2.51cto.com/wyfs02/M01/79/62/wKiom1aPd2KQvpksAADn8SwI3S0520.jpg)
图3整理需要破解的hash值
0x02破解hash值
将准备好的字典ptemp.txt、需要破解的hash值文件win2.hash复制到oclHashcat32程序所在文件夹下，执行一下命令进行破解：
oclHashcat32-m 1000 -a 0 -o winpass1.txt --remove win2.hash ptemp.txt
参数说明：
“-m 1000”表示破解密码类型为“NTLM”；
“-a 0”表示采用字典破解；
“-o”将破解后的结果输出到winpass1.txt；
“--remove win2.hash”表示将移除破解成功的hash；
“ptemp.txt”为密码字典文件。
如果密码字典较大，可能会显示“[s]tatus [p]ause [r]esume [b]ypass [q]uit =>”，键盘输入“s”显示破解状态，输入“p”暂停破解，输入“r”继续破解，输入“b”表示忽略破解，输入“q”表示退出，所有成功破解的结果都会自动保存在“oclHashcat.pot”文件中。破解结束会显示如图4所示的信息。
![wKiom1aPd3fC8jl4AAHimALwuKk690.jpg](http://s4.51cto.com/wyfs02/M02/79/62/wKiom1aPd3fC8jl4AAHimALwuKk690.jpg)
图4显示破解信息
0x03查看破解结果
使用“type winpass1.txt”命令查看破解结果，如图5所示，显示该帐号的密码为“password”。
![wKioL1aPd66gCKebAABgTC46kVY011.jpg](http://s1.51cto.com/wyfs02/M02/79/60/wKioL1aPd66gCKebAABgTC46kVY011.jpg)
图5查看密码破解结果
0x04结束语
   oclHashcat功能强大，基本上能够破解目前市面上常见的密码加密算法，比如Discuz！论坛密码算法md5(md5($pass).$salt)，破解命令为
oclHashcat32 -m 2611 -a 0 -o winpass1.txt --remove dz.hash ptemp.txt；
linuxsha512加密方式： oclHashcat32   -m 1800 sha512linux.txtp.txt；
linux下md5加密方式：oclHashcat32   -m 500   linuxmd5.txt p.txt。
本文所用到的软件等工具打包下载地址：http://www.antian365.com/lab/project01/project01.rar，解压密码antian365
