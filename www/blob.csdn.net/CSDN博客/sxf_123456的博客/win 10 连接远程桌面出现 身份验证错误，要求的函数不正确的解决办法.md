# win 10 连接远程桌面出现 身份验证错误，要求的函数不正确的解决办法 - sxf_123456的博客 - CSDN博客
2019年03月18日 10:07:46[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：42
win 10 教育版和家庭版的解决方案:
1、使用 windows+R 命令打开运行，之后输入regedit 打开注册表
依次打开路径 
`计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System\CredSSP\Parameters`
注意: 如果 \`CredSSP\Parameters 没有则创建该项，`
`之后在Parameters的右键，创建一个DWORD(32位)值，如图所示`
![](https://img-blog.csdnimg.cn/2019031810043743.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=,size_16,color_FFFFFF,t_70)
重命名为: AllowEncryptionOracle
值为: 2
再次尝试连接，若连接失败则重启电脑，再次尝试连接
本方法转载自:[http://www.cnblogs.com/LuckWJL/p/9018710.html](http://www.cnblogs.com/LuckWJL/p/9018710.html)
