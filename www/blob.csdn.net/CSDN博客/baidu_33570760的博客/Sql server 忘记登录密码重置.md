# Sql server 忘记登录密码重置 - baidu_33570760的博客 - CSDN博客
2017年08月24日 15:25:19[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1525
转载自：http://blog.csdn.net/github_37517254/article/details/55049242
摸索SQL SERVER，然而太久没用忘记了sa的登录密码，搜了些资料，做一下笔记。
1.首先确保已经在计算机的“服务”中打开了“SQL Server”和“SQL Server Browser”这两个基本服务，如下图所示；
![](https://img-blog.csdn.net/20170213153525152)
然后打开"SQL Server Management Studio"，打开如下图所示的“连接到服务器”对话框。
![](https://img-blog.csdn.net/20170213152309271)
2.选择身份验证方式为“Windows身份验证”，在“运行”里输入 sqlcmd，在命令提示符窗口输入EXEC
 sp_addsrvrolemember 'DESKTOP-K7NNNE7/wanda', 'sysadmin'，然后回车。（注：'DESKTOP-K7NNNE7/wanda'就是你的计算机名/管理员用户名）
![](https://img-blog.csdn.net/20170213152651287)
![](https://img-blog.csdn.net/20170213163027241)
![](https://img-blog.csdn.net/20170213163049534)
如果"Windows身份验证”无法登录，请确保已经启用SQL Server远程连接（此步骤可在计算机的“开始”——“程序”中找到Microsoft SQL Server 2008 R2子菜单下的“Reporting Services配置管理器”打开并启用你的计算机名的报表服务器），如下图所示。
![](https://img-blog.csdn.net/20170213160604498)
4.然后连接，连接以后在左侧找到“安全性”——“登录名”——“sa”，双击打开就可以在里面重置“Sql Serveer验证”的密码了。
![](https://img-blog.csdn.net/20170213161344655)
