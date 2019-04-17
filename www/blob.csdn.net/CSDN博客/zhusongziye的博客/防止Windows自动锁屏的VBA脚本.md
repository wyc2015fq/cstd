# 防止Windows自动锁屏的VBA脚本 - zhusongziye的博客 - CSDN博客





2018年03月26日 20:58:50[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1148







一个很简单的VBA脚本，可以防止Windows自动锁屏。

主要是利用了一个不常用的键 NUMLOCK。

```
' 先定义一个Shell对象
Set WshShell = WScript.CreateObject("WScript.Shell")

WScript.Sleep 5000
wshShell.SendKeys "{NUMLOCK}"
WScript.Sleep 500
wshShell.SendKeys "{NUMLOCK}"

'设置成正需要接续的时间
for i=1 to 180
'设置成比屏保时间短点就可以
	WScript.Sleep 180000
	wshShell.SendKeys "{NUMLOCK}"
	WScript.Sleep 500
	wshShell.SendKeys "{NUMLOCK}"
next
```




