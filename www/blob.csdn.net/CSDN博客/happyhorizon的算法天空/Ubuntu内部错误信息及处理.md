# Ubuntu内部错误信息及处理 - happyhorizon的算法天空 - CSDN博客
2017年09月20日 17:52:10[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：2688
- [烦人的内部错误信息](#烦人的内部错误信息)
- [解决方案](#解决方案)- [临时关闭Apport错误报告](#临时关闭apport错误报告)
- [永久关闭Apport错误报告](#永久关闭apport错误报告)
- [简单处理: 删除.crash文件](#简单处理-删除crash文件)
# 烦人的内部错误信息
Ubuntu桌面版预装了Apport，它是一个错误收集系统，会收集软件崩溃、未处理异常和其他，包括程序bug，并为调试目的生成崩溃报告。当一个应用程序崩溃或者出现Bug时候，Apport就会通过弹窗警告用户并且询问用户是否提交崩溃报告: 
“Sorry, the application XXXX has closed unexpectedly.”  
“对不起，应用程序XXXX意外关闭了。”  
“Sorry, Ubuntu XX.XX has experienced an internal error.”  
“对不起，Ubuntu XX.XX 发生了一个内部错误。”  
“System program problem detected.”  
“检测到系统程序问题。” 
实际情况并不是Ubuntu容易出现内部错误，而是一旦程序崩溃过一次，就会生成一个.crash文件，记录着崩溃信息，这些崩溃信息保存在目录: 
`/var/crash/`
只要你不去处理，每次开机都会提示你有错误。也就是说：报错并不一定是出现了什么错误，而是曾经出现过错误，而你没有处理。
# 解决方案
## 临时关闭Apport错误报告
如果你想要临时关闭Apport，使用下列命令 
`$ sudo service apport stop`
注意重启Ubuntu系统Apport会继续开启
## 永久关闭Apport错误报告
为了永久关闭Apport，编辑/etc/default/apport，修改下列参数 
`enabled=0`
重启你的Ubuntu系统，Apport将会自动关闭 
如果你再也不会用Apport，有一种简单的方法完全移除它 
`$ sudo apt-get purge apport`
## 简单处理: 删除.crash文件
到/var/crash/目录查看崩溃文件，如果不是什么大问题(通常都没什么大问题)，删除该目录下的崩溃文件，之后就不会再报错误了. 
参考: 
[https://www.zhihu.com/question/57475009/answer/157879375](https://www.zhihu.com/question/57475009/answer/157879375)
[http://www.linuxdiyf.com/linux/20070.html](http://www.linuxdiyf.com/linux/20070.html)
[http://forum.ubuntu.org.cn/viewtopic.php?t=459138](http://forum.ubuntu.org.cn/viewtopic.php?t=459138)
