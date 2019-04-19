# Windows 不能加载本地存储的配置文件。此问题的可能原因是安全权限不足或本地配置 - 三少GG - CSDN博客
2011年07月21日 00:57:10[三少GG](https://me.csdn.net/scut1135)阅读数：30560
现象： 笔记本断电后 重启，进入administrator界面后不再是之前的桌面及我的文档，登录后的桌面像是刚装XP时一样的...不过**在C盘目录下“我的文档”文件夹下可以找到原来的文件**...但怎么恢复原来的桌面呢？ 去控制面板-->管理工具-->事件查看器-->Windows日志-->应用程序 的 错误 里面可以查到
“1.Windows 不能加载本地存储的配置文件。此问题的可能原因是安全权限不足或本地配置2.Windows 不能加载本地存储的配置文件。此问题的可能原因是安全权限不足或本地配置文件损坏。 3.Windows 已经备份了此用户的配置文件。下次此用户登录时，Windows 将自动尝试使用此备份的配置文件。4.Windows 找不到本地配置文件，正在用临时配置文件让您登录。当您注销时，对此配置文件所作的更改将丢失。”
（已经出现过两次）
原因：
所以导致该故障的原因主要有两种：1、Default User文件夹不完整或者被删除了，导致系统无法复制新的一份。这种情况的话需要从另外一个系统拷贝一份保存到Documents and Settings目录下即可。**2、注册表中Default User文件夹的指定路径错误或丢失**。这种情况的话运行regedit，找到HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProfileList ，确保以下两个字符串这样设置： AllUsersProfile 值为 All Users ProfilesDirectory 值为 %SystemDrive%\Documents and Settings 解决办法：**可能因为一些意外的操作比如说突然掉电导致用户配置文件丢失**。用其它帐户登陆 ，把刚才那个用户下的文件备份，然后删掉重新建一个用户就可以了。还有一种情况的原因：就是当前登录的用户为域中的用户，此用户的本地配置文件被误删除或者被破坏，这样在登录时，系统就找不到其配置文件信息。所以出现次错误。当然，解决的办法就是恢复其配置文件。步骤如下： 
解决方法：
1、开机按F8不动到高级选项出现在松手，选“最近一次的正确配置”回车修复。
 2、开机按F8进入安全模式后在退出，选重启或关机在开机，就可以进入正常模式（修复注册表）。 
用Administrator帐户登录后出现这样一个提示信息：您已使用临时配置文件登录，而且没有在C:\Users生成Administrator文件夹。解决方法：打开注册表编辑器，定位到HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProfileList，ProfileList下有几个子项，每个都查看一下，将ProfileImagePath为C:\Users\Administrator的子项删除，我这里是S-1-5-21-1094688545-3406832381-232606862-500.bak。
> 也就是删除注册表中有ID.bak的注册表项
参考资料：
[http://hi.baidu.com/happyccie/blog/item/33dae2166cb4504e20a4e9d8.html](http://hi.baidu.com/happyccie/blog/item/33dae2166cb4504e20a4e9d8.html)
