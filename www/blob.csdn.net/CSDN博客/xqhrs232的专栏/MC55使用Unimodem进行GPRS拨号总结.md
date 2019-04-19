# MC55使用Unimodem进行GPRS拨号总结 - xqhrs232的专栏 - CSDN博客
2013年02月05日 10:42:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：737
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://blog.mcuol.com/User/fushiqi/article/11296_1.htm](http://blog.mcuol.com/User/fushiqi/article/11296_1.htm)
最近使用MC55来调试WinCE5下的GPRS拨号功能，使用的是CE5自带的unimodem驱动。 
关于如何配置CE5下使用unimodem来进行GPRS拨号上网，请参考： 
[http://www.armce.cn/bbs/misc.php?action=attachcredit&aid=29&formhash=97065cd1](http://www.armce.cn/bbs/misc.php?action=attachcredit&aid=29&formhash=97065cd1)
在配置好一切功能以后，出现了拨号很快就断开连接的情况，提示检查波特率配置什么的。 
网上关于这种情况的解释是： 
“若有下面的现象：能打开COM口，也能“拨号到*99***1#\",然后很快就显示“已连接的设备”，然后就没动 
静了，但不能上网，大概过了两三分钟就跳出一个对话框说“与远程计算机的连接断开，请重拨”。遇到这种 
情况的话请检查确保CTS,RTS没接；端口设置里选的是无硬件流控；安全设置里不选@使用数据加密@使用可扩展 
的身份验证协议（EAP）@选择 微软CHAP选项（必选）其他可选 ” 
这里需要注意的是强调了不接CTS和RTS，还有人说如果引出来了就短接这两根线，但是实际上可以使用硬件流 
控的，下面会介绍。 
我们在确定了配置没有问题的情况下，仍然出现这个问题，于是通过分析串口收发的数据，注意到一个细节: 
AUTORAS:: Dialer notifies: [1] [Dialer Start] 
AT 
AT 
OK 
AT 
AT 
OK 
ATE0V1&C1&D2 
ATE0V1&C1&D2 
OK 
ATS7=120 
OK 
AT+CGDCONT=1,"IP","CMNET" 
OK 
AT 
OK 
AT 
OK 
ATE0V1&C1&D2 
OK 
ATS7=120 
OK 
AT+CGDCONT=1,"IP","CMNET" 
OK 
ATX4 
OK 
ATDT*99# 
CONNECT 
~鮂F}#?}!}#} }9}"}&} }*} } }''''}"}(}"}%}&n蛚-脈#}%?}%?~~鮂F}#?}!} } }.}"}&} } } } }''''}"}(}"?~~鮂F}#?}#} 
} }*}"}&} }*} } 憓=~~鮂F}#?}!}!} }.}"}&} }*} } }''''}"}(}"''''輣~鮂F}#?}"}!} }.}"}&} }*} } }''''}"}(}"} 
9^~~鮂F}#?}!}#} }9}"}&} }*} } }''''}"}(}"}%}&n蛚-脈#}%?}%?~~鮂F}#?}"}#} }9}"}&} }*} } }''''}"}(}"}%}&n蛚- 
脈#}%?}%s*~~? =)?屁}3@f??k 殳涢G畘~? 甒媠 P/?}3R﹑﹕!拈~~? 录~~鈧? 
& ~~鈧? ( - ??儈鮂F ? 鈧? ?M脋~~鈧琖 
eK??U鹴~~鈧? - ??m鄜~鈧? ?? K~ 鐑~~鮂F}#?}%}&} }$8K~~鮂F ? 鮪~+++~鮂F} 
#?}%}&} }$8K~ATH 
NO CARRIER 
ATZ 
OK 
RASCS_Disconnected: Ecode=619 
这里注意到，在CONNECT前面的拨号命令是ATDT*99#，号码倒是没有问题，无论*99#和*99***1#，关键是这个 
ATDT，因为MC55的拨号命令是ATD，那么这个“T”是怎么来的呢 
通过分析源码就可以得到答案。 
拨号的源程序在：D:\WINCE500\PRIVATE\WINCEOS\COMM\TAPI\UNIMODEM\dial.c 
其中拨号的命令是由CreateDialCommands函数生成，经过分析得知，拨号命令是由注册表 
[HKEY_LOCAL_MACHINE\Drivers\Unimodem\Settings] 
   "Prefix"="AT" 
   "Terminator"="" 
   "DialPrefix"="D" 
   "DialSuffix"=";" 
   "Pulse"="P" 
   "Tone"="T" 
   "Blind_Off"="X4" 
   "Blind_On"="X3" 
   "CallSetupFailTimeout"="ATS7=<#>" 
   "Reset"="AT" 
来决定的。可以在common.reg中找到 
分析CreateDialCommands得知，拨号命令由Prefix+DialPrefix+DialType组成的前缀，其中DialType就是配置 
为Tone，Tone在注册表里面的值就是T，所以这个T是这么来的。 
于是在注册表中做出修改 
"Tone"="" 
结果这个问题就没有出现。 
总结： 
1. 可见出现断线现象可能还是AT指令的差异造成的，unimodem需要的是 Hayes-compatible AT-command-based 
modems 
我也没有找出MC55是不是属于Hayes-compatible 
2. 关于硬件流控，经过测试是可以使用的，只要你接了RTS，CTS，在拨号配置里选择硬件流控，可以正常拨号 
并且上网 
3. 还有一点，关于拨号以后打不开网页的问题，确实有可能是信号问题，接上天线以后，上网速度很快。
//===========================================================
备注：：
1、我按这个方法试了还是一个样----显示假连接，弹出验证“波特率”的界面！！！
