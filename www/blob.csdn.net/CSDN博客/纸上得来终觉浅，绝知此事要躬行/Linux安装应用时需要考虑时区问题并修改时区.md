# Linux安装应用时需要考虑时区问题并修改时区 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月31日 11:53:30[boonya](https://me.csdn.net/boonya)阅读数：1796










关于时区的概念，其实初中地理课已经涉及，很多人都多少了解一些，可能只是细节搞不太清楚。为什么会将地球分为不同时区呢？因为地球总是自西向东自转，东边总比西边先看到太阳，东边的时间也总比西边的早。东边时刻与西边时刻的差值不仅要以时计，而且还要以分和秒来计算。整个地球分为二十四时区，每个时区都有自己的本地时间。在国际无线电通信场合，为了统一起见，使用一个统一的时间，称为通用协调时(UTC, Universal Time Coordinated)。UTC与格林尼治平均时(GMT,
 Greenwich Mean Time)一样，都与英国伦敦的本地时相同。

关于时间的几个标准，如下所示，具体可以Google、Baidu搜索一下：

- CST：中国标准时间（China Standard Time），这个解释可能是针对RedHat Linux。
- UTC：协调世界时，又称世界标准时间，简称UTC，从英文国际时间/法文协调时间”Universal Time/Temps Cordonné”而来。中国大陆、香港、澳门、台湾、蒙古国、新加坡、马来西亚、菲律宾、澳洲西部的时间与UTC的时差均为+8，也就是UTC+8。
- GMT：格林尼治标准时间（旧译格林威治平均时间或格林威治标准时间；英语：Greenwich Mean Time，GMT）是指位于英国伦敦郊区的皇家格林尼治天文台的标准时间，因为本初子午线被定义在通过那里的经线。




装过Linux系统的人，可能都会有这样的经历，就是该机器安装windows系统时，时间正确，但是安装了linux系统后，尽管时区选择正确，也会发现系统时间不对。这是由于安装系统时采用了UTC，那么什么是UTC呢，简单的说UTC就是0时区的时间，是国际标准，而中国处于UTC+8时区。另外还有一种时间是当地时间，而windows采用的就是当地时间。所以安装linux系统时，不要选择使用UTC。


## 1.Linux下定时任务运行时间不一致

很多时候，定时任务都是设置到一个固定的时间点进行业务处理。在windows环境下，通常我们的应用可以很好的运行，一旦切换到Linux下就不一定按时运行，因为Linux默认的时区是UTC，而中国所在的时区是UTC+8，所以Windows与Linux之间存在8小时的时差。这是必须要关注的。

Linux下运行日志：



```
2017-03-31 02:22:24,629 [Thread-9254] INFO  [com.wlyd.fmcgwms.util.api.APIHttpClient] - json:{"CustomerCode":"W001","SerialNo":"","UpdateTime":""}
2017-03-31 02:22:30,309 [Thread-9254] INFO  [com.wlyd.fmcgwms.util.api.APIHttpClient] - statusCode:200
2017-03-31 02:22:30,309 [Thread-9254] INFO  [com.wlyd.fmcgwms.util.api.APIHttpClient] - 调用API 花费时间(单位：毫秒)：5680
2017-03-31 02:22:30,309 [Thread-9254] INFO  [com.wlyd.fmcgwms.util.api.APIHttpClient] - 调用接口状态：0
2017-03-31 02:22:30,309 [Thread-9254] INFO  [com.wlyd.fmcgwms.service.platform.impl.APIForWaasServiceImpl] - 调用WAAS接口.........../updateproduct RESPONSE：{"IsSuccess":false,"OperationDesc":""}
```


实际上windows显示的北京时间是10点多。


## 2.Linux下查询当前系统所属时区



**1：使用date命令查看时区**



```
[root@db-server ~]# date -R
Sun, 11 Jan 2017 07:10:28 -0800
[root@db-server ~]#
```




如上所示，上面命令输出了-0800表示西八区，是美国旧金山所在的时区，下面表示我们国家的东八区（+0800）



```
[root@lnx01 ~]# date -R
Sun, 11 Jan 2017 23:06:02 +0800
```



**2：查看clock系统配置文件**



```
[root@db-server ~]# more /etc/sysconfig/clock
ZONE="America/Los_Angeles"
UTC=true
ARC=false
```



如上所示，表示该系统设置的时区为“America/Los_Angeles”，也就是西八区。




## 3.快速设置Linux系统时区

使用tzselect 命令：



```
root@ubuntu:~# tzselect
Please identify a location so that time zone rules can be set correctly.
Please select a continent, ocean, "coord", or "TZ".
 1) Africa
 2) Americas
 3) Antarctica
 4) Arctic Ocean
 5) Asia
 6) Atlantic Ocean
 7) Australia
 8) Europe
 9) Indian Ocean
10) Pacific Ocean
11) coord - I want to use geographical coordinates.
12) TZ - I want to specify the time zone using the Posix TZ format.
#? 5
Please select a country whose clocks agree with yours.
 1) Afghanistan		  18) Israel		    35) Palestine
 2) Armenia		  19) Japan		    36) Philippines
 3) Azerbaijan		  20) Jordan		    37) Qatar
 4) Bahrain		  21) Kazakhstan	    38) Russia
 5) Bangladesh		  22) Korea (North)	    39) Saudi Arabia
 6) Bhutan		  23) Korea (South)	    40) Singapore
 7) Brunei		  24) Kuwait		    41) Sri Lanka
 8) Cambodia		  25) Kyrgyzstan	    42) Syria
 9) China		  26) Laos		    43) Taiwan
10) Cyprus		  27) Lebanon		    44) Tajikistan
11) East Timor		  28) Macau		    45) Thailand
12) Georgia		  29) Malaysia		    46) Turkmenistan
13) Hong Kong		  30) Mongolia		    47) United Arab Emirates
14) India		  31) Myanmar (Burma)	    48) Uzbekistan
15) Indonesia		  32) Nepal		    49) Vietnam
16) Iran		  33) Oman		    50) Yemen
17) Iraq		  34) Pakistan
#? 9
Please select one of the following time zone regions.
1) Beijing Time
2) Xinjiang Time
#? 1

The following information has been given:

	China
	Beijing Time

Therefore TZ='Asia/Shanghai' will be used.
Local time is now:	Fri Mar 31 11:49:05 CST 2017.
Universal Time is now:	Fri Mar 31 03:49:05 UTC 2017.
Is the above information OK?
1) Yes
2) No
#? 1

You can make this change permanent for yourself by appending the line
	TZ='Asia/Shanghai'; export TZ
to the file '.profile' in your home directory; then log out and log in again.

Here is that TZ value again, this time on standard output so that you
can use the /usr/bin/tzselect command in shell scripts:
Asia/Shanghai
root@ubuntu:~#
```

注意：tzselect命令只告诉你选择的时区的写法，并不会生效。所以现在它还不是东8区北京时间。你可以在.profile、.bash_profile或者/etc/profile中设置正确的TZ环境变量并导出。 例如在.bash_profile里面设置 TZ='Asia/Shanghai'; export TZ并使其生效。


参考文章：[http://www.cnblogs.com/kerrycode/p/4217995.html](http://www.cnblogs.com/kerrycode/p/4217995.html)




