# centos把用户加入sudoers - BlueSky - CSDN博客
2015年11月17日 11:20:59[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：5059
- 切换到root用户，su root 
2.chmod u+w /etc/sudoers 
3.vi /etc/sudoers 
4.在root ALL=(ALL) ALL,在他下面添加xxx ALL=(ALL) ALL (这里的xxx是你的用户名) 
ps:这里说下你可以sudoers添加下面四行中任意一条 
youuser            ALL=(ALL)                ALL 
%youuser           ALL=(ALL)                ALL 
youuser            ALL=(ALL)                NOPASSWD: ALL 
%youuser           ALL=(ALL)                NOPASSWD: ALL
第一行:允许用户youuser执行sudo命令(需要输入密码). 
第二行:允许用户组youuser里面的用户执行sudo命令(需要输入密码). 
第三行:允许用户youuser执行sudo命令,并且在执行的时候不输入密码. 
第四行:允许用户组youuser里面的用户执行sudo命令,并且在执行的时候不输入密码. 
5.安全起见，chmod u-w /etc/sudoers
