# 在Android模拟器中模拟GPS、打电话、发短信…… - xqhrs232的专栏 - CSDN博客
2011年12月11日 22:51:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：682标签：[电话																[android																[sms																[eclipse																[list																[cmd](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=sms&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=电话&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://apps.hi.baidu.com/share/detail/18564882](http://apps.hi.baidu.com/share/detail/18564882)
相关帖子::[http://digdeeply.org/archives/10211501.html/comment-page-1](http://digdeeply.org/archives/10211501.html/comment-page-1)
首先，当然要打开模拟器
然后，连接模拟器
C:\Users\BillGates>telnet localhost 5554
Android Console: type 'help' for a list of commands
OK
其中5554为模拟器在本机的端口，可以使用adb devices查看：
C:\Users\BillGates>adb devices
List of devices attached
emulator-5554   device
连接模拟器后输入help命令查看帮助，输入命令、回车即可查看命令格式
geo是GPS、sms是发短信、gsm是打电话等等
1.用geo命令模拟发送GPS信号：
geo fix 经度 纬度
即相当于将手机移动到指定的经纬度
也可以用geo nmea模拟发送nmea内容
2.用SMS命令发短信
sms send 10086 haha
模拟10086给你的模拟器发送内容为“haha”的短信
3.用GSM命令打电话
gsm call 10086
模拟10086给你的模拟器打电话
………………………………………………
模拟器转换横屏 Ctrl+F11
2010-10-16
好吧，我得说这些都是在cmd下，其实用eclipse的DDMS环境下就可以可视化完成。配置完安装环境后eclipse的右上角应该会出现一个小绿机器人后接"DDMS”，点它就进入DDMS视图，该视图是对应模拟器的，所以必须启动模拟器才会出现内容。也可以通过Window-》Open Perspective-》DDMS进入，在这里可以对模拟器进行任意操作，拖放文件、模拟打电话等等都不在话下。
