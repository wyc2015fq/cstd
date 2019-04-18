# Windows7 安装SQL Server Management Studio时出现29506错误的解决办法 - weixin_33985507的博客 - CSDN博客
2012年06月07日 23:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Windows7下安装SQLServer2005_SSMSEE.msi，经常会遇到29506这个错误代码。这需要我们在安装的时候以管理员的身份运行。可是我们右键单击时发现没有以管理员权限运行的选项.
解决方法：
1：新建一个记事本，输入msiexec /i  path\SQLServer2005_SSMSEE.msi 然后另存为.cmd格式。(比如像这样  msiexec /i E:\\SQLServer2005_SSMSEE.msi)
2：按win+r快捷键，打开运行窗口，输入cmd进入dos界面
3：在dos界面中输入command prompt
4：这时右键单击刚刚创建的那个.cmd文件，会发现多了一个“以管理员身份运行”，点击以管理员身份运行，安装sql2005就可以,不会再提示错误.
