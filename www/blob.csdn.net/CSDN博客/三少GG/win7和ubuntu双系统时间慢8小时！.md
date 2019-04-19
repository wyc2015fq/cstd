# win7和ubuntu双系统时间慢8小时！ - 三少GG - CSDN博客
2010年12月28日 19:14:00[三少GG](https://me.csdn.net/scut1135)阅读数：3958
前一段时间win7的系统时间，机子没上网，每天开机时，总会慢8小时，不知道怎么回事，因为忙别的事情，所以过了那几天才找原因，后来发现windows time服务没有自动，所以想当然认为是这个原因。用了几次也没发现什么异常，因为今天开机时候没插网线，直接进了ubuntu，发现时间快了8小时，插上网线就正常了，所以觉得不太正常，所以重启进了win7，发现慢了8小时，这才意识到可能是两个系统设置不同，导致的这个时间问题。于是google：两个概念：**utc即universal time coordinated，协调世界时gmt即greenwich mean time**，格林尼治平时windows与 mac/linux 缺省看待系统硬件时间的方式是不一样的：windows把系统硬件时间当作本地时间(local time)，即操作系统中显示的时间跟bios中显示的时间是一样的。 
linux/unix/mac把硬件时间当作 
utc，操作系统中显示的时间是硬件时间经过换算得来的，比如说北京时间是gmt 8，则系统中显示时间是硬件时间 8。这样，当pc中同时有多系统共存时，就出现了问题。假如你的ubuntu设置的时区都为北京时间东八区，当前系统时间为9：00am。则此时硬件中存储的实际是utc时间1:00am。这时你重启进入windows后，你会发现windows系统中显示的时间是 1:00am，比ubuntu中慢了八个小时。同理，你在windows中更改或用网络同步了系统时间后，再到ubuntu中去看，系统就会快了**8小时**。在实行
夏令时的地区，情况可能会更复杂些。解决这个问题的方法:1. 可让 ubuntu 不使用 utc 时间与 windows 保持一致。ubuntu默认开启utc,即协调世界时，而win7是使用这种计时方式，这将导致的结果就是windows和ubuntu时间计算有差异
你可以使用以下方法得到一致的时间**：1.****sudo gedit /etc/default/rcS找到这一行：utc=yes把yes改为no**
2. 修改 windows 
对硬件时间的对待方式，这样只在 windows 上修改后就无需在ubuntu 上设置了。让 windows 把硬件时间当作 utc开始-运行-cmd，打开命令行程序(vista则要以管理员方式打开命令行程序方
可有权限访问注册表)，在命令行中输入下面命令并回车代码:reg add 
hklm/system/currentcontrolset/control/timezoneinformation /v 
realtimeisuniversal /t reg_dreplacement /d 1                              **方法1本人试验有效!!!**
**或者！**
XP系统的时间设置不变，mac设成“冰岛——雷克雅末克”的时区，时间调成北京时间！
因为Mac认为bios的时间时gmt时间，即格林尼治时间就是0时区，北京时东八区
而windows认为bios时间是当地时间Local time。
所以如果两个系统都选择北京时区的话，你会发现window时间＋8＝Mac时间。
ANOTHER WAY:
WinXP和MacOSX缺省看待PC的CMOS记录的时钟是不一样的。
WinXP将这个时钟作为本地时间来看待，也就是CMOS时间就是北京时间。
MacOSX将这个时钟作为Coordinated Universal Time (UTC) 世界标准时间看待，也就是Greenwich Mean Time (GMT) 格林威志时间。
所以如果你在MacOSX和WinXP都选北京时间作为本地时区是，一旦连到互联网上，同步过时间后，就会造成时间的不一致。
我们可以将windows xp的缺省对待CMOS的方式改成UTC，也就是和MacOSX一致，方法如下
**修改windows注册表，在HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Control/TimeZoneInformation/中加一项DWORD，RealTimeIsUniversal，把值设为1。**这样你在windows xp 和 MacOSX下将本地时区都设到北京时间，不论是Windows还是MacOSX同步过时间后，都不会影响到另一边。
