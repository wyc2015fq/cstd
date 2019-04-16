# ubuntu16.04的root默认密码设置 - 菜鸟路上的小白 - CSDN博客





2016年09月05日 17:33:38[somnus_小凯](https://me.csdn.net/u012486840)阅读数：31130







- 默认root密码是随机的，即每次开机都有一个新的root密码。我们可以在终端输命令 sudo passwd，然后输入当前用户的密码，enter
- 终端会提示我们输入新的密码并确认，此时的密码就是root新密码。修改成功后，输入命令 su root，再输入新的密码就ok了。
- 分析没有默认的，因为你还没给root设置密码，第一个 user 是在 admin 组 ，所以他可以给 root 设置密码 , sosudo passwd root[sudo] password for you ：---> 输入你的密码，不会显示 
- 

Enter new UNIX password: 

--- > 设置root 密码Retype new UNIX password: 


- 
用安装时建立的用户登陆后在Shell执行sudo passwd root回车Password:

- 

后输入刚建立的用户密码回车Enter new UNIX password:

这时输入要设置的root密码两次就会看到 

password updated successfully了！


- 
安装后可以到用户和组里面去改，随便改的。要在登录时用root的话，可以在“登录界面”里改，让管理员登录系统的选项就可以了




