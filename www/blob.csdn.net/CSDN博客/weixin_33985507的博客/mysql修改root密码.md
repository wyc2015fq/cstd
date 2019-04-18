# mysql修改root密码 - weixin_33985507的博客 - CSDN博客
2018年06月07日 11:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
- 刚开始安装时,因为没有设置root密码,可以用以下命令行设置密码:
```
//密码为ppf123;
$ ..../mysqladmin -u root password 'ppf123'
```
- 要修改root密码:
```
$ .../mysqladmin -u root -p password ppf1234
Enter password:
```
出现`Enter password:`时,输入旧密码就好了.
