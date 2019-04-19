# （GPS移植三部曲）Linux下移植GPS应用程序之常见问题的分析与解决方法之三 - 2019跟着小虎玩着去软考 - CSDN博客
2012年05月22日 00:30:29[littletigerat](https://me.csdn.net/littletigerat)阅读数：3905
GPS应用程序移植常见问题分析与解决
|**ID**|**Issue Description**|**Analyse**|**Causation**|**Solution**|
|----|----|----|----|----|
|1|导航系统一运行，就退出|使用ps命令，发现进程中cld_navi应用程序不存在|Linux系统对大小写字母敏感，也就是区分大小写|对目录NAVIONE下的文件名和目录名逐一核对大小写，发现地图格式cld文件大小写不正确|
|2|发现导航系统运行慢|在输入激活码的时候,点击数字,就发现慢;|1.cat /proc/meminfo查看内存使用情况，已用的，未用的；2.stop查看是哪个进程耗CPU3.单独分析第一屏界面显示慢的问题4.分析到底是读文件慢，还是显示Image慢5.对执行的函数，做耗时测试|1、drawImage耗时340ms改用framebuffer显示机制2、颜色转换耗时180ms去掉颜色深度16色转32色的机制3、矩阵变换耗时340ms，因为凯立德以前给客户做的是宽屏分辨率640X480(宽X高)，而我司的屏分辨率480X640(宽X高)|
|3|采集GPS数据全部都为0|1.能接受的GPS数据，说明GPS芯片良好，能正常工作2.接受的GPS数据不对，说明接受GPS天线，信号不好；|在办公室做测试，窗户封闭，信号不好|露天或者打开窗户，靠近窗户做测试|
|4|运行导航的时候,有如下提示:Cannot open keyboard: No such file or directory|Lily开发板，没有该问题|没有指定键盘,触摸屏的驱动路径|export QWS_KEYBOAR=$QWS_KEYBOAR:/dev/input/event1|
|5|点击触摸屏，无法进入导航的主界面|1.qpe的应用程序，也就是Idle界面，可以点击触摸屏，说明触摸屏驱动程序正常；2.唯独导航系统，触摸屏不好使，说明env环境变量没有配置好；3.Lily开发板的导航系统触摸屏正常，而Bali开发板的导航系统触摸屏不正常|Qpe开发平台的开发环境发生改变|**运行****.**/opt/Qtopia/qpe.env，重新配置系统参数，环境变量|
|6|如何将文件拷贝到SD卡||Window不识别linux系统的ext3文件格式|1.linux系统直接识别Window系统的Fat16,Fat32文件系统；先在Window系统直接格式化SD卡,再拷贝文件，拿到linux系统下用2.如果是小文件，直接SecureCRT 5.1串口工具rz命令，或者【Transfer】-【Start Zmodem Upload】串口协议RS232传输文件|
|7|插SD卡，linux系统死机重启|偶尔发生，不是每次都有，应该是静电所致|人带静电，电压过大，导致瞬间电流过大|带接地的蓝色手环|
|8|启动Bali板，不能进入Idle界面，始终停留在Qtopia这一帧图像上|1.linux命令，可以正确执行，如ls,cd等，说明linux系统没有问题2.换了张别人的SD卡，能够正常启动，进入带大Icon的界面，以及MainMenu主菜单，说明开发版正常，操作步骤正常；|做卡的程序不对，也就是makesd.sh等脚本不正确，步骤是对的|root/release/makesd_B0_Bali_Flash_20090122_CL7960_cysun_v0.5目录下重新做卡；|
|9|运行提示找不到libQtSvg.so.4库文件|1.执行env查看环境变量，有哪些环境变量和本地变量2.echo $LD_LIBRARY_PATH查看库路径具体是什么3.执行export命令，看有哪些环境变量，已经被导出，可供其他外部程序使用||1.通过命令find / -name libQtSvg.so.4在根目录下查找库文件2.设置库的路径，便于程序执行搜索到：export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/Qtopia/image/lib/|
|10|连接SecureCRT 5.1，串口程序没有反映，就是黑色界面，没有一个字符|1.分析SecureCRT 5.1软件安装好没有2.串口线的驱动安装好没有3.电源安装好没有4.SD卡做好没有|都有可能，逐一排查|1.先确认串口驱动程序安装好【我的电脑】--【属性】-【HardWare】-【Device Manager】-【Ports (COM &LPT)】2.端口选择正确；3.电源是否插好，包括2个方面：一与电源接线板；而与电路开发板4.按电路开发板上的POWER KEY给电路开发版供电，送电。|
